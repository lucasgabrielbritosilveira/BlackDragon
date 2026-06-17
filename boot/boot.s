.section ".text.boot"

.global _start
.extern kernel_main

_start:
    mrs x0, mpidr_el1
    and x0, x0, #3
    cbz x0, master_core

halt_core:
    wfe
    b halt_core

master_core:
    ldr x0, =_stack_top
    mov sp, x0
    bl kernel_main

hang:
    wfe
    b hang
    