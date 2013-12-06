// --------------------------------------------------------------
//
// Receiving values wirelessly from all robots and send data
// to MATLAB for debugging through USB
//
// --------------------------------------------------------------



// --------------------------------------------------------------
// Includes & Constant defines
// --------------------------------------------------------------
#include "m_general.h"
#include "m_bus.h"
#include "m_rf.h"
#include "m_usb.h"
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N_CLOCK 0
#define PACKET_LENGTH 41
#define REC_ADDRESS 0X60
#define CHANNEL 1
#define USB 1               // If using the USB set to 1


#define ANALOG 1               // If receiving the ADC values from RF set to 1

// --------------------------------------------------------------
// Global Variables
// --------------------------------------------------------------
int flag = 0;                                                           // Flag to check what is being received from MATLAB
volatile int flag_data = 0;                                             // Flag to check when we get data
char buffer_rec[PACKET_LENGTH] = {0};                                   // Input buffer from other M2

struct robots {             // Structure for variables of each robot
    int state;                  // Robot's current state
    int x;                      // x position on the ring
    int y;                      // y position on the ring
    int yaw;                    // Robot's orientation
    int status_go_to_goal;      // Status of state GO_TO_GOAL
    int dir_x;                  // Direction x towards goal
    int dir_y;                  // Direction y towards goal
    int dir_angle;              // Angle towards goal
    int dist_goal;              // Distance to goal
    int camera_x;               // Camera offset x in pixels
    int camera_y;               // Camera offset y in pixels
    int commands_var;           // Various commands for robots
    int diff_theta;             // Difference of current orientation and angle towards goal
    int OCR1B_PWM;              // PWM for Left Motor
    int OCR1C_PWM;              // PWM for Right Motor
    int bank;                   // Direction of turning of robot towards goal
    int PT_LO;                  // ADC value for Phototransistor at Left Outside
    int PT_LI;                  // ADC value for Phototransistor at Left Inside
    int PT_RI;                  // ADC value for Phototransistor at Right Inside
    int PT_RO;                  // ADC value for Phototransistor at Right Outside
    int PT_BR;                  // ADC value for Phototransistor at Back Right
    int PT_BL;                  // ADC value for Phototransistor at Back Left
    int PT_PUCK;                // ADC value for Phototransistor for Have Puck
} robot1, robot2, robot3;   // Robot 1,2,3 variables


// --------------------------------------------------------------
// Function Prototypes
// --------------------------------------------------------------
void send_to_MATLAB(void);
void init_M2(void);

int temp = 0;



// --------------------------------------------------------------
//  MAIN
// --------------------------------------------------------------
int main(void)
{
    init_M2();      // Initialize M2
	sei();          // Enable interruptions
	
	//Main loop
	while (1)
	{
		if (flag_data == 1)     // If we received from RF from robots
		{
            cli();              // Disable interupts to read the values without interrupting
            
            int robot_num = buffer_rec[0];      // Check from which robot we received
            
            if (robot_num == 1) {               // If we received from robot 1
                robot1.state = (int)buffer_rec[1];
                robot1.x = (int)buffer_rec[2];
                robot1.y = (int)buffer_rec[3];
                robot1.yaw = (int)buffer_rec[4]*128 + (int)buffer_rec[5];
                robot1.status_go_to_goal = (int)buffer_rec[6];
                robot1.dir_x = (int)buffer_rec[7]*128 + (int)buffer_rec[8];
                robot1.dir_y = (int)buffer_rec[9]*128 + (int)buffer_rec[10];
                robot1.dir_angle = (int)buffer_rec[11]*128 + (int)buffer_rec[12];
                robot1.dist_goal = (int)buffer_rec[13]*128 + (int)buffer_rec[14];
                robot1.camera_x = (int)buffer_rec[15]*128 + (int)buffer_rec[16];
                robot1.camera_y = (int)buffer_rec[17]*128 + (int)buffer_rec[18];
                robot1.commands_var = (int)buffer_rec[19];
                robot1.diff_theta = (int)buffer_rec[20]*128 + (int)buffer_rec[21];
                robot1.OCR1B_PWM = (int)buffer_rec[22]*128 + (int)buffer_rec[23];
                robot1.OCR1C_PWM = (int)buffer_rec[24]*128 + (int)buffer_rec[25];
                robot1.bank = (int)buffer_rec[26];
                
                if (ANALOG) {
                    robot1.PT_LO = (int)buffer_rec[27]*128 + (int)buffer_rec[28];
                    robot1.PT_LI = (int)buffer_rec[29]*128 + (int)buffer_rec[30];
                    robot1.PT_RI = (int)buffer_rec[31]*128 + (int)buffer_rec[32];
                    robot1.PT_RO = (int)buffer_rec[33]*128 + (int)buffer_rec[34];
                    robot1.PT_BR = (int)buffer_rec[35]*128 + (int)buffer_rec[36];
                    robot1.PT_BL = (int)buffer_rec[37]*128 + (int)buffer_rec[38];
                    robot1.PT_PUCK = (int)buffer_rec[39]*128 + (int)buffer_rec[40];
                }
                
            }
            
            else if (robot_num == 2) {          // If we received from robot 2
                robot2.state = (int)buffer_rec[1];
                robot2.x = (int)buffer_rec[2];
                robot2.y = (int)buffer_rec[3];
                robot2.yaw = (int)buffer_rec[4]*128 + (int)buffer_rec[5];
                robot2.status_go_to_goal = (int)buffer_rec[6];
                robot2.dir_x = (int)buffer_rec[7]*128 + (int)buffer_rec[8];
                robot2.dir_y = (int)buffer_rec[9]*128 + (int)buffer_rec[10];
                robot2.dir_angle = (int)buffer_rec[11]*128 + (int)buffer_rec[12];
                robot2.dist_goal = (int)buffer_rec[13]*128 + (int)buffer_rec[14];
                robot2.camera_x = (int)buffer_rec[15]*128 + (int)buffer_rec[16];
                robot2.camera_y = (int)buffer_rec[17]*128 + (int)buffer_rec[18];
                robot2.commands_var = (int)buffer_rec[19];
                robot2.diff_theta = (int)buffer_rec[20]*128 + (int)buffer_rec[21];
                robot2.OCR1B_PWM = (int)buffer_rec[22]*128 + (int)buffer_rec[23];
                robot2.OCR1C_PWM = (int)buffer_rec[24]*128 + (int)buffer_rec[25];
                robot2.bank = (int)buffer_rec[26];
                
                if (ANALOG) {
                    robot2.PT_LO = (int)buffer_rec[27]*128 + (int)buffer_rec[28];
                    robot2.PT_LI = (int)buffer_rec[29]*128 + (int)buffer_rec[30];
                    robot2.PT_RI = (int)buffer_rec[31]*128 + (int)buffer_rec[32];
                    robot2.PT_RO = (int)buffer_rec[33]*128 + (int)buffer_rec[34];
                    robot2.PT_BR = (int)buffer_rec[35]*128 + (int)buffer_rec[36];
                    robot2.PT_BL = (int)buffer_rec[37]*128 + (int)buffer_rec[38];
                    robot2.PT_PUCK = (int)buffer_rec[39]*128 + (int)buffer_rec[40];
                }
                
            }
            
            else if (robot_num == 3) {          // If we received from robot 3
                robot3.state = (int)buffer_rec[1];
                robot3.x = (int)buffer_rec[2];
                robot3.y = (int)buffer_rec[3];
                robot3.yaw = (int)buffer_rec[4]*128 + (int)buffer_rec[5];
                robot3.status_go_to_goal = (int)buffer_rec[6];
                robot3.dir_x = (int)buffer_rec[7]*128 + (int)buffer_rec[8];
                robot3.dir_y = (int)buffer_rec[9]*128 + (int)buffer_rec[10];
                robot3.dir_angle = (int)buffer_rec[11]*128 + (int)buffer_rec[12];
                robot3.dist_goal = (int)buffer_rec[13]*128 + (int)buffer_rec[14];
                robot3.camera_x = (int)buffer_rec[15]*128 + (int)buffer_rec[16];
                robot3.camera_y = (int)buffer_rec[17]*128 + (int)buffer_rec[18];
                robot3.commands_var = (int)buffer_rec[19];
                robot3.diff_theta = (int)buffer_rec[20]*128 + (int)buffer_rec[21];
                robot3.OCR1B_PWM = (int)buffer_rec[22]*128 + (int)buffer_rec[23];
                robot3.OCR1C_PWM = (int)buffer_rec[24]*128 + (int)buffer_rec[25];
                robot3.bank = (int)buffer_rec[26];
                
                if (ANALOG) {
                    robot3.PT_LO = (int)buffer_rec[27]*128 + (int)buffer_rec[28];
                    robot3.PT_LI = (int)buffer_rec[29]*128 + (int)buffer_rec[30];
                    robot3.PT_RI = (int)buffer_rec[31]*128 + (int)buffer_rec[32];
                    robot3.PT_RO = (int)buffer_rec[33]*128 + (int)buffer_rec[34];
                    robot3.PT_BR = (int)buffer_rec[35]*128 + (int)buffer_rec[36];
                    robot3.PT_BL = (int)buffer_rec[37]*128 + (int)buffer_rec[38];
                    robot3.PT_PUCK = (int)buffer_rec[39]*128 + (int)buffer_rec[40];
                }
                
            }
            
			flag_data = 0;      // Reset the flag that done receiving form RF
            m_green(OFF);       // Turn off indicator of receiving from RF
            sei();              // Enable back interrupts
			
		}
        
        send_to_MATLAB();       // Communicate with MATLAB
	}
}
// --------------------------------------------------------------


// --------------------------------------------------------------
// Interruption handler for the data received from the module (D2)
// --------------------------------------------------------------
ISR(INT2_vect)
{
	m_rf_read(buffer_rec,PACKET_LENGTH);        // Read buffer from RF from robots
	flag_data = 1;                              // Set the flag that we received
	m_green(ON);                                // Indicator receiving from RF
}
// --------------------------------------------------------------




// --------------------------------------------------------------
// Initialize M2
// --------------------------------------------------------------
void init_M2(void){
    m_red(ON);                          // Indicator for initilization
    
	m_clockdivide(N_CLOCK);             // Set the clock system prescaler
    
    m_bus_init();                       // Initialize bus
    
	if (USB) {                          // If I am using a USB then
        m_usb_init();                   // Initialize USB and
        while(!m_usb_isconnected());    // wait for a connection
    }
	
	m_rf_open(CHANNEL,REC_ADDRESS,PACKET_LENGTH);   // Open the RF channel
	
	m_red(OFF);                         // Turn off the LEDs
    m_green(OFF);                       // ^
    
}
// --------------------------------------------------------------








// --------------------------------------------------------------
// Send Data through USB
// --------------------------------------------------------------
void send_to_MATLAB(){
    if (USB){
        // Send/Receive data through USB whenever MATLAB asks for it
        // Check if there is something in the RX buffer and how many bytes it is
        int data_to_read = m_usb_rx_available();
        if(data_to_read)
        {
            
            if (flag == 0) {                    // If we haven't received anything before
                while(!m_usb_rx_available());   // Wait to receive instructions
                
                
                flag = m_usb_rx_char();         // Get the first byte as what to do next
                
                if (flag == 1){                 // If MATLAB needs the Position and Orientation data send them ASAP
                    m_usb_rx_flush();           // Flush the RX buffer
                    
                    m_usb_tx_int(robot1.x);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot1.y);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot1.yaw);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot1.status_go_to_goal);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot1.dir_x);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot1.dir_y);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot1.dir_angle);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot1.dist_goal);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot1.camera_x);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot1.camera_y);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot1.commands_var);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot1.diff_theta);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot1.bank);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot1.OCR1B_PWM);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot1.OCR1C_PWM);
                    m_usb_tx_string("\n");
                    
                    flag = 0;                   // Reset the flag that we haven't received anything
                }
                
                else if (flag == 4){                 // If MATLAB needs the Position and Orientation data send them ASAP
                    m_usb_rx_flush();           // Flush the RX buffer
                    
                    m_usb_tx_int(robot1.PT_LO);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot1.PT_LI);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot1.PT_RI);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot1.PT_RO);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot1.PT_BL);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot1.PT_BR);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot1.PT_PUCK);
                    m_usb_tx_string("\n");
                    
                    flag = 0;                   // Reset the flag that we haven't received anything
                }
                
                
                else if (flag == 2){                 // If MATLAB needs the Position and Orientation data send them ASAP
                    m_usb_rx_flush();           // Flush the RX buffer
                    
                    m_usb_tx_int(robot2.x);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot2.y);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot2.yaw);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot2.status_go_to_goal);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot2.dir_x);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot2.dir_y);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot2.dir_angle);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot2.dist_goal);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot2.camera_x);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot2.camera_y);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot2.commands_var);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot2.diff_theta);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot2.bank);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot2.OCR1B_PWM);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot2.OCR1C_PWM);
                    m_usb_tx_string("\n");
                    
                    flag = 0;                   // Reset the flag that we haven't received anything
                }
                
                else if (flag == 5){                 // If MATLAB needs the Position and Orientation data send them ASAP
                    m_usb_rx_flush();           // Flush the RX buffer
                    
                    m_usb_tx_int(robot2.PT_LO);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot2.PT_LI);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot2.PT_RI);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot2.PT_RO);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot2.PT_BL);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot2.PT_BR);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot2.PT_PUCK);
                    m_usb_tx_string("\n");
                    
                    flag = 0;                   // Reset the flag that we haven't received anything
                }
                
                else if (flag == 3){                 // If MATLAB needs the Position and Orientation data send them ASAP
                    m_usb_rx_flush();           // Flush the RX buffer
                    
                    m_usb_tx_int(robot3.x);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot3.y);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot3.yaw);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot3.status_go_to_goal);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot3.dir_x);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot3.dir_y);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot3.dir_angle);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot3.dist_goal);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot3.camera_x);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot3.camera_y);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot3.commands_var);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot3.diff_theta);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot3.bank);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot3.OCR1B_PWM);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot3.OCR1C_PWM);
                    m_usb_tx_string("\n");
                    
                    flag = 0;                   // Reset the flag that we haven't received anything
                }
                
                else if (flag == 6){                 // If MATLAB needs the Position and Orientation data send them ASAP
                    m_usb_rx_flush();           // Flush the RX buffer
                    
                    m_usb_tx_int(robot3.PT_LO);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot3.PT_LI);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot3.PT_RI);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot3.PT_RO);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot3.PT_BL);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot3.PT_BR);
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot3.PT_PUCK);
                    m_usb_tx_string("\n");
                    
                    flag = 0;                   // Reset the flag that we haven't received anything
                }
                
            }
            
            
            else {
                // More coming up here
            }
            
        }
    }
}
// --------------------------------------------------------------



