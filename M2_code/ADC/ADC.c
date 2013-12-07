/*
 * ADC.c
 *
 * Created: 04/12/2013 20:54:47
 *  Author: Alex Miranda Añon
 */ 


#include <avr/io.h>
#include "m_usb.h"


//Function prototypes
void init(void);

int main(void)
{
	char rx_buffer;
	m_clockdivide(0);
	//m_usb_init(); // connect usb
	//m_red(ON);
	init();
    while(1)
    {
        //Check if the flag is on
        if(check(ADCSRA,ADIF))
        {
			//while(!m_usb_rx_available());  	//wait for an indication from the computer
			//rx_buffer = m_usb_rx_char();  	//grab the computer packet
			//m_green(TOGGLE);
			
	        //m_usb_tx_int(ADC);
			//m_usb_tx_char('\n');
			
	        //After doing the conversion reset flag
	        set(ADCSRA,ADIF);
        }
    }
}

//Initialization function
void init(void)
{
	//Set the reference voltage to Vcc
	clear(ADMUX,REFS1);
	set(ADMUX,REFS0);
	
	//Set the ADC Clock Prescaler (/16)
	set(ADCSRA,ADPS2);
	clear(ADCSRA,ADPS1);
	clear(ADCSRA,ADPS0);
	
	//Disable digital output
	set(DIDR0,ADC0D);
	
	//Set the triggering to free-running
	set(ADCSRA,ADATE);
	
	//Disable ADC and Single-Ended Channel Selection
	clear(ADCSRA,ADEN);
	clear(ADCSRB,MUX5);
	clear(ADMUX,MUX2);
	clear(ADMUX,MUX1);
	clear(ADMUX,MUX0);
	
	//Enable ADC Subsystem & Begin Conversion
	set(ADCSRA,ADEN);
	set(ADCSRA,ADSC);
}