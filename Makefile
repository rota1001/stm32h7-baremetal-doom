CROSS_COMPILE ?= arm-linux-gnueabi-
.PHONY: all clean

SRCS = src/boot.c src/rcc.c src/usart.c src/ltdc.c src/qspi.c
LINKER = linker.ld

OBJS = $(SRCS:%.c=build/%.o)

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy


CFLAGS = -mcpu=cortex-m7 -mthumb
CFLAGS += -nostartfiles -g -nostdlib -ffreestanding
CFLAGS += -Iinclude

all: build/firmware.bin

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/firmware.out: $(OBJS) $(LINKER)
	$(LD) -T $(LINKER) -Map=build/main.map -o $@ $(OBJS)

build/firmware.bin: build/firmware.out
	$(OBJCOPY) -O binary $< $@

flash: build/firmware.bin
	st-flash --reset write build/firmware.bin 0x8000000

clean:
	rm -rf build/*
