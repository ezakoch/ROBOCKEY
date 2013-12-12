/*
 * GccApplication1.c
 *
 * Created: 09/12/2013 17:21:23
 * 
 */ 


#include "m_general.h"
#include "m_rf.h"
#include "m_bus.h"

#define CHANNEL_SYSTEM 1

#define INITIAL_STATE 0
#define SYSTEM_STATE 99
#define STOP_STATE 21
#define BLUE_LED_STATE 22
#define SEN_ADDRESS_SYSTEM 0xDA
#define ALEX_ADDRESS_SYSTEM 0x42
#define PACKET_LENGTH_SYSTEM 10

#define TIME_TO_TURN 125

void turnOnBlueLED(void);
void turnOffBlueLED(void);
void set_timer0(void);
void start_timer0(void);
void stop_timer0(void);

volatile int flag_system  = 0;
volatile int GO = 0;

int state = STOP_STATE;

//Variable for receiving data
char buffer_rec[PACKET_LENGTH_SYSTEM] = {0};



int main(void)
{

	m_bus_init();
	m_rf_open(CHANNEL_SYSTEM,ALEX_ADDRESS_SYSTEM,PACKET_LENGTH_SYSTEM);
	m_red(OFF);
	m_green(OFF);
	sei();
	 
    while(1)
    {
		if (flag_system == 1)
		{            
			m_red(TOGGLE);
			m_rf_read(buffer_rec,PACKET_LENGTH_SYSTEM);
			state = SYSTEM_STATE;
			flag_system = 0;
			
		}
		if (GO == 1)
			m_green(TOGGLE);
		
        switch (state)
        {
			case INITIAL_STATE:
				;
				break;
			case SYSTEM_STATE:
				switch (buffer_rec[0])
				{
					//Comm test
					case 0xA0:
					state = BLUE_LED_STATE;
					m_red(TOGGLE);
					m_wait(1500);
					m_red(TOGGLE);
					break;
				
					//Play
					case 0xA1:
					//if (pause_bool)
					//{
					//state = past_state;
					//pause_bool = 0;
					//}else
					//{
					//state = INITIAL_STATE;
					//}.
					state = INITIAL_STATE;
					turnOnBlueLED();
					m_green(TOGGLE);
					m_wait(1500);
					m_green(TOGGLE);
					break;
				
					//Pause
					case 0xA4:
					state = STOP_STATE;
					break;
				
					//Halftime
					case 0xA6:
					state = STOP_STATE;
					break;
					
					//Game over
					case 0xA7:
					state = STOP_STATE;
					break;
				
					default:
					break;
				}
				break; //go out of system case
			
			
			// --------------------------------------------------------------
			// BLUE LED STATE
			// --------------------------------------------------------------
			case BLUE_LED_STATE:
			turnOnBlueLED();
			m_wait(3000);
			turnOffBlueLED();
			state = STOP_STATE;
			break;
			
			
			// --------------------------------------------------------------
			// STOP STATE
			// --------------------------------------------------------------
			case STOP_STATE:
			turnOffBlueLED();
			break;
			// --------------------------------------------------------------
			
			
			
			// --------------------------------------------------------------
			// DEFAULT STATE
			// --------------------------------------------------------------
			default:
			break;
			//while(1)
			//{
			//m_red(TOGGLE);
			//m_green(TOGGLE);
			//m_wait(250);
			//}
			// --------------------------------------------------------------
			
		}
    }
}

void turnOnBlueLED(void)
{
	set(PORTD,5);
}

void turnOffBlueLED(void)
{
	clear(PORTD,5);
}
void set_timer0(void)
{
	OCR0A = TIME_TO_TURN;
	
	//Set to UP to OCR0A
	clear(TCCR0B,WGM02);
	set(TCCR0A,WGM01);
	clear(TCCR0A,WGM00);
	
	//Set to OFF
	clear(TCCR0B,CS02);
	clear(TCCR0B,CS01);
	clear(TCCR0B,CS00);
	
	//Demask Overflow interrupt
	set(TIMSK0,TOIE0);
}

void start_timer0(void)
{
	//Set timer prescaler to /256
	set(TCCR0B,CS02);
	clear(TCCR0B,CS01);
	clear(TCCR0B,CS00);
	
	
}

void stop_timer0(void)
{
	//Set timer prescaler to /0
	clear(TCCR0B,CS02);
	clear(TCCR0B,CS01);
	clear(TCCR0B,CS00);
	
	TCNT0 = 0;
	
}

ISR(INT2_vect)
{
	//Read
	//past_state = state;
	//state = SYSTEM_STATE;
	flag_system = 1;
	m_green(TOGGLE); // Indicator receiving from RF
}

ISR(TIMER0_OVF_vect){
	GO = 1;
}
