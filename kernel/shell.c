#include "shell.h"

#include "../drivers/uart.h"
#include "kprintf.h"
#include "panic.h"

#define BUFFER_SIZE 64

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