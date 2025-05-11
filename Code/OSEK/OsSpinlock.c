// *****************************************************************************************************************
// Filename    : OsSpinlock.c
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
// Description : Spinlock implementation
// 
// *****************************************************************************************************************

//------------------------------------------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------------------------------------------
#include"OsTcb.h"
#include"OsAPIs.h"

//------------------------------------------------------------------------------------------------------------------
/// \brief  
///
/// \descr  
///
/// \param  
///
/// \return 
//------------------------------------------------------------------------------------------------------------------
OsStatusType OS_GetSpinlock(OsSpinlockIdType SpinlockId)
{
  if(SpinlockId >= OS_NUMBER_OF_SPINLOCKS)
  {
    return(E_OS_ID);
  }
  else
  {
    /* get active core id */
    const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

    /* check if this core has acquired already a spinlock */
    if(OCB_Cfg[osActiveCore]->OsSpinlockNestingDepth > 0)
    {
      /* only one spinlock at a time could be aquired */
      return(E_OS_NESTING_DEADLOCK);
    }

    /* acquire the hw spinlock */
    osHwAcquireSpinLock(&OCB_Cfg[osActiveCore]->pSpinlock[SpinlockId]->Lock);

    /* disable the cat2 interrupts to avoid deadlock within the current core */
    OS_SuspendOSInterrupts();

    /* set the occupied task */
    OCB_Cfg[osActiveCore]->pSpinlock[SpinlockId]->CurrentOccupiedTask = OCB_Cfg[osActiveCore]->OsCurrentTaskId;

    /* increment spinlock nesting depth variable */
    OCB_Cfg[osActiveCore]->OsSpinlockNestingDepth++;
    
    return(E_OK);
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
OsStatusType OS_ReleaseSpinlock(OsSpinlockIdType SpinlockId)
{
  if(SpinlockId >= OS_NUMBER_OF_SPINLOCKS)
  {
    return(E_OS_ID);
  }
  else
  {
    /* get active core id */
    const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

    /* check if SpinlockId is already acquired by the task trying to release it */
    if(OCB_Cfg[osActiveCore]->pSpinlock[SpinlockId]->CurrentOccupiedTask != OCB_Cfg[osActiveCore]->OsCurrentTaskId)
    {
      return(E_OS_STATE);
    }

    /* decrement spinlock nesting depth */
    OCB_Cfg[osActiveCore]->OsSpinlockNestingDepth--;

    /* release the hw spinlock */
    osHwReleaseSpinLock(&OCB_Cfg[osActiveCore]->pSpinlock[SpinlockId]->Lock);

    /* enable the cat2 interrupts previously disabled to avoid deadlock within the current core */
    OS_ResumeOSInterrupts();
    
    return(E_OK);
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
OsStatusType OS_TryToGetSpinlock(OsSpinlockIdType SpinlockId, OsTryToGetSpinlockType* Success)
{
  if(SpinlockId >= OS_NUMBER_OF_SPINLOCKS)
  {
    return(E_OS_ID);
  }
  else
  {
    /* get active core id */
    const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

    /* check if this core has acquired already a spinlock */
    if(OCB_Cfg[osActiveCore]->OsSpinlockNestingDepth > 0)
    {
      /* only one spinlock at a time could be aquired */
      return(E_OS_NESTING_DEADLOCK);
    }

    /* try to acquire the hw spinlock */
    *Success = (OsTryToGetSpinlockType)osHwTryToAcquireSpinLock(&OCB_Cfg[osActiveCore]->pSpinlock[SpinlockId]->Lock);

    if(TRYTOGETSPINLOCK_SUCCESS == *Success)
    {
      /* disable the cat2 interrupts to avoid deadlock within the current core */
      OS_SuspendOSInterrupts();

      /* set the occupied task */
      OCB_Cfg[osActiveCore]->pSpinlock[SpinlockId]->CurrentOccupiedTask = OCB_Cfg[osActiveCore]->OsCurrentTaskId;

      /* increment spinlock nesting depth variable */
      OCB_Cfg[osActiveCore]->OsSpinlockNestingDepth++;
    }

    return(E_OK);
  }
}
