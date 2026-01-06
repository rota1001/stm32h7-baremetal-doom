# STM32H7 Bare Metal Doom
This is a port of [doomgeneric](https://github.com/ozkl/doomgeneric) running on STM32H750 without **ST HAL Library**.
## Demo
https://www.youtube.com/watch?v=qiCk467Afwo

## Where's the challenge?
In comparison to STM32F429(, which most of the STM32 DOOM run on), STM32H750 has more constrained flash and SRAM. Although the external DRAM can be added through FMC, the required pins are not exposed on my board.
Thus, I only have 128KB internal flash, totally 1MB SRAM fragments and 16MB QSPI flash.

## Features
- **Without HAL:** Pure register-level programming.
- **VGA Video Output:** Generates 640x480 @ 60Hz VGA signal using only 320x200 framebuffer and LTDC support (and a handmade DAC).
- **Bootloader:** Codes in internal flash shift the control to the main code in 16MB QUAD SPI flash
- **Multi-zone Memory Management:** Extend the original zone implement to support non-contiguous memory zones.

## License
This project is licensed under **GPLv2**.
