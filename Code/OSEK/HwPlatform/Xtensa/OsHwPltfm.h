// *****************************************************************************
// Filename    : OsHwPltfm.h
// 
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 18.01.2023
// 
// Description : Hardware platform header file
// 
// *****************************************************************************

#ifndef __HARDWARE_PLATFORM_H__
#define __HARDWARE_PLATFORM_H__

//=========================================================================================
// Includes
//=========================================================================================
#include"OsTypes.h"
#include"OsAsm.h"
#include"esp32s3.h"

//=========================================================================================
// Definitions
//=========================================================================================
#define osDispatch()              osHwDispatch()
#define ENABLED_IRQ_MASK          (uint32_t)((1ul << 7) | (1ul << 6) | (1ul << 5))
#define ENABLE_INTERRUPTS()       osSetEnableIRQMask(ENABLED_IRQ_MASK)//osUnmaskAllInterrupts()
#define DISABLE_INTERRUPTS()      osSetEnableIRQMask(0)//osMaskAllInterrupts()

#define OS_INTERRUPT_NESTING_DEPTH_LEVEL  8

//=========================================================================================
// ESP32S3 Interrupt Matrix
//=========================================================================================
#define INTERRUPT_MATRIX_BASE  0x600C2000UL

#define ACTIVE_CAT1_IRQ_NUMBER  31ul
#define ACTIVE_CAT2_IRQ_NUMBER  5ul

typedef uint32_t (*pINTERRUPT_COREx_INTR_STATUS_REG)[4];
typedef uint32_t (*pINTERRUPT_COREx_INTR_CONFIG_REG)[99];

#define SYSTEM_BASE  0x600C0000UL
#define SYSTEM_CPU_INTR_FROM_CPU_0   *(volatile uint32_t*)(SYSTEM_BASE + 0x0030UL)
#define SYSTEM_CPU_INTR_FROM_CPU_1   *(volatile uint32_t*)(SYSTEM_BASE + 0x0034UL)

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
uint32 osHwTryToAcquireSpinLock(uint32* lock);

void osGenerateCrossCoreInterrupt(OsCoreId ActiveCore, OsCoreId TargetCore);
void osClearCrossCoreInterrupt(void);
#endif
