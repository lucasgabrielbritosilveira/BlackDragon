#include "../drivers/uart.h"
#include "panic.h"
void kernel_main(void)
{
    uart_init();

    PANIC("ERROR");
    
    uart_puts("panic\n");

    while (1)
    {
        asm volatile("wfe");
    }
}