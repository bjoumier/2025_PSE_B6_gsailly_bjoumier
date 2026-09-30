#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "display.h"
#include "i2c.h"
#include "max30102.h"
#include "timekeeper.h"
#include "timer.h"
#include "uart.h"
#include "usb_comm.h"

int main(void)
{
    CLKSEL0 = 0b00010101;   // sélection de l'horloge externe
    CLKSEL1 = 0b00001111;   // minimum de 8Mhz
    CLKPR = 0b10000000;     // modification du diviseur d'horloge (CLKPCE=1)
    CLKPR = 0;              // 0 pour pas de diviseur (diviseur de 1)


	// Init modules
    init_display();
    timer_init();
    timekeeper_init();
    uart_init(9600); //9600 = baudrate par défaut HC-05
    usb_init(); // garde USB actif (si dispo)
    max30102_init(); // capteur BPM

    sei();

	// Heure par défaut
    timekeeper_set(14, 30, 0); 
    
    uint32_t last_display_update = 0;

    uint16_t bpm = 80;

	// Buffer Bluetooth
    static char buffer[32];
    static uint8_t index = 0;

    while(1)
    {
		usb_task();
    	
        timekeeper_update();
       
         // Réception Bluetooth
        if(uart_available())
        {
            char c = uart_read();

            if(c == '\n' || c == '\r')
            {
                buffer[index] = '\0';

                uint8_t h;
                uint8_t m;
                uint8_t s;

                if(sscanf(buffer, "T%hhu:%hhu:%hhu", &h, &m, &s) == 3)
                {
                    timekeeper_set(h, m, s);
                }

                index = 0;
            }
            else
            {
                if(index < sizeof(buffer) - 1)
                {
                    buffer[index++] = c;
                }
            }
        }
        // Rafraîchissement écran à 5 FPS
        if(millis() - last_display_update >= 200)
        {
            last_display_update += 200;

            display_ui(
                bpm,
                timekeeper_get()
            );
        }
    }

    return 0;
}
