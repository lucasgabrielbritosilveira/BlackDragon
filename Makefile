CROSS_COMPILE = aarch64-linux-gnu-

CC      = $(CROSS_COMPILE)gcc
LD      = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy

BUILD_DIR  = build
OUTPUT_DIR = output

ELF = $(OUTPUT_DIR)/kernel8.elf
IMG = $(OUTPUT_DIR)/kernel8.img

CFLAGS = \
    -Wall \
    -Wextra \
    -ffreestanding \
    -nostdlib \
    -nostartfiles \
    -mgeneral-regs-only

LDFLAGS = -T linker.ld

SRC = \
    boot/boot.s \
    kernel/kernel.c \
    kernel/kprintf.c \
    kernel/panic.c \
	kernel/shell.c \
    drivers/uart.c

OBJ = \
    $(BUILD_DIR)/boot.o \
    $(BUILD_DIR)/kernel.o \
    $(BUILD_DIR)/kprintf.o \
    $(BUILD_DIR)/panic.o \
	$(BUILD_DIR)/shell.o \
    $(BUILD_DIR)/uart.o

all: $(IMG)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

$(BUILD_DIR)/boot.o: boot/boot.s | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/uart.o: drivers/uart.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/kernel.o: kernel/kernel.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/panic.o: kernel/panic.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/kprintf.o: kernel/kprintf.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/shell.o: kernel/shell.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(ELF): $(OBJ) | $(OUTPUT_DIR)
	$(LD) $(LDFLAGS) $(OBJ) -o $@

$(IMG): $(ELF)
	$(OBJCOPY) $(ELF) -O binary $@

run: $(IMG)
	qemu-system-aarch64 \
		-M raspi3b \
		-kernel $(IMG) \
		-serial stdio \
		-display none

debug: $(IMG)
	qemu-system-aarch64 \
		-M raspi3b \
		-kernel $(IMG) \
		-S -s \
		-serial stdio \
		-display none

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(OUTPUT_DIR)

rebuild: clean all

.PHONY: all run debug clean rebuild