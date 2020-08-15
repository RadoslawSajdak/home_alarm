/*
 * TWI library
 * Created: 04.2020
 * Author : Mateusz Kozyra
 * Purpose : Part of project made at AGH UST.
 * Description : Main library, necessary for communication with external devices 
 * Features : TWI/I2C communication read and write
 */ 


#ifndef TWI_H_
#define TWI_H_

#define F_CPU 16000000UL//16 MHz
#include <util/delay.h>#include <avr/io.h>
#include <stdbool.h>



/************************************************************************
 @brief Start TWI transmission
************************************************************************/
void twi_start();

/************************************************************************
 @brief Send byte through TWI - only if transmission started

 @param[in] data - data to send
************************************************************************/
void twi_write_byte(uint8_t data);

/************************************************************************
 @brief Read byte send by slave-transmitter

 @param[in] use_ACK value{0 or 1} bool type decide if send ACK bit or not
	 after receiving data
	 
 @return 8bit from slave-transmitter 
************************************************************************/
uint8_t twi_read_byte(uint8_t use_ACK);

/************************************************************************
 @brief Stop transmission - send stop bit
************************************************************************/
void twi_stop();

/************************************************************************
 @brief Full tx function - start transmission, send few bytes of data 
	and stop transmission

 @param[in] address - slave device address 
 @param[in] *data - pointer where starts data to send
 @param[in] amount - number of bytes to send 
 @param[in] stop - stop transmission or hold line
************************************************************************/
// void twi_tx(uint8_t adress, uint8_t *data, uint8_t amount, bool stop);

/************************************************************************
 @brief Full rx function - start transmission, read few bytes of data 
	and stop transmission

 @param[in] address - slave device address 
 @param[in] *data - pointer where we store received data
 @param[in] amount - number of bytes to read
 @param[in] stop - stop transmission or hold line
************************************************************************/
// void twi_rx(uint8_t adress, uint8_t *data, uint8_t amount, bool stop);

#endif  /*TWI_H_*/ 