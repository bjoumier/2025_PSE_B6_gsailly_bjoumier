#include "usb_comm.h"

#include <string.h>

void usb_init(void)
{
}

void usb_task(void)
{
}

uint8_t usb_read_line(char* buffer, uint8_t max_len)
{
    static uint8_t already_sent = 0;

    if(already_sent)
    {
        return 0;
    }

    strcpy(buffer, "T16:42:10");

    already_sent = 1;

    return 1;
}
