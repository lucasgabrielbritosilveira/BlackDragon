#include "../drivers/uart.h"
#include "panic.h"
#include "shell.h"

void kernel_main(void)
{
    shell_run();

    while (1)
    {
        asm volatile("wfe");
    }
}