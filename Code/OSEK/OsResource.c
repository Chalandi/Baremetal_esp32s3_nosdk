// *****************************************************************************************************************
// Filename    : OsResource.c
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
// Description : Event & resource mechanism implementation
// 
// *****************************************************************************************************************

//------------------------------------------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------------------------------------------
#include"OsTcb.h"
#include"OsAPIs.h"

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_GetResource
///
/// \descr  This call serves to enter critical sections in the code that are assigned to the resource referenced 
///         by <ResID>. A critical section shall always be left using ReleaseResource
///
/// \param  OsResourceType ResID: Reference to resource
///
/// \return OsStatusType
//------------------------------------------------------------------------------------------------------------------
OsStatusType OS_GetResource(OsResourceType ResID)
{
  if(ResID < OS_NUMBER_OF_RESOURCES)
  {
    const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());
    const osObjectCoreAsgn_t osLocalResourceAssignment = osGetLocalResourceAssignment(ResID);
    const OsResourceType LocalResID = osLocalResourceAssignment.local_id;
    
    if(((OCB_Cfg[osActiveCore]->pRes[LocalResID]->AuthorizedTask & (1ul << OCB_Cfg[osActiveCore]->OsCurrentTaskId)) != 0) &&
       OCB_Cfg[osActiveCore]->pRes[LocalResID]->Occupied == 0)
    {
      /* The resource is available, mark it now as occupied */
      OCB_Cfg[osActiveCore]->pRes[LocalResID]->Occupied = 1ul;

      /* reserve the resource to the current task */
      OCB_Cfg[osActiveCore]->pRes[LocalResID]->CurrentOccupiedTask = OCB_Cfg[osActiveCore]->OsCurrentTaskId;

      /* Set the ceilling prio of the resource to the current task */
      OCB_Cfg[osActiveCore]->pTcb[OCB_Cfg[osActiveCore]->OsCurrentTaskId]->CeilingPrio = OCB_Cfg[osActiveCore]->pRes[LocalResID]->ResCeilingPrio;
      OCB_Cfg[osActiveCore]->pTcb[OCB_Cfg[osActiveCore]->OsCurrentTaskId]->Prio = OCB_Cfg[osActiveCore]->pTcb[OCB_Cfg[osActiveCore]->OsCurrentTaskId]->CeilingPrio;

      return(E_OK);
    }
    else
    {
      /* Resource is occupied by another task */
      osInternalError(E_OS_ACCESS);
    }
        
  }
  else
  {
    osInternalError(E_OS_ID);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_ReleaseResource
///
/// \descr  ReleaseResource is the counterpart of GetResource and serves to leave critical sections in the code 
///         that are assigned to the resource referenced by <ResID>
///
/// \param  OsResourceType ResID: Reference to resource
///
/// \return OsStatusType
//------------------------------------------------------------------------------------------------------------------
OsStatusType OS_ReleaseResource(OsResourceType ResID)
{
  if(ResID < OS_NUMBER_OF_RESOURCES)
  {
    const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());
    const osObjectCoreAsgn_t osLocalResourceAssignment = osGetLocalResourceAssignment(ResID);
    const OsResourceType LocalResID = osLocalResourceAssignment.local_id;

    if((OCB_Cfg[osActiveCore]->pRes[LocalResID]->CurrentOccupiedTask == OCB_Cfg[osActiveCore]->OsCurrentTaskId) && (OCB_Cfg[osActiveCore]->pRes[LocalResID]->Occupied == 1ul))
    {
      /* Release the resource */
      OCB_Cfg[osActiveCore]->pRes[LocalResID]->Occupied = 0;
      OCB_Cfg[osActiveCore]->pRes[LocalResID]->CurrentOccupiedTask = OCB_Cfg[osActiveCore]->OsNumberOfTasks;
      
      /* Set the default prio to the current task */
      OCB_Cfg[osActiveCore]->pTcb[OCB_Cfg[osActiveCore]->OsCurrentTaskId]->CeilingPrio = 0;
      OCB_Cfg[osActiveCore]->pTcb[OCB_Cfg[osActiveCore]->OsCurrentTaskId]->Prio = OCB_Cfg[osActiveCore]->pTcb[OCB_Cfg[osActiveCore]->OsCurrentTaskId]->FixedPrio;

      /* Call the scheduler */
      (void)osSchedule();

      return(E_OK);
    }
    else
    {
      osInternalError(E_OS_NOFUNC);
    }
  }
  else
  {
    osInternalError(E_OS_ID);
  }  
}
