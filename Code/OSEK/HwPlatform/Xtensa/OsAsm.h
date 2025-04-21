// *****************************************************************************
// Filename    : OsAsm.h
// 
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 08.03.2025
// 
// Description : OS low-level functions prototypes
// 
// *****************************************************************************

#ifndef __OS_ASM_H__
#define __OS_ASM_H__

#include <stdint.h>

#ifndef PFUNC_TYPEDEF
  #define PFUNC_TYPEDEF
  typedef void (*pFunc)(void);
#endif

void osDispatchHandler(void);
void osCat2IsrWrapper(void);
void osStartNewTask(uint32_t StackFramePtr, pFunc TaskFuncPtr);
void osSetIntVectTableAddress(uint32_t address);
void osHwDispatch(void);
void osSetPrivateTimer(uint32_t timer_id, uint32_t ticks);
void osSetEnableIRQMask(uint32_t mask);
void _osHwTimerInit(void);
void osMaskAllInterrupts(void);
void osUnmaskAllInterrupts(void);
void osClearPendingIRQ(uint32_t irq_mask);
void osSetPriorityLevel(uint32_t level);

uint32_t osGetInterruptLevel(void);
uint32_t osGetCurrentSP(void);
uint32_t osGetCurrentPS(void);
uint32_t osGetInterruptEnableReg(void);
uint32_t osHwSearchForHighPrio(void);


#endif
