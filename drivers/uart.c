#include "uart.h"

#define UART0_BASE 0x3F201000UL

#define UART_DR    (*(volatile unsigned int *)(UART0_BASE + 0x00))
#define UART_FR    (*(volatile unsigned int *)(UART0_BASE + 0x18))
#define UART_IBRD  (*(volatile unsigned int *)(UART0_BASE + 0x24))
#define UART_FBRD  (*(volatile unsigned int *)(UART0_BASE + 0x28))
#define UART_LCRH  (*(volatile unsigned int *)(UART0_BASE + 0x2C))
#define UART_CR    (*(volatile unsigned int *)(UART0_BASE + 0x30))
#define UART_IMSC  (*(volatile unsigned int *)(UART0_BASE + 0x38))
#define UART_ICR   (*(volatile unsigned int *)(UART0_BASE + 0x44))

void uart_init(void)
{
    UART_CR = 0;
    UART_ICR = 0x7FF;
    UART_IBRD = 1;
    UART_FBRD = 40;
    UART_LCRH = (1 << 4) | (3 << 5);
    UART_CR = (1 << 0) | (1 << 8) | (1 << 9);
}

void uart_putc(char c)
{
    while (UART_FR & (1 << 5))
    {
    
    }

    UART_DR = (unsigned int)c;
}

void uart_puts(const char *str)
{
    while (*str)
    {
        if (*str == '\n')
        {
            uart_putc('\r');
        }

        uart_putc(*str++);
    }
}

char uart_getc(void){
    while(UART_FR & (1 << 4));

    return (char)(UART_DR & 0xFF);
}