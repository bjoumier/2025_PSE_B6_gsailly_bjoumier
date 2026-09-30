#include "uart.h"

#include <avr/io.h>

void uart_init(uint32_t baud)
{
    // Double speed mode
    UCSR1A = (1 << U2X1);

    // Calcul UBRR pour U2X = 1
    uint16_t ubrr = (F_CPU / 8 / baud) - 1;

    UBRR1H = (ubrr >> 8);
    UBRR1L = ubrr;

    // RX + TX
    UCSR1B = (1 << RXEN1) | (1 << TXEN1);

    // 8 bits, 1 stop, no parity
    UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);
}

void uart_write(char c)
{
    while(!(UCSR1A & (1 << UDRE1)));

    UDR1 = c;
}

char uart_read(void)
{
    while(!(UCSR1A & (1 << RXC1)));

    return UDR1;
}

uint8_t uart_available(void)
{
    return (UCSR1A & (1 << RXC1));
}
