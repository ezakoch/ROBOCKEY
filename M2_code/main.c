//Sending values using Wireless

//Includes & Constant defines
#include "m_general.h"
#include "m_bus.h"
#include "m_rf.h"
#include "m_wii.h"
#include "Functions/Localize.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#define N_CLOCK 0
#define NUM_LEDS 8
#define SIZE_ARRAY_BLOBS 12
#define PACKET_LENGTH 14
#define SEN_ADDRESS 0x60
#define REC_ADDRESS_AUX 0X49
#define CHANNEL 1
#define GO_TO_GOAL 1
#define INITIAL_STATE 0
#define GOAL_A_POS_X -115
#define GOAL_A_POS_Y 0
#define GOAL_B_POS_X 115
#define GOAL_B_POS_Y 0
#define THRESHOLD_ANGLE_GOAL 7
#define THRESHOLD_DIST_GOAL 10

//Function prototypes
void set_timer1(void);
//void set_timer3(void);
void set_timer4(void);
void init_ports(void);
void init_analog (void);
void get_analog_val (int id);
void stop_motor(void);
void turn_right(void);
void turn_left(void);
void go_fwd(void);


//Variable used to check timer
volatile int flag_timer = 0;

//Variable for states
int state = INITIAL_STATE;

//Function prototypes
ISR(INT2_vect);

//Main function
int main(void)
{
    //Variable declaration
    unsigned char wii_OK = 0;
    unsigned char localize_OK = 0;
    int status_go_to_goal = 0;
    int goal_pos_x = 0, goal_pos_y = 0;
    
    //Variables debugging
    float dir_x = 0;
    float dir_y = 0;
    float dir_angle = 0;
    float dist_goal = 0;
    
    //
    unsigned char output_buffer [PACKET_LENGTH] = {0};

    //State(1): [0] identification, [1] current state
    unsigned char state_debug [PACKET_LENGTH] = {0};
    
    // Position(2): [0] identification,[1] x int,[2] x decimal,[3] y int,[4] y decimal, [5] theta 1st int,[6] theta 2nd int,[7] theta decimal
    signed char position [PACKET_LENGTH] = {0};
    
    //Analog values for each phototransistor(3): [0] identification,[0+i+1] 1st digit analog, [1+i+1] 1st digit analog (i=1...8)
    unsigned char LED_analog [PACKET_LENGTH] = {0};
    
    //Pixel position of stars(4): [0] identification, [0+i+1] 1st x,  [0+i+2] 2nd x, [0+i+3] 1st y, [0+i+4] 2nd y (i=1...4)
    unsigned char stars_wii [PACKET_LENGTH] = {0};
    
    //Counts how many times the solenoid has been shooted(5): [0] identification, [1] current count
    unsigned char counter_solenoid [PACKET_LENGTH] = {0};
    
    //Battery indicator in % (6): [0] identification, [1] current value
    unsigned char battery_ind [PACKET_LENGTH] =  {0};
    
    //General vars(7): [0] identification,[1] current value (i=1,2...)
    signed char general_vars [PACKET_LENGTH] = {0};
    
    //Variable for the wii cam blobs
    unsigned int blobs_wii[SIZE_ARRAY_BLOBS];
    
    int x_robot = 0, y_robot = 0, theta_robot = 0;
    int LED_values_raw [NUM_LEDS] = {0};
    div_t aux_conversion;
    
    //Set the clock system prescaler
    m_clockdivide(N_CLOCK);
    
    //Turn off the LEDs
    m_green(OFF);
    m_red(OFF);
    
    m_red(ON);
    //Initialize bus
    m_bus_init();
    
    //Initialize ports
    init_ports();
    
    //Set timer 1 for motor
    set_timer1();
    
    //Set timer 3 for solenoid
    //set_timer3();
    
    //Set timer to every 0.05 s (20 Hz)
    set_timer4();
    
    //Set the ADC
    init_analog();
    
    //Initialize wii camera
    char aux = 0;
    while(!aux)
    {
        aux = m_wii_open();
    }
    
    m_wait(1000);
    m_red(OFF);
    
    //Open the channel
    m_rf_open(CHANNEL,REC_ADDRESS_AUX,PACKET_LENGTH);
    
    //Enable interruptions
    sei();
    
    
    
    
    //Main loop
    while (1)
    {
        // Motor testing
        if (check(PINB,2)) {
            turn_left();
        }
        else{
            turn_right();
        }
        
        //LOCALIZATION CODE
        //Get the blobs
        cli();
        wii_OK = m_wii_read(blobs_wii);
        sei();
        
        //If data received correctly
        if (wii_OK)
        {
            //Get the position and orientation of the robot from the constellation
            localize_OK = localize(blobs_wii[0],blobs_wii[3],blobs_wii[6],blobs_wii[9],blobs_wii[1],blobs_wii[4],blobs_wii[7],blobs_wii[10],&x_robot,&y_robot,&theta_robot);
            
            //If we computed data correctly send it if not filter (NEEEEED TO CHANGEEEEEEE!!!!!!!!!!!!!!!)
            
            if (!localize_OK)
            {
                m_red(ON);
                //x_robot = -110;
                //y_robot = 70;
                //theta_robot = 765;
                
            }else
                m_red(OFF);
            
            
        }
        
        /*
         //ANALOG CODE
         int i;
         for (i=0;i<NUM_LEDS;i++)
         {
         get_analog_val(i);
         
         //Wait until flag is on
         while(!check(ADCSRA,ADIF));
         LED_values_raw[i] = ADC;
         
         //After doing the conversion reset flag
         set(ADCSRA,ADIF);
         }
         */
        
        
        //SEND COMMANDS
        if (flag_timer == 1)
        {
            //State
            state_debug[0] = 1;
            state_debug[1] = state;
            output_buffer[0]=state;
            output_buffer[1]=x_robot;
            output_buffer[2]=y_robot;
            aux_conversion = div(theta_robot,128);

            //Put packets together for sending
            output_buffer[3] = (signed char)aux_conversion.quot;
            output_buffer[4] = (signed char)aux_conversion.rem;

            //Debugging
            output_buffer[5] = (signed char)status_go_to_goal;
            aux_conversion = div((int)dir_x,128);
            output_buffer[6] = (signed char)aux_conversion.quot;
            output_buffer[7] = (signed char)aux_conversion.rem;
            aux_conversion = div((int)dir_y,128);
            output_buffer[8] = (signed char)aux_conversion.quot;
            output_buffer[9] = (signed char)aux_conversion.rem;
            aux_conversion = div((int)dir_angle,128);
            output_buffer[10] = (signed char)aux_conversion.quot;
            output_buffer[11] = (signed char)aux_conversion.rem;
            aux_conversion = div((int)dist_goal,128);
            output_buffer[12] = (signed char)aux_conversion.quot;
            output_buffer[13] = (signed char)aux_conversion.rem;

            m_rf_send(SEN_ADDRESS,output_buffer,PACKET_LENGTH);
            
            /*
            //Position
            position[0] = 2;
            position[1] = (signed char) x_robot;
            position[2] = (signed char) y_robot;
            aux_conversion = div(theta_robot,128);
            position[3] = (signed char)aux_conversion.quot;
            position[4] = (signed char)aux_conversion.rem;
            */
            //m_rf_send(SEN_ADDRESS,position,PACKET_LENGTH);
            
            
            /*//Analog values
             LED_analog[0] = 3;
             int i;
             for (i=0;i<NUM_LEDS;i++)
             {
             aux_conversion = div(LED_values_raw[i],256);
             LED_analog[2*i+1] = aux_conversion.quot;
             LED_analog[2*i+2] = aux_conversion.rem;
             }
             m_rf_send(SEN_ADDRESS,LED_analog,PACKET_LENGTH);*/
            
            
            /*
            //General variables
            general_vars [0] = 7;
            general_vars [1] = (signed char)status_go_to_goal;
            aux_conversion = div(dir_x,128);
            general_vars[2] = (signed char)aux_conversion.quot;
            general_vars[3] = (signed char)aux_conversion.rem;
            aux_conversion = div(dir_y,128);
            general_vars[4] = (signed char)aux_conversion.quot;
            general_vars[5] = (signed char)aux_conversion.rem;
            aux_conversion = div(dir_angle,128);
            general_vars[6] = (signed char)aux_conversion.quot;
            general_vars[7] = (signed char)aux_conversion.rem;
            aux_conversion = div(dist_goal,128);
            general_vars[8] = (signed char)aux_conversion.quot;
            general_vars[9] = (signed char)aux_conversion.rem;
            
            m_rf_send(SEN_ADDRESS,general_vars,PACKET_LENGTH);
            */
            
            //Reset flag
            flag_timer = 0;
            m_green(OFF);
        }
        
        
        
        //STATE COMMANDS
        switch (state)
        {
            case INITIAL_STATE:
                if (check(PINB,2))
                {
                    goal_pos_x = GOAL_A_POS_X;
                    goal_pos_y = GOAL_A_POS_Y;
                }else
                {
                    goal_pos_x = GOAL_B_POS_X;
                    goal_pos_y = GOAL_B_POS_Y;
                }
                status_go_to_goal = 0;
                state = GO_TO_GOAL;
                break;
                
            case GO_TO_GOAL:
                ;
                /*
                int dir_x = 0;
                int dir_y = 0;
                int dir_angle = 0;
                int dist_goal = 0;
                */
                if (status_go_to_goal == 0)
                {
                    dir_x = goal_pos_x-x_robot;
                    dir_y = goal_pos_y-y_robot;
                    dir_angle = atan2(-dir_x,dir_y)*180/M_PI;
                    
                    status_go_to_goal = 1;
                }else if (status_go_to_goal == 1)
                {
                    if ((theta_robot >= dir_angle-THRESHOLD_ANGLE_GOAL) && (theta_robot <= dir_angle+THRESHOLD_ANGLE_GOAL)
                        status_go_to_goal = 2;
                    else
                    {
                        float angle_dir_aux = dir_angle-180;
                        float add_360 = 0;
                        if (angle_dir_aux < -180)
                        {
                            angle_dir_aux += 360;
                            add_360 = 1;
                        }
                        
                        
                        if (add_360 == 0 && (angle_dir_aux <= theta_robot && theta_robot <= dir_angle))
                            turn_left();
                        else if (add_360 == 0 && (angle_dir_aux > theta_robot && theta_robot > dir_angle))
                            turn_right();
                        else if (add_360 == 1 && (angle_dir_aux <= theta_robot && theta_robot <= dir_angle))
                            turn_right();
                        else if (add_360 == 1 && (angle_dir_aux > theta_robot && theta_robot > dir_angle))
                            turn_left();
                        
                    }
                }else if (status_go_to_goal == 2)
                {
                    dist_goal = sqrt((x_robot-goal_pos_x)*(x_robot-goal_pos_x)+(y_robot-goal_pos_y)*(y_robot-goal_pos_y));
                    if (dist_goal < THRESHOLD_DIST_GOAL)
                        status_go_to_goal = 3;
                    else
                    {
                        
                        if (theta_robot < dir_angle-THRESHOLD_ANGLE_GOAL || theta_robot > dir_angle+THRESHOLD_ANGLE_GOAL)
                            status_go_to_goal = 0;
                        else
                            go_fwd();
                        
                    }
                }
                else if (status_go_to_goal == 3)
                {
                    stop_motor();
                    status_go_to_goal = 4;
                    state = 2;
                }
                break;
            default:
                stop_motor();
                while(1)
                {
                    //m_red(TOGGLE);
                    //m_green(TOGGLE);
                    m_wait(250);
                }
        }
        
        
    }
    
    
}

void set_timer1(void)
{
    //Set B6 and B7 as output
    set(DDRB,6);
    set(DDRB,7);
    
    OCR1A = 500;
    OCR1B = 0;
    OCR1C = 0;
    
    //Set to UP to OCR1A
    set(TCCR1B,WGM13);
    set(TCCR1B,WGM12);
    set(TCCR1A,WGM11);
    set(TCCR1A,WGM10);
    
    //Set to clear at OCR1B, set at rollover
    set(TCCR1A,COM1B1);
    clear(TCCR1A,COM1B0);
    
    //Set to clear at OCR1C, set at rollover
    set(TCCR1A,COM1C1);
    clear(TCCR1A,COM1C0);
    
    //Set timer prescaler to /1
    clear(TCCR1B,CS12);
    clear(TCCR1B,CS11);
    set(TCCR1B,CS10);
}


/*//Timer 3 Initialization (SOLENOID)
 void set_timer3(void)
 {
 OCR3A = 391;
 
 //Set C6 as output (debugging)
 set(DDRC,6);
 
 //Set to UP to OCR3A
 clear(TCCR3B,WGM33);
 set(TCCR3B,WGM32);
 clear(TCCR3A,WGM31);
 clear(TCCR3A,WGM30);
 
 //Set to toggle (debugging)
 clear(TCCR3A,COM3A1);
 set(TCCR3A,COM3A0);
 
 //Demask OCR3A interrupt
 set(TIMSK3,OCIE3A);
 
 //Set timer prescaler to /1024
 set(TCCR3B,CS32);
 clear(TCCR3B,CS31);
 set(TCCR3B,CS30);
 }*/

//Timer 4 specifications (SENDING PROCEDURE)
void set_timer4(void)
{
    //Set the counter variable
    OCR4C = 195;
    
    //Set UP to OCR4C
    clear(TCCR4D,WGM41);
    clear(TCCR4D,WGM40);
    
    //Set the interruption to overflow
    set(TIMSK4,TOIE4);
    
    //Set prescaler to /4096
    set(TCCR4B,CS43);
    set(TCCR4B,CS42);
    clear(TCCR4B,CS41);
    set(TCCR4B,CS40);
}

//A/D Initialization
void init_analog(void)
{
    //Set the reference voltage to Vcc
    clear(ADMUX,REFS1);
    set(ADMUX,REFS0);
    
    //Set the ADC Clock Prescaler (/128)
    set(ADCSRA,ADPS2);
    set(ADCSRA,ADPS1);
    set(ADCSRA,ADPS0);
    
    //Disable set(DIDR2,ADC9D);
    set(DIDR0,ADC0D);
    set(DIDR0,ADC1D);
    set(DIDR0,ADC4D);
    set(DIDR0,ADC5D);
    set(DIDR0,ADC6D);
    set(DIDR0,ADC7D);
    set(DIDR2,ADC8D);
    set(DIDR2,ADC9D);
    
    //Set the triggering to free-running
    set(ADCSRA,ADATE);
    
}

void get_analog_val(int id)
{
    //Disable ADC
    clear(ADCSRA,ADEN);
    
    //Check which analog device we want to get the values from and select it
    switch (id)
    {
        case 0:
            //Single-Ended Channel Selection (F0)
            clear(ADCSRB,MUX5);
            clear(ADMUX,MUX2);
            clear(ADMUX,MUX1);
            clear(ADMUX,MUX0);
            break;
            
        case 1:
            //Single-Ended Channel Selection (F1)
            clear(ADCSRB,MUX5);
            clear(ADMUX,MUX2);
            clear(ADMUX,MUX1);
            set(ADMUX,MUX0);
            break;
            
        case 2:
            //Single-Ended Channel Selection (F4)
            clear(ADCSRB,MUX5);
            set(ADMUX,MUX2);
            clear(ADMUX,MUX1);
            clear(ADMUX,MUX0);
            break;
            
        case 3:
            //Single-Ended Channel Selection (F5)
            clear(ADCSRB,MUX5);
            set(ADMUX,MUX2);
            clear(ADMUX,MUX1);
            set(ADMUX,MUX0);
            break;
            
        case 4:
            //Single-Ended Channel Selection (F6)
            clear(ADCSRB,MUX5);
            set(ADMUX,MUX2);
            set(ADMUX,MUX1);
            clear(ADMUX,MUX0);
            break;
            
        case 5:
            //Single-Ended Channel Selection (F7)
            clear(ADCSRB,MUX5);
            set(ADMUX,MUX2);
            set(ADMUX,MUX1);
            set(ADMUX,MUX0);
            break;
            
        case 6:
            //Single-Ended Channel Selection (D4)
            set(ADCSRB,MUX5);
            clear(ADMUX,MUX2);
            clear(ADMUX,MUX1);
            clear(ADMUX,MUX0);
            break;
            
        case 7:
            //Single-Ended Channel Selection (D6)
            set(ADCSRB,MUX5);
            clear(ADMUX,MUX2);
            clear(ADMUX,MUX1);
            set(ADMUX,MUX0);
            break;
            
    }
    
    //Enable ADC Subsystem & Begin Conversion
    set(ADCSRA,ADEN);
    set(ADCSRA,ADSC);
    set(ADCSRA,ADIF);
}

void init_ports(void)
{
    //B0 and B1 as outputs
    set(DDRB,0);
    set(DDRB,1);
    set(PORTB,0);
    clear(PORTB,1);
    
    //Set B2 as input
    clear(DDRB,2);
    //set(PORTB,2);
}


void stop_motor(void)
{
    OCR1B = 0;
    OCR1C = 0;
}

void turn_right(void)
{
    set(PORTB,0);
    clear(PORTB,1);
    OCR1B = OCR1A;
    OCR1C = OCR1A;
}

void turn_left(void)
{
    clear(PORTB,0);
    set(PORTB,1);
    OCR1B = OCR1A;
    OCR1C = OCR1A;
}

void go_fwd(void)
{
    set(PORTB,0);
    set(PORTB,1);
    OCR1B = OCR1A;
    OCR1C = OCR1A;
}

/*ISR(TIMER3_COMPA_vect)
 {
 m_red(ON);
 flag_timer = 1;
 }*/

ISR(TIMER4_OVF_vect)
{
    m_green(ON);
    flag_timer = 1;
}
