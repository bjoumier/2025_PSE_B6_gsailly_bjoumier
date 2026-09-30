#include "display.h"

#include "u8g2_u8x8_avr.h"

#include <stdio.h>

u8g2_t u8g2;

void init_display(void)
{
    u8g2_Setup_ssd1315_i2c_128x64_noname_1(
        &u8g2,
        U8G2_R0,
        u8x8_byte_avr_hw_i2c,
        u8x8_avr_gpio_and_delay
    );

    u8g2_InitDisplay(&u8g2);

    u8g2_SetPowerSave(&u8g2, 0);
}

void display_ui(uint16_t bpm, ClockTime time)
{
    char bpm_buffer[32];

    char time_buffer[32];

    sprintf(bpm_buffer, "%u BPM", bpm);

    sprintf(
        time_buffer,
        "%02u:%02u:%02u",
        time.hours,
        time.minutes,
        time.seconds
    );

    u8g2_FirstPage(&u8g2);

    do
    {
        u8g2_SetFont(&u8g2, u8g2_font_6x10_tf);

        u8g2_DrawStr(&u8g2, 10, 15, "Heart Rate");

        u8g2_DrawStr(&u8g2, 10, 35, bpm_buffer);

        u8g2_DrawStr(&u8g2, 10, 55, time_buffer);
    }
    while(u8g2_NextPage(&u8g2));
}
