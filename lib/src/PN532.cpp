#include "PN532.h"

PN532::PN532(uint8_t irq)
{
	_irq = irq;
	_address = PN532_I2C_ADDRESS;
	DDRD  &= ~(1 << _irq);
	PORTD |= (1 << _irq);
	
	if (irq == PIND2)
	{
		
		EICRA |= (1 << ISC01);
		EIMSK |= (1 << INT0);
		EIFR  |= (1 << INTF0);
	}
	else if (irq == PIND3)
	{
		EICRA |= (1 << ISC11);
		EIMSK |= (1 << INT1);
		EIFR  |= (1 << INTF1);
	}
	
	
}

void PN532::send_command(uint8_t * cmd, uint8_t cmdlen)
{
	_delay_ms(2);
	uint8_t checksum = PN532_PREAMBLE + PN532_PREAMBLE + PN532_STARTCODE2;
	twi_start();
	twi_write_byte(_address,PN532_PREAMBLE);
	twi_write_byte(_address,PN532_PREAMBLE);
	twi_write_byte(_address,PN532_STARTCODE2);
	twi_write_byte(_address,cmdlen);
	twi_write_byte(_address,~cmdlen + 1);
	twi_write_byte(_address, PN532_HOSTTOPN532);
	checksum += PN532_HOSTTOPN532;
	
	for (uint8_t i = 0; i < cmdlen -1; i++)
	{
		twi_write_byte(_address,cmd[i]);
		checksum += cmd[i];
	}
	
	twi_write_byte(_address, ~checksum);
	twi_write_byte(_address, PN532_POSTAMBLE);
	twi_stop();
	
}

void PN532::read_data(uint8_t * buffer, uint8_t nbytes)
{
	_delay_ms(2);
	twi_start();
	for (uint8_t i = 0; i < nbytes; i++)
	{
		_delay_ms(1);
		buffer[i] = twi_read_byte(_address,1);
	}
	twi_stop();
}
uint8_t PN532::get_status(void)
{
	uint8_t foo = PORTD & (1 << _irq);
	if (foo == 1) return PN532_I2C_BUSY;
	else return PN532_I2C_READY;
}