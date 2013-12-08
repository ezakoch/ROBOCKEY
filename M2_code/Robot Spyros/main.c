//Sending values using Wireless

//Includes & Constant defines
#include "m_general.h"
#include "m_bus.h"
#include "m_rf.h"
#include "m_wii.h"
#include "Localize.h"
#include "Init_functions.h"
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
#define ALEX_ADDRESS_SYSTEM 0x40
#define SEN_ADDRESS_DEBUG 0x60
#define REC_ADDRESS_DEBUG 0XF5
#define CHANNEL_SYSTEM 1
#define CHANNEL_DEBUG 1
#define GO_TO_GOAL 1
#define GO_TO_GOAL_CURVED 2
#define FIND_PUCK 3
#define INITIAL_STATE 0
#define SYSTEM_STATE 99
#define STOP_STATE 21
#define BLUE_LED_STATE 22
#define GOAL_A_POS_X -100 //-115
#define GOAL_A_POS_Y 0
#define GOAL_B_POS_X 115
#define GOAL_B_POS_Y 0
//#define GOAL_A_POS_X 0
//#define GOAL_A_POS_Y 0
//#define GOAL_B_POS_X 0
//#define GOAL_B_POS_Y 0
#define THRESHOLD_ANGLE_GOAL 15
#define THRESHOLD_DIST_GOAL 5
#define PWM_SPEED_TURN_LFT 2000 //RIGHT NOT TURNING WITH LESS THAN 380
#define PWM_SPEED_TURN_RGHT 2000
//#define PWM_SPEED_FWD_LFT 393
//#define PWM_SPEED_FWD_RGHT 380
#define PWM_SPEED_FWD_LFT 2800//3300
#define PWM_SPEED_FWD_RGHT 2800//3300

#define PWM_MIN_LEFT 1500
#define PWM_MIN_RGHT 1500
#define WEIGHT_TURN 5
#define WEIGTH_FWD 1
#define TIME_STOP 1000
#define TURNING_ANGLE 180.0
#define THRESHOLD_PUCK_CENTER_OUTSIDE 0.25
#define THRESHOLD_PUCK_CENTER_INSIDE 0.15
#define THRESHOLD_SWITCH_EXTERIOR 40
#define	THRESHOLD_PUCK_NOT_FIND 20



#define Kp 8
#define Kp_move 10
#define Kd 500
#define time 0.002



int diff_error = 0, cur_error = 0, prev_error = 0;
float TARGETS_X[2]={0.0};
float TARGETS_Y[2]={0.0};
int theta_robot = 0;


void calculate_diff_theta(float theta_des, float* err_theta, int* dir_to_turn);

//Function prototypes
void set_timer1(void);
//void set_timer3(void);
void set_timer4(void);
void set_timer0(void);
void start_timer0(void);
void stop_timer0(void);

void init_ports(void);
void init_analog (void);
void get_analog_val (int id);
void stop_motor(void);
void turn_right(void);
void turn_left(void);
void go_fwd(void);
void go_bwd(void);
//void move_robot(float theta, float dist, int dir);
void turn_robot(float theta, int dir, float diff);
void move_robot(float theta, int dir, float diff);
void turnOnBlueLED(void);
void turnOffBlueLED(void);
void celebrate(void);

//Variable used to check timer
volatile int flag_timer = 0;
volatile int flag_turn = 1;

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
    m_disableJTAG();
    
    //Variable declaration
    unsigned char wii_OK = 0;
    unsigned char localize_OK = 0;
    int status_go_to_goal = 0;
    int goal_pos_x = 0, goal_pos_y = 0;
	int pause_bool = 0;
	int enemy_rob1_x = 0,enemy_rob1_y = 0,enemy_rob2_x = 0,enemy_rob2_y = 0,enemy_rob3_x = 0,enemy_rob3_y = 0;
	int scoreA = 0,scoreB = 0;
	int PT1_left_outside = 0, PT2_left_inside = 0, PT3_right_inside = 0, PT4_right_outside = 0, PT5_back_right = 0, PT6_back_left = 0, PT7_have_puck = 0;
    
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
    
    int x_robot = 0, y_robot = 0;
    div_t aux_conversion;
    
    
    // --------------------------------------------------------------
    // Initialization
    // --------------------------------------------------------------
    m_clockdivide(N_CLOCK);             // Set the clock system prescaler
    m_green(OFF);                       // Turn off the LEDs
    m_red(OFF);                         // ^
	
	m_red(ON);                          // Initialize RED light indicator
    
    m_bus_init();                       // Initialize bus
    init_ports();                       // Initialize ports
    set_timer1();                       // Set timer 1 for motor
    set_timer0();                       // Set timer 0 for main loop frame rate
    start_timer0();                     // Start timer 0
    //set_timer3();                     // Set timer 3 for solenoid
    set_timer4();                       // Set timer 4 to every 0.1 s (10 Hz) to send data
    init_analog();                      // Set the ADC System
    
    char aux = 0;                       // Initialize wii camera
	while(!aux)                         // ^
    {                                   // ^
        aux = m_wii_open();             // ^
    };                                  // ^
    
    //    m_rf_open(CHANNEL_SYSTEM,ALEX_ADDRESS_SYSTEM,PACKET_LENGTH_SYSTEM);       // Open the RF channel
	m_rf_open(CHANNEL_DEBUG,REC_ADDRESS_DEBUG,PACKET_LENGTH_DEBUG);
    
    sei();                              // Enable interruptions
    m_red(OFF);                         // Turn off RED light initialize finished indicator
    
    // Initialize Target Waypoints
    //
    //    TARGETS_X[0] = GOAL_A_POS_X;
    //    TARGETS_Y[0] = GOAL_A_POS_Y;
    //    TARGETS_X[1] = GOAL_B_POS_X;
    //    TARGETS_Y[1] = GOAL_B_POS_Y;
    //
    TARGETS_X[0] = 0;
    TARGETS_Y[0] = 0;
    TARGETS_X[1] = 80;
    TARGETS_Y[1] = 30;
    
    int TARGET_NUM = 0;
    // --------------------------------------------------------------
    
    
    
    
    // --------------------------------------------------------------
    // Main loop
    // --------------------------------------------------------------
    while (1)
    {
        
        if (flag_turn == 0) {
            
            // --------------------------------------------------------------
            // LOCALIZATION CODE
            // --------------------------------------------------------------
            cli();                                          // Clear Interupts to not interfere with the mWii
            wii_OK = m_wii_read(blobs_wii);                 // Get the blobs
            sei();                                          // Enable back the interupts
            
            // If data received correctly
            if (wii_OK)
            {
                // Get the position and orientation of the robot from the constellation
                localize_OK = localize(blobs_wii[0],blobs_wii[3],blobs_wii[6],blobs_wii[9],blobs_wii[1],blobs_wii[4],blobs_wii[7],blobs_wii[10],&x_robot,&y_robot,&theta_robot,&cam_X,&cam_Y);
                
            }
            // --------------------------------------------------------------
            
            
            
            
            // --------------------------------------------------------------
            // ANALOG CODE
            // --------------------------------------------------------------
            int i;
            for (i=0;i<NUM_LEDS;i++)
            {
                get_analog_val(i);              // Get the values for each ADC pin
                while(!check(ADCSRA,ADIF));     // Wait until flag is on
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
                
                set(ADCSRA,ADIF);               // After doing the conversion reset flag
            }
            
            
            // --------------------------------------------------------------
            // SEND COMMANDS
            // --------------------------------------------------------------
            if (flag_timer == 1)
            {
                // SEND DATA TO THE MAIN SYSTEM
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
                 
                 // SEND DATA TO THE M2 CONNECTED TO MATLAB
                 //Open the channel
                 m_rf_open(CHANNEL_DEBUG,REC_ADDRESS_DEBUG,PACKET_LENGTH_DEBUG);*/
                
                output_buffer[0] = 2;
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
                
                flag_timer = 0;         //Reset flag
                // m_green(OFF);
            }
            
            
            // --------------------------------------------------------------
            // STATE COMMANDS
            // --------------------------------------------------------------
            switch (state)
            {
                    long stop_counter = 0;
                    
                // --------------------------------------------------------------
                // FIND PUCK STATE
                // --------------------------------------------------------------
                case FIND_PUCK:
                    
                    ////Check if we have the puck
                    //if (PT7_have_puck > 250)
                    //{
					//stop_motor();
					//state = GO_TO_GOAL_WITH_PUCK
					//status_go_to_goal = 0;
					//break;
                    //}
                    //
                    ////Check if we have the puck
                    //if (PT2_left_inside > 1000 &&  PT3_right_inside > 1000)
                    //{
					//stop_motor();
					//state = GO_TO_GOAL_WITH_PUCK;
					//status_go_to_goal = 0;
					//break;
                    //}
                    
                    if (status_go_to_goal == 0)
                    {
                        turnOffBlueLED();
                        
                        int max_lr = 0;
                        int half_range = 0;
                        
                        //Check at which quadrant we are
                        //Check if the puck is in the left or in the right
                        if (PT1_left_outside > PT4_right_outside)
                            max_lr = 0;
                        else
                            max_lr = 1;
                        
                        //Check if the puck is up or down
                        if (max_lr == 0)
                        {
                            if (PT1_left_outside >= PT6_back_left)
                                half_range = 0;
                            else
                                half_range = 1;
                        }
                        else
                        {
                            if (PT4_right_outside >= PT5_back_right)
                                half_range = 0;
                            else
                                half_range = 1;
                        }
                        
                        //Move around until the puck is in front
                        //Case where the puck is in in front
                        if (half_range == 0)
                        {
                            int max_pt = 0;
                            if (PT1_left_outside >= PT4_right_outside)
                                max_pt = PT1_left_outside;
                            else
                                max_pt = PT4_right_outside;
                            
                            if (abs(PT1_left_outside-PT4_right_outside) <= THRESHOLD_PUCK_CENTER_OUTSIDE*max_pt && max_pt >= THRESHOLD_PUCK_NOT_FIND)
                            {
                                if ((PT2_left_inside+PT3_right_inside)/2.0 < THRESHOLD_SWITCH_EXTERIOR)
                                    go_fwd();
                                else
                                {
                                    status_go_to_goal = 1;
                                    turnOnBlueLED();
                                    //m_wait(4000);
                                    //turnOffBlueLED();
                                }
                                
                            }else if (abs(PT1_left_outside-PT4_right_outside) <= THRESHOLD_PUCK_CENTER_OUTSIDE*max_pt && max_pt < THRESHOLD_PUCK_NOT_FIND)
                                turn_left();
                            else
                            {
                                if (PT1_left_outside > PT4_right_outside)
                                    turn_left();
                                else
                                    turn_right();
                            }
                            //Case where the puck is in the back
                        }else
                        {
                            if (PT6_back_left >= PT5_back_right)
                                turn_left();
                            else
                                turn_right();
                        }
                    }
                    else if (status_go_to_goal == 1)
                    {
                        //Internal PTs direction
                        if ((PT2_left_inside+PT3_right_inside)/2.0 < THRESHOLD_SWITCH_EXTERIOR)
                            status_go_to_goal = 0;
                        else
                        {
                            int max_pt = 0;
                            if (PT2_left_inside >= PT3_right_inside)
                                max_pt = PT2_left_inside;
                            else
                                max_pt = PT3_right_inside;
                            
                            if (abs(PT2_left_inside-PT3_right_inside) < THRESHOLD_PUCK_CENTER_INSIDE*max_pt)
                                go_fwd();
                            else
                            {
                                if (PT2_left_inside>=PT3_right_inside)
                                    turn_left();
                                else
                                    turn_right();
                            }
                        }
                    }
                    
                    break;
                // --------------------------------------------------------------
                    
                    
                    
                // --------------------------------------------------------------
                // INITIAL STATE
                // --------------------------------------------------------------
                case INITIAL_STATE:
                    if (check(PINB,2))
                    {
                        goal_pos_x = GOAL_A_POS_X;
                        goal_pos_y = GOAL_A_POS_Y;
                        //                        goal_pos_x = TARGETS_X[TARGET_NUM];
                        //                        goal_pos_y = TARGETS_Y[TARGET_NUM];
                        //
                    }else
                    {
                        goal_pos_x = GOAL_B_POS_X;
                        goal_pos_y = GOAL_B_POS_Y;
                    }
                    status_go_to_goal = 0;
                    //                    state = GO_TO_GOAL_CURVED;
                    state = FIND_PUCK;
                    break;
                // --------------------------------------------------------------
                    
                    
                    
                // --------------------------------------------------------------
                // GO TO GOAL CURVED STATE
                // --------------------------------------------------------------
                case GO_TO_GOAL_CURVED:
                    
                    dir_x = goal_pos_x-x_robot;
                    dir_y = goal_pos_y-y_robot;
                    dir_angle = atan2(-dir_x,dir_y)*180/M_PI;
                    
                    calculate_diff_theta(dir_angle,&diff_theta, &bank);
                    
                    
                    cur_error = diff_theta;
                    diff_error = cur_error - prev_error;
                    prev_error = cur_error;
                    
                    if (status_go_to_goal == 0)
                    {
                        {
                            dist_goal = sqrt((x_robot-goal_pos_x)*(x_robot-goal_pos_x)+(y_robot-goal_pos_y)*(y_robot-goal_pos_y));
                            if (dist_goal < THRESHOLD_DIST_GOAL){
                                status_go_to_goal = 2;
                            }
                            else if (diff_theta < THRESHOLD_ANGLE_GOAL) {
                                status_go_to_goal = 1;
                            }
                            else
                                turn_robot(diff_theta,bank,diff_error);
                            
                        }
                        
                    }
                    
                    else if (status_go_to_goal == 1){
                        dist_goal = sqrt((x_robot-goal_pos_x)*(x_robot-goal_pos_x)+(y_robot-goal_pos_y)*(y_robot-goal_pos_y));
                        if (dist_goal < THRESHOLD_DIST_GOAL){
                            status_go_to_goal = 2;
                            set(PORTD,5);
                            
                            //                            stop_motor();
                            //                            stop_counter = 0;
                            //                            go_bwd();
                            //
                            //                            while(stop_counter<TIME_STOP)
                            //                            {
                            //                                stop_counter++;
                            //                            }
                            //stop_motor();
                            //m_wait(1000);
                        }
                        else if (diff_theta > 2.0*THRESHOLD_ANGLE_GOAL)
                        {
                            status_go_to_goal = 0;
                            set(PORTD,5);
                        }
                        else {
                            clear(PORTD,5);
                            move_robot(diff_theta,bank,diff_error);
                        }
                        
                    }
                    else if (status_go_to_goal == 2){
                        
                        calculate_diff_theta(0,&diff_theta, &bank);
                        
                        
                        if (diff_theta < THRESHOLD_ANGLE_GOAL)
                        {
                            status_go_to_goal = 3;
                            //stop_motor();
                            //m_wait(1000);
                            //set(PORTD,5);
                        }
                        else {
                            turn_robot(diff_theta,bank,diff_error);
                        }
                        
                    }
                    else if (status_go_to_goal == 3)
                    {
                        
                        stop_motor();
                        
                        
                        //m_wait(1000);
                        //clear(PORTD,5);
                        if (TARGET_NUM == 0) {
                            TARGET_NUM = 1;
                            
                        }
                        else {
                            TARGET_NUM = 0;
                            //                            clear(PORTD,5);
                        }
                        goal_pos_x = TARGETS_X[TARGET_NUM];
                        goal_pos_y = TARGETS_Y[TARGET_NUM];
                        status_go_to_goal = 0;
                        
                        //state = STOP_STATE;
                        
                        //state = STOP_STATE;
                        
                    }
                    break;
                // --------------------------------------------------------------
                
                    
                    
                // --------------------------------------------------------------
                // SYSTEM STATE
                // --------------------------------------------------------------
                case SYSTEM_STATE:
                    switch ((int)buffer_rec[0])
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
                    
                    
                // --------------------------------------------------------------
                // BLUE LED STATE
                // --------------------------------------------------------------
                case BLUE_LED_STATE:
                    stop_motor();
                    turnOnBlueLED();
                    state = STOP_STATE;
                    break;
                    
                    
                // --------------------------------------------------------------
                // STOP STATE
                // --------------------------------------------------------------
                case STOP_STATE:
                    m_green(ON);
                    stop_motor();
                    break;
                // --------------------------------------------------------------
                
                    
                    
                // --------------------------------------------------------------
                // DEFAULT STATE
                // --------------------------------------------------------------
                default:
                    stop_motor();
                    //while(1)
                    //{
                    //m_red(TOGGLE);
                    //m_green(TOGGLE);
                    //m_wait(250);
                    //}
                // --------------------------------------------------------------
            }
            
            
        }
    }
    
    flag_turn = 1;
    start_timer0();
}
// --------------------------------------------------------------


// --------------------------------------------------------------
// CALCULATE ERROR IN ORIENTATION and OPTIMAL TURNING
// --------------------------------------------------------------
void calculate_diff_theta(float theta_des, float* err_theta, int* dir_to_turn){
    
    float angle_dir_aux = theta_des-180;
    float add_360 = 0;
    if (angle_dir_aux < -180)
    {
        angle_dir_aux += 360;
        add_360 = 1;
    }
    
    
    if (add_360 == 0 && (angle_dir_aux <= theta_robot && theta_robot <= theta_des))
    {
        *err_theta = theta_des - theta_robot;
        *dir_to_turn = 0;
    }
    else if (add_360 == 0 && (angle_dir_aux > theta_robot || theta_robot > theta_des))
    {
        if (theta_robot < 0)
            *err_theta = (theta_robot+360) - theta_des;
        else
            *err_theta = (theta_robot) - theta_des;
        *dir_to_turn = 1;
    }
    else if (add_360 == 1 && ((theta_robot <=theta_des && theta_robot >=-180) || ((theta_robot >= angle_dir_aux) && (theta_robot <= 180))))
    {
        if (theta_robot < 0)
            *err_theta = theta_des - theta_robot;
        else
            *err_theta = (theta_des + 360) - theta_robot;
        *dir_to_turn = 0;
    }
    else if (add_360 == 1 && (theta_robot > theta_des && theta_robot < angle_dir_aux))
    {
        *err_theta = theta_robot - theta_des;
        *dir_to_turn = 1;
    }else {
        err_theta = 0;
        dir_to_turn = 0;
    }
}
// --------------------------------------------------------------



// --------------------------------------------------------------
// ORIENTATION CONTROLLER TO TURN ROBOT
// --------------------------------------------------------------
void turn_robot(float theta, int dir, float diff){
	if (dir == 1) {             // Move with a right curve
		//OCR1C = PWM_SPEED_FWD_LFT;
		OCR1B = (int)(PWM_MIN_RGHT+theta*Kp);// + diff*Kd);
        OCR1C = (int)(PWM_MIN_RGHT+theta*Kp);// + diff*Kd);
        
        set(PORTB,3);
        clear(PORTD,3);
	}
	else
	{                      // Move with a left curve
		//OCR1B = PWM_SPEED_FWD_RGHT;
		OCR1C = (int)(PWM_MIN_LEFT+theta*Kp);// + diff*Kd);;
        OCR1B = (int)(PWM_MIN_LEFT+theta*Kp);// + diff*Kd);
        
        clear(PORTB,3);
        set(PORTD,3);
		
	}
}
// --------------------------------------------------------------



// --------------------------------------------------------------
// ORIENTATION CONTROLLER TO MOVE ROBOT FORWARD
// --------------------------------------------------------------
void move_robot(float theta, int dir, float diff){
	if (dir == 1) {             // Move with a right curve
		//OCR1C = PWM_SPEED_FWD_LFT;
		OCR1B = (int)(PWM_SPEED_FWD_LFT);
        OCR1C = (int)(PWM_SPEED_FWD_RGHT+theta*Kp_move);
        
        clear(PORTB,3);
        clear(PORTD,3);
	}
	else
	{                      // Move with a left curve
		//OCR1B = PWM_SPEED_FWD_RGHT;
        OCR1B = (int)(PWM_SPEED_FWD_LFT+theta*Kp_move);
		OCR1C = (int)(PWM_SPEED_FWD_RGHT);
    
        clear(PORTB,3);
        clear(PORTD,3);
		
	}
}
// --------------------------------------------------------------





// --------------------------------------------------------------
// STOP MOTORS
// --------------------------------------------------------------
void stop_motor(void)
{
    OCR1B = 0;
    OCR1C = 0;
}
// --------------------------------------------------------------


// --------------------------------------------------------------
// TURN LEFT
// --------------------------------------------------------------
void turn_left(void)
{
    clear(PORTB,3);
    set(PORTD,3);
	OCR1C = PWM_SPEED_TURN_LFT;
    OCR1B = PWM_SPEED_TURN_RGHT;
}
// --------------------------------------------------------------



// --------------------------------------------------------------
// TURN RIGHT
// --------------------------------------------------------------
void turn_right(void)
{
    set(PORTB,3);
    clear(PORTD,3);
    OCR1C = PWM_SPEED_TURN_LFT;
    OCR1B = PWM_SPEED_TURN_RGHT;
}
// --------------------------------------------------------------


// --------------------------------------------------------------
// GO BACKWARDS
// --------------------------------------------------------------
void go_bwd(void)
{
    set(PORTB,3);
    set(PORTD,3);
    OCR1C = PWM_SPEED_FWD_LFT;
    OCR1B = PWM_SPEED_FWD_RGHT;
}
// --------------------------------------------------------------


// --------------------------------------------------------------
// GO FORWARD
// --------------------------------------------------------------
void go_fwd(void)
{
	clear(PORTB,3);
	clear(PORTD,3);
	OCR1C = PWM_SPEED_FWD_LFT;
	OCR1B = PWM_SPEED_FWD_RGHT;
}
// --------------------------------------------------------------



// --------------------------------------------------------------
// CELEBRATE
// --------------------------------------------------------------
void celebrate(void)
{
	
}
// --------------------------------------------------------------




// --------------------------------------------------------------
// INTERUPTS
// --------------------------------------------------------------
ISR(TIMER4_OVF_vect)
{
    //m_green(ON);
    flag_timer = 1;
    stop_timer0();
}

ISR(TIMER0_OVF_vect)
{
    //m_green(ON);
    flag_turn = 0;
    stop_timer0();
    
}


/*ISR(INT2_vect)
 {
 //Read
 m_rf_read(buffer_rec,PACKET_LENGTH_SYSTEM);
 past_state = state;
 state = SYSTEM_STATE;
 //m_green(ON); // Indicator receiving from RF
 }*/


/*ISR(TIMER3_COMPA_vect)
 {
 m_red(ON);
 flag_timer = 1;
 }*/

// --------------------------------------------------------------
