#include "i2c.h"

#include <avr/io.h>


void i2c_init(void)
{	
	/*
	// Prescaler = 1
    TWSR = 0x00;
    // 200kHz avec F_CPU = 16MHz
    TWBR = 12;
    // Active TWI
    TWCR = (1 << TWEN);
    */
}

void i2c_start(uint8_t address)
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT)));

    TWDR = address;

    TWCR = (1 << TWINT) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT)));
}

void i2c_write(uint8_t data)
{
    TWDR = data;

    TWCR = (1 << TWINT) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT)));
}

void i2c_stop(void)
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

uint8_t i2c_read_ack(void)
{
    // Autorise ACK après réception
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);

    uint16_t timeout = 10000;
    while (!(TWCR & (1 << TWINT)) && timeout--);

    return TWDR;
}

uint8_t i2c_read_nack(void)
{
    // Pas d’ACK (dernier octet)
    TWCR = (1 << TWINT) | (1 << TWEN);

    uint16_t timeout = 10000;
    while (!(TWCR & (1 << TWINT)) && timeout--);

    return TWDR;
}
