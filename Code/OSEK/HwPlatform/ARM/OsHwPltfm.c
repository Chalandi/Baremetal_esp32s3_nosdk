// *****************************************************************************
// Filename    : OsHwPltDep.c
// 
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 27.12.2017
// 
// Description : Hardware Platform definition for ARM Cortex-M33
// 
// *****************************************************************************

//------------------------------------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------------------------------------
#include"OsTypes.h"
#include"OsHwPltfm.h"
#include"OsInternal.h"
#include"OsTcb.h"
#include"RP2350.h"

//------------------------------------------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------------------------------------------
#define OS_REMAP_ARM_INTERRUPT_PRIO

//------------------------------------------------------------------------------------------------------------------
// Static functions prototypes
//------------------------------------------------------------------------------------------------------------------
static uint32 osRemapOsPriorityValue(uint32 level, uint32 IntPrioBit);

//------------------------------------------------------------------------------------------------------------------
// Globals
//------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------
/// \brief  osIsInterruptContext
///
/// \descr  This function check the cpu mode (thread or interrupt).
///
/// \param  void
///
/// \return boolean: TRUE -> Cpu in interrupt context, FALSE -> Cpu not in interrupt context
//------------------------------------------------------------------------------------------------------------------
boolean osIsInterruptContext(void)
{
  return((GET_ICSR_VECTACTIVE() == 0) ? FALSE : TRUE);
}

//-----------------------------------------------------------------------------
/// \brief
///
/// \descr
///
/// \param
///
/// \return
//-----------------------------------------------------------------------------
boolean osIsInterruptDisabled(void)
{
  return(OsGetSysPrimaskReg());
}

//-----------------------------------------------------------------------------
/// \brief
///
/// \descr
///
/// \param
///
/// \return
//-----------------------------------------------------------------------------
void osHwTimerInit(void)
{
  SysTickTimer_Init();
}

//-----------------------------------------------------------------------------
/// \brief
///
/// \descr
///
/// \param
///
/// \return
//-----------------------------------------------------------------------------
void osHwTimerStart(void)
{
  SysTickTimer_Start(SYS_TICK_MS(1));
}

//-----------------------------------------------------------------------------
/// \brief
///
/// \descr
///
/// \param
///
/// \return
//-----------------------------------------------------------------------------
void osHwTimerReload(void)
{
  SysTickTimer_Reload(SYS_TICK_MS(1));
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  osInitInterrupts
///
/// \descr  Init the cat2 interrupts
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void osInitInterrupts(void)
{
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

  ISR(Undefined);

  uint32 IntPrioBit = OsHwGetInterruptPrioBits();

  for (uint32 InterruptIndex = 0; InterruptIndex < OCB_Cfg[osActiveCore]->pInt->OsNbrOfInterrupts; InterruptIndex++)
  {
    if (OCB_Cfg[osActiveCore]->pInt->OsIsrLookupTablePtr[InterruptIndex].IsrFunc != pISR(Undefined))
    {
      if((InterruptIndex == 4U) || (InterruptIndex == 5U)  || (InterruptIndex == 6U) || (InterruptIndex == 11U) || (InterruptIndex == 14U) || (InterruptIndex == 15U))
      {
        /* set the system handler priority level */
        SCB_SHPRx[InterruptIndex - 4U] = ((uint8)osRemapOsPriorityValue(OCB_Cfg[osActiveCore]->pInt->OsIsrLookupTablePtr[InterruptIndex].Prio, IntPrioBit) << (8U - IntPrioBit));
      }
      else if(InterruptIndex > 15U)
      {
        /* set the NVIC interrupt priority level */
        NVIC_IPRx[InterruptIndex - 16U] |= ((uint8)osRemapOsPriorityValue(OCB_Cfg[osActiveCore]->pInt->OsIsrLookupTablePtr[InterruptIndex].Prio, IntPrioBit) << (8U - IntPrioBit));

        /* clear the NVIC interrupt pending */
        NVIC_ICPRx[(InterruptIndex - 16U) / 32U] |= (1UL << ((InterruptIndex - 16U) % 32U));

        /* enable the NVIC interrupt */
        NVIC_ISERx[(InterruptIndex - 16U) / 32U] |= (1UL << ((InterruptIndex - 16U) % 32U));
      }
      else
      {
        /* priority non configurable */
      }
    }
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
void osSetInterruptPriorityMask(uint32 level)
{
  uint32 IntPrioBit = OsHwGetInterruptPrioBits();

  level = osRemapOsPriorityValue(level, IntPrioBit);

  OsSetSysBasepriReg((level << (8U - IntPrioBit)));
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
uint32 osGetInterruptPriorityMask(void)
{
  uint32 level = 0;
  uint32 IntPrioBit = OsHwGetInterruptPrioBits();

  OsGetSysBasepriReg((uint32*)&level);
  level >>= (8U - IntPrioBit);

  return(osRemapOsPriorityValue(level, IntPrioBit));
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
static uint32 osRemapOsPriorityValue(uint32 level, uint32 IntPrioBit)
{
#ifdef OS_REMAP_ARM_INTERRUPT_PRIO
  /* note: remap the level prio: ARM is using lower value for highest prio 
     contrary to the OS which uses lower value for lower prio */
  if(level > ((1ul << IntPrioBit) - 1))
  {
    level = (1ul << IntPrioBit) - 1;
  }

  level = ((1ul << IntPrioBit) - 1) - level;
#endif
  return(level);
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
uint32 osGetInterruptGlobalMask(void)
{
  return((OsGetSysPrimaskReg() == 1ul) ? 0ul : 1ul);
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  osGetActiveInterruptVectorId
///
/// \descr  This function returns the id of the active interrupt vector.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
uint32 osGetActiveInterruptVectorId(void)
{
  return(GET_ICSR_VECTACTIVE());
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
uint8 osGetCoreId(void)
{
  return((uint8_t)HW_PER_SIO->CPUID.reg);
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
void osClearPendingInterrupt(uint32_t InterruptId)
{
  NVIC->ICPR[(InterruptId / 32)] |= (1ul << (InterruptId % 32));
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
void osGenerateCrossCoreInterrupt(OsCoreId ActiveCore, OsCoreId TargetCore)
{
  (void)ActiveCore;

  HW_PER_SIO->DOORBELL_OUT_SET.reg |= (1ul << TargetCore);
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
void osClearCrossCoreInterrupt(void)
{
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());
  HW_PER_SIO->DOORBELL_IN_CLR.reg |= (1ul << osActiveCore);
}

uint32_t osHwTryToAcquireSpinLock(uint32_t* lock)
{
  (void)lock;
  #warning "osHwTryToAcquireSpinLock is not implemented"
  return 0;
}
