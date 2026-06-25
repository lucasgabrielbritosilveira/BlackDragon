#include "timer.h"

static unsigned long global_timer_frequency = 0;

static inline unsigned long read_cntfrq_el0(void) {
  unsigned long value;
  asm volatile("mrs %0, cntfrq_el0" : "=r"(value));
  return value;
}

static inline unsigned long read_cntpct_el0(void) {
  unsigned long value;
  asm volatile("mrs %0, cntpct_el0" : "=r"(value));
  return value;
}

void timer_init(void) { global_timer_frequency = read_cntfrq_el0(); }

unsigned long timer_frequency(void) { return global_timer_frequency; }

unsigned long timer_counter(void) { return read_cntpct_el0(); }

void timer_busy_wait_us(unsigned long us) {
  unsigned long start;
  unsigned long ticks;
  unsigned long target;

  if (global_timer_frequency == 0) {
    timer_init();
  }

  start = timer_counter();
  ticks = (global_timer_frequency * us) / 1000000UL;
  target = start + ticks;

  while (timer_counter() < target) {
    asm volatile("nop");
  }
}

void timer_busy_wait_ms(unsigned long ms) { timer_busy_wait_us(ms * 1000UL); }