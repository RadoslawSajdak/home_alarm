/*
 * Home_alarm.cpp
 *
 * Created: 2020-08-11 14:31:06
 * Author : Sajdak
 */ 
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include "avr/interrupt.h"
#include "stdio.h"
#include "avr/delay.h"
#include "uart.h"

#define BUFF_SIZE		25


int main(void)
{
	DDRB |= (1 << PINB5);
	
	EICRA = (1 << ISC01) ;
	EIMSK = (1 << INT0);
	PORTD |= (1 << PIND2);
	sei();
    /* Replace with your application code */
	uart0_init(UART_BAUD_SELECT(9600,F_CPU));
	uint16_t buffer[BUFF_SIZE];
	
	
	
	
    while (1) 
    {
		if (uart0_available() != 0)
		{
			uint8_t number = uart0_available();
			for (int i = 0 ; i < number; i++)
			{
				buffer[i] = uart0_getc();
			}
		}
    }
	return 0;
}
ISR(INT0_vect)
{
	cli();
	_delay_ms(600);
	PORTB ^= (1 << PINB5);
	uart0_puts("ATD+\"+48664059986\"");
	
	sei();
}