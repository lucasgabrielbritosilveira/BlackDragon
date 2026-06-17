#include <stdarg.h>
#include "../drivers/uart.h"
#include "kprintf.h"

static inline void kprint_int(long long num){
    char buffer[32];
    unsigned int i = 0;
    
    if(num == 0) {
        uart_putc('0');
        return;
    }

    if(num < 0) {
        uart_putc('-');
        return;
    }

    while(num > 0){
        buffer[i++] = (num % 10) + '0';
        num /= 10;
        for(int j = i -1; j >= 0; j--){
            uart_putc(buffer[j]);
        }
    }
}

static inline  void kprint_hex(unsigned long long num){
    char buffer[32];
    char hex_digits[] = "0123456789ABCDEF";
    int i = 0;

    if (num == 0) {
        uart_putc('0');
        return;
    }

    while (num > 0) {
        buffer[i++] = hex_digits[num % 16];
        num /= 16;
    }

    for (int j = i - 1; j >= 0; j--) {
        uart_putc(buffer[j]);
    }
}

void kprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    for (int i = 0; fmt[i] != '\0'; i++) {
        if (fmt[i] != '%') {
            if (fmt[i] == '\n') {
                uart_putc('\r'); 
            }
            uart_putc(fmt[i]);
            continue;
        }

        i++; 

        switch (fmt[i]) {
            case '%':
                uart_putc('%');
                break;

            case 'c': { 
                char c = (char)va_arg(args, int);
                uart_putc(c);
                break;
            }

            case 's': { 
                char *s = va_arg(args, char *);
                if (!s) s = "(null)";
                while (*s) {
                    if (*s == '\n') uart_putc('\r');
                    uart_putc(*s++);
                }
                break;
            }

            case 'd': { 
                int d = va_arg(args, int);
                kprint_int(d);
                break;
            }

            case 'x': { 
                unsigned int x = va_arg(args, unsigned int);
                kprint_hex(x);
                break;
            }

            case 'p': { 
                unsigned long long p = va_arg(args, unsigned long long);
                uart_puts("0x");
                kprint_hex(p);
                break;
            }

            default: 
                uart_putc('%');
                uart_putc(fmt[i]);
                break;
        }
    }

    va_end(args);
}