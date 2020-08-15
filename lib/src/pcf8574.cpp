#include "pcf8574.h"

PCF8574::PCF8574(uint8_t address) :
		_PORT(0), _PIN(0), _DDR(0), _address(0)
{
	_address = address;
	twi_start();
	_PIN = twi_read_byte((_address << 1) | (1 << 1));
	twi_stop();
}
void PCF8574::pinMode(uint8_t pin, uint8_t mode)
{
	/* Switch according mode */
	switch (mode) {
		case INPUT:
		_DDR &= ~(1 << pin);
		_PORT &= ~(1 << pin);
		break;

		case INPUT_PULLUP:
		_DDR &= ~(1 << pin);
		_PORT |= (1 << pin);
		break;

		case OUTPUT:
		_DDR |= (1 << pin);
		_PORT &= ~(1 << pin);
		break;

		default:
		break;
	}

	/* Update GPIO values */
	updateGPIO();
}
void PCF8574::digitalWrite(uint8_t pin, uint8_t value) {

	/* Set PORT bit value */
	if (value)
	_PORT |= (1 << pin);
	else
	_PORT &= ~(1 << pin);

	/* Update GPIO values */
	updateGPIO();
}
void PCF8574::updateGPIO()
{
	uint8_t value = (_PIN & ~_DDR) | _PORT;
	
	twi_start();
	twi_write_byte(_address << 1);
	twi_write_byte(value);
	twi_stop();
}