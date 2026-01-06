CROSS_COMPILE ?= arm-none-eabi-
.PHONY: all clean

SRCS = dummy.c am_map.c doomdef.c doomstat.c dstrings.c d_event.c d_items.c d_iwad.c d_loop.c d_main.c d_mode.c d_net.c f_finale.c f_wipe.c g_game.c hu_lib.c hu_stuff.c info.c i_cdmus.c i_endoom.c i_joystick.c i_scale.c i_sound.c i_system.c i_timer.c memio.c m_argv.c m_bbox.c m_cheat.c m_config.c m_controls.c m_fixed.c m_menu.c m_misc.c m_random.c p_ceilng.c p_doors.c p_enemy.c p_floor.c p_inter.c p_lights.c p_map.c p_maputl.c p_mobj.c p_plats.c p_pspr.c p_saveg.c p_setup.c p_sight.c p_spec.c p_switch.c p_telept.c p_tick.c p_user.c r_bsp.c r_data.c r_draw.c r_main.c r_plane.c r_segs.c r_sky.c r_things.c sha1.c sounds.c statdump.c st_lib.c st_stuff.c s_sound.c tables.c v_video.c wi_stuff.c w_checksum.c w_file.c w_main.c w_wad.c z_zone.c w_file_stdc.c i_input.c i_video.c doomgeneric.c
SRCS += doomgeneric_stm32.c
SRCS += main.c
LINKER = linker.ld

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy

FIRMWARE = ../build/firmware.out
# DOOMSRCS = dummy.c am_map.c doomdef.c doomstat.c dstrings.c d_event.c d_items.c d_iwad.c d_loop.c d_main.c d_mode.c d_net.c f_finale.c f_wipe.c g_game.c hu_lib.c hu_stuff.c info.c i_cdmus.c i_endoom.c i_joystick.c i_scale.c i_sound.c i_system.c i_timer.c memio.c m_argv.c m_bbox.c m_cheat.c m_config.c m_controls.c m_fixed.c m_menu.c m_misc.c m_random.c p_ceilng.c p_doors.c p_enemy.c p_floor.c p_inter.c p_lights.c p_map.c p_maputl.c p_mobj.c p_plats.c p_pspr.c p_saveg.c p_setup.c p_sight.c p_spec.c p_switch.c p_telept.c p_tick.c p_user.c r_bsp.c r_data.c r_draw.c r_main.c r_plane.c r_segs.c r_sky.c r_things.c sha1.c sounds.c statdump.c st_lib.c st_stuff.c s_sound.c tables.c v_video.c wi_stuff.c w_checksum.c w_file.c w_main.c w_wad.c z_zone.c w_file_stdc.c i_input.c i_video.c doomgeneric.c
# DOOMSRCS += doomgeneric_stm32.c
# SRCS += $(DOOMSRCS:%.c=doomgeneric/%.c)

OBJS = $(SRCS:%.c=build/%.o)

CFLAGS = -mcpu=cortex-m7 -mthumb
CFLAGS += -nostartfiles -g -nostdlib -ffreestanding
CFLAGS += -I../include

all: build/doom.bin

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/doom1_wad.o: doom1.wad
	arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm \
    --rename-section .data=.rodata,alloc,load,readonly,data,contents \
    doom1.wad build/doom1_wad.o

build/doom.out: $(OBJS) $(LINKER) build/doom1_wad.o
	$(CC) $(CFLAGS) -T $(LINKER) -Wl,-Map=build/main.map -Wl,--gc-sections -o $@ \
	-Wl,--start-group $(OBJS) build/doom1_wad.o -lgcc -Wl,--end-group -Wl,--just-symbols=$(FIRMWARE)

build/doom.bin: build/doom.out
	$(OBJCOPY) -O binary $< $@


clean:
	rm -rf build/*
