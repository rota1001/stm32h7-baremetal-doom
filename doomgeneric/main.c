#include <stdio.h>
#include <regs.h>

extern unsigned long _doom_bss_vma_start, _doom_bss_vma_end,
    _doom_data_vma_start, _doom_data_vma_end, _doom_data_lma;

extern int main(int argc, char **argv);

static void doom_sram_init(void) {
    unsigned long *src = &_doom_data_lma;
    unsigned long *dst = &_doom_data_vma_start;
    while (dst < &_doom_data_vma_end)
        *dst++ = *src++;
    dst = &_doom_bss_vma_start;
    while (dst < &_doom_bss_vma_end)
        *dst++ = 0;
}

static void gpio_init(void)
{
    /**
     * PD8: UP
     * PD9: DOWN
     * PD10: LEFT
     * PD14: RIGHT
     * PD15: ENTER
     * PB15: FIRE
     */
    GPIO_MODER_SET(GPIOD, 8, GPIO_MODER_INPUT);
    GPIO_OSPEEDR_SET(GPIOD, 8, GPIO_OSPEEDR_LOW);
    GPIO_PUPDR_SET(GPIOD, 8, GPIO_PUPDR_PULL_DOWN);
    GPIO_MODER_SET(GPIOD, 9, GPIO_MODER_INPUT);
    GPIO_OSPEEDR_SET(GPIOD, 9, GPIO_OSPEEDR_LOW);
    GPIO_PUPDR_SET(GPIOD, 9, GPIO_PUPDR_PULL_DOWN);
    GPIO_MODER_SET(GPIOD, 10, GPIO_MODER_INPUT);
    GPIO_OSPEEDR_SET(GPIOD, 10, GPIO_OSPEEDR_LOW);
    GPIO_PUPDR_SET(GPIOD, 10, GPIO_PUPDR_PULL_DOWN);
    GPIO_MODER_SET(GPIOD, 14, GPIO_MODER_INPUT);
    GPIO_OSPEEDR_SET(GPIOD, 14, GPIO_OSPEEDR_LOW);
    GPIO_PUPDR_SET(GPIOD, 14, GPIO_PUPDR_PULL_DOWN);
    GPIO_MODER_SET(GPIOD, 15, GPIO_MODER_INPUT);
    GPIO_OSPEEDR_SET(GPIOD, 15, GPIO_OSPEEDR_LOW);
    GPIO_PUPDR_SET(GPIOD, 15, GPIO_PUPDR_PULL_DOWN);
    GPIO_MODER_SET(GPIOB, 15, GPIO_MODER_INPUT);
    GPIO_OSPEEDR_SET(GPIOB, 15, GPIO_OSPEEDR_LOW);
    GPIO_PUPDR_SET(GPIOB, 15, GPIO_PUPDR_PULL_DOWN);
}

__attribute__((section(".text.entry"))) doom_entry(void)
{
    doom_sram_init();
    gpio_init();
    main(0, 0);
}
