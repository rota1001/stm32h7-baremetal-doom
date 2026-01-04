#include <ltdc.h>
#include <mm.h>
#include <qspi.h>
#include <rcc.h>
#include <regs.h>
#include <stdio.h>
#include <string.h>
#include <test.h>

extern unsigned long _stack_top, _axi_sram, _data_vma_start, _data_vma_end,
    _data_lma, _bss_vma_start, _bss_vma_end, _stack_top_on_boot;

void sram_init(void)
{
    RCC_AHB2ENR |=
        RCC_AHB2ENR_SRAM1EN | RCC_AHB2ENR_SRAM2EN | RCC_AHB2ENR_SRAM3EN;
    __asm volatile("dsb");
    unsigned long *src = &_data_lma;
    unsigned long *dst = &_data_vma_start;
    while (dst < &_data_vma_end)
        *dst++ = *src++;

    dst = &_bss_vma_start;
    while (dst < &_bss_vma_end)
        *dst++ = 0;
}

__attribute__((section(".reset_isr"))) void reset_isr()
{
    sram_init();
    asm volatile("msr msp, %[msp]" ::[msp] "r"(&_stack_top));
    rcc_init();
    ltdc_init((void *) &_axi_sram);
    qspi_init();
    usart_init(115200);
    mm_init();
    for (unsigned long i = 0; i < 307200; i++)
        ((volatile unsigned char *) &_axi_sram)[i] = ((i >> 3) & 7) << 5;

    volatile uint8_t *flash_ptr = (uint8_t *) 0x90000000;

    while (1) {
        for (int i = 0; i < 32; i++) {
            print_uint(flash_ptr[i]);
            usart_putc('\r');
            usart_putc('\n');
        }
    }
    while (1) {
        usart_putc('h');
        usart_putc('e');
        usart_putc('l');
        usart_putc('l');
        usart_putc('o');
        usart_putc('\r');
        usart_putc('\n');
    }

    while (1)
        ;
}

#define MSP 0
#define RESET 1

__attribute__((section(".isr_vector"))) unsigned long isr_vec[] =
    {[MSP](unsigned long) & _stack_top, [RESET](unsigned long) reset_isr};
