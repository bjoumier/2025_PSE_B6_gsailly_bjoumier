#ifndef U8G2_U8X8_AVR_H
#define U8G2_U8X8_AVR_H

#include "../lib/U8g2/csrc/u8x8.h"
#include "../lib/U8g2/csrc/u8g2.h"

uint8_t u8x8_avr_gpio_and_delay(
    u8x8_t *u8x8,
    uint8_t msg,
    uint8_t arg_int,
    void *arg_ptr
);

uint8_t u8x8_byte_avr_hw_i2c(
    u8x8_t *u8x8,
    uint8_t msg,
    uint8_t arg_int,
    void *arg_ptr
);

#endif
