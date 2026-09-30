#ifndef TIMEKEEPER_H
#define TIMEKEEPER_H

#include <stdint.h>

typedef struct
{
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;

} ClockTime;

void timekeeper_init(void);

void timekeeper_update(void);

void timekeeper_set(uint8_t hours, uint8_t minutes, uint8_t seconds);

ClockTime timekeeper_get(void);

#endif
