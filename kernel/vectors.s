.section .text.vectors

.align 11

.global vectors

vectors:

/* 0x000 Sync Current EL SP0 */
b sync_handler
.space 124

/* 0x080 IRQ Current EL SP0 */
b irq_handler
.space 124

/* 0x100 FIQ Current EL SP0 */
b fiq_handler
.space 124

/* 0x180 SError Current EL SP0 */
b serr_handler
.space 124

/* 0x200 Sync Current EL SPx */
b sync_handler
.space 124

/* 0x280 IRQ Current EL SPx */
b irq_handler
.space 124

/* 0x300 FIQ Current EL SPx */
b fiq_handler
.space 124

/* 0x380 SError Current EL SPx */
b serr_handler
.space 124

/* 0x400 Sync Lower EL AArch64 */
b sync_handler
.space 124

/* 0x480 IRQ Lower EL AArch64 */
b irq_handler
.space 124

/* 0x500 FIQ Lower EL AArch64 */
b fiq_handler
.space 124

/* 0x580 SError Lower EL AArch64 */
b serr_handler
.space 124

/* 0x600 Sync Lower EL AArch32 */
b sync_handler
.space 124

/* 0x680 IRQ Lower EL AArch32 */
b irq_handler
.space 124

/* 0x700 FIQ Lower EL AArch32 */
b fiq_handler
.space 124

/* 0x780 SError Lower EL AArch32 */
b serr_handler
.space 124


sync_handler:
    bl sync_exception_handler
1:
    b 1b

irq_handler:
    bl irq_exception_handler
1:
    b 1b

fiq_handler:
1:
    b 1b

serr_handler:
1:
    b 1b
    