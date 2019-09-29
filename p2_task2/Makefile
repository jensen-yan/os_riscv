HOST_CC = gcc
CROSS_PREFIX = riscv64-unknown-linux-gnu-
CC = ${CROSS_PREFIX}gcc
OBJDUMP = ${CROSS_PREFIX}objdump
# CFLAGS = -O2  -fno-pic -fno-builtin -nostdinc -N -o bootblock bootblock.s -nostdlib -e main -Wl,-m -T riscv.lds
CFLAGS = -O0 -g -fno-builtin -nostdlib -T riscv.lds -Iinclude -Wall -mcmodel=medany -Iinclude -Idrivers -Itest -Itest/test_project2 -nostdinc
DISK = /dev/sdb
#change sdc to sdb
BOOTLOADER_ENTRYPOINT = 0x10200000
KERNEL_ENTRYPOINT = 0x10201000

ARCH = riscv
ARCH_DIR = ./arch/$(ARCH)
CFLAGS += -Iarch/$(ARCH)/include

SRC_BOOT 	= $(ARCH_DIR)/boot/bootblock.S

SRC_ARCH	= $(ARCH_DIR)/kernel/entry.S $(ARCH_DIR)/kernel/head.S $(ARCH_DIR)/kernel/syscall.S $(ARCH_DIR)/sbi/common.c
SRC_DRIVER	= ./drivers/screen.c
SRC_INIT 	= ./init/main.c
SRC_INT		= ./kernel/irq/irq.c
SRC_LOCK	= ./kernel/locking/lock.c
SRC_SCHED	= ./kernel/sched/sched.c ./kernel/sched/time.c
SRC_MM	= ./kernel/mm/mm.c
SRC_SYSCALL	= ./kernel/syscall/syscall.c
SRC_LIBS	= ./libs/string.c ./libs/printk.c

SRC_TEST	= ./test/test.c
SRC_TEST2	= ./test/test_project2/test_scheduler1.c ./test/test_project2/test_scheduler2.c ./test/test_project2/test_lock1.c ./test/test_project2/test_sleep.c ./test/test_project2/test_timer.c

SRC_MAIN	= ${SRC_ARCH} ${SRC_DRIVER} ${SRC_INIT} ${SRC_INT} ${SRC_LOCK} ${SRC_SCHED} ${SRC_MM} ${SRC_SYSCALL} ${SRC_LIBS} ${SRC_TEST} ${SRC_TEST2}

SRC_IMAGE	= ./tools/createimage.c

.PHONY:all main bootblock clean

all: createimage image asm # floppy

bootblock: $(SRC_BOOT) riscv.lds
	${CC} ${CFLAGS} -o bootblock $(SRC_BOOT) -e main -Ttext=${BOOTLOADER_ENTRYPOINT}

main: $(SRC_MAIN) riscv.lds
	${CC} ${CFLAGS} -o main $(SRC_MAIN) -Ttext=${KERNEL_ENTRYPOINT}

createimage: $(SRC_IMAGE)
	${HOST_CC} ${SRC_IMAGE} -o createimage -ggdb -Wall -g

image: bootblock main createimage
	./createimage --extended bootblock main

clean:
	rm -rf bootblock image createimage main
	find . -name "*.o" -exec rm {} \;

floppy:
	sudo fdisk -l ${DISK}
	sudo dd if=image of=${DISK}2 conv=notrunc

asm:
	${OBJDUMP} -d main > kernel.txt
