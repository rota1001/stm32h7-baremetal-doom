#include <stdio.h>

extern int main(int argc, char **argv);

__attribute__((section(".text.entry"))) doom_entry(void)
{
    // while (1) {
    //     // usart_putc('y');
    //     // usart_putc('e');
    //     // usart_putc('e');
    //     // usart_putc('\r');
    //     // usart_putc('\n');
    //     printf("hello\n");
    // }
    main(0, 0);
}
