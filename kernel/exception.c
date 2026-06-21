#include "exception.h"

#include "kprintf.h"
#include "panic.h"

extern char vectors[];

void exception_init(void) {
  asm volatile("msr vbar_el1, %0" : : "r"(vectors) : "memory");
  asm volatile("isb");
}

static inline unsigned long read_esr_el1(void) {
  unsigned long value;
  asm volatile("mrs %0, esr_el1" : "=r"(value));
  return value;
}

static inline unsigned long read_elr_el1(void) {
  unsigned long value;
  asm volatile("mrs %0, elr_el1" : "=r"(value));
  return value;
}

static inline unsigned long read_far_el1(void) {
  unsigned long value;
  asm volatile("mrs %0, far_el1" : "=r"(value));
  return value;
}

void sync_exception_handler(void) {
  unsigned long esr = read_esr_el1();
  unsigned long ec = (esr >> 26) & 0x3F;
  kprintf("ExceptionClass = 0x%lx\n", ec);
  kprintf("ESR_EL1 0x%lx\n", esr);
  kprintf("ELR_EL1 0x%lx\n", read_elr_el1());
  kprintf("READ_FAR_EL1 0x%lx\n", read_far_el1());
  PANIC("CPU Exception");
}

void irq_exception_handler(void) { kprintf("IRQ Exception \n"); }
