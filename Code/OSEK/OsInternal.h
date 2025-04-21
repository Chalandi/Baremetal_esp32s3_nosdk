// *****************************************************************************
// Filename    : OsInternal.h
// 
// OS          : OSEK 2.2.3  
// 
// CC          : ECC1/BCC1
//
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 27.12.2017
// 
// Description : OS internal services
// 
// *****************************************************************************

#ifndef __OS_INTERNAL_H__
#define __OS_INTERNAL_H__

#include"OsTypes.h"

OsStatusType osSchedule(void);

boolean osIsCat2IntContext(void);

uint32 osDispatcher(uint32 StackPtr);
uint32 osGetHighPrioReadyTaskIdx(const uint32 prio);
uint32 osGetIntNestingLevel(void);
uint32 osGetMaximumStackUsage(uint32 LocalTaskId);
uint32 osGetSavedStackPointer(void);
uint32 osIntCallDispatch(uint32 StackPtr);
uint64 osGetSystemTicksCounter(void);
uint64 osGetSystemTicksElapsedTime(uint64 prvTicks);
uint8_t osGetLogicalCoreId(uint8_t PhysicalCoreId);

void OsIsr_UndefinedFunc(void);
void osAlarmsManagement(void);
void osClearTaskPrioReady(const uint32 prio);
uintptr_t osGetOsHwSchedPrioRegAddress(void);
void osDecNestingDepthLevel(void);
void osDisableIntNesting(void);
void osEnableIntNesting(void);
void osErrTaskExitWithoutTerminate(void);
void osIncNestingDepthLevel(void);
void osKernelError(OsStatusType err);
void osRunCat2Isr(void);
void osSetTaskPrioReady(const uint32 prio);
void osStoreStackPointer(uint32 StackPtrValue);
void osMulticoreSync(void);

osObjectCoreAsgn_t osGetLocalAlarmAssignment(uint32_t SystemAlarmId);
osObjectCoreAsgn_t osGetLocalTaskAssignment(uint32_t SystemTaskId);
osObjectCoreAsgn_t osGetLocalResourceAssignment(uint32_t SystemResourceId);

ISR(SysTickTimer);
ISR(UndefinedFunc);
ISR(osCrossCoreReceiveRequest);

uint8_t  osCircularFifoQueue_IsEmpty(OsCircularFifoQueueDataType* pfifo);
uint8_t  osCircularFifoQueue_IsFull(OsCircularFifoQueueDataType* pfifo);
uint32_t osCircularFifoQueue_GetAvailableSize(OsCircularFifoQueueDataType* pfifo);
uint32_t osCircularFifoQueue_GetCurrentConsumption(OsCircularFifoQueueDataType* pfifo);
uint8_t  osCircularFifoQueue_Push(OsCircularFifoQueueDataType* pfifo, uint8_t* data, uint32_t size);
uint8_t  osCircularFifoQueue_Pop(OsCircularFifoQueueDataType* pfifo, uint8_t* data, uint32_t size);
uint8_t  osCircularFifoQueue_ReadCopy(OsCircularFifoQueueDataType* pfifo, uint8_t* data, uint32_t size);


OsStatusType osCrossCore_SetEvent(OsCoreId ActiveCore, OsCoreId TargetCore, OsTaskType TaskID, OsEventMaskType Mask);
OsStatusType osCrossCore_GetEvent(OsCoreId ActiveCore, OsCoreId TargetCore, OsTaskType TaskID, OsEventMaskRefType Event);
OsStatusType osCrossCore_ActivateTask(OsCoreId ActiveCore, OsCoreId TargetCore, OsTaskType TaskID);
OsStatusType osCrossCore_GetTaskState(OsCoreId ActiveCore, OsCoreId TargetCore, OsTaskType TaskID, OsTaskStateRefType State);
OsStatusType osCrossCore_GetAlarm(OsCoreId ActiveCore, OsCoreId TargetCore, OsAlarmType AlarmID, OsTickRefType Tick);
OsStatusType osCrossCore_SetRelAlarm(OsCoreId ActiveCore, OsCoreId TargetCore, OsAlarmType AlarmID, OsTickType increment, OsTickType cycle);
OsStatusType osCrossCore_SetAbsAlarm(OsCoreId ActiveCore, OsCoreId TargetCore, OsAlarmType AlarmID, OsTickType start, OsTickType cycle);
OsStatusType osCrossCore_CancelAlarm(OsCoreId ActiveCore, OsCoreId TargetCore, OsAlarmType AlarmID);


#endif
