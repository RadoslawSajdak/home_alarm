/*
 * home_alarm.cpp
 *
 * Created: 2020-08-15 19:18:31
 * Author : sajda
 */ 

#include <avr/io.h>
#include "pcf8574.h"
#include "avr/interrupt.h"
#include "avr/delay.h"

int main(void)
{
	PCF8574 expander_gpio(0x3b);
	expander_gpio.pinMode(1, INPUT);
	expander_gpio.digitalWrite(1,HIGH);
	
	/* Setup interrupts */
	DDRD &= ~(1 << PIND2);
	PORTD |= (1 << PIND2);
	EICRA |= (1 << ISC10);
	EIMSK |= (1 << INT0);
	sei();
    /* Replace with your application code */
    while (1) 
    {
    }
}
ISR(INT0_vect)
{
	cli();
	_delay_ms(200);
	DDRB = 0xff;
	PORTB ^= 0xff;
	sei();
}

