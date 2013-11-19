//Sinusoidal

//Includes & Constant defines
#include "m_general.h"
#include "m_bus.h"
#include "m_rf.h"
#include <avr/interrupt.h>

#define N_CLOCK 0
#define PACKET_LENGTH 1
#define REC_ADDRESS 0x49
#define SEN_ADDRESS 0x47
#define CHANNEL 1
#define N_BLINKS_SEND 7

//Variable used to change between duty cycles
volatile int flag_data = 0;
//volatile int flag_button = 0; 

//Variables for buffer
char buffer_rec [PACKET_LENGTH] = {0};
char buffer_send [PACKET_LENGTH] = {N_BLINKS_SEND};
	

//Function prototypes
ISR(INT2_vect);
//ISR(INT6_vect);

//Main function
int main(void)
{
	//Set the clock system prescaler
	m_clockdivide(N_CLOCK);

	//Initialize bus
	m_bus_init();
	
	//Open the channel
	m_rf_open(CHANNEL,REC_ADDRESS,PACKET_LENGTH);
	
	//Set B2 to input and disable pull-up resistor
	clear(DDRB,2);
	clear(PORTB,2);
	
	//Turn off the LEDs
	m_red(OFF);
	m_green(OFF);
	
	//Enable interruptions
	sei();
	
	//Main loop
	while (1)
	{
		//Receiving commands
		if (flag_data == 1)
		{
			//Blink loop
			for(int i=0; i<((int)buffer_rec[0])*2;i++)
			{
				//Toggle and wait for 250 ms, to make it visible
				m_red(TOGGLE);
				m_wait(3000);
			}
			
			//Reset the flag
			flag_data = 0;
		}
		
		
		//Send commands
		//if (flag_button == 1)
		if (check(PINB,2))
		{
			//Send the number of blinks to the external address
			m_green(ON);
			m_rf_send(SEN_ADDRESS, buffer_send,PACKET_LENGTH);
			m_wait(5000);
			m_green(OFF);
			m_wait(5000);
			
			//Reset the flag
			//flag_button = 0;
		}else
			m_green(OFF);
	}
}

//Interruption handler for the data received from the module (D2)
ISR(INT2_vect)
{
	//Read
	m_rf_read(buffer_rec,PACKET_LENGTH);
	flag_data = 1;
}


//Interruption handler for when the button is pressed
/*ISR(INT6_vect)
{
	flag_button = 1;
}*/