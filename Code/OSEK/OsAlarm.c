// *****************************************************************************************************************
// Filename    : OsAlarm.c
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
// Description : Alarm management implementation
// 
// *****************************************************************************************************************

//------------------------------------------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------------------------------------------
#include"OsTcb.h"
#include"OsAPIs.h"

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_GetAlarmBase
///
/// \descr  The system service GetAlarmBase reads the alarm base characteristics.
///
/// \param      OsAlarmType AlarmID (In) : Reference to alarm
///         OsAlarmBaseRefType Info (Out): Reference to structure with constants of the alarm base
///
/// \return OsStatusType
//------------------------------------------------------------------------------------------------------------------
OsStatusType OS_GetAlarmBase(OsAlarmType AlarmID, OsAlarmBaseRefType Info)
{
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

  if(AlarmID < OS_NUMBER_OF_ALARMS)
  {
    const osObjectCoreAsgn_t osLocalAlarmAssignment = osGetLocalAlarmAssignment(AlarmID);
    const OsAlarmType LocalAlarmId = osLocalAlarmAssignment.local_id;

    if(osActiveCore == osLocalAlarmAssignment.pinned_core)
    {
      Info = &OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmId];
      return(E_OK);
    }
    else
    {
      /* do we need multicore support here ? */
      return(E_OK);
    }
  }
  else
  {
    (void)Info;
    osInternalError(E_OS_ID);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_GetAlarm
///
/// \descr  The system service GetAlarm returns the relative value in ticks before the alarm <AlarmID> expires
///
/// \param   OsAlarmType AlarmID (In) : Reference to alarm
///          OsTickRefType Tick (Out) : Relative value in ticks before the alarm <AlarmID> expires
///
/// \return OsStatusType
//------------------------------------------------------------------------------------------------------------------
OsStatusType OS_GetAlarm(OsAlarmType AlarmID, OsTickRefType Tick)
{
  if(AlarmID < OS_NUMBER_OF_ALARMS)
  {
    const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());
    const osObjectCoreAsgn_t osLocalAlarmAssignment = osGetLocalAlarmAssignment(AlarmID);
    if(osActiveCore == osLocalAlarmAssignment.pinned_core)
    {
      *Tick = OCB_Cfg[osActiveCore]->pAlarm[osLocalAlarmAssignment.local_id]->AlarmCheckPoint - (uint32)OCB_Cfg[osActiveCore]->OsSysTickCounter;
      return(E_OK);
    }
    else
    {
      return(osCrossCore_GetAlarm(osActiveCore, osLocalAlarmAssignment.pinned_core, AlarmID, Tick));
    }
  }
  else
  {
    osInternalError(E_OS_ID);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_SetRelAlarm
///
/// \descr  The system service occupies the alarm <AlarmID> element. After <increment> ticks have elapsed, 
///         the task assigned to the alarm <AlarmID> is activated or the assigned event (only for extended tasks) 
///         is set or the alarm-callback routine is called.
///
/// \param   OsAlarmType AlarmID  : Reference to the alarm element
///          OsTickType increment : Relative value in ticks
///          OsTickType cycle     : Cycle value in case of cyclic alarm. In case of single alarms, it shall be zero.
///
/// \return OsStatusType
//------------------------------------------------------------------------------------------------------------------
OsStatusType OS_SetRelAlarm(OsAlarmType AlarmID, OsTickType increment, OsTickType cycle)
{
  if(AlarmID < OS_NUMBER_OF_ALARMS)
  {
    const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());
    const osObjectCoreAsgn_t osLocalAlarmAssignment = osGetLocalAlarmAssignment(AlarmID);
    const OsAlarmType LocalAlarmID = (OsAlarmType)(OsAlarmType)osLocalAlarmAssignment.local_id;

    if(osActiveCore != osLocalAlarmAssignment.pinned_core)
    {
      return(osCrossCore_SetRelAlarm(osActiveCore, osLocalAlarmAssignment.pinned_core, AlarmID, increment, cycle));
    }

    if(cycle == 0 && increment > 0 && OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->Status == ALARM_FREE)
    {
      /* One shot alarm */
      OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->Alarmtype       = ALARM_ONE_SHOT;
      OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->AlarmCategory   = ALARM_RELATIVE;
      OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->InitCycles      = 0;
      OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->InitTicks       = increment;
      OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->Status          = ALARM_USED;
      OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->AlarmCheckPoint = increment + (uint32)OCB_Cfg[osActiveCore]->OsSysTickCounter;
      return(E_OK);        
    }
    else if (cycle != 0 &&  cycle >= increment && OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->Status == ALARM_FREE)
    {
      /* Cyclic alarm */
      OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->Alarmtype       = ALARM_CYCLIC;
      OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->AlarmCategory   = ALARM_RELATIVE;
      OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->InitCycles      = cycle;
      OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->InitTicks       = increment;
      OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->Status          = ALARM_USED;
      OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->AlarmCheckPoint = increment + cycle + (uint32)OCB_Cfg[osActiveCore]->OsSysTickCounter;
      return(E_OK);        
    }
    else if(OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->Status == ALARM_USED)
    {
      osInternalError(E_OS_STATE);
    }
    else
    {
      osInternalError(E_OS_VALUE);
    }
  }
  else
  {
    osInternalError(E_OS_ID);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_SetAbsAlarm
///
/// \descr  The system service occupies the alarm <AlarmID> element. When <start> ticks are reached, 
///         the task assigned to the alarm <AlarmID> is activated or the assigned event (only for extended tasks) 
///         is set or the alarm-callback routine is called.
///
/// \param   OsAlarmType AlarmID  : Reference to the alarm element
///          OsTickType start     : Absolute value in ticks
///          OsTickType cycle     : Cycle value in case of cyclic alarm. In case of single alarms, it shall be zero.
///
/// \return OsStatusType
//------------------------------------------------------------------------------------------------------------------
OsStatusType OS_SetAbsAlarm(OsAlarmType AlarmID, OsTickType start, OsTickType cycle)
{
  if(AlarmID < OS_NUMBER_OF_ALARMS)
  {
    const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());
    const osObjectCoreAsgn_t osLocalAlarmAssignment = osGetLocalAlarmAssignment(AlarmID);
    const OsAlarmType LocalAlarmID = (OsAlarmType)osLocalAlarmAssignment.local_id;

    if(osActiveCore != osLocalAlarmAssignment.pinned_core)
    {
      return(osCrossCore_SetAbsAlarm(osActiveCore, osLocalAlarmAssignment.pinned_core, AlarmID, start, cycle));
    }

    if(cycle == 0 && start > (uint32)OCB_Cfg[osActiveCore]->OsSysTickCounter && OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->Status == ALARM_FREE)
    {
      /* One shot alarm */
      OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->Alarmtype       = ALARM_ONE_SHOT;
      OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->AlarmCategory   = ALARM_ABSOLUTE;
      OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->InitCycles      = 0;
      OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->InitTicks       = start;
      OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->Status          = ALARM_USED;
      OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->AlarmCheckPoint = start;
      return(E_OK);        
    }
    else if (cycle != 0 &&  start > (uint32)OCB_Cfg[osActiveCore]->OsSysTickCounter && OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->Status == ALARM_FREE)
    {
      /* Cyclic alarm */
      OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->Alarmtype       = ALARM_CYCLIC;
      OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->AlarmCategory   = ALARM_ABSOLUTE;
      OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->InitCycles      = cycle;
      OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->InitTicks       = start;
      OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->Status          = ALARM_USED;
      OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->AlarmCheckPoint = start;
      return(E_OK);        
    }
    else if(OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->Status == ALARM_USED)
    {
      osInternalError(E_OS_STATE);
    }
    else
    {
      osInternalError(E_OS_VALUE);
    }
  }
  else
  {
    osInternalError(E_OS_ID);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_CancelAlarm
///
/// \descr  The system service cancels the alarm <AlarmID>
///
/// \param   OsAlarmType AlarmID : Reference to the alarm element
///
/// \return OsStatusType
//------------------------------------------------------------------------------------------------------------------
OsStatusType OS_CancelAlarm(OsAlarmType AlarmID)
{
  if(AlarmID < OS_NUMBER_OF_ALARMS)
  {
    const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());
    const osObjectCoreAsgn_t osLocalAlarmAssignment = osGetLocalAlarmAssignment(AlarmID);
    const OsAlarmType LocalAlarmID = (OsAlarmType)osLocalAlarmAssignment.local_id;

    if(osActiveCore != osLocalAlarmAssignment.pinned_core)
    {
      return(osCrossCore_CancelAlarm(osActiveCore, osLocalAlarmAssignment.pinned_core, AlarmID));
    }

    OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->Status          = ALARM_FREE;
    OCB_Cfg[osActiveCore]->pAlarm[LocalAlarmID]->AlarmCheckPoint = 0;
    return(E_OK);    
  }
  else
  {
    osInternalError(E_OS_ID);
  }  
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  Alarms Management
///
/// \descr  This function is called by the ISR of system tick interrupt
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void osAlarmsManagement(void)
{
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

  for(uint32 AlarmID =0; AlarmID < OCB_Cfg[osActiveCore]->OsNumberOfAlarms; AlarmID++)
  {
    if((OCB_Cfg[osActiveCore]->pAlarm[AlarmID]->AlarmCheckPoint <= (uint32)OCB_Cfg[osActiveCore]->OsSysTickCounter) && (OCB_Cfg[osActiveCore]->pAlarm[AlarmID]->Status == ALARM_USED))
    {
      /* Update Timers */
      if(OCB_Cfg[osActiveCore]->pAlarm[AlarmID]->AlarmCategory == ALARM_RELATIVE &&  OCB_Cfg[osActiveCore]->pAlarm[AlarmID]->Alarmtype == ALARM_ONE_SHOT)
      {
        OCB_Cfg[osActiveCore]->pAlarm[AlarmID]->AlarmCheckPoint = 0;
      }
      else if(OCB_Cfg[osActiveCore]->pAlarm[AlarmID]->AlarmCategory == ALARM_RELATIVE &&  OCB_Cfg[osActiveCore]->pAlarm[AlarmID]->Alarmtype == ALARM_CYCLIC)
      {
        OCB_Cfg[osActiveCore]->pAlarm[AlarmID]->AlarmCheckPoint = OCB_Cfg[osActiveCore]->pAlarm[AlarmID]->InitTicks + OCB_Cfg[osActiveCore]->pAlarm[AlarmID]->InitCycles + (uint32)OCB_Cfg[osActiveCore]->OsSysTickCounter;
      }
      else if(OCB_Cfg[osActiveCore]->pAlarm[AlarmID]->AlarmCategory == ALARM_ABSOLUTE &&  OCB_Cfg[osActiveCore]->pAlarm[AlarmID]->Alarmtype == ALARM_ONE_SHOT)
      {
        OCB_Cfg[osActiveCore]->pAlarm[AlarmID]->AlarmCheckPoint = 0;
      }
      else if(OCB_Cfg[osActiveCore]->pAlarm[AlarmID]->AlarmCategory == ALARM_ABSOLUTE &&  OCB_Cfg[osActiveCore]->pAlarm[AlarmID]->Alarmtype == ALARM_CYCLIC)
      {
        OCB_Cfg[osActiveCore]->pAlarm[AlarmID]->AlarmCheckPoint = OCB_Cfg[osActiveCore]->pAlarm[AlarmID]->InitCycles + (uint32)OCB_Cfg[osActiveCore]->OsSysTickCounter;
      }
      else
      {
        /* Do Nothing */
      }
      
      /* Execute Action */
      if(OCB_Cfg[osActiveCore]->pAlarm[AlarmID]->Action == ALARM_SET_EVENT)
      {
        OS_SetEvent((OsTaskType)OCB_Cfg[osActiveCore]->pAlarm[AlarmID]->TaskId, (OsEventMaskType)OCB_Cfg[osActiveCore]->pAlarm[AlarmID]->Event);
      }
      else if(OCB_Cfg[osActiveCore]->pAlarm[AlarmID]->Action == ALARM_ACTIVE_TASK)
      {
        OS_ActivateTask((OsTaskType)OCB_Cfg[osActiveCore]->pAlarm[AlarmID]->TaskId);
      }
      else if(OCB_Cfg[osActiveCore]->pAlarm[AlarmID]->Action == ALARM_CALLBACK)
      {
        if(OCB_Cfg[osActiveCore]->pAlarm[AlarmID]->CallBackFunc != (void*)0)
        {
          OCB_Cfg[osActiveCore]->pAlarm[AlarmID]->CallBackFunc();
        }
        else
        {
          /* NULL function pointer -> HookError could be called */
        }
      }      
      else
      {
        /* Do nothing */
      }
    }
  }

}
