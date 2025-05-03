// *****************************************************************************************************************
// Filename    : OsIpc.c
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
// Description : Inter-process communication implementation
// 
// *****************************************************************************************************************

//------------------------------------------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------------------------------------------
#include "osTcb.h"
#include "OsAPIs.h"

//------------------------------------------------------------------------------------------------------------------
/// \brief  
///
/// \descr  
///
/// \param  
///
/// \return 
//------------------------------------------------------------------------------------------------------------------
IpcStatus OS_IpcSendData(OsIpcMbxCfgType* const MsgBox, OsIpcMbxdataType const* Msgdata)
{

  const uint16 size = sizeof(Msgdata->id) + sizeof(Msgdata->size) + Msgdata->size;
  OsTaskStateType taskstate;

  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

  if((IPC_MBX_MODE_LOCAL == MsgBox->mode) && (osActiveCore != osTaskCoreAsgnLookupTable[MsgBox->owner].pinned_core))
  {
    /* the active core is not allowed to use the mailbox */
    return(IPC_CROSS_CORE_NOT_ALLOWED);
  }

  /* get the owner state status */
  (void)OS_GetTaskState(MsgBox->owner, &taskstate);

  /* check if the target task is suspended or not */
  if(taskstate == SUSPENDED)
  {
    return(IPC_OWNER_TASK_SUSPENDED);
  }

  /* check if the queue has enough space */
  if(size <= osCircularFifoQueue_GetAvailableSize(MsgBox->pQueue))
  {
    if(IPC_MBX_MODE_LOCAL == MsgBox->mode)
    {
      if(E_OK != OS_GetResource(MsgBox->res))
      {
        return(IPC_GET_RESOURCE_ERROR);
      }
    }
    else
    {
      osHwAcquireSpinLock(&MsgBox->lock);
      OS_SuspendOSInterrupts();
    }

    /* push data into the queue */
    (void)osCircularFifoQueue_Push(MsgBox->pQueue, (uint8*)&Msgdata->id, sizeof(Msgdata->id));
    (void)osCircularFifoQueue_Push(MsgBox->pQueue, (uint8*)&Msgdata->size, sizeof(Msgdata->size));
    (void)osCircularFifoQueue_Push(MsgBox->pQueue, (uint8*)Msgdata->pdata, Msgdata->size);

    if(IPC_MBX_MODE_LOCAL == MsgBox->mode)
    {
      if(E_OK != OS_ReleaseResource(MsgBox->res))
      {
        return(IPC_RELEASE_RESOURCE_ERROR);
      }
    }
    else
    {
      osHwReleaseSpinLock(&MsgBox->lock);
      OS_ResumeOSInterrupts();
    }

    /* set event */
    OS_SetEvent(MsgBox->owner,MsgBox->evt);

    return(IPC_OK);
  }
  else
  {
    return(IPC_QUEUE_IS_FULL);
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
IpcStatus OS_IpcReceiveData(OsIpcMbxCfgType* const  MsgBox, OsIpcMbxdataType const* Msgdata)
{
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

  /* check if the current task is the owner task */
  if((OCB_Cfg[osActiveCore]->OsCurrentTaskId != osTaskCoreAsgnLookupTable[MsgBox->owner].local_id)
  || (osActiveCore != osTaskCoreAsgnLookupTable[MsgBox->owner].pinned_core))
  {
    /* the calling task is not allowed to use the mailbox */
    return(IPC_WRONG_OWNER_TASK);
  }

  /* check if the queue is not empty */
  if(!osCircularFifoQueue_IsEmpty(MsgBox->pQueue))
  {
    if(IPC_MBX_MODE_LOCAL == MsgBox->mode)
    {
      if(E_OK != OS_GetResource(MsgBox->res))
      {
        return(IPC_GET_RESOURCE_ERROR);
      }
    }
    else
    {
      osHwAcquireSpinLock(&MsgBox->lock);
      OS_SuspendOSInterrupts();
    }

    /* push data into the queue */
    (void)osCircularFifoQueue_Pop(MsgBox->pQueue, (uint8*)&Msgdata->id, sizeof(Msgdata->id));
    (void)osCircularFifoQueue_Pop(MsgBox->pQueue, (uint8*)&Msgdata->size, sizeof(Msgdata->size));
    (void)osCircularFifoQueue_Pop(MsgBox->pQueue, (uint8*)Msgdata->pdata, Msgdata->size);

    if(IPC_MBX_MODE_LOCAL == MsgBox->mode)
    {
      if(E_OK != OS_ReleaseResource(MsgBox->res))
      {
        return(IPC_RELEASE_RESOURCE_ERROR);
      }
    }
    else
    {
      osHwReleaseSpinLock(&MsgBox->lock);
      OS_ResumeOSInterrupts();
    }

    return(IPC_OK);
  }
  else
  {
    return(IPC_QUEUE_IS_EMPTY);
  }
}
