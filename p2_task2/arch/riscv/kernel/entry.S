#include <asm.h>
#include <csr.h>

.macro STI
.endm

.macro CLI
.endm

.macro SAVE_CONTEXT
  LOCAL _restore_kernel_tpsp
  LOCAL _save_context
  /*
   * If coming from userspace, preserve the user thread pointer and load
   * the kernel thread pointer.  If we came from the kernel, sscratch
   * will contain 0, and we should continue on the current TP.
   */
  csrrw tp, CSR_SSCRATCH, tp  
  //交换tp，sscratch
  bnez tp, _save_context      
  //kernel时候，tp为0

_restore_kernel_tpsp:
  csrr tp, CSR_SSCRATCH       
  //tp = sscratch ?
  sd sp, PCB_KERNEL_SP(tp)    
  //存入内核sp
_save_context:
  sd sp, PCB_USER_SP(tp)      
  //sp in 8(current_runing),存入用户sp
  ld sp, PCB_KERNEL_SP(tp)    
  //取出内核sp
  addi sp, sp, -(OFFSET_SIZE) 
  //sp - 288

  /* TODO: store all general purpose registers here! */
  sd ra, OFFSET_REG_RA(sp)    
  //除了sp，都存入栈中

  sd gp, OFFSET_REG_GP(sp)
  sd fp, OFFSET_REG_FP(sp)
  sd t0, OFFSET_REG_T0(sp)
  sd t1, OFFSET_REG_T1(sp)
  sd t2, OFFSET_REG_T2(sp)
  sd s0, OFFSET_REG_S0(sp)
  sd s1, OFFSET_REG_S1(sp)
  sd a0, OFFSET_REG_A0(sp)
  sd a1, OFFSET_REG_A1(sp)
  sd a2, OFFSET_REG_A2(sp)
  sd a3, OFFSET_REG_A3(sp)
  sd a4, OFFSET_REG_A4(sp)
  sd a5, OFFSET_REG_A5(sp)
  sd a6, OFFSET_REG_A6(sp)
  sd a7, OFFSET_REG_A7(sp)
  sd s2, OFFSET_REG_S2(sp)
  sd s3, OFFSET_REG_S3(sp)
  sd s4, OFFSET_REG_S4(sp)
  sd s5, OFFSET_REG_S5(sp)
  sd s6, OFFSET_REG_S6(sp)
  sd s7, OFFSET_REG_S7(sp)
  sd s8, OFFSET_REG_S8(sp)
  sd s9, OFFSET_REG_S9(sp)
  sd s10, OFFSET_REG_S10(sp)
  sd s11, OFFSET_REG_S11(sp)
  sd t3, OFFSET_REG_T3(sp)
  sd t4, OFFSET_REG_T4(sp)
  sd t5, OFFSET_REG_T5(sp)
  sd t6, OFFSET_REG_T6(sp)
  /*
   * Disable user-mode memory access as it should only be set in the
   * actual user copy routines.
   *
   * Disable the FPU to detect illegal usage of floating point in kernel
   * space.
   */
  li t0, SR_SUM | SR_FS

  ld s0, PCB_USER_SP(tp)
  csrrc s1, CSR_SSTATUS, t0
  csrr s2, CSR_SEPC
  csrr s3, CSR_STVAL
  csrr s4, CSR_SCAUSE
  csrr s5, CSR_SSCRATCH
  sd s0,   OFFSET_REG_SP(sp)
  sd s1,   OFFSET_REG_SSTATUS(sp)
  sd s2,   OFFSET_REG_SEPC(sp)
  sd s3,   OFFSET_REG_SBADADDR(sp)
  sd s4,   OFFSET_REG_SCAUSE(sp)
  sd s5,   OFFSET_REG_TP(sp)
.endm

.macro RESTORE_CONTEXT
  /* Restore all registers */
  //ld sp, PCB_KERNEL_SP(a1)
  //弹出一部分后，继续弹出剩下的寄存器
  ld a0, OFFSET_REG_SSTATUS(sp)
  //a0 = 0
  ld a2, OFFSET_REG_SEPC(sp)
  //a2 = entry point
  csrw CSR_SSTATUS, a0
  csrw CSR_SEPC, a2

  ld ra, OFFSET_REG_RA(sp)    
  //除了sp，都取出来

  ld gp, OFFSET_REG_GP(sp)
  ld tp, OFFSET_REG_TP(sp)
  ld t0, OFFSET_REG_T0(sp)
  ld t1, OFFSET_REG_T1(sp)
  ld t2, OFFSET_REG_T2(sp)
  ld s0, OFFSET_REG_S0(sp)
  ld s1, OFFSET_REG_S1(sp)
  ld a0, OFFSET_REG_A0(sp)
  ld a1, OFFSET_REG_A1(sp)
  ld a2, OFFSET_REG_A2(sp)
  ld a3, OFFSET_REG_A3(sp)
  ld a4, OFFSET_REG_A4(sp)
  ld a5, OFFSET_REG_A5(sp)
  ld a6, OFFSET_REG_A6(sp)
  ld a7, OFFSET_REG_A7(sp)
  ld s2, OFFSET_REG_S2(sp)
  ld s3, OFFSET_REG_S3(sp)
  ld s4, OFFSET_REG_S4(sp)
  ld s5, OFFSET_REG_S5(sp)
  ld s6, OFFSET_REG_S6(sp)
  ld s7, OFFSET_REG_S7(sp)
  ld s8, OFFSET_REG_S8(sp)
  ld s9, OFFSET_REG_S9(sp)
  ld s10, OFFSET_REG_S10(sp)
  ld s11, OFFSET_REG_S11(sp)
  ld t3, OFFSET_REG_T3(sp)
  ld t4, OFFSET_REG_T4(sp)
  ld t5, OFFSET_REG_T5(sp)
  ld t6, OFFSET_REG_T6(sp)

  ld sp, OFFSET_REG_SP(sp)
.endm

// the address of previous pcb in a0，a0为current running！
// the address of next pcb in a1
ENTRY(switch_to)
  /* TODO: store all callee save registers,
   * see the definition of `struct switchto_context` in sched.h*/
  //这一部分是保存在要被切换的进程的kernel栈上的！例如第一次是在pid0栈上，之后是pid2的内核栈上
  //sp的初始状态应该如何确定？
  //sd sp, PCB_USER_SP(a0)
  //ld sp, PCB_KERNEL_SP(a0)
  addi sp, sp, -(SWITCH_TO_SIZE)
  sd ra, SWITCH_TO_RA(sp)
  //sd sp, SWITCH_TO_SP(sp)
  sd s0, SWITCH_TO_S0(sp)
  sd s1, SWITCH_TO_S1(sp)
  sd s2, SWITCH_TO_S2(sp)
  sd s3, SWITCH_TO_S3(sp)
  sd s4, SWITCH_TO_S4(sp)
  sd s5, SWITCH_TO_S5(sp)
  sd s6, SWITCH_TO_S6(sp)
  sd s7, SWITCH_TO_S7(sp)
  sd s8, SWITCH_TO_S8(sp)
  sd s9, SWITCH_TO_S9(sp)
  sd s10, SWITCH_TO_S10(sp)
  sd s11, SWITCH_TO_S11(sp)
  sd sp, PCB_KERNEL_SP(a0)
  // restore next

  ld sp, PCB_KERNEL_SP(a1)
  //pcb的第一个位置存放内核栈指针
  /* TODO: restore all callee save registers,
   * see the definition of `struct switchto_context` in sched.h*/
  ld ra, SWITCH_TO_RA(sp)
  //ld sp, SWITCH_TO_SP(sp)
  ld s0, SWITCH_TO_S0(sp)
  ld s1, SWITCH_TO_S1(sp)
  ld s2, SWITCH_TO_S2(sp)
  ld s3, SWITCH_TO_S3(sp)
  ld s4, SWITCH_TO_S4(sp)
  ld s5, SWITCH_TO_S5(sp)
  ld s6, SWITCH_TO_S6(sp)
  ld s7, SWITCH_TO_S7(sp)
  ld s8, SWITCH_TO_S8(sp)
  ld s9, SWITCH_TO_S9(sp)
  ld s10, SWITCH_TO_S10(sp)
  ld s11, SWITCH_TO_S11(sp)
  
  mv tp, a1     
  //tp = current_running
  addi sp, sp, SWITCH_TO_SIZE
  //sd sp, PCB_KERNEL_SP(a1)
  //ld sp, PCB_USER_SP(a1)
  jr ra
ENDPROC(switch_to)

ENTRY(ret_from_exception)
  RESTORE_CONTEXT
  sret
ENDPROC(ret_from_exception)

.global exception_handler_begin
.global exception_handler_end

ENTRY(exception_handler_entry)
exception_handler_begin:
    // TODO close interrupt
    // jmp exception_handler[i] which decided by CP0_CAUSE
    // Leve2 exception Handler.
exception_handler_end:
ENDPROC(exception_handler_entry)

ENTRY(handle_int)
    // interrupt handler
    // Leve3 exception Handler.
ENDPROC(handle_int)

ENTRY(handle_syscall)
    // system call handler
ENDPROC(handle_syscall)

ENTRY(handle_other)
    // other exception handler
ENDPROC(handle_other)
