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
    test_libc();

    ((void (*)(void)) 0x90000001)();
    printf("[!] PANIC: Unreachable\n");
    while (1)
        ;
}

void hardfault_handler(void)
{
    printf("hardfault\n");
    while (1)
        ;
}

#define MSP 0
#define RESET 1
#define HARDFAULT 3

__attribute__((section(".isr_vector"))) unsigned long isr_vec[] = {
    [MSP](unsigned long) & _stack_top_on_boot,
    [RESET](unsigned long) reset_isr,
    [HARDFAULT](unsigned long) hardfault_handler};
