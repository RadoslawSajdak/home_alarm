#include "TWI.h"
#include "Arduino.h"
class PCF8574
{
public:
	PCF8574(uint8_t address);
	void pinMode(uint8_t pin, uint8_t mode);
	void digitalWrite(uint8_t pin, uint8_t value);
	void digitalRead(uint8_t address, uint8_t size);
private:
	void updateGPIO();
	uint8_t _address;
	uint8_t _PIN;
	uint8_t _PORT;
	uint8_t _DDR;
	
	
};