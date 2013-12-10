/* Name: Localize.c
 * Author: Spyridon Karachalios
 */

#include "Init_functions.h"
#include "m_general.h"



void turnOnBlueLED(void)
{
	set(PORTD,5);
}

void turnOffBlueLED(void)
{
	clear(PORTD,5);
}


void init_ports(void)
{
    //B3 and D3 as outputs
	set(DDRB,3);
	set(DDRD,3);
	set(PORTB,3);
	set(PORTD,3);
    
    //Set B2 as input and enable pull-up
    clear(DDRB,2);
    set(PORTB,2);
	
	//Set D5 as output
	set(DDRD,5);
	clear(PORTD,5);
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
    set(ADCSRA,ADIF);
}


//Timer 4 specifications (SENDING PROCEDURE)
void set_timer4(void)
{
    //Set the counter variable
    OCR4C = 195;
    
    //Set UP to OCR4C
    clear(TCCR4D,WGM41);
    clear(TCCR4D,WGM40);
    
    //Set the interruption to overflow
    set(TIMSK4,TOIE4);
    
    //Set prescaler to /4096
    set(TCCR4B,CS43);
    set(TCCR4B,CS42);
    clear(TCCR4B,CS41);
    set(TCCR4B,CS40);
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



void set_timer1(void)
{
    //Set B6 and B7 as output
    set(DDRB,6);
    set(DDRB,7);
    
    OCR1A = PWM_MAXIMUM;
    OCR1B = 0;
    OCR1C = 0;
    
    //Set to UP to OCR1A
    set(TCCR1B,WGM13);
    set(TCCR1B,WGM12);
    set(TCCR1A,WGM11);
    set(TCCR1A,WGM10);
    
    //Set to clear at OCR1B, set at rollover
    set(TCCR1A,COM1B1);
    clear(TCCR1A,COM1B0);
    
    //Set to clear at OCR1C, set at rollover
    set(TCCR1A,COM1C1);
    clear(TCCR1A,COM1C0);
    
    //Set timer prescaler to /1
    clear(TCCR1B,CS12);
    clear(TCCR1B,CS11);
    set(TCCR1B,CS10);
}


/*//Timer 3 Initialization (SOLENOID)
 void set_timer3(void)
 {
 OCR3A = 391;
 
 //Set C6 as output (debugging)
 set(DDRC,6);
 
 //Set to UP to OCR3A
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
 }*/




// --------------------------------------------------------------









