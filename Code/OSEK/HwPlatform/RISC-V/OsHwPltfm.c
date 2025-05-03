// *****************************************************************************
// Filename    : OsHwPltfm.c
// 
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 18.01.2023
// 
// Description : Hardware platform implementation for WCH RISC-V CH32V307
// 
// *****************************************************************************

//=========================================================================================
// Includes
//=========================================================================================
#include"OsTypes.h"
#include"OsHwPltfm.h"
#include"OsInternal.h"
#include"OsTcb.h"
#include"RP2350.h"

//=========================================================================================
// Functions Prototype
//=========================================================================================


//=========================================================================================
// Globals
//=========================================================================================
//static uint16 osInterrupt_reg_meiea_window[32]  = {0};
//static uint16 osInterrupt_reg_meipra_window[32] = {0};

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
  return((uint32_t)(riscv_read_csr(RVCSR_MCAUSE_OFFSET) >> 31) == 0ul ? FALSE : TRUE);
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
  return((((riscv_read_csr(RVCSR_MSTATUS_OFFSET) >> 3) & 0x01) == 0) ? TRUE : FALSE);
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
  /* configure machine timer to use 150 MHz */
  HW_PER_SIO->MTIME_CTRL.bit.FULLSPEED = 1;
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
  /* set next timeout (machine timer is enabled by default) */
   osHwTimerReload();
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
  volatile uint64_t* const pMTIMECMP = (volatile uint64_t*)&(HW_PER_SIO->MTIMECMP.reg);
  volatile uint64_t* const pMTIME    = (volatile uint64_t*)&(HW_PER_SIO->MTIME.reg);

  *pMTIMECMP = *pMTIME + 150000ul; //1ms
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
  uint16 osInterrupt_reg_meiea_window[32]  = {0};
  uint16 osInterrupt_reg_meipra_window[32] = {0};
  ISR(Undefined);

  for (uint32 InterruptIndex = 0; InterruptIndex < OCB_Cfg[osActiveCore]->pInt->OsNbrOfInterrupts; InterruptIndex++)
  {
    if (OCB_Cfg[osActiveCore]->pInt->OsIsrLookupTablePtr[InterruptIndex].IsrFunc != pISR(Undefined))
    {
      /* enable the interrupt */
      const uint32 meiea_window_bit = InterruptIndex % 16;
      const uint32 meiea_window_idx = InterruptIndex / 16;

      osInterrupt_reg_meiea_window[meiea_window_idx] |= (1 << meiea_window_bit);

      /* set prio */
      const uint32 meipra_window_bit = InterruptIndex % 4;
      const uint32 meipra_window_idx = InterruptIndex / 4;

      osInterrupt_reg_meipra_window[meipra_window_idx] |= (OCB_Cfg[osActiveCore]->pInt->OsIsrLookupTablePtr[InterruptIndex].Prio << (4 * meipra_window_bit));
    }
  }

  /* configure the Hazard3 interrupt controller (Xh3irq) */
  for(uint8 idx = 0; idx <32; idx++)
  {
    const uint32 meiea_val  = (osInterrupt_reg_meiea_window[idx] << 16)  | idx;
    const uint32 meipra_val = (osInterrupt_reg_meipra_window[idx] << 16) | idx;

    riscv_write_csr(RVCSR_MEIEA_OFFSET, meiea_val);
    riscv_write_csr(RVCSR_MEIPRA_OFFSET, meipra_val);
  }

  /* enable external interrupts */
  riscv_set_csr(RVCSR_MIE_OFFSET, RVCSR_MIE_MEIE_BITS);

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
  uint32 new_meinext = (osGetInterruptPriorityMask() & ~(0x1ful << 16)) | (level << 16);
  riscv_write_csr(RVCSR_MEICONTEXT_OFFSET, new_meinext);
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
  return ((riscv_read_csr(RVCSR_MEICONTEXT_OFFSET) >> 16) & 0x1ful);
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
  return((riscv_read_clear_csr(RVCSR_MSTATUS_OFFSET, RVCSR_MSTATUS_MIE_BITS) >> 3ul) & 0x01);
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
  return((riscv_read_set_csr(RVCSR_MEINEXT_OFFSET, 1ul)) >> 2);
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
  (void)InterruptId;
  /* current interrupt pending flag is cleared already in osGetActiveInterruptVectorId() when entering the ISR */
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
  (void)TargetCore;
  HW_PER_SIO->DOORBELL_OUT_SET.reg |= (1ul << 0);
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
  HW_PER_SIO->DOORBELL_IN_CLR.reg |= (1ul << 0);
}

