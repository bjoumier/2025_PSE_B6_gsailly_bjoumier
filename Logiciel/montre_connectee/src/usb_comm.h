#ifndef USB_COMM_H
#define USB_COMM_H

#include <stdint.h>

void usb_init(void);

void usb_task(void);

uint8_t usb_read_line(char* buffer, uint8_t max_len);

#endif
