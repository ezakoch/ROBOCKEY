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
#define PACKET_LENGTH 5
#define REC_ADDRESS 0X47
#define CHANNEL 1

//Variable used to check when we get data
volatile int flag_data = 0;

//Variables for buffer
char buffer_rec[PACKET_LENGTH] = {0};

//Main function
int main(void)
{
	char* end_ptr;
	long value_received;
	
	//Set the clock system prescaler
	m_clockdivide(N_CLOCK);

	//Initialize bus
	m_bus_init();
	
	//Initialize
	m_usb_init();
	
	while(!m_usb_isconnected()); // wait for a connection
	
	//Open the channel
	m_rf_open(CHANNEL,REC_ADDRESS,PACKET_LENGTH);
	
	//Turn off the LEDs
	m_red(OFF);
	m_green(ON);
	
	//Enable interruptions
	sei();
	
	//Main loop
	while (1)
	{
		//Receiving commands
		if (flag_data == 1)
		{
			//Transform the array received to an integer
			if (strcmp(buffer_rec,"0000") == 0)
			{
				value_received = 0;
				m_usb_tx_long(value_received);
			}else
			{
				value_received = strtol(buffer_rec,&end_ptr,10);
				if (value_received != 0)
				m_usb_tx_long(value_received);
				else
				m_usb_tx_char('W');
			}
			m_usb_tx_char('\r');
			
			//Reset the flag
			flag_data = 0;
			m_red(OFF);
		}
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


