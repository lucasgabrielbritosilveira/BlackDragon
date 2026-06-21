#include "kprintf.h"

#include <stdarg.h>

#include "../drivers/uart.h"

static void kprint_uint(unsigned long long value) {
  char buffer[32];
  int i = 0;

  if (value == 0) {
    uart_putc('0');
    return;
  }

  while (value > 0) {
    buffer[i++] = '0' + (value % 10);
    value /= 10;
  }

  while (i--) {
    uart_putc(buffer[i]);
  }
}

static void kprint_int(long long value) {
  if (value < 0) {
    uart_putc('-');
    value = -value;
  }

  kprint_uint((unsigned long long)value);
}

static void kprint_hex(unsigned long long value) {
  char buffer[32];
  const char* hex = "0123456789ABCDEF";
  int i = 0;

  if (value == 0) {
    uart_putc('0');
    return;
  }

  while (value > 0) {
    buffer[i++] = hex[value & 0xF];
    value >>= 4;
  }

  while (i--) {
    uart_putc(buffer[i]);
  }
}

static void kprint_string(const char* str) {
  if (!str) {
    str = "(null)";
  }

  while (*str) {
    if (*str == '\n') {
      uart_putc('\r');
    }

    uart_putc(*str++);
  }
}

void kprintf(const char* fmt, ...) {
  va_list args;

  va_start(args, fmt);

  while (*fmt) {
    if (*fmt != '%') {
      if (*fmt == '\n') {
        uart_putc('\r');
      }

      uart_putc(*fmt++);
      continue;
    }

    fmt++;

    if (*fmt == 'l') {
      fmt++;

      switch (*fmt) {
        case 'd':
          kprint_int(va_arg(args, long));
          break;

        case 'u':
          kprint_uint(va_arg(args, unsigned long));
          break;

        case 'x':
          kprint_hex(va_arg(args, unsigned long));
          break;

        default:
          uart_putc('%');
          uart_putc('l');
          uart_putc(*fmt);
          break;
      }

      fmt++;
      continue;
    }

    switch (*fmt) {
      case '%':
        uart_putc('%');
        break;

      case 'c':
        uart_putc((char)va_arg(args, int));
        break;

      case 's':
        kprint_string(va_arg(args, char*));
        break;

      case 'd':
        kprint_int(va_arg(args, int));
        break;

      case 'u':
        kprint_uint(va_arg(args, unsigned int));
        break;

      case 'x':
        kprint_hex(va_arg(args, unsigned int));
        break;

      case 'p':
        uart_puts("0x");

        kprint_hex((unsigned long long)va_arg(args, void*));
        break;

      default:
        uart_putc('%');
        uart_putc(*fmt);
        break;
    }

    fmt++;
  }

  va_end(args);
}