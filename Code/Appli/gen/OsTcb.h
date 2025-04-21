
// **********************************************************************************************************************
//                                                  OSEK OS Configuration
// **********************************************************************************************************************

// **********************************************************************************************************************
// Filename        : OsTcb.h
//
// Author          : Amine Chalandi
//
// Owner           : Amine Chalandi
//
// Generator       : OilGenTool
//
// Oil file        : D:/Git_Repo/Baremetal_esp32s3_nosdk/Build/../Code/Appli/osek_xtensa_lx7.oil
//
// Generation Time : 22.04.2025 01:49:39
//
// Description     : Auto-generated OS Configuration file
//
// **********************************************************************************************************************

#ifndef __OS_TCB_H__
#define __OS_TCB_H__

#include "OsTypes.h"

typedef enum {
    T1,
    T2,
    T3,
    T4,
    OS_INVALID_TASK,
    OS_NUMBER_OF_TASKS = OS_INVALID_TASK,
}OsTaskId_t;

#define OS_NUMBER_OF_TASKS_ON_CORE0 2
#define OS_NUMBER_OF_TASKS_ON_CORE1 2


typedef enum {
    ALARM_BLINK_BLUE_LED_FAST,
    ALARM_BLINK_BLUE_LED_SLOW,
    ALARM_BLINK_RED_LED_FAST,
    ALARM_BLINK_RED_LED_SLOW,
    OS_INVALID_ALARM,
    OS_NUMBER_OF_ALARMS = OS_INVALID_ALARM
}OsAlarmId_t;

typedef enum {
    OSRES_RESOURCE01,
    RES_SCHEDULER_CORE0,
    RES_SCHEDULER_CORE1,
    OS_INVALID_RESOURCE,
    OS_NUMBER_OF_RESOURCES = OS_INVALID_RESOURCE
}OsResourceId_t;

#define RES_SCHEDULER osResSchedulerMappingPerCore[osGetLogicalCoreId(osGetCoreId())]

typedef enum {
    OSSPINL_SPINLOCK01,
    OSSPINL_SPINLOCK02,
    OS_INVALID_SPINLOCK,
    OS_NUMBER_OF_SPINLOCKS = OS_INVALID_SPINLOCK
}OsSpinlockId_t;

/* task function prototype */
TASK(T1);
TASK(T2);
TASK(T3);
TASK(T4);

/* hooks function */
#define OS_STARTUPHOOK
#define OS_PRETASKHOOK
#define OS_POSTTASKHOOK
#define OS_SHUTDOWNHOOK
#define OS_ERRORHOOK


void osStartupHook_core0(void);
void osPreTaskHook_core0(void);
void osPostTaskHook_core0(void);
void osShutdownHook_core0(OsStatusType error);
void osErrorHook_core0(OsStatusType error);
void osStartupHook_core1(void);
void osPreTaskHook_core1(void);
void osPostTaskHook_core1(void);
void osShutdownHook_core1(OsStatusType error);
void osErrorHook_core1(OsStatusType error);


/* event mask definition */
#define EVT_BLINK_BLUE_LED_FAST    0x1
#define EVT_BLINK_BLUE_LED_SLOW    0x1
#define EVT_BLINK_RED_LED_FAST    0x1
#define EVT_BLINK_RED_LED_SLOW    0x1
#define EVT_TOGGLE_BLUE_LED    0x2

/* wait event mask for each task */
#define T1_WAIT_EVENT_MASK    0x3
#define T2_WAIT_EVENT_MASK    0x1
#define T3_WAIT_EVENT_MASK    0x1
#define T4_WAIT_EVENT_MASK    0x1

#define OS_NUMBER_OF_CORES    2UL

extern const OsIntIsrLtType OsIsrLookupTable_core0[99];
extern const OsIntIsrLtType OsIsrLookupTable_core1[99];
extern const osObjectCoreAsgn_t osTaskCoreAsgnLookupTable[4];
extern const osObjectCoreAsgn_t osAlarmCoreAsgnLookupTable[4];
extern const osObjectCoreAsgn_t osResourceCoreAsgnLookupTable[3];
extern const uint8 osLogicalToPhysicalCoreIdMapping[2];

extern volatile Ocb_t* OCB_Cfg[2];

/* ISR functions prototypes */
ISR(osCrossCoreReceiveRequest);
ISR(osCrossCoreReceiveRequest);
ISR(Undefined);

#endif /* __OS_TCB_H__ */
