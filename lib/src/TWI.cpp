
#include "TWI.h"

void twi_start(){
	TWBR=16;
	TWCR= (1<<TWINT) | (1<<TWEN) | 1<<(TWSTA) ; //clear interrupt flag, enable twi, start
	while (!(TWCR&(1<<TWINT))){};				//wait for interrupt flag in TWCR register
	_delay_ms(10);
}

void twi_write_byte(uint8_t data){
	TWDR = data;								//TWDR store data to send or received
	_delay_us(100);
	TWCR= (1<<TWINT) | (1<<TWEN);				//clear interrupt flag, enable twi
	while (!(TWCR&(1<<TWINT))){};				//wait for interrupt flag in TWCR register
}


uint8_t twi_read_byte(uint8_t use_ACK){
	TWCR= (1<<TWINT) | (1<<TWEN) | (use_ACK<<TWEA);		//clear interrupt flag, enable twi, (generate ACK bit)
	while (!(TWCR&(1<<TWINT))){};
	return TWDR;								//TWDR store data to send or received
}


void twi_stop(){
	TWCR= (1<<TWINT) | (1<<TWEN) | (1<<TWSTO) ;//clear interrupt flag, enable twi, stop
	while (!(TWCR&(1<<TWSTO))){};
	_delay_ms(20);
}