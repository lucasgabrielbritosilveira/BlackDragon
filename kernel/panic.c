#include "panic.h"

#include "kprintf.h"

void panic(const char* msg, const char* file, int line) {
  kprintf("KERNEL PANIC - message: %s file: %s - line: %d\n\n", msg, file,
          line);

  while (1) {
    __asm__ volatile("wfe");
  }
}