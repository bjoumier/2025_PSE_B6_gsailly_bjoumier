#include "timekeeper.h"

#include "timer.h"

static ClockTime g_time = {12, 0, 0};

static uint32_t g_last_second = 0;

void timekeeper_init(void)
{
    g_last_second = millis();
}

void timekeeper_update(void)
{
    if(millis() - g_last_second >= 1000)
    {
        g_last_second += 1000;

        g_time.seconds++;

        if(g_time.seconds >= 60)
        {
            g_time.seconds = 0;

            g_time.minutes++;

            if(g_time.minutes >= 60)
            {
                g_time.minutes = 0;

                g_time.hours++;

                if(g_time.hours >= 24)
                {
                    g_time.hours = 0;
                }
            }
        }
    }
}

ClockTime timekeeper_get(void)
{
    return g_time;
}

void timekeeper_set(
    uint8_t hours,
    uint8_t minutes,
    uint8_t seconds
)
{
    g_time.hours = hours % 24;

    g_time.minutes = minutes % 60;

    g_time.seconds = seconds % 60;
}
