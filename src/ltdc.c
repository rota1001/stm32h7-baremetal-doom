#include <ltdc.h>

static void ltdc_gpio_init(void)
{
    RCC_AHB4ENR |= RCC_AHB4ENR_GPIOAEN;
    RCC_AHB4ENR |= RCC_AHB4ENR_GPIOBEN;
    RCC_AHB4ENR |= RCC_AHB4ENR_GPIOCEN;
    RCC_AHB4ENR |= RCC_AHB4ENR_GPIODEN;
    RCC_AHB4ENR |= RCC_AHB4ENR_GPIOEEN;

    /**
     * PE14: LTDC_CLK
     * PC6:  LTDC_HSYNC
     * PA4:  LTDC_VSYNC
     * PA12: LTDC_R5
     * PB1:  LTDC_R6
     * PE15: LTDC_R7
     * PB11: LTDC_G5
     * PC7:  LTDC_G6
     * PD3:  LTDC_G7
     * PB8:  LTDC_B6
     * PB9:  LTDC_B7
     */

    GPIO_MODER_SET(GPIOE, 14, GPIO_MODER_ALT);
    GPIO_OSPEEDR_SET(GPIOE, 14, GPIO_OSPEEDR_VERY_HIGH);
    GPIO_AFR_SET(GPIOE, 14, 14);

    GPIO_MODER_SET(GPIOC, 6, GPIO_MODER_ALT);
    GPIO_OSPEEDR_SET(GPIOC, 6, GPIO_OSPEEDR_VERY_HIGH);
    GPIO_AFR_SET(GPIOC, 6, 14);

    GPIO_MODER_SET(GPIOA, 4, GPIO_MODER_ALT);
    GPIO_OSPEEDR_SET(GPIOA, 4, GPIO_OSPEEDR_VERY_HIGH);
    GPIO_AFR_SET(GPIOA, 4, 14);


    GPIO_MODER_SET(GPIOA, 12, GPIO_MODER_ALT);
    GPIO_OSPEEDR_SET(GPIOA, 12, GPIO_OSPEEDR_VERY_HIGH);
    GPIO_AFR_SET(GPIOA, 12, 14);

    GPIO_MODER_SET(GPIOB, 1, GPIO_MODER_ALT);
    GPIO_OSPEEDR_SET(GPIOB, 1, GPIO_OSPEEDR_VERY_HIGH);
    GPIO_AFR_SET(GPIOB, 1, 9);

    GPIO_MODER_SET(GPIOE, 15, GPIO_MODER_ALT);
    GPIO_OSPEEDR_SET(GPIOE, 15, GPIO_OSPEEDR_VERY_HIGH);
    GPIO_AFR_SET(GPIOE, 15, 14);


    GPIO_MODER_SET(GPIOB, 11, GPIO_MODER_ALT);
    GPIO_OSPEEDR_SET(GPIOB, 11, GPIO_OSPEEDR_VERY_HIGH);
    GPIO_AFR_SET(GPIOB, 11, 14);

    GPIO_MODER_SET(GPIOC, 7, GPIO_MODER_ALT);
    GPIO_OSPEEDR_SET(GPIOC, 7, GPIO_OSPEEDR_VERY_HIGH);
    GPIO_AFR_SET(GPIOC, 7, 14);

    GPIO_MODER_SET(GPIOD, 3, GPIO_MODER_ALT);
    GPIO_OSPEEDR_SET(GPIOD, 3, GPIO_OSPEEDR_VERY_HIGH);
    GPIO_AFR_SET(GPIOD, 3, 14);


    GPIO_MODER_SET(GPIOB, 8, GPIO_MODER_ALT);
    GPIO_OSPEEDR_SET(GPIOB, 8, GPIO_OSPEEDR_VERY_HIGH);
    GPIO_AFR_SET(GPIOB, 8, 14);

    GPIO_MODER_SET(GPIOB, 9, GPIO_MODER_ALT);
    GPIO_OSPEEDR_SET(GPIOB, 9, GPIO_OSPEEDR_VERY_HIGH);
    GPIO_AFR_SET(GPIOB, 9, 14);
}

static void ltdc_layer1_init(void *frame_buffer)
{
    unsigned long x_start = HSYNC + HBP;
    unsigned long x_end = HSYNC + HBP + LCD_WIDTH - 1;
    unsigned long y_start = VSYNC + VBP;
    unsigned long y_end = VSYNC + VBP + LCD_HEIGHT - 1;

    LTDC_layer1->WHPCR = (x_end << 16) | x_start;
    LTDC_layer1->WVPCR = (y_end << 16) | y_start;

    LTDC_layer1->PFCR = LTDC_PF_L8;
    LTDC_layer1->CFBAR = (unsigned long) frame_buffer;

    unsigned long pitch = LCD_WIDTH * 1;  // Actual bytes per line
    LTDC_layer1->CFBLR = (pitch << 16) | (pitch + 7);
    LTDC_layer1->CFBLNR = LCD_HEIGHT;

    LTDC_layer1->CACR = 255;

    LTDC_layer1->BFCR = (6UL << 8) | 7;
    LTDC_layer1->CR |= (LTDC_LxCR_LEN | LTDC_LxCR_CLUTEN);
}

void ltdc_config_clut(void)
{
    for (unsigned long i = 0; i < 256; i++) {
        uint8_t r = (i >> 5) & 7;
        uint8_t g = (i >> 2) & 7;
        uint8_t b = i & 3;
        LTDC_layer1->CLUTWR = (i << 24) | (r << (16 + (8 - 3))) |
                              (g << (8 + (8 - 3))) | (b << (8 - 2));
    }
}


void ltdc_set_clut(struct ltdc_color *colors)
{
    for (unsigned long i = 0; i < 256; i++) {
        uint8_t r = (colors[i].r >> 5) & 7;
        uint8_t g = (colors[i].g >> 5) & 7;
        uint8_t b = (colors[i].b >> 6) & 3;
        LTDC_layer1->CLUTWR = (i << 24) | (r << (16 + (8 - 3))) |
                              (g << (8 + (8 - 3))) | (b << (8 - 2));
    }
    LTDC->SRCR = LTDC_SRCR_IMR;
}

extern unsigned long _frame_buffer;
unsigned long current;

void ltdc_init(void *frame_buffer)
{
    RCC_APB3ENR |= RCC_APB3ENR_LTDCEN;
    volatile unsigned long dummy = RCC_APB3ENR;
    (void) dummy;
    RCC_APB3RSTR |= RCC_APB3RSTR_LTDCRST;
    for (volatile int i = 0; i < 1000; i++)
        ;
    RCC_APB3RSTR &= ~RCC_APB3RSTR_LTDCRST;
    RCC_D3CCIPR &= ~RCC_D3CCIPR_LTDCSEL_MASK;

    pll3_init();
    ltdc_gpio_init();

    LTDC->GCR &= ~LTDC_GCR_LTDCEN;
    LTDC->GCR &= ~(LTDC_GCR_HSPOL | LTDC_GCR_VSPOL);
    LTDC->GCR |= LTDC_GCR_PCPOL;

    LTDC->SSCR = ((HSYNC - 1) << 16) | (VSYNC - 1);
    LTDC->BPCR = ((HSYNC + HBP - 1) << 16) | (VSYNC + VBP - 1);
    LTDC->AWCR =
        ((HSYNC + HBP + LCD_WIDTH - 1) << 16) | (VSYNC + VBP + LCD_HEIGHT - 1);
    LTDC->TWCR = ((HSYNC + HBP + LCD_WIDTH + HFP - 1) << 16) |
                 (VSYNC + VBP + LCD_HEIGHT + VFP - 1);

    LTDC->BCCR = 0;  // background color

    LTDC->GCR &= ~(1 << 16);
    LTDC->GCR |= LTDC_GCR_LTDCEN;
    ltdc_layer1_init(frame_buffer);
    ltdc_config_clut();
    LTDC->LIPCR = VSYNC + VBP;
    current = VSYNC + VBP;
    LTDC->IER |= LTDC_IER_LIE;
    LTDC->SRCR = LTDC_SRCR_IMR;
    NVIC_IPR[88] = 0;
    NVIC_ISER2 = (1 << 24);
}



void ltdc_irq(void)
{
    if (LTDC->ISR & LTDC_ISR_LIF) {
        LTDC->ICR = LTDC_ICR_CLIF;
        LTDC_layer1->CFBAR = ((unsigned long)&_frame_buffer) - LCD_WIDTH * ((current - VSYNC - VBP) >> 1);
        if (current == VSYNC + VBP + LCD_HEIGHT - 1) {
            current = VSYNC + VBP;
        } else {
            current++;
        }
        LTDC->LIPCR = current;
        LTDC->SRCR = LTDC_SRCR_IMR;
    }
}
