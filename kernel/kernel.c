#include "../drivers/uart.h"
#include "exception.h"
#include "kprintf.h"
#include "panic.h"
#include "shell.h"

void kernel_main(void) {
  exception_init();

  unsigned long el;

  asm volatile("mrs %0, CurrentEL" : "=r"(el));

  kprintf("CurrentEL=%ld\n", el >> 2);

  shell_run();

  while (1) {
    asm volatile("wfe");
  }
}