/*
 * home_alarm.cpp
 *
 * Created: 2020-08-15 19:18:31
 * Author : sajda
 */ 


#include <avr/io.h>
#include "SoftwareSerial.h"
#include "Arduino.h"
//#include "pcf8574.h"
#include "avr/interrupt.h"
//#define  F_CPU 16000000UL
#include "avr/delay.h"
#include "Wire.h"
#include "PN532_I2C.h"
#include "PN532.h"

PN532_I2C pn532i2c(Wire);
PN532	RFID(pn532i2c);

int main(void)
{
	
	
	Serial.begin(115200);
	Serial.println("Started!");
	_delay_ms(500);
	RFID.begin();
// 	expander_gpio.pinMode(1, INPUT);
// 	expander_gpio.digitalWrite(1,HIGH);
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
	_delay_ms(2000);
	DDRB = 0xff;
	PORTB ^= 0xff;
	uint8_t foo = RFID.getFirmwareVersion();
	Serial.println("Hello bro");
	Serial.println(foo);
	//expander_gpio.digitalWrite(1,LOW);
	sei();
}

