#include "timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint32_t g_millis = 0;

void timer_init(void)
{
    // Timer1 CTC
    TCCR1B |= (1 << WGM12);

    // Prescaler 64
    TCCR1B |= (1 << CS11) | (1 << CS10);

    // 8MHz / 64 = 125000 Hz
    // 125000 / 1000 = 125
    OCR1A = 124;

    // interruption compare A
    TIMSK1 |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect)
{
    g_millis++;
}

uint32_t millis(void)
{
    uint32_t ms;

    cli();
    ms = g_millis;
    sei();

    return ms;
}
