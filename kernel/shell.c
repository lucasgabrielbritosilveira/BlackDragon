#include "shell.h"

#include "../drivers/uart.h"
#include "kprintf.h"
#include "panic.h"
#include "timer.h"

#define BUFFER_SIZE 64

static int starts_with(const char* str, const char* prefix) {
  while (*prefix) {
    if (*str != *prefix) {
      return 0;
    }

    str++;
    prefix++;
  }

  return 1;
}

static int parse_ulong(const char* str, unsigned long* out_value) {
  unsigned long value = 0;

  if (!str || !*str) {
    return 0;
  }

  while (*str) {
    if (*str < '0' || *str > '9') {
      return 0;
    }

    value = (value * 10UL) + (unsigned long)(*str - '0');
    str++;
  }

  *out_value = value;
  return 1;
}
static int kstrcmp(const char* s1, const char* s2) {
  while (*s1 && (*s1 == *s2)) {
    s1++;
    s2++;
  }
  return *(unsigned char*)s1 - *(unsigned char*)s2;
}

static void proccess_command(const char* buffer) {
  if (kstrcmp(buffer, "help") == 0) {
    kprintf("Okay\n");
  } else if (kstrcmp(buffer, "break") == 0) {
    kprintf("Breaking... \n");
    asm volatile("brk #0");
  } else if (kstrcmp(buffer, "kprintf") == 0) {
    unsigned long el = 2;
    unsigned long esr = 0x123456789ABCDEF0UL;

    kprintf("EL=%ld\n", el);
    kprintf("ESR=%lx\n", esr);

    kprintf("INT=%d\n", -123);
    kprintf("UINT=%u\n", 123);

    kprintf("HEX=%x\n", 0xDEADBEEF);
    kprintf("PTR=%p\n", (void*)0x80000);
  } else if (kstrcmp(buffer, "timer") == 0) {
    kprintf("Timer frequency: %lu\n", timer_frequency());
    kprintf("Timer counter:   %lu\n", timer_counter());
  } else if (starts_with(buffer, "sleep ")) {
    unsigned long ms = 0;
    const char* arg = buffer + 6;

    if (!parse_ulong(arg, &ms)) {
      kprintf("Invalid number\n");
      return;
    }

    kprintf("Sleeping %lu ms...\n", ms);
    timer_busy_wait_ms(ms);
    kprintf("Done\n");
  } else {
    kprintf("Unknown command\n");
  }
}

void shell_run(void) {
  kprintf("\033[34m");
  kprintf("  ____  _            _      ____                                \n");
  kprintf(
      " | __ )| | __ _  ___| | __ |  _ \\ _ __ __ _  __ _  ___  _ __    \n");
  kprintf(
      " |  _ \\| |/ _` |/ __| |/ / | | | | '__/ _` |/ _` |/ _ \\| '_ \\   \n");
  kprintf(" | |_) | | (_| | (__|   <  | |_| | | | (_| | (_| | (_) | | | |  \n");
  kprintf(
      " |____/|_|\\__,_|\\___|_|\\_\\ |____/|_|  \\__,_|\\__, |\\___/|_| |_|  "
      "\n");
  kprintf("                                            |___/               \n");
  kprintf("\033[0m");  // Reseta a cor

  kprintf("> ");
  char buffer[BUFFER_SIZE];
  int index = 0;

  while (1) {
    char c = uart_getc();

    if (c == '\n' || c == '\r') {
      buffer[index] = '\0';
      kprintf("\n");

      proccess_command(buffer);

      index = 0;
      kprintf("> ");
    }

    else if (c == 127 || c == '\b') {
      if (index > 0) {
        index--;
        uart_putc('\b');
        uart_putc(' ');
        uart_putc('\b');
      }
    } else if (index < BUFFER_SIZE - 1) {
      buffer[index++] = c;
      uart_putc(c);
    }
  }
}