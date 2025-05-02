// *****************************************************************************
// Filename    : OsHwPltfm.h
// 
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 27.12.2017
// 
// Description : Hardware Platform definition for ARM Cortex-M4
// 
// *****************************************************************************

#ifndef __HARDWARE_PLATFORM_H__
#define __HARDWARE_PLATFORM_H__

#include"OsTypes.h"
#include"OsAsm.h"

#define SCB_ICSR             0xE000ED04UL
#define SET_PENDSV()         (*(volatile uint32*)(SCB_ICSR)) |= 1UL<<28
#define CLEAR_PENDSV()       (*(volatile uint32*)(SCB_ICSR)) &= ~(1UL<<28)
#define GET_ICSR_VECTACTIVE()   ((*(volatile uint32*)(SCB_ICSR)) & (uint32)0xFFUL)

#define NVIC_ISERx ((volatile uint32*)(0xE000E100UL))
#define NVIC_IPRx  ((volatile uint8*)(0xE000E400UL))
#define NVIC_ICPRx ((volatile uint32*)(0xE000E280UL))
#define SCB_SHPRx  ((volatile uint8*)(0xE000ED18UL))
#define NVIC_ISPRx ((volatile uint32*)(0xE000E200UL))


#define osDispatch()         SET_PENDSV(); __asm("DSB"); __asm("NOP")

#define ENABLE_INTERRUPTS()       __asm("CPSIE I")
#define DISABLE_INTERRUPTS()      __asm("CPSID I")

#define OS_INTERRUPT_NESTING_DEPTH_LEVEL  8

//=========================================================================================
// SYSTICK
//=========================================================================================
#include "SysTickTimer.h"

boolean osIsInterruptContext(void);
boolean osIsInterruptDisabled(void);

uint32 osGetActiveInterruptVectorId(void);
uint32 osGetInterruptPriorityMask(void);
uint32 osGetInterruptGlobalMask(void);

void osCatchAllCpuExceptions(void);
void osHwTimerInit(void);
void osHwTimerReload(void);
void osHwTimerStart(void);
void osInitInterrupts(void);
void osSetInterruptPriorityMask(uint32 level);
void osClearPendingInterrupt(uint32_t InterruptId);
uint8 osGetCoreId(void);

void osHwAcquireSpinLock(uint32_t* lock);
void osHwReleaseSpinLock(uint32_t* lock);

void osGenerateCrossCoreInterrupt(OsCoreId ActiveCore, OsCoreId TargetCore);
void osClearCrossCoreInterrupt(void);
uint32_t osHwTryToAcquireSpinLock(uint32_t* lock);

#endif
