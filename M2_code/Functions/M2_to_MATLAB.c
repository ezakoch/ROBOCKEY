/* Name: M2_to_MATLAB.c
 * Author: Spyridon Karachalios
 */

#include <avr/io.h>
#include "m_general.h"
#include "m_usb.h"
#include "m_bus.h"
#include "m_imu.h"
#include "math.h"
#include <stdio.h>

// --------------------------------------------------------------
#define USB 0               // If using the USB set to 1
// --------------------------------------------------------------

#define F_M2 16000000       // Frequency of the M2

// --------------------------------------------------------------
// Prototype for functions
// --------------------------------------------------------------
void init(void);
void send_to_MATLAB(void);
// --------------------------------------------------------------



// --------------------------------------------------------------
// Global Variables
// --------------------------------------------------------------

// General variables
int flag = 0;                                                           // Flag to check what is being received from MATLAB
long powers[6] = {1,10,100,1000,10000,100000};                          // Precalculated powers of 10 for byte conversion







// --------------------------------------------------------------
// MAIN
// --------------------------------------------------------------
int main(void)
{
    // Initialize the M2
    init();
    
    // Enable Interupts
    sei();
    
    /* Main Loop... Runs forever.. */
    while (true) {
        
        send_to_MATLAB();       // Communicate with MATLAB
    
    }
    
    return 0;       // If for some reason it reaches here exit
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
                if (flag == 3){                 // If MATLAB needs the IMU raw data send them ASAP
                    m_usb_rx_flush();           // Flush the RX buffer
                    unsigned char ret = m_imu_raw(&raw_data[0]);    // Check if IMU returned anything
                    if (ret) {
                        // Put the IMU data into the TX buffer seperated by "string terminator"
                        int kk;
                        for (kk = 0; kk<6; kk++) {
                            m_usb_tx_int(raw_data[kk]);
                            m_usb_tx_string("\n");
                        }
                        
                        // Send the TX buffer to MATLAB
                        m_usb_tx_push();
                    }
                    flag = 0;       // Reset the flag that we haven't received anything
                }
                else if (flag == 6){        // If MATLAB needs the filtered angle computed in the M2 send it ASAP
                    m_usb_rx_flush();       // Flush the RX buffer
                    
                    // Convert float into string to send the FILTERED IMU ANGLE
                    char b[6] = {0,0,0,0,0,0};
                    sprintf(b, "%d.%02u", (int)imu_angle_filtered, (unsigned int)(((imu_angle_filtered-((int)imu_angle_filtered)) * 100)));
                    int i;
                    for (i=0; i<6; i++) {
                        m_usb_tx_char(b[i]);
                    }
                    m_usb_tx_string("\n");      // Add "line terminator" to indicate end of transmission
                    
                    // Convert float into string to send the ANGLE of the ACCELEROMETER
                    sprintf(b, "%d.%02u", (int)imu_angle_accel, (unsigned int)(((imu_angle_accel-((int)imu_angle_accel)) * 100)));
                    
                    for (i=0; i<6; i++) {
                        m_usb_tx_char(b[i]);
                    }
                    m_usb_tx_string("\n");      // Add "line terminator" to indicate end of transmission
                    
                    // Convert float into string to send the ANGLE of the GYROSCOPE
                    sprintf(b, "%d.%02u", (int)imu_angle_gyro, (unsigned int)(((imu_angle_gyro-((int)imu_angle_gyro)) * 100)));
                
                    for (i=0; i<6; i++) {
                        m_usb_tx_char(b[i]);
                    }
                    m_usb_tx_string("\n");      // Add "line terminator" to indicate end of transmission
                    

                    m_usb_tx_push();            // Send the TX buffer to MATLAB
                    
                    flag = 0;                   // Reset the flag that we haven't received anything
                }
                else if (flag == 8) {   // Send the OCR1B / PID Control
                    m_usb_rx_flush();        // Flush the RX buffer just in case we missed something
                    
                    // Send the PID Control to MATLAB
                    m_usb_tx_long(OCR1B);
                    m_usb_tx_string("\n");  // Add "line terminator" to indicate end of transmission
                    
                    flag = 0;   // Reset the flag that we haven't received anything
                }
            }
            
            
            else if (flag == 2) {   // If MATLAB sent the KP gain of the PID controller
                // Read the value and convert it to long
                int i;
                kp = 0;     // Reset kp to recalculate
                for(i=data_to_read-2; i>=0; i--){
                    while(!m_usb_rx_available());
                    kp = kp + powers[i]*(m_usb_rx_char()-'0');
                }
                
                // Flush the RX buffer just in case we missed something
                m_usb_rx_flush();
                
                // Echo the received KP back to MATLAB
                m_usb_tx_long(kp);
                m_usb_tx_string("\n");      // Add "line terminator" to indicate end of transmission
                
                flag = 0;   // Reset the flag that we haven't received anything
            }
            
            
            else if (flag == 4) {   // If MATLAB sent the KI gain of the PID controller
                // Read the value and convert it to long
                int i;
                ki = 0;     // Reset ki to recalculate
                for(i=data_to_read-2; i>=0; i--){
                    while(!m_usb_rx_available());
                    ki = ki + powers[i]*(m_usb_rx_char()-'0');
                }
                
                // Flush the RX buffer just in case we missed something
                m_usb_rx_flush();
                
                // Echo the received KI back to MATLAB
                m_usb_tx_long(ki);
                m_usb_tx_string("\n");  // Add "line terminator" to indicate end of transmission
                
                flag = 0;   // Reset the flag that we haven't received anything
            }
            
            
            else if (flag == 5) {   // If MATLAB sent the KD gain of the PID controller
                // Read the value and convert it to long
                int i;
                kd = 0;     // Reset kd
                for(i=data_to_read-2; i>=0; i--){
                    while(!m_usb_rx_available());
                    kd = kd + powers[i]*(m_usb_rx_char()-'0');
                }
                
                // Flush the RX buffer just in case we missed something
                m_usb_rx_flush();
                
                // Echo the received KD back to MATLAB
                m_usb_tx_long(kd);
                m_usb_tx_string("\n");  // Add "line terminator" to indicate end of transmission
                
                flag = 0;   // Reset the flag that we haven't received anything
            }
            
            
            else if (flag == 7) {   // If MATLAB read and sent the OCR1B for the PWM of motors
                // Read the value and convert it to long
                int i;
                int temp = 0;
                for(i=data_to_read-2; i>=0; i--){
                    while(!m_usb_rx_available());
                    temp = temp + powers[i]*(m_usb_rx_char()-'0');
                }
                OCR1B = temp;
                // Flush the RX buffer just in case we missed something
                m_usb_rx_flush();
                
                // Echo the received KD back to MATLAB
                m_usb_tx_long(OCR1B);
                m_usb_tx_string("\n");  // Add "line terminator" to indicate end of transmission
                
                flag = 0;   // Reset the flag that we haven't received anything
            }
            
            
            else {
                // More coming up here
            }
        }
    }
}
// --------------------------------------------------------------

// --------------------------------------------------------------
// Initialize M2
// --------------------------------------------------------------
void init(void){
    
    // Initialize RED LED as on for the Initialization of M2
    m_red(ON);
    
    // Set the System Clock at 16MHz
    m_clockdivide(0);
    
    
    // Initialize the mBus
    m_bus_init();
    
    if (USB) {                          // If I am using a USB then
        m_usb_init();                   // Initialize USB and
        while(!m_usb_isconnected());    // wait for a connection
    }
    
    
    // Turn off the RED LED when done
    m_red(OFF);
}
// --------------------------------------------------------------

