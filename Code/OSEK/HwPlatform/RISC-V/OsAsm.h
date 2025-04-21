// *****************************************************************************
// Filename    : OsAsm.h
// 
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 27.12.2017
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

uint32_t osGetCurrentSP(void);
uint32_t osHwSearchForHighPrio(void);

#endif
