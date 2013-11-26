//Sending values using Wireless

//Includes & Constant defines
#include "m_general.h"
#include "m_bus.h"
#include "m_rf.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#define N_CLOCK 1
#define PACKET_LENGTH 5
#define SEN_ADDRESS 0x47
#define REC_ADDRESS_AUX 0X49
#define CHANNEL 1

//Function prototypes
void set_timer3(void);

//Variable used to check timer
volatile int flag_timer = 0;

//Variables for buffer
char buffer_send [PACKET_LENGTH] = {0};


//Function prototypes
ISR(INT2_vect);

//Main function
int main(void)
{
	//Variable declaration
	int nums = -120;
	
	//Set the clock system prescaler
	m_clockdivide(N_CLOCK);

	//Initialize bus
	m_bus_init();
	
	set_timer3();
	
	//Open the channel
	m_rf_open(CHANNEL,REC_ADDRESS_AUX,PACKET_LENGTH);
	
	//Turn off the LEDs
	m_green(ON);
	m_red(OFF);
	
	//Enable interruptions
	sei();
	
	//Main loop
	while (1)
	{
		if (nums <= 120)
		{
			//Receiving commands
			if (flag_timer == 1)
			{
				sprintf(buffer_send,"%04d",nums);
				m_rf_send(SEN_ADDRESS,buffer_send,PACKET_LENGTH);
				
				//Move to next number
				nums += 5;
				
				//Reset flag
				flag_timer = 0;
				m_wait(200);
				m_red(OFF);
			}
		}
	}
}

//Timer 3 Initialization
void set_timer3(void)
{
	OCR3A = 7812;
	
	//Set C6 as output (debugging)
	set(DDRC,6);

	//Set to UP to OCR1A
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
}

ISR(TIMER3_COMPA_vect)
{
	m_red(ON);
	flag_timer = 1;
}
