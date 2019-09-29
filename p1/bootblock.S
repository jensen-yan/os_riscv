#include <asm/sbidef.h>
#include <asm/sbiasm.h>

// #define BOOT_LOADER_SIG_OFFSET 0x1fe
// #define OS_SIZE_LOC (BOOT_LOADER_SIG_OFFSET - 2)
// #define OS_SIZE_LOC 0x1fc

.equ os_size_loc, 0x102001fc

// 1. kernel address (move kernel to here ~)
.equ kernel, 0x10201000
#.equ kernel, 0x10200000

// 2. kernel main address (jmp here to start kernel main!)
.equ kernel_main, 0x10201000
#.equ kernel_main, 0x10200000

.text
.global main

main:
	/* fence on all memory and I/O */
	fence

	/* output a \n */
	li a0, 10
	SBI_CALL SBI_CONSOLE_PUTCHAR

	/* 1) task1 call BIOS print string "It's bootblock!" */
	/* TODO: */
	la a0,msg
	SBI_CALL SBI_CONSOLE_PUTSTR	#printstr
	/* 2) task2 call BIOS read kernel in SD card and jump to kernel start */

	/* read kernel in SD card, you should read `os_size` sectors */
	/* TODO: */
	li a0, kernel   	#kernel address
	#li a1, 1			#read 1 block
	li t0, os_size_loc
	lh a1, (t0)	        #read the sector num
	li a2, 1			#first block
	#li t0, kernel
	SBI_CALL SBI_SD_READ

	/* output a \n */
	li a0, 10
	SBI_CALL SBI_CONSOLE_PUTCHAR

	/* jump to main */
	call kernel_main		#jump to kernel_main
// while(1) --> stop here
stop:
	j stop
	nop

.data

msg: .string "It's a bootloader...\n\r"

