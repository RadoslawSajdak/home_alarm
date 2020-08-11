/*
 * Home_alarm.cpp
 *
 * Created: 2020-08-11 14:31:06
 * Author : Sajdak
 */ 

#include <avr/io.h>
#include "avr/interrupt.h"
#include "stdio.h"
#include "avr/delay.h"

#include "SoftwareSerial.h"
#include "Sim800l.h"

Sim800l SIM;
SoftwareSerial mySerial(5, 4); //SIM800L Tx & Rx is connected to Arduino #3 & #2



int main(void)
{
	mySerial.begin(9600);
	DDRB |= (1 << PINB5);
	
	EICRA = (1 << ISC11) | (1 << ISC01) ;
	EIMSK = (1 << INT1) | (1 << INT0);
	PORTD |= (1 << PIND3) | (1 << PIND2);
	sei();
    /* Replace with your application code */
	
	
	
    while (1) 
    {
		;
    }
	return 0;
}
ISR(INT0_vect)
{
	cli();
	_delay_ms(1000);
	PORTB ^= (1 << PINB5);

	mySerial.println("ATD+ +48793380333;");
	
	sei();
}

ISR(INT1_vect)
{
	cli();
	_delay_ms(500);
	mySerial.println("ATH");
	sei();
}