// *****************************************************************************
// Filename    : OsHwPltfm.c
//
// Core        : Xtensa Lx7
// 
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 09.03.2025
// 
// Description : Hardware platform implementation
// 
// *****************************************************************************

//=========================================================================================
// Includes
//=========================================================================================
#include"OsTypes.h"
#include"OsHwPltfm.h"
#include"OsInternal.h"
#include"OsTcb.h"

//=========================================================================================
// Functions Prototype
//=========================================================================================

//=========================================================================================
// Globals
//=========================================================================================

//------------------------------------------------------------------------------------------------------------------
/// \brief  osIsInterruptContext
///
/// \descr  This function check the cpu mode (thread or interrupt).
///
/// \param  void
///
/// \return boolean: TRUE -> Cpu is in interrupt context, FALSE -> Cpu is not in interrupt context
//------------------------------------------------------------------------------------------------------------------
boolean osIsInterruptContext(void)
{
  #define EXCM_BIT_MASK (1ul << 4)
  return((osGetCurrentPS() & EXCM_BIT_MASK) == EXCM_BIT_MASK ? TRUE : FALSE);
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
  return((osGetInterruptEnableReg() == 0ul) ? TRUE : FALSE);
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
  _osHwTimerInit();
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
  osSetPrivateTimer(0, 80000);
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
  osSetPrivateTimer(0, 80000);
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  osInitInterrupts
///
/// \descr  Init the cat1 and cat2 interrupts
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void osInitInterrupts(void)
{
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());
  
  ISR(Undefined);

  volatile pINTERRUPT_COREx_INTR_CONFIG_REG* InterruptConfig[2] = {
      (volatile pINTERRUPT_COREx_INTR_CONFIG_REG*)(INTERRUPT_MATRIX_BASE + 0x0000ul),
      (volatile pINTERRUPT_COREx_INTR_CONFIG_REG*)(INTERRUPT_MATRIX_BASE + 0x0800ul)
  };

  for (uint32 InterruptIndex = 0; InterruptIndex < OCB_Cfg[osActiveCore]->pInt->OsNbrOfInterrupts; InterruptIndex++)
  {
    if (OCB_Cfg[osActiveCore]->pInt->OsIsrLookupTablePtr[InterruptIndex].IsrFunc != pISR(Undefined))
    {
      if(OCB_Cfg[osActiveCore]->pInt->OsIsrLookupTablePtr[InterruptIndex].category == 1)
      {
        /* remap the cat1 interrupts to cpu IRQ31 - level 5 (highest prio) */
        (((uint32*)InterruptConfig[osActiveCore])[InterruptIndex] = ACTIVE_CAT1_IRQ_NUMBER);
      }
      else
      {
        /* remap the cat2 interrupts to cpu IRQ5 - level 1 (lowest prio) */
        (((uint32*)InterruptConfig[osActiveCore])[InterruptIndex] = ACTIVE_CAT2_IRQ_NUMBER);
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
  osSetPriorityLevel(level);
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
  return(osGetInterruptLevel());
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  osCatchAllCpuExceptions
///
/// \descr  This function handle all CPU exceptions.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void osCatchAllCpuExceptions(void)
{
  DISABLE_INTERRUPTS();
  for(;;);
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
uint32 osGetInterruptGlobalMask(void)
{
  return((osGetInterruptLevel() == 15) ? 0ul : 1ul);
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
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

  volatile pINTERRUPT_COREx_INTR_STATUS_REG* InterruptStatus[2] = {
      (volatile pINTERRUPT_COREx_INTR_STATUS_REG*)(INTERRUPT_MATRIX_BASE + 0x018cul),
      (volatile pINTERRUPT_COREx_INTR_STATUS_REG*)(INTERRUPT_MATRIX_BASE + 0x098Cul)
  };

  volatile pINTERRUPT_COREx_INTR_CONFIG_REG* InterruptConfig[2] = {
      (volatile pINTERRUPT_COREx_INTR_CONFIG_REG*)(INTERRUPT_MATRIX_BASE + 0x0000ul),
      (volatile pINTERRUPT_COREx_INTR_CONFIG_REG*)(INTERRUPT_MATRIX_BASE + 0x0800ul)
  };

  for(uint32_t reg = 0; reg < 4; reg++)
  {
    for(uint32_t pos = 0; pos < 32; pos++)
    {
      if(((((uint32*)InterruptStatus[osActiveCore])[reg] & (1ul << pos)) != 0) && (((uint32*)InterruptConfig[osActiveCore])[(reg * 32) + pos] == ACTIVE_CAT2_IRQ_NUMBER))
      {
        return((reg * 32) + pos);
      }
    }
  }
  
  /* no active vector was found which is not expected to happen */
  osKernelError(E_OS_SYS_API_ERROR);
  return(0);
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
  if(InterruptId < 32ul)
  {
    osClearPendingIRQ((uint32_t)(1ul << InterruptId));
  }
  else
  {
    /* xtensa lx7 has only 32 IRQs */
    osKernelError(E_OS_SYS_API_ERROR);
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
void osGenerateCrossCoreInterrupt(OsCoreId ActiveCore, OsCoreId TargetCore)
{
  if((ActiveCore == 1) && (TargetCore == 0))
  {
    /* trigger cross-core interrupt on core0 */
    SYSTEM_CPU_INTR_FROM_CPU_1 = 1;
  }
  else if((ActiveCore == 0) && (TargetCore == 1))
  {
    /* trigger cross-core interrupt on core1 */
    SYSTEM_CPU_INTR_FROM_CPU_0 = 1;
  }
  else
  {
    /* unsupported core */
    osKernelError(E_OS_SYS_API_ERROR);
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
void osClearCrossCoreInterrupt(void)
{
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

  if(osActiveCore)
  {
    /* clear cross-core interrupt request on core1 */
    SYSTEM_CPU_INTR_FROM_CPU_0 = 0;
  }
  else
  {
    /* clear cross-core interrupt request on core0 */
    SYSTEM_CPU_INTR_FROM_CPU_1 = 0;
  }
}