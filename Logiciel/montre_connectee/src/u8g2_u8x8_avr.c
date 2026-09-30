#include "u8g2_u8x8_avr.h"

#include <avr/io.h>
#include <util/delay.h>

#include "i2c.h"

uint8_t u8x8_avr_gpio_and_delay(
    u8x8_t *u8x8,
    uint8_t msg,
    uint8_t arg_int,
    void *arg_ptr
)
{
    switch(msg)
    {
        case U8X8_MSG_GPIO_AND_DELAY_INIT:

            i2c_init();

            break;

        case U8X8_MSG_DELAY_MILLI:

            while(arg_int--)
            {
                _delay_ms(1);
            }

            break;
    }

    return 1;
}

uint8_t u8x8_byte_avr_hw_i2c(
    u8x8_t *u8x8,
    uint8_t msg,
    uint8_t arg_int,
    void *arg_ptr
)
{
    uint8_t *data;

    switch(msg)
    {
        case U8X8_MSG_BYTE_SEND:

            data = (uint8_t *)arg_ptr;

            while(arg_int > 0)
            {
                i2c_write(*data);
                data++;
                arg_int--;
            }

            break;

        case U8X8_MSG_BYTE_INIT:

            i2c_init();

            break;

        case U8X8_MSG_BYTE_START_TRANSFER:

            i2c_start(u8x8_GetI2CAddress(u8x8));

            break;

        case U8X8_MSG_BYTE_END_TRANSFER:

            i2c_stop();

            break;

        default:
            return 0;
    }

    return 1;
}
