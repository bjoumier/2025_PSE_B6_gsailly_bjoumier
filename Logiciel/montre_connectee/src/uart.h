#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_init(uint32_t baud);

void uart_write(char c);

char uart_read(void);

uint8_t uart_available(void);

#endif
