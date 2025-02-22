/******************************************************************************************
  Filename    : Mcu.c
  
  Core        : Xtensa LX7
  
  MCU         : ESP32-S3
    
  Author      : Chalandi Amine
 
  Owner       : Chalandi Amine
  
  Date        : 22.02.2025
  
  Description : Mcu basic functions for ESP32-S3
  
******************************************************************************************/

//=============================================================================
// Includes
//=============================================================================
#include "esp32s3.h"
#include "stdint.h"

//=============================================================================
// Prototypes
//=============================================================================
void Mcu_StartCore1(void);
void _start(void);
void Mcu_ClockInit(void);
void Mcu_InitCore(void);

//-----------------------------------------------------------------------------------------
/// \brief  
///
/// \param  
///
/// \return 
//-----------------------------------------------------------------------------------------
void Mcu_StartCore1(void)
{
  /* unstall core 1 */
  RTC_CNTL->OPTIONS0.bit.SW_STALL_APPCPU_C0            = 0;
  RTC_CNTL->SW_CPU_STALL.bit.SW_STALL_APPCPU_C1        = 0;
  SYSTEM->CORE_1_CONTROL_0.bit.CONTROL_CORE_1_RUNSTALL = 0;

  /* enable clock for core 1 */
  SYSTEM->CORE_1_CONTROL_0.bit.CONTROL_CORE_1_CLKGATE_EN = 1;

  /* reset core 1 */
  SYSTEM->CORE_1_CONTROL_0.bit.CONTROL_CORE_1_RESETING   = 1;
  SYSTEM->CORE_1_CONTROL_0.bit.CONTROL_CORE_1_RESETING   = 0;

  /* note: In ESP32-S3, when the reset of the core1 is released, the core1 starts executing the bootROM code and get stuck
         into a trap waiting for the entry address to be received from core0
         this is achievable by writing the core1 entry address to the register SYSTEM_CORE_1_CONTROL_1_REG from core0. */

  /* set the core1 entry address */
   SYSTEM->CORE_1_CONTROL_1.reg = (uint32_t)&_start;
}

//-----------------------------------------------------------------------------------------
/// \brief  
///
/// \param  
///
/// \return 
//-----------------------------------------------------------------------------------------
void Mcu_ClockInit(void)
{
   /* set the core clock to 240 MHz and APB clock to 80 MHz*/
   SYSTEM->CPU_PERI_CLK_EN.reg = 7;
   SYSTEM->SYSCLK_CONF.reg     = 0x401;
}

//-----------------------------------------------------------------------------------------
/// \brief  
///
/// \param  
///
/// \return 
//-----------------------------------------------------------------------------------------
void Mcu_InitCore(void)
{
  /* disable the super watchdog */
  RTC_CNTL->SWD_WPROTECT.reg = 0x8F1D312A;
  RTC_CNTL->WDTCONFIG1.reg = 0;
  RTC_CNTL->SWD_CONF.reg = (1ul << 30);
  RTC_CNTL->SWD_WPROTECT.reg = 0;
  
  /* disable Timer Group 0 WDT */
  TIMG0->WDTWPROTECT.reg = 0x50d83aa1;
  TIMG0->WDTCONFIG0.reg  = 0;
  TIMG0->WDTWPROTECT.reg = 0;

  /* set all gpio as output low */
  GPIO->ENABLE_W1TS.reg = 0xFFFFFFFF;
  GPIO->ENABLE1_W1TS.reg = 0xFFFFFFFF;
  GPIO->OUT.reg   = 0;
  GPIO->OUT1.reg  = 0; 
}
