/******************************************************************************************
  Filename    : main.c
  
  Core        : Xtensa LX7
  
  MCU         : ESP32-S3
    
  Author      : Chalandi Amine
 
  Owner       : Chalandi Amine
  
  Date        : 22.02.2025
  
  Description : Application main function
  
******************************************************************************************/

//=============================================================================
// Includes
//=============================================================================
#include "Platform_Types.h"
#include "esp32s3.h"

//=============================================================================
// Defines
//=============================================================================
#define CORE0_LED  (1ul << 7)
#define CORE1_LED  (1ul << 6)
#define APB_FREQ_MHZ  80000000ul
#define LED_BLINK_FREQ_1HZ  (APB_FREQ_MHZ/2)

//=============================================================================
// Macros
//=============================================================================

/* Macros for the WS2812 */
#define WS2812_PIN             48u
#define WS2812_HIGH            GPIO->OUT1.reg |= (1ul << (WS2812_PIN - 32))
#define WS2812_LOW             GPIO->OUT1.reg &= ~((1ul << (WS2812_PIN - 32)))
#define WS2812_ONE             WS2812_HIGH; WS2812_HIGH; WS2812_HIGH; WS2812_LOW
#define WS2812_ZERO            WS2812_HIGH; WS2812_LOW; WS2812_LOW; WS2812_LOW

  #define WS2812_GREEN_ONLY() \
  /* Green */                 \
  WS2812_ONE;                 \
  WS2812_ONE;                 \
  WS2812_ONE;                 \
  WS2812_ONE;                 \
  WS2812_ONE;                 \
  WS2812_ONE;                 \
  WS2812_ONE;                 \
  WS2812_ONE;                 \
  /* Red */                   \
  WS2812_ZERO;                \
  WS2812_ZERO;                \
  WS2812_ZERO;                \
  WS2812_ZERO;                \
  WS2812_ZERO;                \
  WS2812_ZERO;                \
  WS2812_ZERO;                \
  WS2812_ZERO;                \
  /* Blue */                  \
  WS2812_ZERO;                \
  WS2812_ZERO;                \
  WS2812_ZERO;                \
  WS2812_ZERO;                \
  WS2812_ZERO;                \
  WS2812_ZERO;                \
  WS2812_ZERO;                \
  WS2812_ZERO

  #define WS2812_RED_ONLY() \
  /* Green */               \
  WS2812_ZERO;              \
  WS2812_ZERO;              \
  WS2812_ZERO;              \
  WS2812_ZERO;              \
  WS2812_ZERO;              \
  WS2812_ZERO;              \
  WS2812_ZERO;              \
  WS2812_ZERO;              \
  /* Red */                 \
  WS2812_ONE;               \
  WS2812_ONE;               \
  WS2812_ONE;               \
  WS2812_ONE;               \
  WS2812_ONE;               \
  WS2812_ONE;               \
  WS2812_ONE;               \
  WS2812_ONE;               \
  /* Blue */                \
  WS2812_ZERO;              \
  WS2812_ZERO;              \
  WS2812_ZERO;              \
  WS2812_ZERO;              \
  WS2812_ZERO;              \
  WS2812_ZERO;              \
  WS2812_ZERO;              \
  WS2812_ZERO

  #define WS2812_BLUE_ONLY() \
  /* Green */                \
  WS2812_ZERO;               \
  WS2812_ZERO;               \
  WS2812_ZERO;               \
  WS2812_ZERO;               \
  WS2812_ZERO;               \
  WS2812_ZERO;               \
  WS2812_ZERO;               \
  WS2812_ZERO;               \
  /* Red */                  \
  WS2812_ZERO;               \
  WS2812_ZERO;               \
  WS2812_ZERO;               \
  WS2812_ZERO;               \
  WS2812_ZERO;               \
  WS2812_ZERO;               \
  WS2812_ZERO;               \
  WS2812_ZERO;               \
  /* Blue */                 \
  WS2812_ONE;                \
  WS2812_ONE;                \
  WS2812_ONE;                \
  WS2812_ONE;                \
  WS2812_ONE;                \
  WS2812_ONE;                \
  WS2812_ONE;                \
  WS2812_ONE

//=============================================================================
// Prototypes
//=============================================================================
void main(void);
void main_c1(void);
void blink_led(void);

extern void Mcu_StartCore1(void);
extern uint32_t get_core_id(void);
extern void enable_irq(uint32_t mask);
extern void set_cpu_private_timer1(uint32_t ticks);

//=============================================================================
// Globals
//=============================================================================


//-----------------------------------------------------------------------------------------
/// \brief  main function
///
/// \param  void
///
/// \return void
//-----------------------------------------------------------------------------------------
void main(void)
{
  GPIO->OUT.reg |= CORE0_LED;

  /* enable all interrupts on core 0 */
  enable_irq((uint32_t)-1);

  Mcu_StartCore1();

  /* set the private cpu timer1 for core 0 */
  set_cpu_private_timer1(LED_BLINK_FREQ_1HZ);

  for(;;);
}

//-----------------------------------------------------------------------------------------
/// \brief  main function
///
/// \param  void
///
/// \return void
//-----------------------------------------------------------------------------------------
void main_c1(void)
{
  GPIO->OUT.reg |= CORE1_LED;

  /* enable all interrupts on core 1 */
  enable_irq((uint32_t)-1);

  /* set the private cpu timer1 for core 1 */
  set_cpu_private_timer1(LED_BLINK_FREQ_1HZ);

  for(;;);
}
//-----------------------------------------------------------------------------------------
/// \brief  main function
///
/// \param  void
///
/// \return void
//-----------------------------------------------------------------------------------------
void blink_led(void)
{
  static uint32_t color = 0;
  /* reload the private timer1 */
  set_cpu_private_timer1(LED_BLINK_FREQ_1HZ);
  
  /* toggle the leds */
  if(get_core_id())
  {
    GPIO->OUT.reg ^= CORE1_LED;

    if(color == 0)
    {
        WS2812_GREEN_ONLY();
    }
    else if(color == 1)
    {
        WS2812_RED_ONLY();
    }
    else
    {
        WS2812_BLUE_ONLY();
    }

    if(++color > 2)
      color^=color;
  }
  else
  {
    GPIO->OUT.reg ^= CORE0_LED;
  }
}
