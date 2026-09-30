#include "max30102.h"
#include "i2c.h"
#include "timer.h"
#include <util/delay.h>

#define MAX30102_ADDR 0x57

#define REG_MODE_CONFIG      0x09
#define REG_SPO2_CONFIG      0x0A
#define REG_LED1_PA          0x0C
#define REG_LED2_PA          0x0D
#define REG_FIFO_DATA        0x07
#define REG_FIFO_CONFIG      0x08

#define BUFFER_SIZE 32

static uint32_t last_beat = 0;
static uint16_t bpm = 0;
static uint8_t beat_detected = 0;

static uint32_t ir_buffer[BUFFER_SIZE];
static uint8_t ir_index = 0;

static void write_reg(uint8_t reg, uint8_t value)
{
    i2c_start((MAX30102_ADDR << 1) | 0);

    i2c_write(reg);
    i2c_write(value);

    i2c_stop();
}

void max30102_init()
{
    // Reset
    write_reg(0x09, 0x40);
    _delay_ms(100);

    // FIFO pointers reset
    write_reg(0x04, 0x00);
    write_reg(0x05, 0x00);
    write_reg(0x06, 0x00);

    // FIFO config
    write_reg(0x08, 0x0F);

    // SPO2 mode
    write_reg(0x09, 0x03);

    // SPO2 config:
    // ADC range = 4096nA
    // Sample rate = 100Hz
    // Pulse width = 411us (18 bits)
    write_reg(0x0A, 0x27);

    // LED power
    write_reg(0x0C, 0xFF);
    write_reg(0x0D, 0xFF); // RED
    write_reg(0x0D, 0x24); // IR

    _delay_ms(100);
}

uint32_t max30102_read_ir(void)
{
    uint8_t temp[6];

    i2c_start((MAX30102_ADDR << 1) | 0);
    i2c_write(REG_FIFO_DATA);

    i2c_start((MAX30102_ADDR << 1) | 1);

    for(uint8_t i = 0; i < 5; i++)
    {
        temp[i] = i2c_read_ack();
    }

    temp[5] = i2c_read_nack();

    i2c_stop();

    // RED sample
    uint32_t red =
        ((uint32_t)temp[0] << 16) |
        ((uint32_t)temp[1] << 8)  |
        temp[2];

    // IR sample
    uint32_t ir =
        ((uint32_t)temp[3] << 16) |
        ((uint32_t)temp[4] << 8)  |
        temp[5];

    ir &= 0x03FFFF;

    return ir;
}

uint8_t max30102_detect_beat(uint32_t ir)
{
    static uint32_t threshold = 50000;
    static uint8_t state = 0;

    if(ir > threshold && state == 0)
    {
        state = 1;

        uint32_t now = millis();
        uint32_t delta = now - last_beat;
        last_beat = now;

        if(delta > 300 && delta < 2000)
        {
            uint16_t new_bpm = 60000 / delta;

            // lissage BPM
            bpm = (bpm * 3 + new_bpm) / 4;
        }

        return 1;
    }

    if(ir < threshold)
    {
        state = 0;
    }

    return 0;
}

uint16_t max30102_get_bpm(void)
{
    return bpm;
}

void max30102_update_sample(void)
{
    uint32_t ir = max30102_read_ir();

    ir_buffer[ir_index++] = ir;

    if (ir_index >= BUFFER_SIZE)
        ir_index = 0;
}

uint32_t max30102_get_latest(void)
{
    if (ir_index == 0)
        return ir_buffer[BUFFER_SIZE - 1];

    return ir_buffer[ir_index - 1];
}
