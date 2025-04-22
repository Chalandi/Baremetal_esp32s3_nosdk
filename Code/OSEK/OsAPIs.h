// *****************************************************************************
// Filename    : OsAPIs.h
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
// Description : Os APIs prototypes
// 
// *****************************************************************************

#ifndef __OSAPIS_H__
#define __OSAPIS_H__

#include"OsTypes.h"
#include"OsInternal.h"

OsAppModeType OS_GetActiveApplicationMode(void);
OsStatusType OS_ActivateTask(OsTaskType TaskID);
OsStatusType OS_CancelAlarm(OsAlarmType AlarmID);
OsStatusType OS_ChainTask(OsTaskType TaskID);
OsStatusType OS_ClearEvent(OsEventMaskType Mask);
OsStatusType OS_GetAlarm(OsAlarmType AlarmID, OsTickRefType Tick);
OsStatusType OS_GetAlarmBase(OsAlarmType AlarmID, OsAlarmBaseRefType Info);
OsStatusType OS_GetEvent(OsTaskType TaskID, OsEventMaskRefType Event);
OsStatusType OS_GetResource(OsResourceType ResID);
OsStatusType OS_GetTaskID(OsTaskRefType TaskID);
OsStatusType OS_GetTaskState(OsTaskType TaskID, OsTaskStateRefType State);
OsStatusType OS_ReleaseResource(OsResourceType ResID);
OsStatusType OS_Schedule(void);
OsStatusType OS_SetAbsAlarm(OsAlarmType AlarmID, OsTickType start, OsTickType cycle);
OsStatusType OS_SetEvent(OsTaskType TaskID, OsEventMaskType Mask);
OsStatusType OS_SetRelAlarm(OsAlarmType AlarmID, OsTickType increment, OsTickType cycle);
OsStatusType OS_TerminateTask(void);
OsStatusType OS_WaitEvent(OsEventMaskType Mask);
OsStatusType OS_GetSpinlock(OsSpinlockIdType SpinlockId);
OsStatusType OS_ReleaseSpinlock(OsSpinlockIdType SpinlockId);
OsStatusType OS_TryToGetSpinlock(OsSpinlockIdType SpinlockId, OsTryToGetSpinlockType* Success);

IpcStatus OS_IpcSendData(OsIpcMbxCfgType* const  MsgBox, OsIpcMbxdataType const* Msgdata);
IpcStatus OS_IpcReceiveData(OsIpcMbxCfgType* const  MsgBox, OsIpcMbxdataType const* Msgdata);

void OS_DisableAllInterrupts(void);
void OS_EnableAllInterrupts(void);
void OS_ResumeAllInterrupts(void);
void OS_ResumeOSInterrupts(void);
void OS_ShutdownOS(OsStatusType Error);
void OS_StartOS(OsAppModeType Mode);
void OS_SuspendAllInterrupts(void);
void OS_SuspendOSInterrupts(void);

#endif

