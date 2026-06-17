#include "panic.h"
#include "kprintf.h"



void panic(const char *msg, const char *file, int line) {
    // Desativa as interrupções aqui no futuro (quando as implementares)
    
    kprintf("KERNEL PANIC - message: %s file: %s - line: %d\n\n", msg, file, line);
    
    while (1) {
        __asm__ volatile("wfe"); 
    }
}