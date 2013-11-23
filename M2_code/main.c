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
#define REC_ADDRESS 0X47
#define CHANNEL 1

// --------------------------------------------------------------
#define USB 1               // If using the USB set to 1
// --------------------------------------------------------------


// --------------------------------------------------------------
// Global Variables
// --------------------------------------------------------------

// General variables
int flag = 0;                                                           // Flag to check what is being received from MATLAB
long powers[6] = {1,10,100,1000,10000,100000};                          // Precalculated powers of 10 for byte conversion
volatile int flag_data = 0;                                             // Flag to check when we get data
char buffer_rec[PACKET_LENGTH] = {0};                                   // Input buffer from other M2
int temp[6] = {-30,0,30,0,0,50};                                   // Input buffer from other M2
int position[2] = {0,0};
char received_data;

int robot_x, robot_y, robot_yaw;


void send_to_MATLAB(void);

long value_received;

//Main function
int main(void)
{
	char* end_ptr;
	
	
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

	
	//Enable interruptions
	sei();
	
	//Main loop
	while (1)
	{
		//Receiving commands
		if (flag_data == 1)
		{
            received_data = (int)buffer_rec[0];
            
            if (received_data == 2) {

                robot_x = (int)buffer_rec[1];
                if (robot_x==127) {
                    m_green(ON);
                }
                robot_y = (int)buffer_rec[2];
                robot_yaw = (int)buffer_rec[3]*128 + (int)buffer_rec[4];
                m_red(OFF);
            }
            else {
                
                
                int i;
                for (i=0; i<PACKET_LENGTH; i++) {
                    position[i] = (int)buffer_rec[i];
                }
            }
            
			//Reset the flag
			flag_data = 0;
			
		}
        
        
        send_to_MATLAB();       // Communicate with MATLAB
        
	}
}

//Interruption handler for the data received from the module (D2)
ISR(INT2_vect)
{
	//Read
	m_rf_read(buffer_rec,PACKET_LENGTH);
	flag_data = 1;
	m_red(ON);
}











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
                if (flag == 3){                 // If MATLAB needs the Position and Orientation data send them ASAP
                    m_usb_rx_flush();           // Flush the RX buffer
                    
                    m_usb_tx_long(value_received);
                    m_usb_tx_string("\n");
                    m_usb_tx_push();            // Send the TX buffer to MATLAB
                    
                    flag = 0;                   // Reset the flag that we haven't received anything
                    
                }
                
                
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
                
                if (flag == 6){                 // If MATLAB needs the Position and Orientation data send them ASAP
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
                
                
            }
            
            /*
             else {
             // More coming up here
             }
             */
        }
    }
}
// --------------------------------------------------------------



