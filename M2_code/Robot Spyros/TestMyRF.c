/* Name: 4d.c
 * Author: Spyridon Karachalios
 */

#include <avr/io.h>
#include "m_general.h"
#include "m_usb.h"
#include "m_bus.h"
#include "m_rf.h"

#define USB 1
#define F_M2 16000000       // Frequency of the M2

#define CHANNEL 1           // RF Channel to connect to
#define RXADDRESS 0x41      // RF Address to connect to
#define PACKET_LENGTH 10     // Length of received packages


// Prototype for functions
void init(void);                // Initialize M2

// Global Variables
volatile int flag_rx = 0;               // Flag to be used for the ISR for mRF
char buffer[PACKET_LENGTH] = {0,0,0,0,0,0,0,0,0,0};   // Package received from mRF


// --------------------------------------------------------------
// MAIN
// --------------------------------------------------------------
int main(void)
{
    // Initialize the M2
    init();
    
    
    // Enable global interrupts
    sei();
    
    
    /* Main Loop... Runs forever.. */
    while (true) {
        // Whenever a package is received
        if (flag_rx == 1){
            flag_rx = 0;
            m_rf_read(buffer,PACKET_LENGTH);    // Pull the packet
            
            //Send through USB the received packet
             if (USB){
             m_usb_tx_hex(buffer[0]);
             m_usb_tx_string("\n");
             }
            
        }
    }
    
    return 0;       // If for some reason it reaches here exit
}
// --------------------------------------------------------------




// --------------------------------------------------------------
// Interrupt Service Routine
// when mRF receives a package
// --------------------------------------------------------------
ISR(INT2_vect)
{
    m_green(TOGGLE);
    flag_rx = 1;        // Set the flag for received packet
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
    
    
    
    
     if (USB) {
     m_usb_init();                // Initialize USB and
     while(!m_usb_isconnected()); // wait for a connection
     }
    
    m_rf_open(CHANNEL,RXADDRESS,PACKET_LENGTH);
    
    m_red(OFF);
    // Turn off the RED LED when done
    
}
// --------------------------------------------------------------

