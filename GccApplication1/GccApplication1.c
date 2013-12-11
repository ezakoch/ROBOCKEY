/*
 * GccApplication1.c
 *
 * Created: 09/12/2013 17:21:23
 *  Author: Alex Miranda Añon
 */ 


#include <avr/io.h>
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
#define TIME_STOP 1000

void turnOnBlueLED(void);
void turnOffBlueLED(void);
void stop_motor(void);
void celebrate(void);

volatile int flag_system  = 0;

int state = STOP_STATE;

//Variable for receiving data
char buffer_rec[PACKET_LENGTH_SYSTEM] = {0};



int main(void)
{
	int enemy_rob1_x = 0,enemy_rob1_y = 0,enemy_rob2_x = 0,enemy_rob2_y = 0,enemy_rob3_x = 0,enemy_rob3_y = 0;
	int scoreA = 0,scoreB = 0;
	long stop_counter = 0;
	
	m_rf_open(CHANNEL_SYSTEM,ALEX_ADDRESS_SYSTEM,PACKET_LENGTH_SYSTEM);
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
		
        switch (state)
        {
			case INITIAL_STATE:
				;
				break;
			case SYSTEM_STATE:
				m_wait(500);
				m_red(TOGGLE);
				switch (buffer_rec[0])
				{
					//Comm test
					case 0xA0:
					state = BLUE_LED_STATE;
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
					//}
					state = INITIAL_STATE;
					turnOnBlueLED();
					break;
				
					//Goal A
					case 0xA2:
					if (check(PINB,2))
					celebrate();
					stop_motor();
					scoreA = buffer_rec[1];
					scoreB = buffer_rec[2];
					state = STOP_STATE;
					break;
				
					//Goal B
					case 0xA3:
					if (!check(PINB,2))
					celebrate();
					stop_motor();
					scoreA = buffer_rec[1];
					scoreB = buffer_rec[2];
					state = STOP_STATE;
					break;
				
					//Pause
					case 0xA4:
					state = STOP_STATE;
					break;
				
					//Halftime
					case 0xA6:
					stop_counter = 0;
					state = STOP_STATE;
					break;
				
							//Game over
					case 0xA7:
					if (check(PINB,2))
					{
						if (scoreA > scoreB)
						celebrate();
					}else
					{
						if (scoreA < scoreB)
						celebrate();
					}
					stop_motor();
					state = STOP_STATE;
					break;
				
					//Enemy positions
					case 0xA8:
					enemy_rob1_x = buffer_rec[2];
					enemy_rob1_y = buffer_rec[3];
					enemy_rob2_x = buffer_rec[5];
					enemy_rob2_y = buffer_rec[6];
					enemy_rob3_x = buffer_rec[8];
					enemy_rob3_y = buffer_rec[9];
					break;
				
					default:
					break;
				}
				break; //go out of system case
			
			
			// --------------------------------------------------------------
			// BLUE LED STATE
			// --------------------------------------------------------------
			case BLUE_LED_STATE:
			stop_motor();
			turnOnBlueLED();
			m_wait(3000);
			turnOffBlueLED();
			state = STOP_STATE;
			break;
			
			
			// --------------------------------------------------------------
			// STOP STATE
			// --------------------------------------------------------------
			case STOP_STATE:
			//m_green(ON);
			turnOffBlueLED();
			stop_motor();
			break;
			// --------------------------------------------------------------
			
			
			
			// --------------------------------------------------------------
			// DEFAULT STATE
			// --------------------------------------------------------------
			default:
			stop_motor();
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

void stop_motor(void)
{
	OCR1B = 0;
	OCR1C = 0;
}

void celebrate(void)
{
	
}

void turnOnBlueLED(void)
{
	set(PORTD,5);
}

void turnOffBlueLED(void)
{
	clear(PORTD,5);
}

ISR(INT2_vect)
{
	//Read
	//past_state = state;
	//state = SYSTEM_STATE;
	flag_system = 1;
	m_green(TOGGLE); // Indicator receiving from RF
}