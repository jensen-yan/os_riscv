/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * *
 *            Copyright (C) 2018 Institute of Computing Technology, CAS
 *               Author : Han Shukai (email : hanshukai@ict.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * */

#ifndef INCLUDE_REGS_H_
#define INCLUDE_REGS_H_

/* This is for struct TrapFrame in scheduler.h
 * Stack layout for all exceptions:
 *
 * ptrace needs to have all regs on the stack. If the order here is changed,
 * it needs to be updated in include/asm-mips/ptrace.h
 *
 * The first PTRSIZE*5 bytes are argument save space for C subroutines.
 */

#define OFFSET_REG_ZERO         0

/* return address */
#define OFFSET_REG_RA           8

/* pointers */
#define OFFSET_REG_SP           16 // stack
#define OFFSET_REG_GP           24 // global
#define OFFSET_REG_TP           32 // thread

/* temporary */
#define OFFSET_REG_T0           40
#define OFFSET_REG_T1           48
#define OFFSET_REG_T2           56

/* saved register */
#define OFFSET_REG_S0           64
#define OFFSET_REG_S1           72

/* args */
#define OFFSET_REG_A0           80
#define OFFSET_REG_A1           88
#define OFFSET_REG_A2           96
#define OFFSET_REG_A3           104
#define OFFSET_REG_A4           112
#define OFFSET_REG_A5           120
#define OFFSET_REG_A6           128
#define OFFSET_REG_A7           136

/* saved register */
#define OFFSET_REG_S2           144
#define OFFSET_REG_S3           152
#define OFFSET_REG_S4           160
#define OFFSET_REG_S5           168
#define OFFSET_REG_S6           176
#define OFFSET_REG_S7           184
#define OFFSET_REG_S8           192
#define OFFSET_REG_S9           200
#define OFFSET_REG_S10          208
#define OFFSET_REG_S11          216

/* temporary register */
#define OFFSET_REG_T3           224
#define OFFSET_REG_T4           232
#define OFFSET_REG_T5           240
#define OFFSET_REG_T6           248

/* privileged register */
#define OFFSET_REG_SSTATUS      256
#define OFFSET_REG_SEPC         264
#define OFFSET_REG_SBADADDR     272
#define OFFSET_REG_SCAUSE       280

/* Size of stack frame, word/double word alignment */
#define OFFSET_SIZE             288

#define PCB_KERNEL_SP    0
#define PCB_USER_SP      8

/* offset in switch_to */
#define SWITCH_TO_RA     0
#define SWITCH_TO_SP     8
#define SWITCH_TO_S0     16
#define SWITCH_TO_S1     24
#define SWITCH_TO_S2     32
#define SWITCH_TO_S3     40
#define SWITCH_TO_S4     48
#define SWITCH_TO_S5     56
#define SWITCH_TO_S6     64
#define SWITCH_TO_S7     72
#define SWITCH_TO_S8     80
#define SWITCH_TO_S9     88
#define SWITCH_TO_S10    96
#define SWITCH_TO_S11    104

#define SWITCH_TO_SIZE   112

/*
 * Copyright (C) 1994, 1995, 1996, 1997, 2000, 2001 by Ralf Baechle
 *
 * Copyright (C) 2001, Monta Vista Software
 * Author: jsun@mvista.com or jsun@junsun.net
 */

#define CP0_INDEX           $0
#define CP0_RANDOM          $1
#define CP0_ENTRYLO0        $2
#define CP0_ENTRYLO1        $3
#define CP0_CONTEXT         $4
#define CP0_PAGEMASK        $5
#define CP0_WIRED           $6
#define CP0_BADVADDR        $8
#define CP0_COUNT           $9
#define CP0_ENTRYHI         $10
#define CP0_COMPARE         $11
#define CP0_STATUS          $12

/* --------- CP0_STATUS (low 16bits) ------------ *
 * NAME | IM7~IM0  | 000 | KSU | ERL | EXL | IE |
 * ----------------------------------------------
 * BITS |     8    |  3  |  2  |  1  |  1  |  1 |
 * ----------------------------------------------
 * INIT | 1000 0000| 000 | 00  |  0  |  0  |  1 |
 * ---------------------------------------------- */
#define CP0_CAUSE           $13
/* ----- CP0_CAUSE (low 16bits) ------ *
 * NAME |  IP7~IP0  | 0 | ExcCode | 0 |
 * -----------------------------------
 * BITS |     8     | 1 |    5    | 2 |
 * ----------------------------------- */

#define CP0_EPC             $14
#define CP0_PRID            $15
#define CP0_CONFIG          $16
#define CP0_LLADDR          $17
#define CP0_WATCHLO         $18
#define CP0_WATCHHI         $19
#define CP0_XCONTEXT        $20
#define CP0_FRAMEMASK       $21
#define CP0_DIAGNOSTIC      $22
#define CP0_PERFORMANCE     $25
#define CP0_ECC             $26
#define CP0_CACHEERR        $27
#define CP0_TAGLO           $28
#define CP0_TAGHI           $29
#define CP0_ERROREPC        $30

#define STATUS_CU0          0x10000000
#define STATUS_IE           0x1
#define CAUSE_IPL           0xff00
#define CAUSE_EXCCODE       0x7c

#endif
