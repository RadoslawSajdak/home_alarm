#include "TWI.h"

#define LOW			 0
#define HIGH		 1

#define INPUT		 0
#define INPUT_PULLUP 1
#define OUTPUT		 2
class PCF8574
{
public:
	PCF8574(uint8_t address);
	void pinMode(uint8_t pin, uint8_t mode);
	void digitalWrite(uint8_t pin, uint8_t value);
private:
	void updateGPIO();
	uint8_t _address;
	uint8_t _PIN;
	uint8_t _PORT;
	uint8_t _DDR;
	
	
};