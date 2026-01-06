#include <rcc.h>
#include <regs.h>
#include <stdint.h>


#define LCD_WIDTH (640 / 2)
#define LCD_HEIGHT 400

#define HSYNC (96 / 2)
#define HBP (48 / 2)
#define HFP (16 / 2)
#define VSYNC 2
#define VBP (33 + 40)
#define VFP (10 + 40)

struct ltdc_color {
    uint32_t b:8;
    uint32_t g:8;
    uint32_t r:8;
    uint32_t a:8;
};

void ltdc_init(void *frame_buffer);

void ltdc_irq(void);

void ltdc_set_clut(struct ltdc_color *colors);
