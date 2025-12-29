#include <rcc.h>
#include <regs.h>
#include <stdint.h>


#define LCD_WIDTH 640
#define LCD_HEIGHT 480

#define HSYNC 96
#define HBP 48
#define HFP 16
#define VSYNC 2
#define VBP 33
#define VFP 10

void ltdc_init(void *frame_buffer);
