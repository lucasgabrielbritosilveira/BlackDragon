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
    bl down_to_el1

kernel_hang:
    wfe
    b kernel_hang

down_to_el1:
    ldr x0, =0x80000000
    msr hcr_el2, x0
    ldr x0, =_stack_top
    msr sp_el1, x0
    mov x0, #0x3C5
    msr spsr_el2, x0
    adr x0, el1_entry
    msr elr_el2, x0
    eret

el1_entry:
    ldr x0, =_stack_top
    mov sp, x0
    bl kernel_main

secondary_hang:
    wfe
    b secondary_hang
