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
#define NUM_LEDS 7
#define SIZE_ARRAY_BLOBS 12
#define PACKET_LENGTH_DEBUG 32
#define PACKET_LENGTH_SYSTEM 10
#define SEN_ADDRESS_SYSTEM 0xDA
#define ALEX_ADDRESS_SYSTEM 0x42
#define SEN_ADDRESS_DEBUG 0x60
#define REC_ADDRESS_DEBUG 0X49
#define CHANNEL_SYSTEM 1
#define CHANNEL_DEBUG 1
#define GO_TO_GOAL 1
#define GO_TO_GOAL_CURVED 2
#define GO_TO_GOAL_WITH_PUCK 3
#define INITIAL_STATE 0
#define SYSTEM_STATE 99
#define STOP_STATE 21
#define BLUE_LED_STATE 22
#define GOAL_A_POS_X -115
#define GOAL_A_POS_Y 0
#define GOAL_B_POS_X 115
#define GOAL_B_POS_Y 0
//#define GOAL_A_POS_X 0
//#define GOAL_A_POS_Y 0
//#define GOAL_B_POS_X 0
//#define GOAL_B_POS_Y 0
#define THRESHOLD_ANGLE_GOAL 7
#define THRESHOLD_DIST_GOAL 20
#define PWM_SPEED_TURN_LFT 2500 //RIGHT NOT TURNING WITH LESS THAN 380
#define PWM_SPEED_TURN_RGHT 2500
#define PWM_SPEED_FWD_LFT 4000//3300
#define PWM_SPEED_FWD_RGHT 4000//3300
#define PWM_MAXIMUM 5000
#define PWM_MIN_LEFT 363
#define PWM_MIN_RGHT 350
#define WEIGHT_TURN 5
#define WEIGTH_FWD 1
#define TIME_STOP 40000
#define TIME_STOP_TURNING 7500
#define TURNING_ANGLE 160.0
#define PWM_SPEED_CIRCLE_LFT 2350
#define RATIO_TURNING_LFT 0.83
#define PWM_SPEED_CIRCLE_RGHT 2300
#define RATIO_TURNING_RGHT 0.845



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
void go_bwd(void);
void circle_left(void);
void circle_right(void);
//void move_robot(float theta, float dist, int dir);
void move_robot(float theta, int dir);
void turnOnBlueLED(void);
void turnOffBlueLED(void);
void celebrate(void);

//Variable used to check timer
volatile int flag_timer = 0;

//Variable for states
int state = INITIAL_STATE; //CHANGE TO SYSTEM STATE?????????????????????????????
int past_state = INITIAL_STATE;

//Variable for receiving data
char buffer_rec[PACKET_LENGTH_SYSTEM] = {0};   

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
	int pause_bool = 0;
	int enemy_rob1_x = 0,enemy_rob1_y = 0,enemy_rob2_x = 0,enemy_rob2_y = 0,enemy_rob3_x = 0,enemy_rob3_y = 0;
	int scoreA = 0,scoreB = 0;
	int PT1_left_outside = 0, PT2_left_inside = 0, PT3_right_inside = 0, PT4_right_outside = 0, PT5_back_right = 0, PT6_back_left = 0, PT7_have_puck = 0;
	int circle_started_before = 0;
    
    //Variables debugging
    float dir_x = 0;
    float dir_y = 0;
    float dir_angle = 0;
    float dist_goal = 0;
    float diff_theta = 0;
    int bank = 0;
	int cam_X = 0, cam_Y = 0, commands_var = 0;
	int timer_switch = 0;
	
	//System packet
	signed char send_buffer[PACKET_LENGTH_SYSTEM] = {0};
		
	//Debug packet
	signed char output_buffer [PACKET_LENGTH_DEBUG] = {0};
	
    //Variable for the wii cam blobs
    unsigned int blobs_wii[SIZE_ARRAY_BLOBS];
    
    int x_robot = 0, y_robot = 0, theta_robot = 0;
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
    
    //Set timer to every 0.1 s (10 Hz)
    set_timer4();
    
    //Set the ADC
    init_analog();
    
    //Initialize wii camera
    char aux = 0;
	while(!aux)
    {
        aux = m_wii_open();
    };
    
    m_wait(1000);
    m_red(OFF);
	
    
    //Open the channel
    //m_rf_open(CHANNEL_SYSTEM,ALEX_ADDRESS_SYSTEM,PACKET_LENGTH_SYSTEM);
	m_rf_open(CHANNEL_DEBUG,REC_ADDRESS_DEBUG,PACKET_LENGTH_DEBUG);
		
    //Enable interruptions
    sei();
    
    //Main loop
    while (1)
    {
		
		/*// Motor testing
        if (check(PINB,2))
        {
	        turn_left();
	        m_red(ON);
        }
        else
        {
	        turn_right();
	        m_red(OFF);
	    }*/

		/*// Motor testing
        if (check(PINB,2)) 
		{
			go_fwd();
			m_red(ON);
		}
        else
		{
			go_bwd();
			m_red(OFF);
        }*/
		
		/*//Move circle
		if (check(PINB,2))
		{
			circle_left();
			m_red(ON);
		}
		else
		{
			circle_right();
			m_red(OFF);
		}*/
		
		
		
        //LOCALIZATION CODE
        //Get the blobs
        cli();
        wii_OK = m_wii_read(blobs_wii);
        sei();
        
        //If data received correctly
        if (wii_OK)
        {
            //Get the position and orientation of the robot from the constellation
            localize_OK = localize(blobs_wii[0],blobs_wii[3],blobs_wii[6],blobs_wii[9],blobs_wii[1],blobs_wii[4],blobs_wii[7],blobs_wii[10],&x_robot,&y_robot,&theta_robot,&cam_X,&cam_Y);
 
		}
        
        //ANALOG CODE
        for (int i=0;i<NUM_LEDS;i++)
        {
			get_analog_val(i);
         
			//Wait until flag is on
			while(!check(ADCSRA,ADIF));
			switch(i)
			{
				case(0):
					PT1_left_outside = ADC;
					break;
				case(1): 
					PT2_left_inside = ADC;
					break;
				case(2):
					PT3_right_inside = ADC;
					break;
				case(3):
					PT4_right_outside = ADC;
					break;
				case(4):
					PT5_back_right = ADC;
					break;
				case(5): 
					PT6_back_left = ADC;
					break;
				case(6):
					PT7_have_puck = ADC;
					break;
			}
         
			//After doing the conversion reset flag
			set(ADCSRA,ADIF);
        }
         
        
        
        //SEND COMMANDS
        if (flag_timer == 1)
        {
			/*if (timer_switch == 0)
			{
				//Create the packet to send to system
				send_buffer[0] = ALEX_ADDRESS_SYSTEM;
				send_buffer[1] = x_robot;
				send_buffer[2] = y_robot;
				m_rf_send(SEN_ADDRESS_SYSTEM,send_buffer,PACKET_LENGTH_SYSTEM);
				timer_switch = 1;
			}
			else
			{
            		
				//DEBUG COMMANDS SENDING
				//Open the channel
				m_rf_open(CHANNEL_DEBUG,REC_ADDRESS_DEBUG,PACKET_LENGTH_DEBUG);*/	
            
                output_buffer[0] = 1;
				output_buffer[1] = state;
				output_buffer[2] = x_robot;
				output_buffer[3] = y_robot;
							
				aux_conversion = div(theta_robot,128);
				output_buffer[4] = (signed char)aux_conversion.quot;
				output_buffer[5] = (signed char)aux_conversion.rem;
			
				output_buffer[6] = (signed char)status_go_to_goal;
			
				aux_conversion = div((int)dir_angle,128);
				output_buffer[7] = (signed char)aux_conversion.quot;
				output_buffer[8] = (signed char)aux_conversion.rem;
			
				aux_conversion = div((int)dist_goal,128);
				output_buffer[9] = (signed char)aux_conversion.quot;
				output_buffer[10] = (signed char)aux_conversion.rem;
			
				aux_conversion = div(cam_X,128);
				output_buffer[11] = (signed char)aux_conversion.quot;
				output_buffer[12] = (signed char)aux_conversion.rem;
			
				aux_conversion = div(cam_Y,128);
				output_buffer[13] = (signed char)aux_conversion.quot;
				output_buffer[14] = (signed char)aux_conversion.rem;
			
				output_buffer[15] = (signed char)commands_var;
			
				aux_conversion = div((int)diff_theta,128);
				output_buffer[16] = (signed char)aux_conversion.quot;
				output_buffer[17] = (signed char)aux_conversion.rem;
			
				aux_conversion = div(PT1_left_outside,128);
				output_buffer[18] = (signed char)aux_conversion.quot;
				output_buffer[19] = (signed char)aux_conversion.rem;
				
				aux_conversion = div(PT2_left_inside,128);
				output_buffer[20] = (signed char)aux_conversion.quot;
				output_buffer[21] = (signed char)aux_conversion.rem;
				
				aux_conversion = div(PT3_right_inside,128);
				output_buffer[22] = (signed char)aux_conversion.quot;
				output_buffer[23] = (signed char)aux_conversion.rem;
				
				aux_conversion = div(PT4_right_outside,128);
				output_buffer[24] = (signed char)aux_conversion.quot;
				output_buffer[25] = (signed char)aux_conversion.rem;
				
				aux_conversion = div(PT5_back_right,128);
				output_buffer[26] = (signed char)aux_conversion.quot;
				output_buffer[27] = (signed char)aux_conversion.rem;
				
				aux_conversion = div(PT6_back_left,128);
				output_buffer[28] = (signed char)aux_conversion.quot;
				output_buffer[29] = (signed char)aux_conversion.rem;
				
				aux_conversion = div(PT7_have_puck,128);
				output_buffer[30] = (signed char)aux_conversion.quot;
				output_buffer[31] = (signed char)aux_conversion.rem;
			
				m_rf_send(SEN_ADDRESS_DEBUG,output_buffer,PACKET_LENGTH_DEBUG);
				m_red(TOGGLE);
			
				/*//Open again the system channel			
				m_rf_open(CHANNEL_SYSTEM,ALEX_ADDRESS_SYSTEM,PACKET_LENGTH_SYSTEM);
				timer_switch = 0;
			}*/
			
			
			//Reset flag
			flag_timer = 0;
			//m_green(OFF);
				
			
        }
		
        //STATE COMMANDS
        switch (state)
        {
			long stop_counter = 0;
            case INITIAL_STATE:
				//m_green(ON);
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
                state = GO_TO_GOAL_WITH_PUCK;
                break;
                
            case GO_TO_GOAL:
                if (status_go_to_goal == 0)
                {
                    dir_x = goal_pos_x-x_robot;
                    dir_y = goal_pos_y-y_robot;
                    dir_angle = atan2(-dir_x,dir_y)*180/M_PI;
                    
                    status_go_to_goal = 1;
                }else if (status_go_to_goal == 1)
                {
					dir_x = goal_pos_x-x_robot;
					dir_y = goal_pos_y-y_robot;
					dir_angle = atan2(-dir_x,dir_y)*180/M_PI;
					
                    if ((theta_robot >= dir_angle-THRESHOLD_ANGLE_GOAL) && (theta_robot <= dir_angle+THRESHOLD_ANGLE_GOAL))
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
						{
                            turn_left();
							commands_var = 1;
                        }
						else if (add_360 == 0 && (angle_dir_aux > theta_robot || theta_robot > dir_angle))
						{
                            turn_right();
							commands_var = 2;
						}
                        else if (add_360 == 1 && ((theta_robot <=dir_angle && theta_robot >=-180) || ((theta_robot >= angle_dir_aux) && (theta_robot <= 180))))
						{
                            turn_left();
							commands_var = 3;
						}
                        else if (add_360 == 1 && (theta_robot > dir_angle && theta_robot < angle_dir_aux))
						{
                            turn_right();
							commands_var = 4;
						}else
							commands_var = 0;
                        
                    }
                }else if (status_go_to_goal == 2)
                {
					state = GO_TO_GOAL_CURVED;
					status_go_to_goal = 0;
					stop_counter = 0;
					if (commands_var == 1 || commands_var == 3)
						turn_right();
					else if (commands_var == 2 || commands_var == 4)
						turn_left();
			
					while(stop_counter<TIME_STOP_TURNING)
					{
						stop_counter++;
					}
					break;
					
                    dist_goal = sqrt((x_robot-goal_pos_x)*(x_robot-goal_pos_x)+(y_robot-goal_pos_y)*(y_robot-goal_pos_y));
                    if (dist_goal < THRESHOLD_DIST_GOAL)
                        status_go_to_goal = 3;
                    else
                    {
                        
                        if (theta_robot < dir_angle-THRESHOLD_ANGLE_GOAL || theta_robot > dir_angle+THRESHOLD_ANGLE_GOAL)
                            status_go_to_goal = 0;
                        else
						{
							commands_var = 5;
                            go_fwd();
						}
						
                        
                    }
                }
                else if (status_go_to_goal == 3)
                {
                    //stop_motor();
                    status_go_to_goal = 0;
                    state = STOP_STATE;
                }
                break;
                
                
            case GO_TO_GOAL_CURVED:
                //m_green(ON);
				turnOnBlueLED();
                if (status_go_to_goal == 0)
                {
                    dist_goal = sqrt((x_robot-goal_pos_x)*(x_robot-goal_pos_x)+(y_robot-goal_pos_y)*(y_robot-goal_pos_y));
                    if (dist_goal < THRESHOLD_DIST_GOAL)
                        status_go_to_goal = 1;
                    else
                    {
                        
                        dir_x = goal_pos_x-x_robot;
                        dir_y = goal_pos_y-y_robot;
                        dir_angle = atan2(-dir_x,dir_y)*180/M_PI;
                        
                        
                        float angle_dir_aux = dir_angle-180;
                        float add_360 = 0;
                        if (angle_dir_aux < -180)
                        {
                            angle_dir_aux += 360;
                            add_360 = 1;
                        }
                        
                        
                        if (add_360 == 0 && (angle_dir_aux <= theta_robot && theta_robot <= dir_angle))
                        {
                            diff_theta = dir_angle - theta_robot;
                            bank = 0;
                            //commands_var = 1;
                        }
                        else if (add_360 == 0 && (angle_dir_aux > theta_robot || theta_robot > dir_angle))
                        {
                            if (theta_robot < 0)
                                diff_theta = (theta_robot+360) - dir_angle;
                            else
                                diff_theta = (theta_robot) - dir_angle;
                            bank = 1;
                            //commands_var = 2;
                        }
                        else if (add_360 == 1 && ((theta_robot <=dir_angle && theta_robot >=-180) || ((theta_robot >= angle_dir_aux) && (theta_robot <= 180))))
                        {
                            if (theta_robot < 0)
                                diff_theta = dir_angle - theta_robot;
                            else
                                diff_theta = (dir_angle + 360) - theta_robot;
                            bank = 0;
                            //commands_var = 3;
                        }
                        else if (add_360 == 1 && (theta_robot > dir_angle && theta_robot < angle_dir_aux))
                        {
                            diff_theta = theta_robot - dir_angle;
                            bank = 1;
                            //commands_var = 4;
                        }else {
                            diff_theta = 0;
                            bank = 0;
                            //commands_var = 0;
                        }
						commands_var = bank;
						
						move_robot(diff_theta,bank);
						//move_robot(diff_theta,dist_goal,bank);                        
                    }
					
                }
                else if (status_go_to_goal == 1)
                {
                    //stop_motor();
					m_green(ON);
                    status_go_to_goal = 0;
					stop_counter = 0;
					go_bwd();
					while(stop_counter<TIME_STOP)
					{
						stop_counter++;
					}
                    state = STOP_STATE;
                }
                break;
				
			case GO_TO_GOAL_WITH_PUCK:
			if (status_go_to_goal == 0)
			{
				dist_goal = sqrt((x_robot-goal_pos_x)*(x_robot-goal_pos_x)+(y_robot-goal_pos_y)*(y_robot-goal_pos_y));
				if (dist_goal < THRESHOLD_DIST_GOAL)
					status_go_to_goal = 1;
				else
				{
					
					dir_x = goal_pos_x-x_robot;
					dir_y = goal_pos_y-y_robot;
					dir_angle = atan2(-dir_x,dir_y)*180/M_PI;
					
					
					float angle_dir_aux = dir_angle-180;
					float add_360 = 0;
					if (angle_dir_aux < -180)
					{
						angle_dir_aux += 360;
						add_360 = 1;
					}
					
					
					if (add_360 == 0 && (angle_dir_aux <= theta_robot && theta_robot <= dir_angle))
					{
						diff_theta = dir_angle - theta_robot;

					}
					else if (add_360 == 0 && (angle_dir_aux > theta_robot || theta_robot > dir_angle))
					{
						if (theta_robot < 0)
							diff_theta = (theta_robot+360) - dir_angle;
						else
							diff_theta = (theta_robot) - dir_angle;

					}
					else if (add_360 == 1 && ((theta_robot <=dir_angle && theta_robot >=-180) || ((theta_robot >= angle_dir_aux) && (theta_robot <= 180))))
					{
						if (theta_robot < 0)
							diff_theta = dir_angle - theta_robot;
						else
							diff_theta = (dir_angle + 360) - theta_robot;
					}
					else if (add_360 == 1 && (theta_robot > dir_angle && theta_robot < angle_dir_aux))
					{
						diff_theta = theta_robot - dir_angle;
					}else 
					{
						diff_theta = 0;
					}
					
					if (diff_theta > -60 && diff_theta <60)
					{
						status_go_to_goal = 0;
						circle_started_before = 0;
						state = GO_TO_GOAL_CURVED;
						break;
					}
					else
					{
						if (y_robot <= 0 && circle_started_before == 0)
						{
							circle_left();
							circle_started_before = 1;
						}
						else if (y_robot > 0 && circle_started_before == 0)
						{
							circle_right();
							circle_started_before = 1;							
						}
					}	
														
				}
				
			}
			else if (status_go_to_goal == 1)
			{
				//stop_motor();
				status_go_to_goal = 0;
				circle_started_before = 0;
				state = STOP_STATE;
			}
			break;
             
			case SYSTEM_STATE:
				switch (buffer_rec[0])
				{
					//Comm test
					case 0xA0:
						state = BLUE_LED_STATE;
						break;
						
					//Play
					case 0xA1:
						//if (pause_bool)
						//{
							//state = past_state;
							//pause_bool = 0;
						//}else
						//{
							//state = INITIAL_STATE;
						//}
						state = INITIAL_STATE;
						m_red(ON);
						break;
					
					//Goal A
					case 0xA2:
						if (check(PINB,2))
							celebrate();
						stop_motor();					
						scoreA = buffer_rec[1];
						scoreB = buffer_rec[2];
						state = STOP_STATE;
						break;
						
					//Goal B
					case 0xA3:
						if (!check(PINB,2))
							celebrate();
						stop_motor();
						scoreA = buffer_rec[1];
						scoreB = buffer_rec[2];
						state = STOP_STATE;
						break;
						
					//Pause
					case 0xA4:
						pause_bool = 1;
						stop_counter = 0;
						go_bwd();
						while(stop_counter<TIME_STOP)
						{
							stop_counter++;
						}
						state = STOP_STATE;
						break;
						
					//Halftime
					case 0xA6:
						stop_counter = 0;
						go_bwd();
						while(stop_counter<TIME_STOP)
						{
							stop_counter++;
						}
						state = STOP_STATE;
						break;
						
					//Game over
					case 0xA7:
						if (check(PINB,2))
						{
							if (scoreA > scoreB)
								celebrate();						
						}else
						{
							if (scoreA < scoreB)
							celebrate();
						}
						stop_motor();
						stop_counter = 0;
						go_bwd();
						while(stop_counter<TIME_STOP)
						{
							stop_counter++;
						}
						state = STOP_STATE;													
						break;
						
					//Enemy positions
					case 0xA8:
						enemy_rob1_x = buffer_rec[2];
						enemy_rob1_y = buffer_rec[3];
						enemy_rob2_x = buffer_rec[5];
						enemy_rob2_y = buffer_rec[6];
						enemy_rob3_x = buffer_rec[8];
						enemy_rob3_y = buffer_rec[9];
						break;					
					
					default:
						break;						
				}
				break;
				
			case BLUE_LED_STATE:
				stop_motor();
				turnOnBlueLED();
				state = STOP_STATE;
				break;
				
			case STOP_STATE:
				m_green(ON);
				stop_motor();
				break;			  
                
            default:
			    stop_motor();
                //while(1)
                //{
                    //m_red(TOGGLE);
                    //m_green(TOGGLE);
                    //m_wait(250);
                //}
        }
        
    }
  
    
}

void set_timer1(void)
{
    //Set B6 and B7 as output
    set(DDRB,6);
    set(DDRB,7);
    
    OCR1A = PWM_MAXIMUM;
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
    //B3 and D3 as outputs
	set(DDRB,3);
	set(DDRD,3);
	set(PORTB,3);
	set(PORTD,3);
    
    //Set B2 as input and enable pull-up
    clear(DDRB,2);
    set(PORTB,2);
	
	//Set D5 as output
	set(DDRD,5);
	clear(PORTD,5);
}


void stop_motor(void)
{
    OCR1B = 0;
    OCR1C = 0;
}

void turn_left(void)
{
    clear(PORTB,3);
   set(PORTD,3);
	OCR1B = PWM_SPEED_TURN_RGHT;
    OCR1C = PWM_SPEED_TURN_LFT;
	//m_green(ON);
}

void turn_right(void)
{
    set(PORTB,3);
    clear(PORTD,3);
    OCR1B = PWM_SPEED_TURN_RGHT;
    OCR1C = PWM_SPEED_TURN_LFT;
	//m_green(ON);
}

void go_bwd(void)
{
    set(PORTB,3);
    set(PORTD,3);
    OCR1B = PWM_SPEED_FWD_RGHT;
    OCR1C = PWM_SPEED_FWD_LFT;
	//m_green(TOGGLE);
}

void go_fwd(void)
{
	clear(PORTB,3);
	clear(PORTD,3);
	OCR1B = PWM_SPEED_FWD_RGHT;
	OCR1C = PWM_SPEED_FWD_LFT;
	//m_green(ON);
}

void circle_left()
{
	clear(PORTB,3);
	clear(PORTD,3);
	OCR1B = PWM_SPEED_CIRCLE_LFT;
	OCR1C = PWM_SPEED_CIRCLE_LFT*RATIO_TURNING_LFT;
	//m_green(ON);
}

void circle_right()
{
	clear(PORTB,3);
	clear(PORTD,3);
	OCR1B = PWM_SPEED_CIRCLE_RGHT*RATIO_TURNING_RGHT;
	OCR1C = PWM_SPEED_CIRCLE_RGHT;
	//m_green(ON);
}

/*void move_robot(float theta, float dist, int dir){
	dist = 0
    if (dir == 0) {             // Move with a right curve
        OCR1B = MIN_SPEED+(MAX_SPEED-MIN_SPEED)*(dist/240.0);
        OCR1C = MIN_SPEED+((theta/180.0)*WEIGHT_TURN+(dist/240.0)*WEIGTH_FWD)*(MAX_SPEED-MIN_SPEED)/(WEIGTH_FWD+WEIGHT_TURN);
    }
    else 
	{                      // Move with a left curve
        OCR1B = MIN_SPEED+((theta/180.0)*WEIGHT_TURN+(dist/240.0)*WEIGTH_FWD)*(MAX_SPEED-MIN_SPEED)/(WEIGTH_FWD+WEIGHT_TURN);
        OCR1C = MIN_SPEED+(MAX_SPEED-MIN_SPEED)*(dist/240.0);
    }
    //m_green(ON);
    clear(PORTB,0);
    set(PORTB,1);
}*/

void move_robot(float theta, int dir){
	if (dir == 1) {             // Move with a right curve
		OCR1C = PWM_SPEED_FWD_LFT;
		if (theta> TURNING_ANGLE)
			OCR1B = PWM_MIN_RGHT;
		else
			OCR1B = PWM_MIN_RGHT+((TURNING_ANGLE - theta)/TURNING_ANGLE)*(PWM_SPEED_FWD_RGHT-PWM_MIN_RGHT);
	}
	else
	{                      
		// Move with a left curve
		OCR1B = PWM_SPEED_FWD_RGHT;		
		if (theta> TURNING_ANGLE)
			OCR1C = PWM_MIN_LEFT;
		else
			OCR1C = PWM_MIN_LEFT+((TURNING_ANGLE - theta)/TURNING_ANGLE)*(PWM_SPEED_FWD_LFT-PWM_MIN_LEFT);
		
	}
	
	clear(PORTB,3);
	clear(PORTD,3);
}

void turnOnBlueLED(void)
{
	set(PORTD,5);
}

void turnOffBlueLED(void)
{
	clear(PORTD,5);
}

void celebrate(void)
{
	
}

/*ISR(TIMER3_COMPA_vect)
 {
 m_red(ON);
 flag_timer = 1;
 }*/

ISR(TIMER4_OVF_vect)
{
    //m_green(ON);
    flag_timer = 1;
}

/*ISR(INT2_vect)
{
	//Read
	m_rf_read(buffer_rec,PACKET_LENGTH_SYSTEM);
	past_state = state;
	state = SYSTEM_STATE;
	//m_green(ON); // Indicator receiving from RF
}*/
