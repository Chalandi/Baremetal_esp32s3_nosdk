/******************************************************************************************
  Filename    : main.c
  
  Core        : RISC-V
  
  MCU         : ESP32-S3
    
  Author      : Chalandi Amine
 
  Owner       : Chalandi Amine
  
  Date        : 22.02.2025
  
  Description : Application main function for ULP-RISC-V Co-processor
  
******************************************************************************************/

//=============================================================================
// Includes
//=============================================================================
#include<stdint.h>

//=============================================================================
// Prototypes
//=============================================================================
void irq_vector(uint32_t irq);
void set_timer_counter(uint32_t timeout);

//=============================================================================
// Defines
//=============================================================================
/* RTC_GPIO18 */
#define RTC_GPIO_BASE         0xA400ul
#define RTC_GPIO_OUT_REG      *(volatile uint32_t*)(RTC_GPIO_BASE + 0x0000ul)
#define RTC_GPIO_ENABLE_REG   *(volatile uint32_t*)(RTC_GPIO_BASE + 0x000Cul)
#define RTC_GPIO_PIN17_REG    *(volatile uint32_t*)(RTC_GPIO_BASE + 0x006Cul)
#define RTC_IO_RTC_PAD17_REG  *(volatile uint32_t*)(RTC_GPIO_BASE + 0x00C8ul)
#define RTC_GPIO_PIN18_REG    *(volatile uint32_t*)(RTC_GPIO_BASE + 0x0070ul)
#define RTC_IO_RTC_PAD18_REG  *(volatile uint32_t*)(RTC_GPIO_BASE + 0x00CCul)

#define RTC_CONTROL_BASE            0x8000ul
#define RTC_CNTL_COCPU_CTRL_REG     *(volatile uint32_t*)(RTC_CONTROL_BASE + 0x0104ul)

#define SENS_REG_BASE               0xC800ul
#define SENS_SAR_COCPU_INT_ENA_REG  *(volatile uint32_t*)(SENS_REG_BASE + 0x00ECul)
#define SENS_SAR_COCPU_INT_CLR_REG  *(volatile uint32_t*)(SENS_REG_BASE + 0x00F4ul)

#define TIMEOUT_500MS   0x8583B0

//=============================================================================
// Macros
//=============================================================================
#define TOGGLE_GPIO18()       RTC_GPIO_OUT_REG ^= (1ul << (10 + 18))
#define TOGGLE_GPIO17()       RTC_GPIO_OUT_REG ^= (1ul << (10 + 17))


//-----------------------------------------------------------------------------------------
/// \brief  main function
///
/// \param  void
///
/// \return void
//-----------------------------------------------------------------------------------------
void main(void)
{
  /* configure GPIO17 and GPIO18 as output low */
  RTC_IO_RTC_PAD18_REG |= (1ul << 19);
  RTC_GPIO_PIN18_REG    = 0;
  RTC_GPIO_ENABLE_REG  |= (1ul << (10 + 18));
  RTC_IO_RTC_PAD17_REG |= (1ul << 19);
  RTC_GPIO_PIN17_REG    = 0;
  RTC_GPIO_ENABLE_REG  |= (1ul << (10 + 18)) | (1ul << (10 + 17)); 
  RTC_GPIO_OUT_REG = 0;

  /* set cpu timer timeout */
  set_timer_counter(TIMEOUT_500MS);

  /* enable software interrupt */
  SENS_SAR_COCPU_INT_ENA_REG |= (1ul << 7);

  for(;;)
  {
     for(uint32_t i=0; i< 0x100000; i++);

     /* trigger SW interrupt */
     RTC_CNTL_COCPU_CTRL_REG |= (1ul << 26);
  }
}

//-----------------------------------------------------------------------------------------
/// \brief  interrupts handler function
///
/// \param  irq number
///
/// \return 
//-----------------------------------------------------------------------------------------
void irq_vector(uint32_t irq)
{
  if((irq & 7ul) != 0)
  {
    TOGGLE_GPIO18();
    set_timer_counter(TIMEOUT_500MS);
  }
  
  if((irq & (1ul << 31)) == (1ul << 31))
  {
     TOGGLE_GPIO17();
    /* clear interrupt flag */
    SENS_SAR_COCPU_INT_CLR_REG |= (1ul << 7);
  }
}

