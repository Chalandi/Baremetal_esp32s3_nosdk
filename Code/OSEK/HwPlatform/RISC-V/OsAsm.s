/******************************************************************************************
  Filename    : OsAsm.s

  Core        : Hazard3 RISC-V

  Author      : Chalandi Amine

  Owner       : Chalandi Amine

  Date        : 26.01.2023

  Description : Context switch and ISR category 2 wrapper

******************************************************************************************/

.file "OsAsm.s"

.equ OS_CPU_CONTEXT_USED_REGISTERS, 31

/*-----------------------------------------------------------------------------------------------------------------*/
/* \brief  OsSaveCpuContext                                                                                        */
/*                                                                                                                 */
/* \descr  Macro that save the current context                                                                     */
/*                                                                                                                 */
/* \param  void                                                                                                    */
/*                                                                                                                 */
/* \return void                                                                                                    */
/*-----------------------------------------------------------------------------------------------------------------*/
.macro OsSaveCpuContext
  addi sp, sp, -OS_CPU_CONTEXT_USED_REGISTERS*4
  sw x1,   0*4(sp)
  sw x3,   1*4(sp)
  sw x4,   2*4(sp)
  sw x5,   3*4(sp)
  sw x6,   4*4(sp)
  sw x7,   5*4(sp)
  sw x8,   6*4(sp)
  sw x9,   7*4(sp)
  sw x10,  8*4(sp)
  sw x11,  9*4(sp)
  sw x12, 10*4(sp)
  sw x13, 11*4(sp)
  sw x14, 12*4(sp)
  sw x15, 13*4(sp)
  sw x16, 14*4(sp)
  sw x17, 15*4(sp)
  sw x18, 16*4(sp)
  sw x19, 17*4(sp)
  sw x20, 18*4(sp)
  sw x21, 19*4(sp)
  sw x22, 20*4(sp)
  sw x23, 21*4(sp)
  sw x24, 22*4(sp)
  sw x25, 23*4(sp)
  sw x26, 24*4(sp)
  sw x27, 25*4(sp)
  sw x28, 26*4(sp)
  sw x29, 27*4(sp)
  sw x30, 28*4(sp)
  sw x31, 29*4(sp)
  csrr x1, mepc
  sw x1,  30*4(sp)
  /*la x1, 0xE000E040
  lw x3, 0(x1)
  sw x3, 31*4(sp)*/
.endm

/*-----------------------------------------------------------------------------------------------------------------*/
/* \brief  OsRestoreCpuContext                                                                                     */
/*                                                                                                                 */
/* \descr  Macro that restore the saved context                                                                    */
/*                                                                                                                 */
/* \param  void                                                                                                    */
/*                                                                                                                 */
/* \return void                                                                                                    */
/*-----------------------------------------------------------------------------------------------------------------*/
.macro OsRestoreCpuContext
  /*lw x3,  31*4(sp)*/
  /*la x1, 0xE000E040*/
  /*sw x3, 0(x1)*/
  lw x1,  30*4(sp)
  csrrw zero, mepc, x1
  lw x1,   0*4(sp)
  lw x3,   1*4(sp)
  lw x4,   2*4(sp)
  lw x5,   3*4(sp)
  lw x6,   4*4(sp)
  lw x7,   5*4(sp)
  lw x8,   6*4(sp)
  lw x9,   7*4(sp)
  lw x10,  8*4(sp)
  lw x11,  9*4(sp)
  lw x12, 10*4(sp)
  lw x13, 11*4(sp)
  lw x14, 12*4(sp)
  lw x15, 13*4(sp)
  lw x16, 14*4(sp)
  lw x17, 15*4(sp)
  lw x18, 16*4(sp)
  lw x19, 17*4(sp)
  lw x20, 18*4(sp)
  lw x21, 19*4(sp)
  lw x22, 20*4(sp)
  lw x23, 21*4(sp)
  lw x24, 22*4(sp)
  lw x25, 23*4(sp)
  lw x26, 24*4(sp)
  lw x27, 25*4(sp)
  lw x28, 26*4(sp)
  lw x29, 27*4(sp)
  lw x30, 28*4(sp)
  lw x31, 29*4(sp)
  addi sp, sp, OS_CPU_CONTEXT_USED_REGISTERS*4
.endm

/*-----------------------------------------------------------------------------------------------------------------*/
/* \brief  void osDispatchHandler(void)                                                                            */
/*                                                                                                                 */
/* \descr  Context switcher                                                                                        */
/*                                                                                                                 */
/* \param  void                                                                                                    */
/*                                                                                                                 */
/* \return void                                                                                                    */
/*-----------------------------------------------------------------------------------------------------------------*/
.section ".text"
.align 4
.globl  osDispatchHandler
.type   osDispatchHandler, % function
.extern osDispatcher

.equ SIO_CPUID, 0xd0000000
.equ SIO_RISCV_SOFTIRQ, 0xd00001a0

osDispatchHandler:
                   OsSaveCpuContext
                   mv a0, sp
                   la a1, SIO_RISCV_SOFTIRQ
                   la a2, SIO_CPUID
                   lw a4, 0(a2)
                   addi a4, a4, 8
                   addi a5, zero, 1
                   sll a5, a5, a4
                   sw a5, 0(a1)
                   jal osDispatcher
                   mv sp, a0
                   OsRestoreCpuContext
                   csrw mcause, zero
                   mret

.size osDispatchHandler, .-osDispatchHandler

/* ---------------------------------------------------------------------------------------------------------------- */
/* \brief  void osCat2IsrWrapper(void)                                                                              */
/*                                                                                                                  */
/* \descr  Wrapper for all category 2 interrupts                                                                    */
/*                                                                                                                  */
/* \param  void                                                                                                     */
/*                                                                                                                  */
/* \return void                                                                                                     */
/* ---------------------------------------------------------------------------------------------------------------- */
.section ".text"
.align 4
.globl  osCat2IsrWrapper
.type   osCat2IsrWrapper, % function
.extern osStoreStackPointer
.extern osRunCat2Isr
.extern osGetSavedStackPointer
.extern osIntCallDispatch
.extern osIncNestingDepthLevel
.extern osDecNestingDepthLevel

osCat2IsrWrapper:
                   OsSaveCpuContext
                   jal osIncNestingDepthLevel
                   mv a0, sp
                   jal osStoreStackPointer
                   jal osRunCat2Isr
                   jal osGetSavedStackPointer
                   jal osIntCallDispatch
                   mv sp, a0
                   jal osDecNestingDepthLevel
                   OsRestoreCpuContext
                   csrw mcause, zero
                   mret

.size osCat2IsrWrapper, .-osCat2IsrWrapper

/* ------------------------------------------------------------------------------------------------------------------ */
/* / \brief  void osStartNewTask(uint32 StackFramePtr, pFunc TaskFuncPtr)                                             */
/* /                                                                                                                  */
/* / \descr  This function start an OSEK Task for the 1st execution                                                   */
/* /                                                                                                                  */
/* / \param  void                                                                                                     */
/* /                                                                                                                  */
/* / \return void                                                                                                     */
/* ------------------------------------------------------------------------------------------------------------------ */
.section ".text"
.align 4
.globl  osStartNewTask
.type   osStartNewTask, % function
.extern osErrTaskExitWithoutTerminate

osStartNewTask:
                 mv sp,a0
                 csrrw zero, mepc, a1
                 lw x1, osErrTaskExitWithoutTerminate
                 mv x3, x0
                 mv x4, x0
                 mv x5, x0
                 mv x6, x0
                 mv x7, x0
                 mv x8, x0
                 mv x9, x0
                 mv x10,x0
                 mv x11,x0
                 mv x12,x0
                 mv x13,x0
                 mv x14,x0
                 mv x15,x0
                 mv x16,x0
                 mv x17,x0
                 mv x18,x0
                 mv x19,x0
                 mv x20,x0
                 mv x21,x0
                 mv x22,x0
                 mv x23,x0
                 mv x24,x0
                 mv x25,x0
                 mv x26,x0
                 mv x27,x0
                 mv x28,x0
                 mv x29,x0
                 mv x30,x0
                 mv x31,x0
                 csrw mcause, zero
                 mret

.size osStartNewTask, .-osStartNewTask

/* ----------------------------------------------------------------------------------------------------------------- */
/*  \brief  osGetCurrentSP : uint32 osGetCurrentSP(void)                                                             */
/*                                                                                                                   */
/*  \descr  Get the current stack pointer register value                                                             */
/*                                                                                                                   */
/*  \param  unsigned int* CurrentSpPtr (out): Current stack pointer register value                                   */
/*                                                                                                                   */
/*  \return void                                                                                                     */
/* ----------------------------------------------------------------------------------------------------------------- */
.section ".text"
.align 4
.globl  osGetCurrentSP
.type   osGetCurrentSP, % function

osGetCurrentSP:
                 mv a0, sp
                 ret

.size osGetCurrentSP, .-osGetCurrentSP


/* ----------------------------------------------------------------------------------------------------------------- */
/*  \brief  osSetIntVectTableAddress : void osSetIntVectTableAddress(uint32 address)                                 */
/*                                                                                                                   */
/*  \descr                                                                                                           */
/*                                                                                                                   */
/*  \param                                                                                                           */
/*                                                                                                                   */
/*  \return void                                                                                                     */
/* ----------------------------------------------------------------------------------------------------------------- */
.section ".text"
.align 4
.globl  osSetIntVectTableAddress
.type   osSetIntVectTableAddress, % function

osSetIntVectTableAddress:
                          /* setup the interrupt vector table */
                          ori a0, a0, 3
                          csrw mtvec, a0
                          ret

.size osSetIntVectTableAddress, .-osSetIntVectTableAddress

/* ----------------------------------------------------------------------------------------------------------------- */
/*  \brief  void osHwAcquireSpinLock(uint32* lock)                                                                   */
/*                                                                                                                   */
/*  \descr                                                                                                           */
/*                                                                                                                   */
/*  \param                                                                                                           */
/*                                                                                                                   */
/*  \return void                                                                                                     */
/* ----------------------------------------------------------------------------------------------------------------- */
.section ".text", "ax"
.align 2
.globl osHwAcquireSpinLock
.type  osHwAcquireSpinLock, @function


osHwAcquireSpinLock:  lr.w a1, (a0)
                      bne zero, a1, osHwAcquireSpinLock
                      add a1, zero, 1
                      sc.w t0, a1, (a0)
                      bnez t0, osHwAcquireSpinLock
                      ret

.size osHwAcquireSpinLock, .-osHwAcquireSpinLock

/* ----------------------------------------------------------------------------------------------------------------- */
/*  \brief  uint32_t osHwTryToAcquireSpinLock(uint32_t* lock)                                                        */
/*                                                                                                                   */
/*  \descr                                                                                                           */
/*                                                                                                                   */
/*  \param                                                                                                           */
/*                                                                                                                   */
/*  \return uint32_t  0 -> not acquired, 1 -> acquired                                                               */
/* ----------------------------------------------------------------------------------------------------------------- */
.section ".text", "ax"
.align 2
.globl osHwTryToAcquireSpinLock
.type  osHwTryToAcquireSpinLock, @function


osHwTryToAcquireSpinLock:  
                      lr.w a1, (a0)
                      bne zero, a1, .L_not_acquired
                      add a1, zero, 1
                      sc.w t0, a1, (a0)
                      bnez t0, .L_not_acquired
                      add a0, x0, 1
                      ret
.L_not_acquired:
                     mv a0, x0
                     ret

.size osHwTryToAcquireSpinLock, .-osHwTryToAcquireSpinLock

/* ----------------------------------------------------------------------------------------------------------------- */
/*  \brief  void osHwReleaseSpinLock(uint32_t* lock)                                                                 */
/*                                                                                                                   */
/*  \descr                                                                                                           */
/*                                                                                                                   */
/*  \param                                                                                                           */
/*                                                                                                                   */
/*  \return void                                                                                                     */
/* ----------------------------------------------------------------------------------------------------------------- */
.section ".text", "ax"
.align 2
.globl osHwReleaseSpinLock
.type  osHwReleaseSpinLock, @function


osHwReleaseSpinLock: lr.w a1, (a0)
                     add a2, zero, 1
                     bne a2, a1, osHwReleaseSpinLock
                     sc.w t0, zero, (a0)
                     bnez t0, osHwReleaseSpinLock
                     ret

.size osHwReleaseSpinLock, .-osHwReleaseSpinLock

/*

-----------------------------------------------------------------
 Register | ABI Name | Description                      | Saver
-----------------------------------------------------------------
 x0       | zero     | Hard-wired zero                  | -
 x1       | ra       | Return address                   | Caller
 x2       | sp       | Stack pointer                    | Callee
 x3       | gp       | Global pointer                   | -
 x4       | tp       | Thread pointer                   | -
 x5-7     | t0-2     | Temporaries                      | Caller
 x8       | s0/fp    | Saved register/frame pointer     | Callee
 x9       | s1       | Saved register                   | Callee
 x10-11   | a0-1     | Function arguments/return values | Caller
 x12-17   | a2-7     | Function arguments               | Caller
 x18-27   | s2-11    | Saved registers                  | Callee
 x28-31   | t3-6     | Temporaries                      | Caller
 f0-7     | ft0-7    | FP temporaries                   | Caller
 f8-9     | fs0-1    | FP saved registers               | Callee
 f10-11   | fa0-1    | FP arguments/return values       | Caller
 f12-17   | fa2-7    | FP arguments                     | Caller
 f18-27   | fs2-11   | FP saved registers               | Callee
 f28-31   | ft8-11   | FP temporaries                   | Caller

*/
