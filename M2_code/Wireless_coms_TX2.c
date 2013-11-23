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

#define N_CLOCK 1
#define SIZE_ARRAY_BLOBS 12
#define PACKET_LENGTH 16
#define SEN_ADDRESS 0x47
#define REC_ADDRESS_AUX 0X49
#define CHANNEL 1

//Function prototypes
void set_timer3(void);


//Variable used to check timer
volatile int flag_timer = 0;

//Function prototypes
ISR(INT2_vect);

//Main function
int main(void)
{
	//Variable declaration
	int wii_OK = 0;
	unsigned char state [PACKET_LENGTH] = {0}; //State: [0] current state
	signed char position [PACKET_LENGTH] = {0}; // Position: [0] x int,[1] x decimal,[2] y int,[3] y decimal, 
												//[4] theta 1st int,[5] theta 2nd int,[6] theta decimal
	unsigned char LED_analog [PACKET_LENGTH] = {0}; //Analog values for each phototransistor: [0+i] 1st digit analog,  [1+i] 1st digit analog (i=1...8)
	unsigned char stars_wii [PACKET_LENGTH] = {0}; //Pixel position of stars: [0+i] 1st x,  [0+i] 2nd x, [0+i] 1st y, [0+i] 2nd y (i=1...4)
	unsigned char counter_solenoid [PACKET_LENGTH] = {0}; //Counts how many times the solenoid has been shooted: [0] current count
	unsigned char battery_ind [PACKET_LENGTH] =  {0}; //Battery indicator %: [0] current value
	unsigned char general_vars [PACKET_LENGTH] = {0}; //General vars: [0] current value (i=1,2...) 
	unsigned int blobs_wii[SIZE_ARRAY_BLOBS]; //Variable for the wii cam blobs
	float x_robot = 0.0f ,y_robot = 0.0f,theta_robot = 0.0f;
		
	//Set the clock system prescaler
	m_clockdivide(N_CLOCK);

	//Initialize bus
	m_bus_init();
	
	//Initialize wii camera
	while(!m_wii_open());
	
	//Set timer to every 0.05 s (20 Hz)
	set_timer3();
	
	//Set the ADC
	init_analog();
		
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
		wii_OK = m_wii_read(blobs_wii);
		if (wii_OK)
		{
			localize(blobs_wii[0],blobs_wii[3],blobs_wii[6],blobs_wii[9],blobs_wii[1],blobs_wii[4],blobs_wii[7],blobs_wii[10],&x_robot,&y_robot,&theta_robot);
		}
		
		
		
		//Send commands
		if (flag_timer == 1)
		{
			buffer_send[0] = x;
			buffer_send[1]= y;
			m_rf_send(SEN_ADDRESS,buffer_send,PACKET_LENGTH);
				
			//Reset flag
			flag_timer = 0;
			m_red(OFF);
		}
	}
}

//Timer 3 Initialization
void set_timer3(void)
{
	OCR3A = 391;
	
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
