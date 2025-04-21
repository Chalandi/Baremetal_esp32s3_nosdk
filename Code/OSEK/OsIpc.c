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
IpcStatus OS_IpcSendData(OsIpcMbxCfgType const * const MsgBox, OsIpcMbxdataType const* Msgdata)
{

  const uint16 size = sizeof(Msgdata->id) + sizeof(Msgdata->size) + Msgdata->size;
  OsTaskStateType taskstate;

  /* check if the sender belongs to the same core of the task owning the mailbox 
     Tasks on the same core using only the resource
     Tasks on different cores will use spinlock after taking the Resource
  */

  /* get the owner state status */
  (void)OS_GetTaskState(MsgBox->owner, &taskstate);

  /* before sending the data check if the queue has enough space or not 
     and the target task not suspended */
  if(size <= osCircularFifoQueue_GetAvailableSize(MsgBox->pQueue) && (taskstate != SUSPENDED))
  {
    /* lock the mailbox */
    OS_GetResource(MsgBox->res);

    /* push data into the queue */
    (void)osCircularFifoQueue_Push(MsgBox->pQueue, (uint8*)&Msgdata->id, sizeof(Msgdata->id));
    (void)osCircularFifoQueue_Push(MsgBox->pQueue, (uint8*)&Msgdata->size, sizeof(Msgdata->size));
    (void)osCircularFifoQueue_Push(MsgBox->pQueue, (uint8*)Msgdata->pdata, Msgdata->size);

    /* unlock the mailbox */
    OS_ReleaseResource(MsgBox->res);

    /* set event */
    OS_SetEvent(MsgBox->owner,MsgBox->evt);

    return(IPC_OK);
  }
  else
  {
    return(IPC_NOK);
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
IpcStatus OS_IpcReceiveData(OsIpcMbxCfgType const * const  MsgBox, OsIpcMbxdataType const* Msgdata)
{
  /* check if the current task is the owner task
  */

  /* check if the queue is not empty */
  if(!osCircularFifoQueue_IsEmpty(MsgBox->pQueue))
  {
    /* lock the mailbox */
    OS_GetResource(MsgBox->res);

    /* push data into the queue */
    (void)osCircularFifoQueue_Pop(MsgBox->pQueue, (uint8*)&Msgdata->id, sizeof(Msgdata->id));
    (void)osCircularFifoQueue_Pop(MsgBox->pQueue, (uint8*)&Msgdata->size, sizeof(Msgdata->size));
    (void)osCircularFifoQueue_Pop(MsgBox->pQueue, (uint8*)Msgdata->pdata, Msgdata->size);

    /* unlock the mailbox */
    OS_ReleaseResource(MsgBox->res);

    return(IPC_OK);
  }
  else
  {
    return(IPC_NOK);
  }
}
