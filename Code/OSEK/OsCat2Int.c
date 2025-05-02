// *****************************************************************************************************************
// Filename    : OsCat2Int.c
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
// Description : OS interrupt category 2 management implementation
// 
// *****************************************************************************************************************

//------------------------------------------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------------------------------------------
#include"OsAPIs.h"
#include"OsInternal.h"
#include"OsTcb.h"
#include"OsAsm.h"
#include"OsHwPltfm.h"

//------------------------------------------------------------------------------------------------------------------
/// \brief  osRunCat2Isr
///
/// \descr  This function is the entry point of all category 2 interrupts
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
 void osRunCat2Isr(void)
{
  const uint32 IntId        = osGetActiveInterruptVectorId();
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

  if(OCB_Cfg[osActiveCore]->pInt->OsIsrLookupTablePtr[IntId].NestingType == NOT_NESTED)
  {
    /* disable the nesting of the current interrupt priority level but keep it enabled for category 1 interrupts */
    osSetInterruptPriorityMask(OCB_Cfg[osActiveCore]->pInt->OsIntCat1LowestPrioLevel);
  }

  /* enable interrupt nesting before calling the ISR */
 osEnableIntNesting();

  /* call the appropriate interrupt service routine */
  OCB_Cfg[osActiveCore]->pInt->OsIsrLookupTablePtr[IntId].IsrFunc();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  
///
/// \descr  
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void osIncNestingDepthLevel(void)
{
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

  /* must be called by the OS with interrupts disabled, no need to lock the internal resource */
  if(0 == osIsInterruptDisabled())
  {
    osKernelError(E_OS_ENABLEDINT);
  }
  
  /* increment the interrupt nesting depth counter */
  OCB_Cfg[osActiveCore]->pInt->OsInterruptNestingDepth++;

  /* check if we exceeded the maximum nesting depth */
  if(OCB_Cfg[osActiveCore]->pInt->OsInterruptNestingDepth >= OS_INTERRUPT_NESTING_DEPTH_LEVEL)
  {
    osKernelError(E_OS_NESTING_DEPTH_EXCEEDED);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  
///
/// \descr  
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void osDecNestingDepthLevel(void)
{
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

  /* must be called by the OS with interrupts disabled, no need to lock the internal resource */
  if(0 == osIsInterruptDisabled())
  {
    osKernelError(E_OS_ENABLEDINT);
  }
  
  /* decrement the interrupt nesting depth counter */
  OCB_Cfg[osActiveCore]->pInt->OsInterruptNestingDepth--;

}

//------------------------------------------------------------------------------------------------------------------
/// \brief  
///
/// \descr  
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
uint32 osGetIntNestingLevel(void)
{
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());
  uint32 osInterruptNestingDepth = 0;

  OS_SuspendOSInterrupts();
  osInterruptNestingDepth = OCB_Cfg[osActiveCore]->pInt->OsInterruptNestingDepth;
  OS_ResumeOSInterrupts();

  return osInterruptNestingDepth;
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  
///
/// \descr  
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void osDisableIntNesting(void)
{
  DISABLE_INTERRUPTS();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  
///
/// \descr  
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void osEnableIntNesting(void)
{
  ENABLE_INTERRUPTS();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  osIsCat2IntContext
///
/// \descr  This function checks if the cpu is in category 2 interrupt context or not.
///
/// \param  void
///
/// \return boolean: TRUE -> Category 2 interrupt context, FALSE -> none category 2 interrupt context
//------------------------------------------------------------------------------------------------------------------
boolean osIsCat2IntContext(void)
{
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());
  return((boolean)OCB_Cfg[osActiveCore]->OsCat2InterruptLevel);
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_DisableAllInterrupts
///
/// \descr  Disable all interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_DisableAllInterrupts(void)
{
  DISABLE_INTERRUPTS();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_EnableAllInterrupts
///
/// \descr  enable all interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_EnableAllInterrupts(void)
{
  ENABLE_INTERRUPTS();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_SuspendAllInterrupts
///
/// \descr  Suspend all interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_SuspendAllInterrupts(void)
{
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

  /* increment the nesting call counter */
  OCB_Cfg[osActiveCore]->pInt->OsInterruptSuspendAllNestingCall++;

  /* check for nested call */
  if(1ul == OCB_Cfg[osActiveCore]->pInt->OsInterruptSuspendAllNestingCall)
  {
    /* Get the active interrupt priority level */
    OCB_Cfg[osActiveCore]->pInt->OsInterruptSavedGlobalMask = osGetInterruptPriorityMask();

    /* mask all interrupts */
    osSetInterruptPriorityMask((uint32_t)-1);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_ResumeAllInterrupts
///
/// \descr  Resume all suspended interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_ResumeAllInterrupts(void)
{
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

  /* decrement the nesting call counter */
  OCB_Cfg[osActiveCore]->pInt->OsInterruptSuspendAllNestingCall--;

  /* check for nested call */
  if(0 == OCB_Cfg[osActiveCore]->pInt->OsInterruptSuspendAllNestingCall)
  {
    /* Restore all interrupts mask prio */
    osSetInterruptPriorityMask(OCB_Cfg[osActiveCore]->pInt->OsInterruptSavedGlobalMask);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_SuspendOSInterrupts
///
/// \descr  Suspend all OS interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_SuspendOSInterrupts(void)
{
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

  /* increment the nesting call counter */
  OCB_Cfg[osActiveCore]->pInt->OsInterruptSuspendOsNestingCall++;

  /* check for nested call */
  if(1ul == OCB_Cfg[osActiveCore]->pInt->OsInterruptSuspendOsNestingCall)
  {
    /* Get the active interrupt priority level */
    OCB_Cfg[osActiveCore]->pInt->OsInterruptSavedPrioLevel = osGetInterruptPriorityMask();

    /* mask OS interrupts */
    osSetInterruptPriorityMask(OCB_Cfg[osActiveCore]->pInt->OsIntCat1LowestPrioLevel);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_ResumeOSInterrupts
///
/// \descr  Resume all suspended OS interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_ResumeOSInterrupts(void)
{
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

  /* decrement the nesting call counter */
  OCB_Cfg[osActiveCore]->pInt->OsInterruptSuspendOsNestingCall--;

  /* check for nested call */
  if(0 == OCB_Cfg[osActiveCore]->pInt->OsInterruptSuspendOsNestingCall)
  {
    /* Restore the mask prio */
    osSetInterruptPriorityMask(OCB_Cfg[osActiveCore]->pInt->OsInterruptSavedPrioLevel);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsIsr_UndefinedFunc
///
/// \descr  default handler for EI level interrupt
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OsIsr_UndefinedFunc(void)
{
  osKernelError(E_OS_DISABLEDINT);
}
