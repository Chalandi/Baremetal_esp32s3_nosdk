// *****************************************************************************
// Filename    : OsHwPltfm.h
// 
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 18.01.2023
// 
// Description : Hardware platform header file for WCH RISC-V CH32V307
// 
// *****************************************************************************

#ifndef __HARDWARE_PLATFORM_H__
#define __HARDWARE_PLATFORM_H__

//=========================================================================================
// Includes
//=========================================================================================
#include"OsTypes.h"
#include"OsAsm.h"

#include "core_arch.h"
#include "RP2350.h"

//=========================================================================================
// Definitions
//=========================================================================================
#define osDispatch()              HW_PER_SIO->RISCV_SOFTIRQ.reg = (1ul << (HW_PER_SIO->CPUID.reg)); __asm("fence"); riscv_set_csr(RVCSR_MIE_OFFSET, RVCSR_MIE_MSIE_BITS);

#define ENABLE_INTERRUPTS()       riscv_set_csr(RVCSR_MSTATUS_OFFSET, RVCSR_MSTATUS_MIE_BITS)
#define DISABLE_INTERRUPTS()      riscv_clear_csr(RVCSR_MSTATUS_OFFSET, RVCSR_MSTATUS_MIE_BITS)

#define OS_INTERRUPT_NESTING_DEPTH_LEVEL  8

//=========================================================================================
// Externs
//=========================================================================================
extern uint32 osIntVectTable;

//=========================================================================================
// Functions Prototype
//=========================================================================================
boolean osIsInterruptContext(void);
boolean osIsInterruptDisabled(void);

uint32 osGetActiveInterruptVectorId(void);
uint32 osGetInterruptGlobalMask(void);
uint32 osGetInterruptPriorityMask(void);
uint8 osGetCoreId(void);

void osCatchAllCpuExceptions(void);
void osHwTimerInit(void);
void osHwTimerReload(void);
void osHwTimerStart(void);
void osInitInterrupts(void);
void osSetInterruptPriorityMask(uint32 level);
void osClearPendingInterrupt(uint32_t InterruptId);

void osHwAcquireSpinLock(uint32_t* lock);
void osHwReleaseSpinLock(uint32_t* lock);

void osGenerateCrossCoreInterrupt(OsCoreId ActiveCore, OsCoreId TargetCore);
void osClearCrossCoreInterrupt(void);
uint32_t osHwTryToAcquireSpinLock(uint32_t* lock);
#endif
