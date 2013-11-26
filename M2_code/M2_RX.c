//Receiving values from Wireless

//Includes & Constant defines
#include "m_general.h"
#include "m_bus.h"
#include "m_rf.h"
#include "m_usb.h"
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N_CLOCK 0
#define PACKET_LENGTH 17
#define REC_ADDRESS 0X60
#define CHANNEL 1

// --------------------------------------------------------------
#define USB 1               // If using the USB set to 1
// --------------------------------------------------------------


// --------------------------------------------------------------
// Global Variables
// --------------------------------------------------------------

// General variables
int flag = 0;                                                           // Flag to check what is being received from MATLAB
volatile int flag_data = 0;                                             // Flag to check when we get data
char buffer_rec[PACKET_LENGTH] = {0};                                   // Input buffer from other M2
int position[2] = {0,0};

int robot_x, robot_y, robot_yaw;
int status_go_to_goal, dir_x, dir_y, dir_angle, dist_goal;


void send_to_MATLAB(void);
void init_M2(void);

long value_received;


// --------------------------------------------------------------
//  MAIN
// --------------------------------------------------------------
int main(void)
{
    init_M2();
    
	//Enable interruptions
	sei();
	
	//Main loop
	while (1)
	{
		//Receiving commands
		if (flag_data == 1)
		{
            int received_data = buffer_rec[0];
            
            if (received_data == 2) {
                robot_x = (int)buffer_rec[1];
                robot_y = (int)buffer_rec[2];
                robot_yaw = (int)buffer_rec[3]*256 + (int)buffer_rec[4];
                
            }
            else if (received_data == 7) {
                status_go_to_goal = 1;//(int)buffer_rec[1];
                dir_x = (int)buffer_rec[2]*256 + (int)buffer_rec[3];
                dir_y = (int)buffer_rec[4]*256 + (int)buffer_rec[5];
                dir_angle = (int)buffer_rec[6]*256 + (int)buffer_rec[7];
                dist_goal = (int)buffer_rec[8]*256 + (int)buffer_rec[9];
                
            }
            else {
                
            }
            
			//Reset the flag
			flag_data = 0;
            m_green(OFF);
			
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
	//Read
	m_rf_read(buffer_rec,PACKET_LENGTH);
	flag_data = 1;
	m_green(ON); // Indicator receiving from RF
}
// --------------------------------------------------------------




// --------------------------------------------------------------
// Initialize M2
// --------------------------------------------------------------
void init_M2(void){
    m_red(ON);
    
	//Set the clock system prescaler
	m_clockdivide(N_CLOCK);
    
    //Initialize bus
    m_bus_init();
    
	if (USB) {                          // If I am using a USB then
        m_usb_init();                   // Initialize USB and
        while(!m_usb_isconnected());    // wait for a connection
    }
	
	//Open the channel
	m_rf_open(CHANNEL,REC_ADDRESS,PACKET_LENGTH);
	
	//Turn off the LEDs
	m_red(OFF);
    m_green(OFF);
    
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
                
				
                if (flag == 5){                 // If MATLAB needs the Position and Orientation data send them ASAP
                    m_usb_rx_flush();           // Flush the RX buffer
                    
                    int i;
                    for (i = 0; i < PACKET_LENGTH; i++) {
                        //m_usb_tx_char(buffer_rec[i]);      // x1 , y1 , x2 , y2 , x3 , y3
                        m_usb_tx_int(position[i]);      // x1 , y1 , x2 , y2 , x3 , y3
                        m_usb_tx_string("\n");
                    }
                    m_usb_tx_push();            // Send the TX buffer to MATLAB
                    
                    flag = 0;                   // Reset the flag that we haven't received anything
                }
				
                
                else if (flag == 6){                 // If MATLAB needs the Position and Orientation data send them ASAP
                    m_usb_rx_flush();           // Flush the RX buffer
                    
                    m_usb_tx_int(robot_x);      // x1
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot_y);      // x1
                    m_usb_tx_string("\n");
                    m_usb_tx_int(robot_yaw);      // x1
                    m_usb_tx_string("\n");
                    m_usb_tx_push();            // Send the TX buffer to MATLAB
                    
                    flag = 0;                   // Reset the flag that we haven't received anything
                    
                }
				
                else if (flag == 7){                 // If MATLAB needs the General Variables
                    m_usb_rx_flush();           // Flush the RX buffer
                    
                    m_usb_tx_int(status_go_to_goal);      // Status of the state GO_TO_GOAL
                    m_usb_tx_string("\n");
                    m_usb_tx_int(dir_x);      // Direction x towards goal
                    m_usb_tx_string("\n");
                    m_usb_tx_int(dir_y);      // Direciton y towards goal
                    m_usb_tx_string("\n");
                    m_usb_tx_int(dir_angle);      // Direction to goal
                    m_usb_tx_string("\n");
                    m_usb_tx_int(dist_goal);      // Distance to goal
                    m_usb_tx_string("\n");
                    m_usb_tx_push();            // Send the TX buffer to MATLAB
                    
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



