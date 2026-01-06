CROSS_COMPILE ?= arm-none-eabi-
.PHONY: all clean

SRCS = src/boot.c src/rcc.c src/usart.c src/ltdc.c src/qspi.c \
	   src/libc.c src/test.c src/mm.c
LINKER = linker.ld

OBJS = $(SRCS:%.c=build/%.o)

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy


CFLAGS = -mcpu=cortex-m7 -mthumb
CFLAGS += -nostartfiles -g -nostdlib -ffreestanding
CFLAGS += -Iinclude -Idoomgeneric

all: build/firmware.bin

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/firmware.out: $(OBJS) $(LINKER)
	$(CC) $(CFLAGS) -T $(LINKER) -Wl,-Map=build/main.map -Wl,--gc-sections -o $@ \
	-Wl,--start-group $(OBJS) -lgcc -lc -lm -Wl,--end-group

build/firmware.bin: build/firmware.out
	$(OBJCOPY) -O binary $< $@

flash: build/firmware.bin
	st-flash --reset write build/firmware.bin 0x8000000

flash-dummy: dummy.bin
	st-flash --reset write dummy.bin 0x8000000

doom:
	make -C doomgeneric clean
	make -C doomgeneric


flash-doom: doomgeneric/build/doom.bin doom
	make flash-dummy
	minipro -p 'W25Q128JV@SOIC8' --spi_clock=30 -w doomgeneric/build/doom.bin -s
	make flash

clean:
	rm -rf build/*
