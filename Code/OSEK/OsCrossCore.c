// *****************************************************************************************************************
// Filename    : OsCrossCore.c
// 
// OS          : OSEK 2.2.3
// 
// CC          : ECC1/BCC1
//
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 14.10.2024
// 
// Description : cross core service implementation
// 
// *****************************************************************************************************************

//------------------------------------------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------------------------------------------
#include"OsAPIs.h"
#include"OsTcb.h"
#include"OsHwPltfm.h"

//------------------------------------------------------------------------------------------------------------------
// defines
//------------------------------------------------------------------------------------------------------------------
#define CROSS_CORE_REQ_RES_HEADER_SIZE  3ul
#define IDX_0_CORE                      0ul
#define IDX_1_SERVICE                   1ul
#define IDX_2_REQ_SIZE                  2ul
#define IDX_2_RSP_SIZE                  2ul
#define IDX_3_REQ_PARAM                 3ul
#define IDX_3_RSP_STATUS                3ul

//------------------------------------------------------------------------------------------------------------------
// Types
//------------------------------------------------------------------------------------------------------------------
typedef enum
{
  OS_CROSS_CORE_SERVICE_SET_EVENT = 0,
  OS_CROSS_CORE_SERVICE_GET_EVENT,
  OS_CROSS_CORE_SERVICE_ACTIVATE_TASK,
  OS_CROSS_CORE_SERVICE_GET_TASK_STATE,
  OS_CROSS_CORE_SERVICE_GET_ALARM,
  OS_CROSS_CORE_SERVICE_SET_REL_ALARM,
  OS_CROSS_CORE_SERVICE_SET_ABS_ALARM,
  OS_CROSS_CORE_SERVICE_CANCEL_ALARM,
  OS_CROSS_CORE_TOTAL_NUMBER_OF_SUPPORTED_SERVICES
}OsCrossCoreServiceType;

typedef void (*osCrossCoreExecuteFuncType)(uint32_t* pRequest);

//------------------------------------------------------------------------------------------------------------------
// prototypes
//------------------------------------------------------------------------------------------------------------------
static _Bool osCrossCoreSendRequest(OsCoreId ActiveCore, OsCoreId TargetCore, uint32_t* pRequest, uint32_t* pResponse);
static void osCrossCore_ExecuteSetEvent(uint32_t* pRequest);
static void osCrossCore_ExecuteGetEvent(uint32_t* pRequest);
static void osCrossCore_ExecuteActivateTask(uint32_t* pRequest);
static void osCrossCore_ExecuteGetTaskState(uint32_t* pRequest);
static void osCrossCore_ExecuteGetAlarm(uint32_t* pRequest);
static void osCrossCore_ExecuteSetRelAlarm(uint32_t* pRequest);
static void osCrossCore_ExecuteSetAbsAlarm(uint32_t* pRequest);
static void osCrossCore_ExecuteCancelAlarm(uint32_t* pRequest);

//------------------------------------------------------------------------------------------------------------------
// Globals
//------------------------------------------------------------------------------------------------------------------
static const osCrossCoreExecuteFuncType osCrossCoreLookupTable[OS_CROSS_CORE_TOTAL_NUMBER_OF_SUPPORTED_SERVICES] = 
{
  &osCrossCore_ExecuteSetEvent,
  &osCrossCore_ExecuteGetEvent,
  &osCrossCore_ExecuteActivateTask,
  &osCrossCore_ExecuteGetTaskState,
  &osCrossCore_ExecuteGetAlarm,
  &osCrossCore_ExecuteSetRelAlarm,
  &osCrossCore_ExecuteSetAbsAlarm,
  &osCrossCore_ExecuteCancelAlarm
};

//------------------------------------------------------------------------------------------------------------------
/// \brief
///
/// \descr
///
/// \param
///
/// \return
//------------------------------------------------------------------------------------------------------------------
static _Bool osCrossCoreSendRequest(OsCoreId ActiveCore, OsCoreId TargetCore, uint32_t* pRequest, uint32_t* pResponse)
{
  /* 1- Disable os interrupt 
     2- Send Request (fill the HW FIFO and avoid anything that may trigger interrupt for error)
         when send the first request it must
     3- pull Response (poll the HW FIFO)
     4- Remove Pending interrupt on the sending core
     5- restore os interrupts (if running from task level)
     6- return the received response

     pRequest[0] ----> logic Id of the requesting Core
     pRequest[1] ----> Id of the requested os service
     pRequest[2] ----> number of parameters
     pRequest[3] ----> parameter 1 ...

     #define IDX_0_CORE            0
     #define IDX_1_SERVICE         1
     #define IDX_2_REQ_SIZE        2
     #define IDX_2_RSP_SIZE        2
     #define IDX_3_REQ_PARAM       3
     #define IDX_3_RSP_STATUS      3

  */
  #define TIMEOUT_MS      5
  uint32_t timeout      = 0;
  uint64_t initial_tick = 0;

  /* acquire the local spinlock */
  osHwAcquireSpinLock((uint32_t*)&OCB_Cfg[TargetCore]->OsCrossCoreBufferLock);

  /* disable OS interrupt to avoid deadlock on current core */
  OS_SuspendOSInterrupts();

  /* sending the request */
  for(uint32_t cpt=0; cpt < (pRequest[IDX_2_REQ_SIZE] + CROSS_CORE_REQ_RES_HEADER_SIZE); cpt++)
  {
    OCB_Cfg[TargetCore]->OsCrossCoreBuffer[cpt] = pRequest[cpt];
  }

  /* generate the cross core interrupt on the target core */
  osGenerateCrossCoreInterrupt(ActiveCore, TargetCore);

  /* release the spin lock */
  osHwReleaseSpinLock((uint32_t*)&OCB_Cfg[TargetCore]->OsCrossCoreBufferLock);

  /* enable OS interrupt */
  OS_ResumeOSInterrupts();

  /* get current systick counter value*/
  initial_tick = osGetSystemTicksCounter();
  
  /* wait for the sync flag to be set by the target core or a timeout */
  while((OCB_Cfg[TargetCore]->OsCrossCoreBufferSyncFlag == 0) && (timeout == 0))
  {
    if(TIMEOUT_MS < osGetSystemTicksElapsedTime(initial_tick))
        timeout = 1;
  }

  if(OCB_Cfg[TargetCore]->OsCrossCoreBufferSyncFlag)
  {
    /* acquire the local spinlock */
    osHwAcquireSpinLock((uint32_t*)&OCB_Cfg[TargetCore]->OsCrossCoreBufferLock);

    /* disable OS interrupt to avoid deadlock on current core */
    OS_SuspendOSInterrupts();

    /* receive the response */
    for(uint32_t cpt=0; cpt <= (pRequest[IDX_2_RSP_SIZE] + CROSS_CORE_REQ_RES_HEADER_SIZE); cpt++)
    {
      pResponse[cpt] = OCB_Cfg[TargetCore]->OsCrossCoreBuffer[cpt];
    }

    /* clear the sync flag */
    OCB_Cfg[TargetCore]->OsCrossCoreBufferSyncFlag = 0;

    /* release the spin lock */
    osHwReleaseSpinLock((uint32_t*)&OCB_Cfg[TargetCore]->OsCrossCoreBufferLock);

    /* enable OS interrupt */
    OS_ResumeOSInterrupts();

    return(1);
  }
  else
  {
    return(0);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief
///
/// \descr
///
/// \param
///
/// \return
//------------------------------------------------------------------------------------------------------------------
ISR(osCrossCoreReceiveRequest)
{
  /* this interrupt is triggered when the handled core receives requests from other cores
     it must execute the request inside the ISR and send the response back

     pRequest[0] ----> logic Id of the requesting Core
     pRequest[1] ----> Id of the requested os service
     pRequest[2] ----> number of os service in-param
     pRequest[3] ----> 1st parameter value 
     ...

     pResponse[0] ----> logic Id of the responding Core
     pResponse[1] ----> Id of the requested os service + 0x40
     pResponse[2] ----> number of os service out-param
     pResponse[3] ----> 1st response value (os service status)
     ...
  */
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

  /* clear the cross-core interrupt flag */
  osClearCrossCoreInterrupt();

  /* disable interrupt nesting here for better real-time behavior on the requesting core */
  while(0);

  const uint32 RequestedService = OCB_Cfg[osActiveCore]->OsCrossCoreBuffer[IDX_1_SERVICE];

  /* call the appropriate OS service */
  (void)(osCrossCoreLookupTable[RequestedService])((uint32_t*)OCB_Cfg[osActiveCore]->OsCrossCoreBuffer);

  /* notify the requesting core */
  OCB_Cfg[osActiveCore]->OsCrossCoreBufferSyncFlag = 1;

  /* enable nesting */
  while(0);

}

//------------------------------------------------------------------------------------------------------------------
/// \brief
///
/// \descr
///
/// \param
///
/// \return
//------------------------------------------------------------------------------------------------------------------
OsStatusType osCrossCore_SetEvent(OsCoreId ActiveCore, OsCoreId TargetCore, OsTaskType TaskID, OsEventMaskType Mask)
{
  uint32_t Request[5]  = {ActiveCore, OS_CROSS_CORE_SERVICE_SET_EVENT, 2u, TaskID, Mask};
  uint32_t Response[5] = {0};

  /* dispatch the request to the appropriate core */
  if(osCrossCoreSendRequest(ActiveCore, TargetCore, Request, Response))
  {
    return(Response[IDX_3_RSP_STATUS]);
  }
  else
  {
    return(E_OS_MULTICORE_COM_LOST);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief
///
/// \descr
///
/// \param
///
/// \return
//------------------------------------------------------------------------------------------------------------------
static void osCrossCore_ExecuteSetEvent(uint32_t* pRequest)
{
  const OsTaskType TaskID    = (const OsTaskType)pRequest[IDX_3_REQ_PARAM];
  const OsEventMaskType Mask = (const OsEventMaskType)pRequest[IDX_3_REQ_PARAM + 1];

  /* execute the os service */
  OsStatusType status = OS_SetEvent(TaskID, Mask);

  /* update the response */
  pRequest[IDX_0_CORE]       = osGetLogicalCoreId(osGetCoreId());
  pRequest[IDX_1_SERVICE]   += 0x40;
  pRequest[IDX_2_RSP_SIZE]   = 1;
  pRequest[IDX_3_RSP_STATUS] = (uint32_t)status;
}

//------------------------------------------------------------------------------------------------------------------
/// \brief
///
/// \descr
///
/// \param
///
/// \return
//------------------------------------------------------------------------------------------------------------------
OsStatusType osCrossCore_GetEvent(OsCoreId ActiveCore, OsCoreId TargetCore, OsTaskType TaskID, OsEventMaskRefType Event)
{
  uint32_t Request[5] = {ActiveCore, OS_CROSS_CORE_SERVICE_GET_EVENT, 1u, TaskID, 0};
  uint32_t Response[5] = {0};

  /* dispatch the request to the appropriate core */
  if(osCrossCoreSendRequest(ActiveCore, TargetCore, Request, Response))
  {
    *Event = Response[IDX_3_RSP_STATUS + 1];
    return(Response[IDX_3_RSP_STATUS]);
  }
  else
  {
    return(E_OS_MULTICORE_COM_LOST);
  }

}

//------------------------------------------------------------------------------------------------------------------
/// \brief
///
/// \descr
///
/// \param
///
/// \return
//------------------------------------------------------------------------------------------------------------------
static void osCrossCore_ExecuteGetEvent(uint32_t* pRequest)
{
  const OsTaskType      TaskID = (const OsTaskType)pRequest[IDX_3_REQ_PARAM];
  const OsEventMaskType Event  = 0;

  /* execute the os service */
  OsStatusType status = OS_GetEvent(TaskID, (OsEventMaskRefType)&Event);

  /* update the response */
  pRequest[IDX_0_CORE]           = osGetLogicalCoreId(osGetCoreId());
  pRequest[IDX_1_SERVICE]       += 0x40;
  pRequest[IDX_2_RSP_SIZE]       = 2;
  pRequest[IDX_3_RSP_STATUS]     = (uint32_t)status;
  pRequest[IDX_3_RSP_STATUS + 1] = (uint32_t)Event;
}

//------------------------------------------------------------------------------------------------------------------
/// \brief
///
/// \descr
///
/// \param
///
/// \return
//------------------------------------------------------------------------------------------------------------------
OsStatusType osCrossCore_ActivateTask(OsCoreId ActiveCore, OsCoreId TargetCore, OsTaskType TaskID)
{
  uint32_t Request[4] = {ActiveCore, OS_CROSS_CORE_SERVICE_ACTIVATE_TASK, 1u, TaskID};
  uint32_t Response[4] = {0};

  /* dispatch the request to the appropriate core */
  if(osCrossCoreSendRequest(ActiveCore, TargetCore, Request, Response))
  {
    return(Response[IDX_3_RSP_STATUS]);
  }
  else
  {
    return(E_OS_MULTICORE_COM_LOST);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief
///
/// \descr
///
/// \param
///
/// \return
//------------------------------------------------------------------------------------------------------------------
static void osCrossCore_ExecuteActivateTask(uint32_t* pRequest)
{
  const OsTaskType TaskID = (const OsTaskType)pRequest[IDX_3_REQ_PARAM];
  
  OsStatusType status = OS_ActivateTask(TaskID);

  /* update the response */
  pRequest[IDX_0_CORE]        = osGetLogicalCoreId(osGetCoreId());
  pRequest[IDX_1_SERVICE]    += 0x40;
  pRequest[IDX_2_RSP_SIZE]    = 1;
  pRequest[IDX_3_RSP_STATUS]  = (uint32_t)status;
}

//------------------------------------------------------------------------------------------------------------------
/// \brief
///
/// \descr
///
/// \param
///
/// \return
//------------------------------------------------------------------------------------------------------------------
OsStatusType osCrossCore_GetTaskState(OsCoreId ActiveCore, OsCoreId TargetCore, OsTaskType TaskID, OsTaskStateRefType State)
{
  uint32_t Request[5] = {ActiveCore, OS_CROSS_CORE_SERVICE_GET_TASK_STATE, 1u, TaskID, 0};
  uint32_t Response[5] = {0};

  /* dispatch the request to the appropriate core */
  if(osCrossCoreSendRequest(ActiveCore, TargetCore, Request, Response))
  {
    *State = Response[IDX_3_RSP_STATUS + 1];
    return(Response[IDX_3_RSP_STATUS]);
  }
  else
  {
    return(E_OS_MULTICORE_COM_LOST);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief
///
/// \descr
///
/// \param
///
/// \return
//------------------------------------------------------------------------------------------------------------------
static void osCrossCore_ExecuteGetTaskState(uint32_t* pRequest)
{
  const OsTaskType TaskID = (const OsTaskType)pRequest[IDX_3_REQ_PARAM];
  OsTaskStateType State  = 0;
  
  OsStatusType status = OS_GetTaskState(TaskID, (OsTaskStateRefType)&State);

  /* update the response */
  pRequest[IDX_0_CORE]        = osGetLogicalCoreId(osGetCoreId());
  pRequest[IDX_1_SERVICE]    += 0x40;
  pRequest[IDX_2_RSP_SIZE]    = 2;
  pRequest[IDX_3_RSP_STATUS]  = (uint32_t)status;
  pRequest[IDX_3_RSP_STATUS + 1]  = (uint32_t)State;

}

//------------------------------------------------------------------------------------------------------------------
/// \brief
///
/// \descr
///
/// \param
///
/// \return
//------------------------------------------------------------------------------------------------------------------
OsStatusType osCrossCore_GetAlarm(OsCoreId ActiveCore, OsCoreId TargetCore, OsAlarmType AlarmID, OsTickRefType Tick)
{
  uint32_t Request[5] = {ActiveCore, OS_CROSS_CORE_SERVICE_GET_ALARM, 1u, (uint32_t)AlarmID, 0};
  uint32_t Response[5] = {0};

  /* dispatch the request to the appropriate core */
  if(osCrossCoreSendRequest(ActiveCore, TargetCore, Request, Response))
  {
    *Tick = Response[IDX_3_RSP_STATUS + 1];
    return(Response[IDX_3_RSP_STATUS]);
  }
  else
  {
    return(E_OS_MULTICORE_COM_LOST);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief
///
/// \descr
///
/// \param
///
/// \return
//------------------------------------------------------------------------------------------------------------------
static void osCrossCore_ExecuteGetAlarm(uint32_t* pRequest)
{
  const OsAlarmType AlarmID = (const OsAlarmType)pRequest[IDX_3_REQ_PARAM];
  const OsTickType  Tick    = 0;

  OsStatusType status = OS_GetAlarm(AlarmID, (OsTickRefType)&Tick);

  /* update the response */
  pRequest[IDX_0_CORE]        = osGetLogicalCoreId(osGetCoreId());
  pRequest[IDX_1_SERVICE]    += 0x40;
  pRequest[IDX_2_RSP_SIZE]    = 2;
  pRequest[IDX_3_RSP_STATUS]  = (uint32_t)status;
  pRequest[IDX_3_RSP_STATUS + 1]  = (uint32_t)Tick;
}

//------------------------------------------------------------------------------------------------------------------
/// \brief
///
/// \descr
///
/// \param
///
/// \return
//------------------------------------------------------------------------------------------------------------------
OsStatusType osCrossCore_SetRelAlarm(OsCoreId ActiveCore, OsCoreId TargetCore, OsAlarmType AlarmID, OsTickType increment, OsTickType cycle)
{
  uint32_t Request[6] = {ActiveCore, OS_CROSS_CORE_SERVICE_SET_REL_ALARM, 3u, (uint32_t)AlarmID, (uint32_t)increment, (uint32_t)cycle};
  uint32_t Response[6] = {0};

  /* dispatch the request to the appropriate core */
  if(osCrossCoreSendRequest(ActiveCore, TargetCore, Request, Response))
  {
    return(Response[IDX_3_RSP_STATUS]);
  }
  else
  {
    return(E_OS_MULTICORE_COM_LOST);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief
///
/// \descr
///
/// \param
///
/// \return
//------------------------------------------------------------------------------------------------------------------
static void osCrossCore_ExecuteSetRelAlarm(uint32_t* pRequest)
{
  const OsAlarmType AlarmID   = (const OsAlarmType)pRequest[IDX_3_REQ_PARAM];
  const OsTickType  increment = (const OsTickType)pRequest[IDX_3_REQ_PARAM + 1];
  const OsTickType  cycle     = (const OsTickType)pRequest[IDX_3_REQ_PARAM + 2];

  OsStatusType status = OS_SetRelAlarm(AlarmID, increment, cycle);

  /* update the response */
  pRequest[IDX_0_CORE]        = osGetLogicalCoreId(osGetCoreId());
  pRequest[IDX_1_SERVICE]    += 0x40;
  pRequest[IDX_2_RSP_SIZE]    = 1;
  pRequest[IDX_3_RSP_STATUS]  = (uint32_t)status;
}

//------------------------------------------------------------------------------------------------------------------
/// \brief
///
/// \descr
///
/// \param
///
/// \return
//------------------------------------------------------------------------------------------------------------------
OsStatusType osCrossCore_SetAbsAlarm(OsCoreId ActiveCore, OsCoreId TargetCore, OsAlarmType AlarmID, OsTickType start, OsTickType cycle)
{
  uint32_t Request[6] = {ActiveCore, OS_CROSS_CORE_SERVICE_SET_ABS_ALARM, 3u, (uint32_t)AlarmID, (uint32_t)start, (uint32_t)cycle};
  uint32_t Response[6] = {0};

  /* dispatch the request to the appropriate core */
  if(osCrossCoreSendRequest(ActiveCore, TargetCore, Request, Response))
  {
    return(Response[IDX_3_RSP_STATUS]);
  }
  else
  {
    return(E_OS_MULTICORE_COM_LOST);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief
///
/// \descr
///
/// \param
///
/// \return
//------------------------------------------------------------------------------------------------------------------
static void osCrossCore_ExecuteSetAbsAlarm(uint32_t* pRequest)
{
  const OsAlarmType AlarmID = (const OsAlarmType)pRequest[IDX_3_REQ_PARAM];
  const OsTickType  start   = (const OsTickType)pRequest[IDX_3_REQ_PARAM + 1];
  const OsTickType  cycle   = (const OsTickType)pRequest[IDX_3_REQ_PARAM + 2];

  OsStatusType status = OS_SetAbsAlarm(AlarmID, start, cycle);

  /* update the response */
  pRequest[IDX_0_CORE]        = osGetLogicalCoreId(osGetCoreId());
  pRequest[IDX_1_SERVICE]    += 0x40;
  pRequest[IDX_2_RSP_SIZE]    = 1;
  pRequest[IDX_3_RSP_STATUS]  = (uint32_t)status;
}

//------------------------------------------------------------------------------------------------------------------
/// \brief
///
/// \descr
///
/// \param
///
/// \return
//------------------------------------------------------------------------------------------------------------------
OsStatusType osCrossCore_CancelAlarm(OsCoreId ActiveCore, OsCoreId TargetCore, OsAlarmType AlarmID)
{
  uint32_t Request[4] = {ActiveCore, OS_CROSS_CORE_SERVICE_CANCEL_ALARM, 1u, (uint32_t)AlarmID};
  uint32_t Response[4] = {0};

  /* dispatch the request to the appropriate core */
  if(osCrossCoreSendRequest(ActiveCore, TargetCore, Request, Response))
  {
    return(Response[IDX_3_RSP_STATUS]);
  }
  else
  {
    return(E_OS_MULTICORE_COM_LOST);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief
///
/// \descr
///
/// \param
///
/// \return
//------------------------------------------------------------------------------------------------------------------
static void osCrossCore_ExecuteCancelAlarm(uint32_t* pRequest)
{
  const OsAlarmType AlarmID = (const OsAlarmType)pRequest[IDX_3_REQ_PARAM];

  OsStatusType status = OS_CancelAlarm(AlarmID);

  /* update the response */
  pRequest[IDX_0_CORE]        = osGetLogicalCoreId(osGetCoreId());
  pRequest[IDX_1_SERVICE]    += 0x40;
  pRequest[IDX_2_RSP_SIZE]    = 1;
  pRequest[IDX_3_RSP_STATUS]  = (uint32_t)status;
}
