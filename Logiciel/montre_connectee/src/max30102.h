#ifndef MAX30102_H
#define MAX30102_H

#include <stdint.h>

void max30102_init(void);

uint32_t max30102_read_ir(void);

uint8_t max30102_detect_beat(uint32_t ir_value);

uint16_t max30102_get_bpm(void);

void max30102_update_sample(void);

uint32_t max30102_get_latest(void);

#endif
