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

void osDispatchHandler(void);
void osCat2IsrWrapper(void);
void osStartNewTask(uint32_t StackFramePtr, pFunc TaskFuncPtr);
void osGetCurrentSP(uint32_t* CurrentSpPtr);
void OsGetPSR(uint32_t* CurrentPsr);
void OsSetSysCtrlReg(uint32_t CtrlRegValue);
void OsGetSysCtrlReg(uint32_t* CtrlRegValue);
void OsSetSysBasepriReg(uint32_t BasepriRegValue);
void OsGetSysBasepriReg(uint32_t* BasepriRegValue);

uint32_t OsGetSysPrimaskReg(void);
uint32_t OsHwGetInterruptPrioBits(void);
uint32_t osHwSearchForHighPrio(void);

#endif
