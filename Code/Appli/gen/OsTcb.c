
// **********************************************************************************************************************
//                                                  OSEK OS Configuration
// **********************************************************************************************************************

// **********************************************************************************************************************
// Filename        : OsTcb.c
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

#include "OsTcb.h"

//=============================================================================
// OS Configuration Summary
//=============================================================================
/*
  OS:
  ╒════════╤═════════════════════════════════╕
  │ Name   │ Properties                      │
  ╞════════╪═════════════════════════════════╡
  │ MyOSEK │ OsMulticore        =  TRUE      │
  │        │ OsStartupHook      =  TRUE      │
  │        │ OsErrorHook        =  TRUE      │
  │        │ OsShutdownHook     =  TRUE      │
  │        │ OsPreTaskHook      =  TRUE      │
  │        │ OsPostTaskHook     =  TRUE      │
  │        │ OsStackCheck       =  TRUE      │
  │        │ OsCpuFrequency     =  16.000000 │
  │        │ OsMaxVectorEntries =  99        │
  │        │ OsTickTime         =  1000      │
  ╘════════╧═════════════════════════════════╛

  Tasks:
  ╒════════╤════════╤════════════╤════════════╤══════════════╤═════════════╤══════════════╤═══════════════════╤═══════════════╕
  │ Task   │ Type   │ Schedule   │   Priority │   Activation │ Autostart   │ Stack size   │   Wait event mask │   Pinned core │
  ╞════════╪════════╪════════════╪════════════╪══════════════╪═════════════╪══════════════╪═══════════════════╪═══════════════╡
  │ T1     │ AUTO   │ FULL       │          2 │            1 │ TRUE        │ 0x400        │                 3 │             0 │
  ├────────┼────────┼────────────┼────────────┼──────────────┼─────────────┼──────────────┼───────────────────┼───────────────┤
  │ T2     │ AUTO   │ FULL       │          5 │            1 │ TRUE        │ 0x400        │                 1 │             0 │
  ├────────┼────────┼────────────┼────────────┼──────────────┼─────────────┼──────────────┼───────────────────┼───────────────┤
  │ T3     │ AUTO   │ FULL       │          1 │            1 │ TRUE        │ 0x400        │                 1 │             1 │
  ├────────┼────────┼────────────┼────────────┼──────────────┼─────────────┼──────────────┼───────────────────┼───────────────┤
  │ T4     │ AUTO   │ FULL       │         15 │            1 │ TRUE        │ 0x400        │                 1 │             1 │
  ╘════════╧════════╧════════════╧════════════╧══════════════╧═════════════╧══════════════╧═══════════════════╧═══════════════╛
  Total number of tasks: 4

  Task's events: 
  ╒════════╤═════════════════════════╕
  │ Task   │ Events                  │
  ╞════════╪═════════════════════════╡
  │ T1     │ EVT_BLINK_BLUE_LED_FAST │
  │        │ EVT_TOGGLE_BLUE_LED     │
  ├────────┼─────────────────────────┤
  │ T2     │ EVT_BLINK_BLUE_LED_SLOW │
  ├────────┼─────────────────────────┤
  │ T3     │ EVT_BLINK_RED_LED_FAST  │
  ├────────┼─────────────────────────┤
  │ T4     │ EVT_BLINK_RED_LED_SLOW  │
  ╘════════╧═════════════════════════╛

  Task's resources: 
  ╒════════╤══════════════════╕
  │ Task   │ Resources        │
  ╞════════╪══════════════════╡
  │ T1     │ OSRES_RESOURCE01 │
  ├────────┼──────────────────┤
  │ T2     │ OSRES_RESOURCE01 │
  ├────────┼──────────────────┤
  │ T3     │                  │
  ├────────┼──────────────────┤
  │ T4     │                  │
  ╘════════╧══════════════════╛

  Events:
  ╒═════════════════════════╤════════╕
  │ EVENT                   │ MASK   │
  ╞═════════════════════════╪════════╡
  │ EVT_BLINK_BLUE_LED_FAST │ 0x1    │
  ├─────────────────────────┼────────┤
  │ EVT_BLINK_BLUE_LED_SLOW │ 0x1    │
  ├─────────────────────────┼────────┤
  │ EVT_BLINK_RED_LED_FAST  │ 0x1    │
  ├─────────────────────────┼────────┤
  │ EVT_BLINK_RED_LED_SLOW  │ 0x1    │
  ├─────────────────────────┼────────┤
  │ EVT_TOGGLE_BLUE_LED     │ 0x2    │
  ╘═════════════════════════╧════════╛
  Total number of events: 5

  Resources:
  ╒═════════════════════╤════════════════════╤════════════╤════════╤═══════════════╕
  │ Resource            │   Priority ceiling │ Property   │   Mask │   Pinned core │
  ╞═════════════════════╪════════════════════╪════════════╪════════╪═══════════════╡
  │ OSRES_RESOURCE01    │                  2 │ STANDARD   │      3 │             0 │
  ├─────────────────────┼────────────────────┼────────────┼────────┼───────────────┤
  │ RES_SCHEDULER_CORE0 │                  3 │ STANDARD   │      3 │             0 │
  ├─────────────────────┼────────────────────┼────────────┼────────┼───────────────┤
  │ RES_SCHEDULER_CORE1 │                  2 │ STANDARD   │      3 │             1 │
  ╘═════════════════════╧════════════════════╧════════════╧════════╧═══════════════╛
  Total number of resources: 3

  Priority Scheme on core0:
  ╒═════════════════════╤════════════╕
  │ Name                │   Priority │
  ╞═════════════════════╪════════════╡
  │ T1                  │          0 │
  ├─────────────────────┼────────────┤
  │ T2                  │          1 │
  ├─────────────────────┼────────────┤
  │ OSRES_RESOURCE01    │          2 │
  ├─────────────────────┼────────────┤
  │ RES_SCHEDULER_CORE0 │          3 │
  ╘═════════════════════╧════════════╛
  Total priority levels on core0: 4

  Priority Scheme on core1:
  ╒═════════════════════╤════════════╕
  │ Name                │   Priority │
  ╞═════════════════════╪════════════╡
  │ T3                  │          0 │
  ├─────────────────────┼────────────┤
  │ T4                  │          1 │
  ├─────────────────────┼────────────┤
  │ RES_SCHEDULER_CORE1 │          2 │
  ╘═════════════════════╧════════════╛
  Total priority levels on core1: 3

  Alarms:
  ╒═══════════════════════════╤═════════════╤═════════════╤═════════════════════════╤════════╤══════════╤═════════════╤════════════╤═══════════════╕
  │ Name                      │ Alarmtime   │ Cycletime   │ Event                   │ Task   │ Action   │ Autostart   │ Callback   │   Pinned core │
  ╞═══════════════════════════╪═════════════╪═════════════╪═════════════════════════╪════════╪══════════╪═════════════╪════════════╪═══════════════╡
  │ ALARM_BLINK_BLUE_LED_FAST │ -           │ -           │ EVT_BLINK_BLUE_LED_FAST │ T1     │ SETEVENT │ FALSE       │ -          │             0 │
  ├───────────────────────────┼─────────────┼─────────────┼─────────────────────────┼────────┼──────────┼─────────────┼────────────┼───────────────┤
  │ ALARM_BLINK_BLUE_LED_SLOW │ -           │ -           │ EVT_BLINK_BLUE_LED_SLOW │ T2     │ SETEVENT │ FALSE       │ -          │             0 │
  ├───────────────────────────┼─────────────┼─────────────┼─────────────────────────┼────────┼──────────┼─────────────┼────────────┼───────────────┤
  │ ALARM_BLINK_RED_LED_FAST  │ -           │ -           │ EVT_BLINK_RED_LED_FAST  │ T3     │ SETEVENT │ FALSE       │ -          │             1 │
  ├───────────────────────────┼─────────────┼─────────────┼─────────────────────────┼────────┼──────────┼─────────────┼────────────┼───────────────┤
  │ ALARM_BLINK_RED_LED_SLOW  │ -           │ -           │ EVT_BLINK_RED_LED_SLOW  │ T4     │ SETEVENT │ FALSE       │ -          │             1 │
  ╘═══════════════════════════╧═════════════╧═════════════╧═════════════════════════╧════════╧══════════╧═════════════╧════════════╧═══════════════╛
  Total number of alarms: 4

  Interrupts on core0:
  ╒═══════════════════════════╤════════════╤══════════╤════════╤═══════════╕
  │ Name                      │   Category │   Vector │   Prio │ Nesting   │
  ╞═══════════════════════════╪════════════╪══════════╪════════╪═══════════╡
  │ osCrossCoreReceiveRequest │          2 │       80 │      0 │ FALSE     │
  ╘═══════════════════════════╧════════════╧══════════╧════════╧═══════════╛
  Total number of used interrupts: 1
  Total number of CPU interrupts : 99

  Interrupts on core1:
  ╒═══════════════════════════╤════════════╤══════════╤════════╤═══════════╕
  │ Name                      │   Category │   Vector │   Prio │ Nesting   │
  ╞═══════════════════════════╪════════════╪══════════╪════════╪═══════════╡
  │ osCrossCoreReceiveRequest │          2 │       79 │      0 │ FALSE     │
  ╘═══════════════════════════╧════════════╧══════════╧════════╧═══════════╛
  Total number of used interrupts: 1
  Total number of CPU interrupts : 99
*/

/********************************************************************************************************************/
/* Task: T1 (core0) */
/********************************************************************************************************************/
TASK(T1);
CREATE_STACK(T1, 0x400);
static OsTcbType OsTcb_T1 = {
                                         pTASK(T1), /* Function */
                                         0, /* FixedPrio */
                                         TRUE, /* Autostart */
                                         pTSTACK(T1), /* pstack_top */
                                         pBSTACK(T1), /* pstack_bot */
                                         pTSTACK(T1), /* pCurrentStackPointer */
                                         SUSPENDED, /* TaskStatus */
                                         0, /* Prio */
                                         0, /* CeilingPrio */
                                         0, /* SetEvtMask */
                                         0x3, /* WaitEvtMask */ 
                                         1, /* MaxAllowedMultipleActivation */
                                         0, /* MultipleActivation */
                                         EXTENDED, /* TaskType */
                                         FULL_PREEMPT /* TaskSchedType */
                                       };

/********************************************************************************************************************/
/* Task: T2 (core0) */
/********************************************************************************************************************/
TASK(T2);
CREATE_STACK(T2, 0x400);
static OsTcbType OsTcb_T2 = {
                                         pTASK(T2), /* Function */
                                         1, /* FixedPrio */
                                         TRUE, /* Autostart */
                                         pTSTACK(T2), /* pstack_top */
                                         pBSTACK(T2), /* pstack_bot */
                                         pTSTACK(T2), /* pCurrentStackPointer */
                                         SUSPENDED, /* TaskStatus */
                                         0, /* Prio */
                                         0, /* CeilingPrio */
                                         0, /* SetEvtMask */
                                         0x1, /* WaitEvtMask */ 
                                         1, /* MaxAllowedMultipleActivation */
                                         0, /* MultipleActivation */
                                         EXTENDED, /* TaskType */
                                         FULL_PREEMPT /* TaskSchedType */
                                       };

/********************************************************************************************************************/
/* Task: T3 (core1) */
/********************************************************************************************************************/
TASK(T3);
CREATE_STACK(T3, 0x400);
static OsTcbType OsTcb_T3 = {
                                         pTASK(T3), /* Function */
                                         0, /* FixedPrio */
                                         TRUE, /* Autostart */
                                         pTSTACK(T3), /* pstack_top */
                                         pBSTACK(T3), /* pstack_bot */
                                         pTSTACK(T3), /* pCurrentStackPointer */
                                         SUSPENDED, /* TaskStatus */
                                         0, /* Prio */
                                         0, /* CeilingPrio */
                                         0, /* SetEvtMask */
                                         0x1, /* WaitEvtMask */ 
                                         1, /* MaxAllowedMultipleActivation */
                                         0, /* MultipleActivation */
                                         EXTENDED, /* TaskType */
                                         FULL_PREEMPT /* TaskSchedType */
                                       };

/********************************************************************************************************************/
/* Task: T4 (core1) */
/********************************************************************************************************************/
TASK(T4);
CREATE_STACK(T4, 0x400);
static OsTcbType OsTcb_T4 = {
                                         pTASK(T4), /* Function */
                                         1, /* FixedPrio */
                                         TRUE, /* Autostart */
                                         pTSTACK(T4), /* pstack_top */
                                         pBSTACK(T4), /* pstack_bot */
                                         pTSTACK(T4), /* pCurrentStackPointer */
                                         SUSPENDED, /* TaskStatus */
                                         0, /* Prio */
                                         0, /* CeilingPrio */
                                         0, /* SetEvtMask */
                                         0x1, /* WaitEvtMask */ 
                                         1, /* MaxAllowedMultipleActivation */
                                         0, /* MultipleActivation */
                                         EXTENDED, /* TaskType */
                                         FULL_PREEMPT /* TaskSchedType */
                                       };

/********************************************************************************************************************/
/* OsTasksConfig (core0) */
/********************************************************************************************************************/
static const OsTcbType* OsTasksConfig_core0[2] = {
    &OsTcb_T1,
    &OsTcb_T2,
};

/********************************************************************************************************************/
/* OsTasksConfig (core1) */
/********************************************************************************************************************/
static const OsTcbType* OsTasksConfig_core1[2] = {
    &OsTcb_T3,
    &OsTcb_T4,
};

/********************************************************************************************************************/
/* osTaskCoreAsgnLookupTable */
/********************************************************************************************************************/
const osObjectCoreAsgn_t osTaskCoreAsgnLookupTable[4] = {
    {.local_id=0, .pinned_core=0}, /* T1 */
    {.local_id=1, .pinned_core=0}, /* T2 */
    {.local_id=0, .pinned_core=1}, /* T3 */
    {.local_id=1, .pinned_core=1}, /* T4 */
};

/********************************************************************************************************************/
/* Alarm: ALARM_BLINK_BLUE_LED_FAST (core0) */
/********************************************************************************************************************/
static OsAlarmConfigType OsAlarm_ALARM_BLINK_BLUE_LED_FAST = {
                                                     0, /* InitTicks */
                                                     0, /* InitCycles */
                                                     EVT_BLINK_BLUE_LED_FAST, /* Event */
                                                     T1, /* TaskId */
                                                     ALARM_SET_EVENT, /* Action */
                                                     ALARM_NO_AUTOSTART, /* AutoStart */
                                                     ALARM_FREE, /* Status */
                                                     0, /* Alarmtype */
                                                     0, /* AlarmCategory */
                                                     0, /* AlarmCheckPoint */
                                                     0 /* CallBackFunc */
                                                   };

/********************************************************************************************************************/
/* Alarm: ALARM_BLINK_BLUE_LED_SLOW (core0) */
/********************************************************************************************************************/
static OsAlarmConfigType OsAlarm_ALARM_BLINK_BLUE_LED_SLOW = {
                                                     0, /* InitTicks */
                                                     0, /* InitCycles */
                                                     EVT_BLINK_BLUE_LED_SLOW, /* Event */
                                                     T2, /* TaskId */
                                                     ALARM_SET_EVENT, /* Action */
                                                     ALARM_NO_AUTOSTART, /* AutoStart */
                                                     ALARM_FREE, /* Status */
                                                     0, /* Alarmtype */
                                                     0, /* AlarmCategory */
                                                     0, /* AlarmCheckPoint */
                                                     0 /* CallBackFunc */
                                                   };

/********************************************************************************************************************/
/* Alarm: ALARM_BLINK_RED_LED_FAST (core1) */
/********************************************************************************************************************/
static OsAlarmConfigType OsAlarm_ALARM_BLINK_RED_LED_FAST = {
                                                     0, /* InitTicks */
                                                     0, /* InitCycles */
                                                     EVT_BLINK_RED_LED_FAST, /* Event */
                                                     T3, /* TaskId */
                                                     ALARM_SET_EVENT, /* Action */
                                                     ALARM_NO_AUTOSTART, /* AutoStart */
                                                     ALARM_FREE, /* Status */
                                                     0, /* Alarmtype */
                                                     0, /* AlarmCategory */
                                                     0, /* AlarmCheckPoint */
                                                     0 /* CallBackFunc */
                                                   };

/********************************************************************************************************************/
/* Alarm: ALARM_BLINK_RED_LED_SLOW (core1) */
/********************************************************************************************************************/
static OsAlarmConfigType OsAlarm_ALARM_BLINK_RED_LED_SLOW = {
                                                     0, /* InitTicks */
                                                     0, /* InitCycles */
                                                     EVT_BLINK_RED_LED_SLOW, /* Event */
                                                     T4, /* TaskId */
                                                     ALARM_SET_EVENT, /* Action */
                                                     ALARM_NO_AUTOSTART, /* AutoStart */
                                                     ALARM_FREE, /* Status */
                                                     0, /* Alarmtype */
                                                     0, /* AlarmCategory */
                                                     0, /* AlarmCheckPoint */
                                                     0 /* CallBackFunc */
                                                   };

/********************************************************************************************************************/
/* OsAlarmsConfig (core0) */
/********************************************************************************************************************/
static const OsAlarmConfigType* OsAlarmsConfig_core0[2] = {
    &OsAlarm_ALARM_BLINK_BLUE_LED_FAST,
    &OsAlarm_ALARM_BLINK_BLUE_LED_SLOW,
};

/********************************************************************************************************************/
/* OsAlarmsConfig (core1) */
/********************************************************************************************************************/
static const OsAlarmConfigType* OsAlarmsConfig_core1[2] = {
    &OsAlarm_ALARM_BLINK_RED_LED_FAST,
    &OsAlarm_ALARM_BLINK_RED_LED_SLOW,
};

/********************************************************************************************************************/
/* osAlarmCoreAsgnLookupTable */
/********************************************************************************************************************/
const osObjectCoreAsgn_t osAlarmCoreAsgnLookupTable[4] = {
    {.local_id=0, .pinned_core=0}, /* ALARM_BLINK_BLUE_LED_FAST */
    {.local_id=1, .pinned_core=0}, /* ALARM_BLINK_BLUE_LED_SLOW */
    {.local_id=0, .pinned_core=1}, /* ALARM_BLINK_RED_LED_FAST */
    {.local_id=1, .pinned_core=1}, /* ALARM_BLINK_RED_LED_SLOW */
};

/********************************************************************************************************************/
/* Resource: OSRES_RESOURCE01 (core0) */
/********************************************************************************************************************/
static OsResourceConfigType OsResource_OSRES_RESOURCE01 = {
                                                            2, /* ResCeilingPrio */
                                                            0, /* CurrentOccupiedTask */
                                                            3 /* AuthorizedTask */
                                                          };

/********************************************************************************************************************/
/* Resource: RES_SCHEDULER_CORE0 (core0) */
/********************************************************************************************************************/
static OsResourceConfigType OsResource_RES_SCHEDULER_CORE0 = {
                                                            3, /* ResCeilingPrio */
                                                            0, /* CurrentOccupiedTask */
                                                            3 /* AuthorizedTask */
                                                          };

/********************************************************************************************************************/
/* Resource: RES_SCHEDULER_CORE1 (core1) */
/********************************************************************************************************************/
static OsResourceConfigType OsResource_RES_SCHEDULER_CORE1 = {
                                                            2, /* ResCeilingPrio */
                                                            0, /* CurrentOccupiedTask */
                                                            3 /* AuthorizedTask */
                                                          };

/********************************************************************************************************************/
/* OsResourceConfig (core0) */
/********************************************************************************************************************/
static const OsResourceConfigType* OsResourcesConfig_core0[2] = {
    &OsResource_OSRES_RESOURCE01,
    &OsResource_RES_SCHEDULER_CORE0,
};

/********************************************************************************************************************/
/* OsResourceConfig (core1) */
/********************************************************************************************************************/
static const OsResourceConfigType* OsResourcesConfig_core1[1] = {
    &OsResource_RES_SCHEDULER_CORE1,
};

/********************************************************************************************************************/
/* OsResSchedulerMappingPerCore */
/********************************************************************************************************************/
static const OsResourceId_t osResSchedulerMappingPerCore[2] __attribute__((used)) = {
    RES_SCHEDULER_CORE0,
    RES_SCHEDULER_CORE1,
};

/********************************************************************************************************************/
/* osResourceCoreAsgnLookupTable */
/********************************************************************************************************************/
const osObjectCoreAsgn_t osResourceCoreAsgnLookupTable[3] = {
    {.local_id=0, .pinned_core=0}, /* OSRES_RESOURCE01 */
    {.local_id=1, .pinned_core=0}, /* RES_SCHEDULER_CORE0 */
    {.local_id=0, .pinned_core=1}, /* RES_SCHEDULER_CORE1 */
};

/********************************************************************************************************************/
/* Spinlock: OSSPINL_SPINLOCK01 */
/********************************************************************************************************************/
static OsSpinlockConfigType OsSpinlock_OSSPINL_SPINLOCK01 = {0};

/********************************************************************************************************************/
/* Spinlock: OSSPINL_SPINLOCK02 */
/********************************************************************************************************************/
static OsSpinlockConfigType OsSpinlock_OSSPINL_SPINLOCK02 = {0};

/********************************************************************************************************************/
/* OsSpinlockConfig */
/********************************************************************************************************************/
static const OsSpinlockConfigType* OsSpinlocksConfig[2] = {
    &OsSpinlock_OSSPINL_SPINLOCK01,
    &OsSpinlock_OSSPINL_SPINLOCK02,
};

/********************************************************************************************************************/
/* Interrupt LUT (Core0) */
/********************************************************************************************************************/
const OsIntIsrLtType OsIsrLookupTable_core0[99] = {
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 0 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 1 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 2 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 3 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 4 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 5 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 6 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 7 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 8 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 9 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 10 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 11 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 12 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 13 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 14 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 15 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 16 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 17 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 18 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 19 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 20 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 21 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 22 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 23 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 24 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 25 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 26 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 27 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 28 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 29 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 30 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 31 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 32 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 33 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 34 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 35 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 36 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 37 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 38 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 39 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 40 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 41 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 42 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 43 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 44 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 45 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 46 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 47 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 48 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 49 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 50 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 51 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 52 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 53 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 54 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 55 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 56 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 57 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 58 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 59 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 60 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 61 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 62 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 63 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 64 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 65 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 66 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 67 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 68 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 69 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 70 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 71 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 72 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 73 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 74 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 75 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 76 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 77 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 78 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 79 */
    {pISR(osCrossCoreReceiveRequest), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2}, /* Interrupt vector 80 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 81 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 82 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 83 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 84 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 85 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 86 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 87 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 88 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 89 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 90 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 91 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 92 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 93 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 94 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 95 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 96 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 97 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 98 */
};

/********************************************************************************************************************/
/* OsInterruptsConfig_core0 */
/********************************************************************************************************************/
OsInterruptConfigType OsInterruptsConfig_core0 = {
    99, /* OsNbrOfInterrupts */
    0xfffffffful, /* OsIntCat1LowestPrioLevel */
    0, /* OsInterruptNestingDepth */
    0, /* OsInterruptSavedPrioLevel */
    0, /* OsInterruptSavedGlobalMask */
    0, /* OsInterruptSuspendOsNestingCall */
    0, /* OsInterruptSuspendAllNestingCall */
    {0}, /* OsIntNestSavedStackPointer[OS_INTERRUPT_NESTING_DEPTH_LEVEL] */
    {0}, /* OsIntNestSavedPrioLevel[OS_INTERRUPT_NESTING_DEPTH_LEVEL] */
    (OsIntIsrLtType*)&OsIsrLookupTable_core0[0] /* OsIsrLookupTablePtr */
};

/********************************************************************************************************************/
/* Interrupt LUT (Core1) */
/********************************************************************************************************************/
const OsIntIsrLtType OsIsrLookupTable_core1[99] = {
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 0 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 1 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 2 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 3 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 4 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 5 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 6 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 7 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 8 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 9 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 10 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 11 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 12 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 13 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 14 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 15 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 16 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 17 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 18 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 19 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 20 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 21 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 22 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 23 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 24 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 25 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 26 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 27 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 28 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 29 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 30 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 31 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 32 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 33 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 34 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 35 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 36 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 37 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 38 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 39 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 40 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 41 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 42 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 43 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 44 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 45 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 46 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 47 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 48 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 49 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 50 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 51 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 52 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 53 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 54 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 55 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 56 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 57 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 58 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 59 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 60 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 61 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 62 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 63 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 64 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 65 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 66 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 67 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 68 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 69 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 70 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 71 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 72 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 73 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 74 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 75 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 76 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 77 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 78 */
    {pISR(osCrossCoreReceiveRequest), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2}, /* Interrupt vector 79 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 80 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 81 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 82 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 83 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 84 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 85 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 86 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 87 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 88 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 89 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 90 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 91 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 92 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 93 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 94 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 95 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 96 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 97 */
    {pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2},  /* Interrupt vector 98 */
};

/********************************************************************************************************************/
/* OsInterruptsConfig_core1 */
/********************************************************************************************************************/
OsInterruptConfigType OsInterruptsConfig_core1 = {
    99, /* OsNbrOfInterrupts */
    0xfffffffful, /* OsIntCat1LowestPrioLevel */
    0, /* OsInterruptNestingDepth */
    0, /* OsInterruptSavedPrioLevel */
    0, /* OsInterruptSavedGlobalMask */
    0, /* OsInterruptSuspendOsNestingCall */
    0, /* OsInterruptSuspendAllNestingCall */
    {0}, /* OsIntNestSavedStackPointer[OS_INTERRUPT_NESTING_DEPTH_LEVEL] */
    {0}, /* OsIntNestSavedPrioLevel[OS_INTERRUPT_NESTING_DEPTH_LEVEL] */
    (OsIntIsrLtType*)&OsIsrLookupTable_core1[0] /* OsIsrLookupTablePtr */
};

/********************************************************************************************************************/
/* osLcCfg_core0 */
/********************************************************************************************************************/
static Ocb_t osLcCfg_core0 = {
    (OsTcbType**)&OsTasksConfig_core0[0], /* pTcb */
    (OsAlarmConfigType**)&OsAlarmsConfig_core0[0], /* pAlarm */
    (OsResourceConfigType**)&OsResourcesConfig_core0[0], /* pRes */
    (OsSpinlockConfigType**)&OsSpinlocksConfig[0], /* pSpinlock */
    (OsInterruptConfigType*)&OsInterruptsConfig_core0, /* pInt */
    0, /* OsIsRunning */
    0, /* OsHighPrioReadyTaskId */
    OS_NUMBER_OF_TASKS_ON_CORE0, /* OsCurrentTaskId */
    0, /* OsSysTickCounter */
    0, /* OsIntCallDispatcher */
    0, /* OsLockDispatcher */
    0, /* OsCat2InterruptLevel */
    0, /* OsCurrentSystemStackPtr */
    {0}, /* OsCrossCoreBuffer[10] */
    0, /* OsCrossCoreBufferLock */
    0, /* OsCrossCoreBufferSyncFlag */
    {0}, /* OsHwSchedPrioReg */
    0, /* OsSpinlockNestingDepth */
    2, /* OsNumberOfTasks */
    2, /* OsNumberOfAlarms */
    2,  /* OsNumberOfResources */
    osStartupHook_core0, /* OsStartupHook */
    osPreTaskHook_core0, /* OsPreTaskHook */
    osPostTaskHook_core0, /* OsPostTaskHook */
    osShutdownHook_core0, /* OsShutdownHook */
    osErrorHook_core0 /* OsErrorHook */
};

/********************************************************************************************************************/
/* osLcCfg_core1 */
/********************************************************************************************************************/
static Ocb_t osLcCfg_core1 = {
    (OsTcbType**)&OsTasksConfig_core1[0], /* pTcb */
    (OsAlarmConfigType**)&OsAlarmsConfig_core1[0], /* pAlarm */
    (OsResourceConfigType**)&OsResourcesConfig_core1[0], /* pRes */
    (OsSpinlockConfigType**)&OsSpinlocksConfig[0], /* pSpinlock */
    (OsInterruptConfigType*)&OsInterruptsConfig_core1, /* pInt */
    0, /* OsIsRunning */
    0, /* OsHighPrioReadyTaskId */
    OS_NUMBER_OF_TASKS_ON_CORE1, /* OsCurrentTaskId */
    0, /* OsSysTickCounter */
    0, /* OsIntCallDispatcher */
    0, /* OsLockDispatcher */
    0, /* OsCat2InterruptLevel */
    0, /* OsCurrentSystemStackPtr */
    {0}, /* OsCrossCoreBuffer[10] */
    0, /* OsCrossCoreBufferLock */
    0, /* OsCrossCoreBufferSyncFlag */
    {0}, /* OsHwSchedPrioReg */
    0, /* OsSpinlockNestingDepth */
    2, /* OsNumberOfTasks */
    2, /* OsNumberOfAlarms */
    1,  /* OsNumberOfResources */
    osStartupHook_core1, /* OsStartupHook */
    osPreTaskHook_core1, /* OsPreTaskHook */
    osPostTaskHook_core1, /* OsPostTaskHook */
    osShutdownHook_core1, /* OsShutdownHook */
    osErrorHook_core1 /* OsErrorHook */
};

/********************************************************************************************************************/
/* OCB Configuration */
/********************************************************************************************************************/
volatile Ocb_t* OCB_Cfg[2] = {
    (Ocb_t*)&osLcCfg_core0,
    (Ocb_t*)&osLcCfg_core1,
};

/********************************************************************************************************************/
/* logical to physical core id mapping*/
/********************************************************************************************************************/
const uint8 osLogicalToPhysicalCoreIdMapping[2] = {
    0, /* ESP32S3_0 : ESP - XTENSA_LX7 */
    1, /* ESP32S3_1 : ESP - XTENSA_LX7 */
};
