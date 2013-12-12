//Sending values using Wireless

//Includes & Constant defines
#include "m_general.h"
#include "m_bus.h"
#include "m_rf.h"
#include "m_wii.h"
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
#define PACKET_LENGTH_SYSTEM 10
#define SEN_ADDRESS_SYSTEM 0xDA
#define ALEX_ADDRESS_SYSTEM 0x42
#define CHANNEL_SYSTEM 1
#define GO_TO_GOAL 1
#define GO_TO_GOAL_CURVED 2
#define GO_TO_WPT 80
#define FIND_PUCK 3
#define GO_TO_GOAL_WITH_PUCK 5
#define SYSTEM_STATE 99
#define INITIAL_STATE 0
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
#define THRESHOLD_ANGLE_GOAL 35
#define THRESHOLD_DIST_GOAL 0
#define PWM_SPEED_TURN_LFT 2300     // 2300 spyros          2800 Alex
#define PWM_SPEED_TURN_RGHT 2400    // 2200 spyros          2800 Alex
//#define PWM_SPEED_FWD_LFT 393
//#define PWM_SPEED_FWD_RGHT 380
#define PWM_SPEED_FWD_LFT 3000      //3000 fast spyros 2600 slow spyros     Alex 3000
#define PWM_SPEED_FWD_RGHT 3100    //2900 fast spyros 2500 slow spyros     Alex 3000
#define PWM_MIN_LEFT 2200
#define PWM_MIN_RGHT 2200
#define TURNING_ANGLE 180.0

#define TIME_STOP 1000

#define THRESHOLD_PUCK_CENTER_OUTSIDE 0.25
#define THRESHOLD_PUCK_CENTER_INSIDE 0.35
#define THRESHOLD_PUCK_CENTER_INSIDE2 0.5
#define THRESHOLD_SWITCH_EXTERIOR 40
#define	THRESHOLD_PUCK_NOT_FIND 50

//#define PWM_SPEED_CIRCLE_LFT 2350
//#define RATIO_TURNING_LFT 0.80
//#define PWM_SPEED_CIRCLE_RGHT 2300
//#define RATIO_TURNING_RGHT 0.80

#define PWM_SPEED_CIRCLE_LFT 2300
#define RATIO_TURNING_LFT 0.83
#define PWM_SPEED_CIRCLE_RGHT 2300
#define RATIO_TURNING_RGHT 0.83
#define PWM_SPEED_AFTER_GET_PUCK_LEFT 3800
#define PWM_SPEED_AFTER_GET_PUCK_RIGHT 3800


#define Kp_move_puck 5     // Kp for moving around or towards puck
#define Kp_move_with_puck 0.01    //Kp after getting the puck (less aggressive) 
#define Kp_move 4
#define Kp_turn 8 //Kp for turning
//#define Kd 500
//#define time 0.002
#define NOT_SEE_PUCK 300



int diff_error = 0, cur_error = 0, prev_error = 0;
float TARGETS_X[2]={0.0};
float TARGETS_Y[2]={0.0};
int theta_robot = 0;

//Variable for receiving data
char buffer_rec[PACKET_LENGTH_SYSTEM] = {0};


void calculate_diff_theta(float theta_des, float* err_theta, int* dir_to_turn);

//Function prototypes
void set_timer1(void);
//void set_timer3(void);
void set_timer4(void);


void init_analog (void);
void get_analog_val (int id);
void stop_motor(void);
void turn_right(void);
void turn_left(void);
void turn_right_puck(int scale_turn);
void turn_left_puck(int scale_turn);
void go_fwd(void);
void go_bwd(void);
void circle_left(void);
void circle_right(void);
void move_robot_to_puck(int dir, int diff);
void turn_robot(float theta, int dir, float diff);
void move_robot(float theta, int dir);
void move_robot_after_get_puck(int dir, int diff);
void turnOnBlueLED(void);
void turnOffBlueLED(void);
void celebrate(void);

//Variable used to check timer
volatile int flag_timer = 0;
volatile int flag_system  = 0;

//Variable for states
int state = INITIAL_STATE; //CHANGE TO SYSTEM STATE?????????????????????????????
//int past_state = INITIAL_STATE;

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
	int have_puck = 0;
    
    //Variables debugging
    float dir_x = 0;
    float dir_y = 0;
    float dir_angle = 0;
    float dist_goal = 0;
    float diff_theta = 0;
    int bank = 22;
	int cam_X = 0, cam_Y = 0, commands_var = 0;
	int timer_switch = 0;
	int send_debug = 1;
	
	int circle_started_before = 0;
    
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
    //set_timer3();                     // Set timer 3 for solenoid
    set_timer4();                       // Set timer 4 to every 0.1 s (10 Hz) to send data
    init_analog();                      // Set the ADC System
    
    char aux = 0;                       // Initialize wii camera
	while(!aux)                         // ^
    {                                   // ^
        aux = m_wii_open();             // ^
    };                                  // ^
    
	m_rf_open(CHANNEL_SYSTEM,ALEX_ADDRESS_SYSTEM,PACKET_LENGTH_SYSTEM); 
    
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
    TARGETS_X[1] = GOAL_A_POS_X - 15;
    TARGETS_Y[1] = GOAL_A_POS_Y;
    
    int TARGET_NUM = 0;
    // --------------------------------------------------------------
       
    
    // --------------------------------------------------------------
    // Main loop
    // --------------------------------------------------------------
    while (1)
    {
		// --------------------------------------------------------------
		// SYSTEM COMMAND CHECK
		// --------------------------------------------------------------
		if (flag_system == 1)
		{
			m_rf_read(buffer_rec,PACKET_LENGTH_SYSTEM);
			state = SYSTEM_STATE;
			flag_system = 0;
		}
	
		 //--------------------------------------------------------------
         //SEND COMMANDS
         //--------------------------------------------------------------
        if (flag_timer == 1)
		{
			m_green(ON);	
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
			// STATE COMMANDS
			// --------------------------------------------------------------
			switch (state)
			{
				long stop_counter = 0;
	
				case GO_TO_WPT:
	
				//m_green(ON);
	
	
				if (PT1_left_outside > NOT_SEE_PUCK || PT2_left_inside > NOT_SEE_PUCK || PT3_right_inside > NOT_SEE_PUCK || PT4_right_outside > NOT_SEE_PUCK || PT5_back_right > NOT_SEE_PUCK || PT6_back_left > NOT_SEE_PUCK)
				{
					state = FIND_PUCK;
					break;
				}
	
				dir_x = TARGETS_X[TARGET_NUM]-x_robot;
				dir_y = TARGETS_Y[TARGET_NUM]-y_robot;
				dir_angle = atan2(-dir_x,dir_y)*180/M_PI;
	
				calculate_diff_theta(dir_angle,&diff_theta, &bank);
	
	
				cur_error = diff_theta;
				diff_error = cur_error - prev_error;
				prev_error = cur_error;
	
				if (status_go_to_goal == 0)
				{
					dist_goal = sqrt((x_robot-TARGETS_X[TARGET_NUM])*(x_robot-TARGETS_X[TARGET_NUM])+(y_robot-TARGETS_Y[TARGET_NUM])*(y_robot-TARGETS_Y[TARGET_NUM]));
					if (dist_goal < THRESHOLD_DIST_GOAL)
					{
						status_go_to_goal = 2;
					}
		
		
					//move_robot(diff_theta,dist_goal,bank);
					else if (diff_theta < THRESHOLD_ANGLE_GOAL) {
						status_go_to_goal = 1;
					}
					else
					if (bank == 0)
					turn_left_puck(diff_theta);
					else
					turn_right_puck(diff_theta);
				}
	
				else if (status_go_to_goal == 1)
				{
					dist_goal = sqrt((x_robot-TARGETS_X[TARGET_NUM])*(x_robot-TARGETS_X[TARGET_NUM])+(y_robot-TARGETS_Y[TARGET_NUM])*(y_robot-TARGETS_Y[TARGET_NUM]));
					if (dist_goal < THRESHOLD_DIST_GOAL)
					{
						status_go_to_goal = 2;
						//set(PORTD,5);
			
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
						//set(PORTD,5);
					}
					else
					{
						//clear(PORTD,5);
						move_robot_to_puck(diff_theta,bank);
					}
		
				}
				else if (status_go_to_goal == 2){
		
					calculate_diff_theta(0,&diff_theta, &bank);
		
					status_go_to_goal = 3;
		
					if (diff_theta < THRESHOLD_ANGLE_GOAL)
					{
						status_go_to_goal = 3;
						//stop_motor();
						//m_wait(1000);
						//set(PORTD,5);
					}
					else
					{
						if (bank == 0)
						turn_left_puck(diff_theta);
						else
						turn_right_puck(diff_theta);
					}
		
				}
				else if (status_go_to_goal == 3)
				{
					//stop_motor();
					if (TARGET_NUM == 0) {
						TARGET_NUM = 1;
					}
					else {
						TARGET_NUM = 0;
					}
					status_go_to_goal = 0;
					state = FIND_PUCK;
				}
				break;
	
				// --------------------------------------------------------------
				// FIND PUCK STATE
				// --------------------------------------------------------------
				case FIND_PUCK:
				;
				////Check if we have the puck
				//if (PT7_have_puck > 250)
				//{
				//stop_motor();
				//state = GO_TO_GOAL_WITH_PUCK
				//status_go_to_goal = 0;
				//break;
				//}
				//
	
	
				//Check if we have the puck
				if (PT2_left_inside > 1010 &&  PT3_right_inside > 1010)
				{
					state = GO_TO_GOAL_WITH_PUCK;
					//turnOnBlueLED();
		
					status_go_to_goal = 0;
					break;
				}
	
	
	
				//if (status_go_to_goal == 0)
				//{
				//turnOffBlueLED();
	
				int max_lr = 0;
				int half_range = 0;
				int diff_PT_outside = 0;
	
	
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
	
	
				int max_pt_inside = 0;
				if (PT2_left_inside >= PT3_right_inside)
				max_pt_inside = PT2_left_inside;
				else
				max_pt_inside = PT3_right_inside;
	
				int max_pt_outside = 0;
				if (PT1_left_outside >= PT4_right_outside)
				max_pt_outside = PT1_left_outside;
				else
				max_pt_outside = PT4_right_outside;
	
				int max_pt_backs = 0;
				if (PT5_back_right >= PT6_back_left)
				max_pt_backs = PT5_back_right;
				else
				max_pt_backs = PT6_back_left;
	
	
	
				//                // If Insides see the puck
				if (PT1_left_outside < NOT_SEE_PUCK && PT2_left_inside < NOT_SEE_PUCK && PT3_right_inside < NOT_SEE_PUCK && PT4_right_outside < NOT_SEE_PUCK && PT5_back_right < NOT_SEE_PUCK && PT6_back_left < NOT_SEE_PUCK)
				{
					//turnOffBlueLED();
					state = GO_TO_WPT;
					break;
				}
	
				if (((((PT2_left_inside+PT3_right_inside)/2.0) >= 200)) && ((PT1_left_outside <= 700) && (PT4_right_outside <= 700)))
				{
					//turnOnBlueLED();
					status_go_to_goal = 1;
					int diff_PT_inside = abs(PT2_left_inside-PT3_right_inside);
					if (PT2_left_inside >= PT3_right_inside)
					{
						move_robot_to_puck(1,diff_PT_inside);
			
					}
					else
					{
						move_robot_to_puck(0,diff_PT_inside);
					}
				}
				// If Insides do not see the puck
				else if ( (( (PT2_left_inside+PT3_right_inside)/2.0) < 200) && ((PT1_left_outside > 100) || (PT4_right_outside > 100) || (PT6_back_left > 100) || (PT5_back_right > 100)) )
				{
					//turnOffBlueLED();
					status_go_to_goal = 0;
					//Case where the puck is in front
					if (half_range == 0)
					{
						if (PT1_left_outside >= PT4_right_outside)
						{
							turn_left();
						}
						else
						turn_right();
					}
					else
					{
						if (PT6_back_left >= PT5_back_right)
						{
							turn_left();
						}
						else
						turn_right();
			
					}
				}
				else{
					state = GO_TO_WPT;
					//turnOffBlueLED();
				}
	
				break;
				// --------------------------------------------------------------
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
				}
				else
				{
					goal_pos_x = GOAL_B_POS_X;
					goal_pos_y = GOAL_B_POS_Y;
				}
				status_go_to_goal = 0;
				//                    state = GO_TO_GOAL_CURVED;
				state = FIND_PUCK;
				//state = GO_TO_WPT;
				break;
				// --------------------------------------------------------------
	
				// --------------------------------------------------------------
				// GO TO GOAL WITH PUCK
				// --------------------------------------------------------------
	
				case GO_TO_GOAL_WITH_PUCK:
	
					if ( PT2_left_inside < 900 || PT3_right_inside < 900 )
					{
						state = FIND_PUCK;
						status_go_to_goal = 0;
						break;
					}
	
					dist_goal = sqrt((x_robot-goal_pos_x)*(x_robot-goal_pos_x)+(y_robot-goal_pos_y)*(y_robot-goal_pos_y));
					if (dist_goal < THRESHOLD_DIST_GOAL)
					{
						status_go_to_goal = 0;
						state  = FIND_PUCK;
						break;
					}
	
					if (status_go_to_goal == 0)
					{
		
						dir_x = goal_pos_x-x_robot;
						dir_y = goal_pos_y-y_robot;
						dir_angle = atan2(-dir_x,dir_y)*180/M_PI;
		
						calculate_diff_theta(dir_angle,&diff_theta,&bank);
		
						if (diff_theta <20)
						{
							status_go_to_goal = 1;
							break;
						}
						else
						{
			
							if (bank == 0)
							{
								//turnOnBlueLED();
								circle_left();
								circle_started_before = 1;
							}
							else if (bank == 1)
							{
								//turnOffBlueLED();
								circle_right();
								circle_started_before = 1;
							}
			
						}
					}
					else if (status_go_to_goal == 1)
					{
						dir_x = goal_pos_x-x_robot;
						dir_y = goal_pos_y-y_robot;
						dir_angle = atan2(-dir_x,dir_y)*180/M_PI;
		
						calculate_diff_theta(dir_angle,&diff_theta,&bank);
		
						move_robot_after_get_puck(bank,diff_theta);
					}
					break;
	
				// --------------------------------------------------------------
				
				
				// --------------------------------------------------------------
				// SYSTEM STATE
				// --------------------------------------------------------------
				case SYSTEM_STATE:
					m_red(TOGGLE);
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
						//turnOnBlueLED();
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
					break; //go out of system case
	
				// --------------------------------------------------------------
				// BLUE LED STATE
				// --------------------------------------------------------------
				case BLUE_LED_STATE:
				stop_motor();
				turnOnBlueLED();
				m_wait(3000);
				turnOffBlueLED();
				state = STOP_STATE;
				break;
	
	
				// --------------------------------------------------------------
				// STOP STATE
				// --------------------------------------------------------------
				case STOP_STATE:
				//m_green(ON);
				turnOffBlueLED();
				stop_motor();
				break;
				// --------------------------------------------------------------
	
	
	
				// --------------------------------------------------------------
				// DEFAULT STATE
				// --------------------------------------------------------------
				default:
				stop_motor();
				break;
				//while(1)
				//{
				//m_red(TOGGLE);
				//m_green(TOGGLE);
				//m_wait(250);
				//}
				// --------------------------------------------------------------
			}		
			flag_timer = 0;        
		}                  
	}
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
//void turn_robot(float theta, int dir, float diff){
	//if (dir == 1) {             // Move with a right curve
		////OCR1C = PWM_SPEED_FWD_LFT;
		//OCR1B = (int)(PWM_MIN_RGHT+theta*Kp);// + diff*Kd);
		//OCR1C = (int)(PWM_MIN_RGHT+theta*Kp);// + diff*Kd);
		//
		//set(PORTB,3);
		//clear(PORTD,3);
	//}
	//else
	//{                      // Move with a left curve
		////OCR1B = PWM_SPEED_FWD_RGHT;
		//OCR1C = (int)(PWM_MIN_LEFT+theta*Kp);// + diff*Kd);;
		//OCR1B = (int)(PWM_MIN_LEFT+theta*Kp);// + diff*Kd);
		//
		//clear(PORTB,3);
		//set(PORTD,3);
		//
	//}
//}
// --------------------------------------------------------------

// --------------------------------------------------------------
// ORIENTATION CONTROLLER TO MOVE ROBOT FORWARD
// --------------------------------------------------------------
void move_robot(float theta, int dir){
	if (dir == 0) {             // Move with a left curve
		//OCR1C = PWM_SPEED_FWD_LFT;
		OCR1B = (int)(PWM_SPEED_FWD_LFT);
		OCR1C = (int)(PWM_SPEED_FWD_RGHT+theta*Kp_move);
		
	}
	else
	{                      // Move with a right curve
		OCR1B = (int)(PWM_SPEED_FWD_LFT+theta*Kp_move);
		OCR1C = (int)(PWM_SPEED_FWD_RGHT);
	}
	
	clear(PORTB,3);
	clear(PORTD,3);
}
// --------------------------------------------------------------

// --------------------------------------------------------------
// POSITION CONTROLLER TO MOVE ROBOT TOWARDS PUCK
// --------------------------------------------------------------
void move_robot_to_puck(int dir, int diff){
	if (dir == 1) {             // Move with a left curve
		//OCR1C = PWM_SPEED_FWD_LFT;
		OCR1B = (int)(PWM_SPEED_FWD_LFT);
		OCR1C = (int)(PWM_SPEED_FWD_RGHT+diff*Kp_move_puck);
		
	}
	else
	{                      // Move with a right curve
		OCR1B = (int)(PWM_SPEED_FWD_LFT+diff*Kp_move_puck);
		OCR1C = (int)(PWM_SPEED_FWD_RGHT);
	}
	
	clear(PORTB,3);
	clear(PORTD,3);
}
// --------------------------------------------------------------

// --------------------------------------------------------------
// POSITION CONTROLLER TO MOVE ROBOT TOWARDS PUCK
// --------------------------------------------------------------
void move_robot_after_get_puck(int dir, int diff){
	if (dir == 1) {             // Move with a left curve
		//OCR1C = PWM_SPEED_FWD_LFT;
		OCR1B = (int)(PWM_SPEED_AFTER_GET_PUCK_LEFT);
		OCR1C = (int)(PWM_SPEED_AFTER_GET_PUCK_RIGHT+diff*Kp_move_with_puck);
		
		clear(PORTB,3);
		clear(PORTD,3);
	}
	else
	{                      // Move with a right curve
		//OCR1B = PWM_SPEED_FWD_RGHT;
		OCR1B = (int)(PWM_SPEED_AFTER_GET_PUCK_LEFT+diff*Kp_move_with_puck);
		OCR1C = (int)(PWM_SPEED_AFTER_GET_PUCK_RIGHT);
		
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
// TURN LEFT FOR PUCK
// --------------------------------------------------------------
void turn_left_puck(int scale_turn)
{
    clear(PORTB,3);
    set(PORTD,3);
	OCR1C = (int)(PWM_MIN_LEFT+scale_turn*Kp_turn);
    OCR1B = (int)(PWM_MIN_RGHT+scale_turn*Kp_turn);
}
// --------------------------------------------------------------

// --------------------------------------------------------------
// TURN RIGHT PUCK
// --------------------------------------------------------------
void turn_right_puck(int scale_turn)
{
    set(PORTB,3);
    clear(PORTD,3);
    OCR1C = (int)(PWM_MIN_LEFT+scale_turn*Kp_turn);
    OCR1B = (int)(PWM_MIN_RGHT+scale_turn*Kp_turn);
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
	//OCR1C = PWM_SPEED_FWD_LFT;
	//OCR1B = PWM_SPEED_FWD_RGHT;
	OCR1C = PWM_SPEED_AFTER_GET_PUCK_LEFT;
	OCR1B = PWM_SPEED_AFTER_GET_PUCK_RIGHT;
}
// --------------------------------------------------------------

// --------------------------------------------------------------
// CIRCLE_LEFT
// --------------------------------------------------------------
void circle_left()
{
	clear(PORTB,3);
	clear(PORTD,3);
	OCR1B = PWM_SPEED_CIRCLE_LFT;
	OCR1C = PWM_SPEED_CIRCLE_LFT*RATIO_TURNING_LFT;
	//m_green(ON);
}

// --------------------------------------------------------------
// CIRCLE_RIGHT
// --------------------------------------------------------------
void circle_right()
{
	clear(PORTB,3);
	clear(PORTD,3);
	OCR1B = PWM_SPEED_CIRCLE_RGHT*RATIO_TURNING_RGHT;
	OCR1C = PWM_SPEED_CIRCLE_RGHT;
	//m_green(ON);
}

// --------------------------------------------------------------
// CELEBRATE
void celebrate(void)
{
	
}// --------------------------------------------------------------


// --------------------------------------------------------------
// INTERRUPTS
// --------------------------------------------------------------
ISR(TIMER4_OVF_vect)
{
	//m_green(ON);
	flag_timer = 1;
}

ISR(INT2_vect)
{
	//Read
	//past_state = state;
	//state = SYSTEM_STATE;
	flag_system = 1;
	flag_timer= 1;
	m_green(TOGGLE); // Indicator receiving from RF
}


/*ISR(TIMER3_COMPA_vect)
 {
 m_red(ON);
 flag_timer = 1;
 }*/

// --------------------------------------------------------------
