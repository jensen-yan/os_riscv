/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2012 Regents of the University of California
 */

#define CSR_SIE			0x104
#define CSR_SIP			0x144
#define KERNEL_STACK		0x10500000

.section ".entry_function","ax"
.global _start
_start:
  /* Mask all interrupts */
  csrw CSR_SIE, zero
  csrw CSR_SIP, zero

  /* Load the global pointer */
  .option push
  .option norelax
  la gp, __global_pointer$
  .option pop
  
  /* Clear BSS for flat non-ELF images */
  /* bss section is from __bss_start to __BSS_END__ */
  /* TODO: */
  la t0, __bss_start
  la t1, __BSS_END__
do:
  sw zero,(t0)
  addi t0,t0,4
  ble t0,t1,do

  #SBI_CALL SBI_CLEAR_IPI
  /* setup C environment */
  /* set sp to KERNEL_STACK */
  /* TODO: */
  lui sp, %hi(KERNEL_STACK)
  addi sp, sp, %lo(KERNEL_STACK)
  
  call main

loop:
  wfi
  j loop

