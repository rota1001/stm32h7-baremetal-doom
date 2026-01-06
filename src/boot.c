#include <ltdc.h>
#include <mm.h>
#include <qspi.h>
#include <rcc.h>
#include <regs.h>
#include <stdio.h>
#include <string.h>
#include <test.h>

extern unsigned long _stack_top, _frame_buffer, _data_vma_start, _data_vma_end,
    _data_lma, _bss_vma_start, _bss_vma_end, _stack_top_on_boot;

unsigned long systick_cnt = 0;

static void sram_init(void)
{
    unsigned long *src = &_data_lma;
    unsigned long *dst = &_data_vma_start;
    while (dst < &_data_vma_end)
        *dst++ = *src++;

    dst = &_bss_vma_start;
    while (dst < &_bss_vma_end)
        *dst++ = 0;
}

/**
 * No
 */
static void systick_init(void)
{
    SYSTICK_CSR = 0;
    SYSTICK_RVR = (SYSCLK_FREQ / 1000) - 1;
    SYSTICK_CVR = 0;
    SYSTICK_CSR = SYSTICK_TICKINT | SYSTICK_ENABLE | SYSTICK_CLKSOURCE;
}

static void enable_icache(void)
{
    __asm volatile ("dsb");
    __asm volatile ("isb");
    SCB_ICIALLU = 0;
    __asm volatile ("dsb");
    __asm volatile ("isb");
    SCB_CCR |= (1 << 17);
    __asm volatile ("dsb");
    __asm volatile ("isb");
}


__attribute__((section(".reset_isr"))) void reset_isr()
{
    RCC_AHB2ENR |=
        RCC_AHB2ENR_SRAM1EN | RCC_AHB2ENR_SRAM2EN | RCC_AHB2ENR_SRAM3EN;
    __asm volatile("dsb");
    asm volatile("msr msp, %[msp]" ::[msp] "r"(&_stack_top));
    sram_init();
    rcc_init();
    usart_init(115200);
    ltdc_init((void *) &_frame_buffer);
    qspi_init();
    mm_init();
    enable_icache();
    test_libc();
    systick_init();
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


void systick_handler(void)
{
    systick_cnt++;
}

#define MSP 0
#define RESET 1
#define HARDFAULT 3
#define SYSTICK 15
#define LTDC 104

__attribute__((section(".isr_vector"))) unsigned long isr_vec[] = {
    [MSP](unsigned long) & _stack_top_on_boot,
    [RESET](unsigned long) reset_isr,
    [HARDFAULT](unsigned long) hardfault_handler,
    [SYSTICK](unsigned long) systick_handler,
    [LTDC](unsigned long)ltdc_irq
};
