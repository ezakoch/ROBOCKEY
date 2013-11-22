//Mosfet_prova.c

void init_timer1(void);
void init_ports(void);

#include <avr/io.h>
#include "m_general.h"

int main(void)
{
	//Initialization functions
	init_timer1();
	init_ports();
	m_clockdivide(0);
	
	while(1)
	{
		if (check(PINB,1))
			clear(PORTB,0);
		else
			set(PORTB,0);
	}
}

void init_ports(void)
{
	//Set B0 as output
	set(DDRB,0);
	
	//Set B1 as input
	clear(DDRB,1);
	
	//Set as low
	clear(PORTB,0);
	
	//Enable pull-up
	set(PORTB,1);
}

//Timer Initialization
void init_timer1(void)
{
	OCR1A = 1020;
	
	//Set B5 as output
	set(DDRB,5);

	//Set to UP to UP to 0x03FF
	clear(TCCR1B,WGM13);
	set(TCCR1B,WGM12);
	set(TCCR1A,WGM11);
	set(TCCR1A,WGM10);

	//clear at OCR1A, set at rollover
	set(TCCR1A,COM1A1);
	clear(TCCR1A,COM1A0);
	
	//Set prescaler to 1
	clear(TCCR1B,CS12);
	clear(TCCR1B,CS11);
	set(TCCR1B,CS10);
	
}