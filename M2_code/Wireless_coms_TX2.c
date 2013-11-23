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

#define N_CLOCK 0
#define NUM_LEDS 8
#define SIZE_ARRAY_BLOBS 12
#define PACKET_LENGTH 17
#define SEN_ADDRESS 0x47
#define REC_ADDRESS_AUX 0X49
#define CHANNEL 1

//Function prototypes
void set_timer3(void);
void init_analog (void);
void get_analog_val (int id);

//Variable used to check timer
volatile int flag_timer = 0;

//Function prototypes
ISR(INT2_vect);

//Main function
int main(void)
{
	//Variable declaration
	unsigned char wii_OK = 0;
	unsigned char localize_OK = 0;
	
	unsigned char state [PACKET_LENGTH] = {0}; //State(1): [0] identification, [1] current state
		
	signed char position [PACKET_LENGTH] = {0}; // Position(2): [0] identification,[1] x int,[2] x decimal,[3] y int,[4] y decimal, 
												//[5] theta 1st int,[6] theta 2nd int,[7] theta decimal
																								
	unsigned char LED_analog [PACKET_LENGTH] = {0}; //Analog values for each phototransistor(3): [0] identification,[0+i+1] 1st digit analog,  
													//[1+i+1] 1st digit analog (i=1...8)
													
	unsigned char stars_wii [PACKET_LENGTH] = {0}; //Pixel position of stars(4): [0] identification, [0+i+1] 1st x,  [0+i+2] 2nd x, [0+i+3] 1st y, 
												   //[0+i+4] 2nd y (i=1...4)
												   
	unsigned char counter_solenoid [PACKET_LENGTH] = {0}; //Counts how many times the solenoid has been shooted(5): [0] identification, [1] current count
		
	unsigned char battery_ind [PACKET_LENGTH] =  {0}; //Battery indicator in % (6): [0] identification, [1] current value
		
	unsigned char general_vars [PACKET_LENGTH] = {0}; //General vars(7): [0] identification,[1] current value (i=1,2...) 
		
	unsigned int blobs_wii[SIZE_ARRAY_BLOBS]; //Variable for the wii cam blobs
	
	int x_robot = 0, y_robot = 0, theta_robot = 0;
	int LED_values_raw [NUM_LEDS] = {0};
	div_t aux_conversion;
		
	//Set the clock system prescaler
	m_clockdivide(N_CLOCK);
	
	//Turn off the LEDs
	m_green(OFF);
	m_red(OFF);

	//Initialize bus
	m_bus_init();
	
	//Set timer to every 0.05 s (20 Hz)
	set_timer3();
	
	//Set the ADC
	init_analog();
	
	//Initialize wii camera
	char aux = 0;
	while(!aux)
	{
		aux = m_wii_open();
	}
	
	m_green(ON);
	m_wait(1000);
	m_green(OFF);
		
	//Open the channel
	m_rf_open(CHANNEL,REC_ADDRESS_AUX,PACKET_LENGTH);
	
	//Enable interruptions
	sei();
	
	//Main loop
	while (1)
	{
		//LOCALIZATION CODE
		//Get the blobs
		wii_OK = m_wii_read(blobs_wii);
		
		//If data received correctly
		if (wii_OK)
		{
			//Get the position and orientation of the robot from the constellation
			localize_OK = localize(blobs_wii[0],blobs_wii[3],blobs_wii[6],blobs_wii[9],blobs_wii[1],blobs_wii[4],blobs_wii[7],blobs_wii[10],&x_robot,&y_robot,&theta_robot);
			
			//If we computed data correctly send it if not filter (NEEEEED TO CHANGEEEEEEE!!!!!!!!!!!!!!!)
			if (!localize_OK)
			{
				m_red(TOGGLE);
				x_robot = -110;
				y_robot = 70;
				theta_robot = 765;
				
			}else
				m_green(TOGGLE);
		}
		
		//ANALOG CODE
		
		for (int i=0;i<NUM_LEDS;i++)
		{
			get_analog_val(i);
			
			//Wait until flag is on
			while(!check(ADCSRA,ADIF));
			LED_values_raw[i] = ADC;
			
			//After doing the conversion reset flag
			set(ADCSRA,ADIF);
		}
		
		
		//SEND COMMANDS
		if (flag_timer == 1)
		{
			//Position
			position[0] = 2;
			position[1] = (signed char) x_robot;
			position[2] = (signed char) y_robot;
			aux_conversion = div(theta_robot,128);
			position[3] = (signed char)aux.quot;
			position[4] = (signed char)aux.rem;
			
			m_rf_send(SEN_ADDRESS,position,PACKET_LENGTH);
			
			//Analog values
			LED_analog[0] = 3;
			for (int i=0;i<NUM_LEDS;i++)
			{
				aux_conversion = div(LED_values_raw,256);
				LED_analog[2*i+1] = aux_conversion.quot;
				LED_analog[2*i+2] = aux_conversion.rem;
			}
			m_rf_send(SEN_ADDRESS,LED_analog,PACKET_LENGTH);
				
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

//A/D Initialization
void init_analog(void)
{
	//Set the reference voltage to Vcc
	clear(ADMUX,REFS1);
	set(ADMUX,REFS0);
	
	//Set the ADC Clock Prescaler (/128)
	set(ADCSRA,ADPS2);
	set(ADCSRA,ADPS1);
	set(ADCSRA,ADPS0);
	
	//Disable set(DIDR2,ADC9D);
	set(DIDR0,ADC0D);
	set(DIDR0,ADC1D);	
	set(DIDR0,ADC4D);	
	set(DIDR0,ADC5D);	
	set(DIDR0,ADC6D);	
	set(DIDR0,ADC7D);	
	set(DIDR2,ADC8D);	
	set(DIDR2,ADC9D);		
	
	//Set the triggering to free-running
	set(ADCSRA,ADATE);
	
}

void get_analog_val(int id)
{
	//Disable ADC
	clear(ADCSRA,ADEN);
	
	//Check which analog device we want to get the values from and select it
	switch (id)
	{
		case 0:
			//Single-Ended Channel Selection (F0)
			clear(ADCSRB,MUX5);
			clear(ADMUX,MUX2);
			clear(ADMUX,MUX1);
			clear(ADMUX,MUX0);
			break;
			
		case 1:
			//Single-Ended Channel Selection (F1)
			clear(ADCSRB,MUX5);
			clear(ADMUX,MUX2);
			clear(ADMUX,MUX1);
			set(ADMUX,MUX0);
			break;
			
		case 2:
			//Single-Ended Channel Selection (F4)
			clear(ADCSRB,MUX5);
			set(ADMUX,MUX2);
			clear(ADMUX,MUX1);
			clear(ADMUX,MUX0);
			break;
			
		case 3:
			//Single-Ended Channel Selection (F5)
			clear(ADCSRB,MUX5);
			set(ADMUX,MUX2);
			clear(ADMUX,MUX1);
			set(ADMUX,MUX0);
			break;
			
		case 4:
			//Single-Ended Channel Selection (F6)
			clear(ADCSRB,MUX5);
			set(ADMUX,MUX2);
			set(ADMUX,MUX1);
			clear(ADMUX,MUX0);
			break;
			
		case 5:
			//Single-Ended Channel Selection (F7)
			clear(ADCSRB,MUX5);
			set(ADMUX,MUX2);
			set(ADMUX,MUX1);
			set(ADMUX,MUX0);
			break;
			
		case 6:
			//Single-Ended Channel Selection (D4)
			set(ADCSRB,MUX5);
			clear(ADMUX,MUX2);
			clear(ADMUX,MUX1);
			clear(ADMUX,MUX0);
			break;
			
		case 7:
			//Single-Ended Channel Selection (D6)
			set(ADCSRB,MUX5);
			clear(ADMUX,MUX2);
			clear(ADMUX,MUX1);
			set(ADMUX,MUX0);
			break;
			
	}	

	//Enable ADC Subsystem & Begin Conversion
	set(ADCSRA,ADEN);
	set(ADCSRA,ADSC);
}

ISR(TIMER3_COMPA_vect)
{
	//m_red(ON);
	flag_timer = 1;
}
