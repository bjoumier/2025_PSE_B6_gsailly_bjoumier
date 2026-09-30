#ifndef DISPLAY_H
#define DISPLAY_H

#include "../lib/U8g2/csrc/u8g2.h"

#include "timekeeper.h"

extern u8g2_t u8g2;

void init_display(void);

void display_ui(uint16_t bpm, ClockTime time);

#endif
