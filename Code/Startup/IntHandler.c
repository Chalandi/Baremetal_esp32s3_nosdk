/******************************************************************************************
  Filename    : IntHandler.c
  
  Core        : Xtensa LX7
  
  MCU         : ESP32-S3
    
  Author      : Chalandi Amine
 
  Owner       : Chalandi Amine
  
  Date        : 22.02.2025
  
  Description : Interrupt handler for ESP32-S3
  
******************************************************************************************/

//=============================================================================
// Includes
//=============================================================================
#include <stdint.h>

//=============================================================================
// Functions prototype
//=============================================================================
void Isr_Level1KernelInterrupt(uint32_t irq);
void Isr_Level1UserInterrupt(uint32_t irq);
void Isr_Level2Interrupt(uint32_t irq);
void Isr_Level3Interrupt(uint32_t irq);
void Isr_Level4Interrupt(uint32_t irq);
void Isr_Level5Interrupt(uint32_t irq);

//=============================================================================
// Externs
//=============================================================================
extern void blink_led(void);
extern void systicktimer_1us_base(void);
extern void systicktimer_1ms_base(void);

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
void Isr_Level1KernelInterrupt(uint32_t irq)
{
  (void)irq;
  for(;;);
}

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
void Isr_Level1UserInterrupt(uint32_t irq)
{
  if(irq & (1ul << 6))
    systicktimer_1us_base();
}

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
void Isr_Level2Interrupt(uint32_t irq)
{
  (void)irq;
  for(;;);
}

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
void Isr_Level3Interrupt(uint32_t irq)
{
  if(irq & (1ul << 15))
    blink_led();
}

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
void Isr_Level4Interrupt(uint32_t irq)
{
  (void)irq;
  for(;;);
}

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
void Isr_Level5Interrupt(uint32_t irq)
{
  if(irq & (1ul << 16))
    systicktimer_1ms_base();
}