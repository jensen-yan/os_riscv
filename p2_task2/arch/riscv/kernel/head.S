/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2012 Regents of the University of California
 */

#include <asm.h>
#include <csr.h>

#define INIT_KERNEL_STACK 0x10500000

.section ".entry_function","ax"
ENTRY(_start)
  /* Mask all interrupts */
  csrw CSR_SIE, zero
  csrw CSR_SIP, zero

  /*
   * Disable FPU to detect illegal usage of
   * floating point in kernel space
   */
  li t0, SR_FS
  csrc sstatus, t0

  /* Load the global pointer */
  .option push              //保存编译设置
  .option norelax           //禁止相对寻址
  la gp, __global_pointer$
  .option pop
  
  /* Clear BSS for flat non-ELF images */
  la a3, __bss_start
  la a4, __BSS_END__
do:
  sw zero,(a3)
  addi a3,a3,4
  ble  a3,a4,do

  /* setup C environment */
  /* load pid0_pcb into register `tp` as the kernel's pcb
   * load pid0_stack into register `sp`  as the kernel's stack
   */

  lui tp, %hi(INIT_KERNEL_STACK)    //?
  addi tp, tp, %lo(INIT_KERNEL_STACK)
  lui sp, %hi(INIT_KERNEL_STACK)
  addi sp, sp, %lo(INIT_KERNEL_STACK)


  jal main

loop:
  wfi
  j loop

END(_start)
