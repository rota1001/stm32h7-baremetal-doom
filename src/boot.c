#include <ltdc.h>
#include <qspi.h>
#include <rcc.h>
#include <regs.h>
#include <usart.h>

extern unsigned long _stack_top;
extern unsigned long _axi_sram;



__attribute__((section(".reset_isr"))) void reset_isr()
{
    rcc_init();
    ltdc_init((void *) &_axi_sram);
    qspi_init();
    usart_init(115200);
    for (unsigned long i = 0; i < 307200; i++) {
        ((volatile char *) &_axi_sram)[i] = 3;
    }

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
