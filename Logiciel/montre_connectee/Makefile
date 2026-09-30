CC = avr-gcc

MCU = atmega32u4
F_CPU = 8000000UL

CFLAGS = -Os \
		  -flto \
          -mmcu=$(MCU) \
          -DF_CPU=$(F_CPU) \
          -ffunction-sections \
          -fdata-sections \
          -I./lib/U8g2/csrc \
          -Isrc

LDFLAGS = -mmcu=$(MCU) -flto\
           -Wl,--gc-sections

OBJCOPY = avr-objcopy

AVRDUDE = avrdude
PORT = /dev/ttyACM0

U8G2_DIR = lib/U8g2/csrc

U8G2_SRCS = \
$(U8G2_DIR)/u8g2_setup.c \
$(U8G2_DIR)/u8g2_buffer.c \
$(U8G2_DIR)/u8g2_hvline.c \
$(U8G2_DIR)/u8g2_ll_hvline.c \
$(U8G2_DIR)/u8g2_font.c \
$(U8G2_DIR)/u8g2_fonts.c \
$(U8G2_DIR)/u8g2_d_memory.c \
$(U8G2_DIR)/u8g2_d_setup.c \
$(U8G2_DIR)/u8g2_cleardisplay.c \
$(U8G2_DIR)/u8g2_intersection.c \
$(U8G2_DIR)/u8x8_display.c \
$(U8G2_DIR)/u8x8_setup.c \
$(U8G2_DIR)/u8x8_cad.c \
$(U8G2_DIR)/u8x8_byte.c \
$(U8G2_DIR)/u8x8_gpio.c \
$(U8G2_DIR)/u8x8_string.c \
$(U8G2_DIR)/u8x8_8x8.c \
$(U8G2_DIR)/u8x8_fonts.c \
$(U8G2_DIR)/u8x8_u8toa.c \
$(U8G2_DIR)/u8x8_u16toa.c \
$(U8G2_DIR)/u8x8_d_ssd1315_128x64_noname.c

SRCS = \
src/main.c \
src/display.c \
src/u8g2_u8x8_avr.c \
src/i2c.c \
src/timer.c \
src/timekeeper.c \
src/usb_comm.c \
src/max30102.c \
src/uart.c

OBJS = $(SRCS:.c=.o)
U8G2_OBJS = $(U8G2_SRCS:.c=.o)

all: main.hex

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

main.elf: $(OBJS) $(U8G2_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

main.hex: main.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

upload: main.hex
	sudo dfu-programmer atmega32u4 erase
	sudo dfu-programmer atmega32u4 flash main.hex
	sudo dfu-programmer atmega32u4 reset

clean:
	rm -f $(OBJS) $(U8G2_OBJS) main.elf main.hex
