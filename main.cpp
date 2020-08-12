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
#include "avr/sleep.h"

#include "SoftwareSerial.h"
#include "Arduino.h"
#include "MFRC522.h"

#define REED		PIND2
#define PHOTOCELL	PIND3
SoftwareSerial	Phone(4,5);
MFRC522 RFID;
char number[13] = "+48664059986";


int main(void)
{
	Phone.begin(9600);
	RFID.PCD_Init();
	
	DDRB |= (1 << PINB0) | (1 << PINB5);
	
	EICRA = (1 << ISC11) | (1 << ISC01) | (1 << ISC00) ;
	EIMSK = (1 << INT1) | (1 << INT0);
	PORTD |= (1 << PHOTOCELL) | (1 << REED);
	
    /* Power management */
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
	sei();
	
    while (1) 
    {
		tone(8,3000);
		sleep_cpu();
		
    }
	return 0;
}
ISR(INT0_vect)
{
	cli();
	_delay_ms(4000);
	if (PIND & (1 << REED))
	{
		PORTB ^= (1 << PINB5);
		
	}
	EIFR |= (1 << INTF0);
	sei();
}

ISR(INT1_vect)
{
	cli();
	_delay_ms(500);

	sei();
}

