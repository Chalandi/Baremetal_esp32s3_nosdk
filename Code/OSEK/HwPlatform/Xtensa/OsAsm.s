/******************************************************************************************
  Filename    : OsAsm.s

  Core        : Xtensa Lx7

  Author      : Chalandi Amine

  Owner       : Chalandi Amine

  Date        : 09.03.2025

  Description : Context switch and ISR category 2 wrapper

******************************************************************************************/

/*-----------------------------------------------------------------------------------------------------------------*/
/* Defines                                                                                                         */
/*-----------------------------------------------------------------------------------------------------------------*/
.equ osXtensaStackAlignmentBoundaries, 16

.equ osCpuContextUsedRegs, 16

.equ osCpuContextSize, osCpuContextUsedRegs*4

.equ osEnabledIRQMask, 0xE0 /* IRQs: 7, 6, 5*/

/*-----------------------------------------------------------------------------------------------------------------*/
/* \brief  osAlignStackOnPush                                                                                      */
/*                                                                                                                 */
/* \descr  Macro to reserve aligned stack frame                                                                    */
/*                                                                                                                 */
/* \param  void                                                                                                    */
/*-----------------------------------------------------------------------------------------------------------------*/
.macro osAlignStackOnPush  size
    /* on Xtensa LX7, The stack pointer must be aligned to 16-byte boundaries */
    addi sp, sp, -(((\size) + osXtensaStackAlignmentBoundaries - 1) & ~(osXtensaStackAlignmentBoundaries - 1))
.endm

/*-----------------------------------------------------------------------------------------------------------------*/
/* \brief  osAlignStackOnPop                                                                                       */
/*                                                                                                                 */
/* \descr  Macro to free the reserved aligned stack frame                                                          */
/*                                                                                                                 */
/* \param  void                                                                                                    */
/*-----------------------------------------------------------------------------------------------------------------*/
.macro osAlignStackOnPop  size
    /* on Xtensa LX7, The stack pointer must be aligned to 16-byte boundaries */
    addi sp, sp, (((\size) + osXtensaStackAlignmentBoundaries - 1) & ~(osXtensaStackAlignmentBoundaries - 1))
.endm


/*-----------------------------------------------------------------------------------------------------------------*/
/* \brief  OsSaveCpuContext                                                                                        */
/*                                                                                                                 */
/* \descr  Macro that saves the current cpu context                                                                */
/*                                                                                                                 */
/* \param  void                                                                                                    */
/*-----------------------------------------------------------------------------------------------------------------*/
.macro OsSaveCpuContext
    osAlignStackOnPush osCpuContextSize
    s32i.n a0,  sp,  0*4
    /* disable interrupt */
    movi.n a0, 0
    wsr a0, intenable
    rsync
    s32i.n a2,  sp,  1*4
    s32i.n a3,  sp,  2*4
    s32i.n a4,  sp,  3*4
    s32i.n a5,  sp,  4*4
    s32i.n a6,  sp,  5*4
    s32i.n a7,  sp,  6*4
    s32i.n a8,  sp,  7*4
    s32i.n a9,  sp,  8*4
    s32i.n a10, sp,  9*4
    s32i.n a11, sp, 10*4
    s32i.n a12, sp, 11*4
    s32i.n a13, sp, 12*4
    s32i.n a14, sp, 13*4
    s32i.n a15, sp, 14*4
    rsr a15, epc1
    s32i.n a15, sp, 15*4
.endm

/*-----------------------------------------------------------------------------------------------------------------*/
/* \brief  OsRestoreCpuContext                                                                                     */
/*                                                                                                                 */
/* \descr  Macro that restores the current cpu context                                                             */
/*                                                                                                                 */
/* \param  void                                                                                                    */
/*-----------------------------------------------------------------------------------------------------------------*/
.macro OsRestoreCpuContext
    l32i.n a0,   sp,  0*4
    l32i.n a3,   sp,  2*4
    l32i.n a4,   sp,  3*4
    l32i.n a5,   sp,  4*4
    l32i.n a6,   sp,  5*4
    l32i.n a7,   sp,  6*4
    l32i.n a8,   sp,  7*4
    l32i.n a9,   sp,  8*4
    l32i.n a10,  sp,  9*4
    l32i.n a11,  sp, 10*4
    l32i.n a12,  sp, 11*4
    l32i.n a13,  sp, 12*4
    l32i.n a14,  sp, 13*4
    l32i.n a15, sp, 15*4
    wsr a15, epc1
    l32i.n a15,  sp, 14*4
    /* enable interrupt */
    movi.n a2, osEnabledIRQMask 
    wsr a2, intenable
    rsync
    l32i.n a2,   sp,  1*4
    osAlignStackOnPop osCpuContextSize
.endm

/*-----------------------------------------------------------------------------------------------------------------*/
/* \brief  osCat2IsrWrapperLevel                                                                                   */
/*                                                                                                                 */
/* \descr  Macro that Wrap an ISR cat2 call                                                                        */
/*                                                                                                                 */
/* \param  return instruction  and return level                                                                    */
/*-----------------------------------------------------------------------------------------------------------------*/
.macro osCat2IsrWrapperLevel return_inst  return_from_level
    OsSaveCpuContext
    call0 osIncNestingDepthLevel
    mov.n a2, sp
    call0 osStoreStackPointer
    call0 osRunCat2Isr
    call0 osGetSavedStackPointer
    call0 osIntCallDispatch
    mov.n sp, a2
    call0 osDecNestingDepthLevel
    OsRestoreCpuContext
    \return_inst \return_from_level
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

osDispatchHandler:
                   OsSaveCpuContext
                   mov.n a2, sp
                   call0 osDispatcher
                   mov.n sp, a2
                   OsRestoreCpuContext
                   rfe

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
                   call0 osIncNestingDepthLevel
                   mov.n a2, sp
                   call0 osStoreStackPointer
                   call0 osRunCat2Isr
                   call0 osGetSavedStackPointer
                   call0 osIntCallDispatch
                   mov.n sp, a2
                   call0 osDecNestingDepthLevel
                   OsRestoreCpuContext
                   rfe

.size osCat2IsrWrapper, .-osCat2IsrWrapper

/* ---------------------------------------------------------------------------------------------------------------- */
/* \brief  void osSysTickTimerHandler(void)                                                                         */
/*                                                                                                                  */
/* \descr  Wrapper for systick timer interrupt                                                                      */
/*                                                                                                                  */
/* \param  void                                                                                                     */
/*                                                                                                                  */
/* \return void                                                                                                     */
/* ---------------------------------------------------------------------------------------------------------------- */
.section ".text"
.align 4
.globl  osSysTickTimerHandler
.type   osSysTickTimerHandler, % function
.extern osStoreStackPointer
.extern OsIsr_SysTickTimerFunc
.extern osGetSavedStackPointer
.extern osIntCallDispatch
.extern osIncNestingDepthLevel
.extern osDecNestingDepthLevel

osSysTickTimerHandler:
                   OsSaveCpuContext
                   call0 osIncNestingDepthLevel
                   mov.n a2, sp
                   call0 osStoreStackPointer
                   call0 OsIsr_SysTickTimerFunc
                   call0 osGetSavedStackPointer
                   call0 osIntCallDispatch
                   mov.n sp, a2
                   call0 osDecNestingDepthLevel
                   OsRestoreCpuContext
                   rfe

.size osSysTickTimerHandler, .-osSysTickTimerHandler

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
                 mov.n sp, a2
                 wsr a3, epc1
                 l32r a0, osErrTaskExitWithoutTerminate
                 movi.n a3,  0
                 movi.n a4,  0
                 movi.n a5,  0
                 movi.n a6,  0
                 movi.n a7,  0
                 movi.n a8,  0
                 movi.n a9,  0
                 movi.n a10, 0
                 movi.n a11, 0
                 movi.n a12, 0
                 movi.n a13, 0
                 movi.n a14, 0
                 movi.n a15, 0
                 rsil a2, 0
                 movi.n a2,  0x20 /* UM = 1, INTLEVEL = 0*/
                 wsr a2, ps
                 movi.n a2, osEnabledIRQMask
                 wsr a2, intenable
                 rsync
                 movi.n a2, 0
                 rfe

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
                 mov.n a2, sp
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
.align 4
.globl osHwAcquireSpinLock
.type  osHwAcquireSpinLock, @function


osHwAcquireSpinLock:
.L_acquire_loop:
                     l32ai a3, a2, 0
                     wsr a3, scompare1
                     rsync
                     mov a4, a3
                     addi a3, a3, 1
                     s32c1i a3, a2, 0
                     bne a3, a4, .L_acquire_loop
                     ret

.size osHwAcquireSpinLock, .-osHwAcquireSpinLock

/* ----------------------------------------------------------------------------------------------------------------- */
/*  \brief  int osHwTryToAcquireSpinLock(uint32* lock)                                                               */
/*                                                                                                                   */
/*  \descr                                                                                                           */
/*                                                                                                                   */
/*  \param                                                                                                           */
/*                                                                                                                   */
/*  \return void                                                                                                     */
/* ----------------------------------------------------------------------------------------------------------------- */
.section ".text", "ax"
.align 4
.globl osHwTryToAcquireSpinLock
.type  osHwTryToAcquireSpinLock, @function


osHwTryToAcquireSpinLock:

                     l32ai a3, a2, 0
                     wsr a3, scompare1
                     rsync
                     mov a4, a3
                     addi a3, a3, 1
                     s32c1i a3, a2, 0
                     bne a3, a4, .L_not_acquired
                     movi.n a2, 1
                     ret
.L_not_acquired:     movi.n a2, 0
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
.align 4
.globl osHwReleaseSpinLock
.type  osHwReleaseSpinLock, @function


osHwReleaseSpinLock:
                     movi.n a3, 0
                     s32ri a3, a2, 0
                     ret

.size osHwReleaseSpinLock, .-osHwReleaseSpinLock

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
.section .text
.type osGetCoreId, @function
.align 4
.globl osGetCoreId

.equ  core0_id, 0xcdcd
.equ  core1_id, 0xabab

osGetCoreId:
             rsr.prid a2
             movi a8, core1_id
             beq a2, a8, .L_core1
             movi a2, 0
             ret
.L_core1:
             movi a2, 1
             ret

.size osGetCoreId, .-osGetCoreId

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
.section .text
.type osHwDispatch, @function
.align 4
.globl osHwDispatch


osHwDispatch:
              movi.n a2, 0x80
              wsr.intset a2
              rsync
              ret

.size osHwDispatch, .-osHwDispatch

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
.section .text
.type osGetCurrentPS, @function
.align 4
.globl osGetCurrentPS


osGetCurrentPS:
              rsr a2, ps
              rsync
              ret

.size osGetCurrentPS, .-osGetCurrentPS

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
.section .text
.type osGetInterruptEnableReg, @function
.align 4
.globl osGetInterruptEnableReg


osGetInterruptEnableReg:
              rsr a2, intenable
              rsync
              ret

.size osGetInterruptEnableReg, .-osGetInterruptEnableReg


/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
.section  .text,"ax"
.type osSetEnableIRQMask, @function
.align 4
.global osSetEnableIRQMask

osSetEnableIRQMask:
           wsr a2, intenable
           rsync
           ret

.size osSetEnableIRQMask, .-osSetEnableIRQMask

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
.section  .text,"ax"
.type osMaskAllInterrupt, @function
.align 4
.global osMaskAllInterrupts

osMaskAllInterrupts:
                     rsil a2, 15
                     ret

.size osMaskAllInterrupts, .-osMaskAllInterrupts

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
.section  .text,"ax"
.type osSetPriorityLevel, @function
.align 4
.global osSetPriorityLevel

osSetPriorityLevel:
                     beqi a2, 0, .L_0
                     beqi a2, 1, .L_1
                     beqi a2, 2, .L_2
                     beqi a2, 3, .L_3
                     beqi a2, 4, .L_4
                     beqi a2, 5, .L_5
                     j .L_6
.L_0:                rsil a2, 0
                     ret
.L_1:                rsil a2, 1
                     ret
.L_2:                rsil a2, 2
                     ret
.L_3:                rsil a2, 3
                     ret
.L_4:                rsil a2, 4
                     ret
.L_5:                rsil a2, 5
                     ret
.L_6:                rsil a2, 15
                     ret

.size osSetPriorityLevel, .-osSetPriorityLevel

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
.section  .text,"ax"
.type osClearPendingIRQ, @function
.align 4
.global osClearPendingIRQ

osClearPendingIRQ:
                     wsr a2, intclear
                     ret

.size osClearPendingIRQ, .-osClearPendingIRQ

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
.section  .text,"ax"
.type osGetInterruptLevel, @function
.align 4
.global osGetInterruptLevel

osGetInterruptLevel:
                     rsr a3, ps
                     extui a2, a3, 0, 4
                     ret

.size osGetInterruptLevel, .-osGetInterruptLevel

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
.section  .text,"ax"
.type osUnmaskAllInterrupts, @function
.align 4
.global osUnmaskAllInterrupts

osUnmaskAllInterrupts:
                      rsil a2, 0
                      ret

.size osUnmaskAllInterrupts, .-osUnmaskAllInterrupts

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
.section  .text,"ax"
.type osSetPrivateTimer, @function
.align 4
.global osSetPrivateTimer

osSetPrivateTimer:
                       beqi a2, 0, .L_timer0
                       beqi a2, 1, .L_timer1
                       beqi a2, 2, .L_timer2
                       ret
.L_timer0:
                       rsr  a11, ccount
                       esync
                       add  a11, a11, a3
                       wsr  a11, ccompare0
                       esync
                       ret
.L_timer1:
                       rsr  a11, ccount
                       esync
                       add  a11, a11, a3
                       wsr  a11, ccompare1
                       esync
                       ret
.L_timer2:
                       rsr  a11, ccount
                       esync
                       add  a11, a11, a3
                       wsr  a11, ccompare2
                       esync
                       ret

.size osSetPrivateTimer, .-osSetPrivateTimer

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
.section  .text,"ax"
.type _osHwTimerInit, @function
.align 4
.global _osHwTimerInit

_osHwTimerInit:
                movi.n a2, 0
                wsr  a2, ccount
                esync
                wsr  a2, ccompare0
                esync
                wsr  a2, ccompare1
                esync
                wsr  a2, ccompare2
                esync
                ret

.size _osHwTimerInit, .-_osHwTimerInit

/*  CALL0 ABI:
--------------------------------------------------------------------
|  Register   |  Usage                            |  Preserver     |
--------------------------------------------------------------------
|  a0         |  Return Address                   |  caller-saved  |
|  a1 (sp)    |  Stack Pointer                    | *callee-saved  |
|  a2 – a7    |  Function Arguments (6 registers) |  caller-saved  |
|  a8 – a11   |  Temporary (4 registers)          |  caller-saved  |
|  a12 – a15  |  Temporary (4 registers)          | *callee-saved  |
|  a15        |  Stack-Frame Pointer (optional)   | *callee-saved  |
--------------------------------------------------------------------
*/
