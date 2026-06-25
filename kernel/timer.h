#ifndef TIMER_H_
#define TIMER_H_

void timer_init(void);

unsigned long timer_frequency(void);
unsigned long timer_counter(void);

void timer_busy_wait_us(unsigned long us);
void timer_busy_wait_ms(unsigned long ms);

#endif