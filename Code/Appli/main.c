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
#include "printf.h"

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
#define WS2812_ENABLED
#define WS2812_PIN             48u
#define WS2812_HIGH            GPIO->OUT1.reg |= (1ul << (WS2812_PIN - 32))
#define WS2812_LOW             GPIO->OUT1.reg &= ~((1ul << (WS2812_PIN - 32)))
#define WS2812_ONE             WS2812_HIGH; WS2812_HIGH; WS2812_HIGH; WS2812_LOW
#define WS2812_ZERO            WS2812_HIGH; WS2812_LOW; WS2812_LOW; WS2812_LOW


#define WS2812_GREEN_ONLY()  /* G - 0x40 */ WS2812_ZERO; WS2812_ZERO; WS2812_ONE; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO;  \
                             /* R - 0x00 */ WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; \
                             /* B - 0x00 */ WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO

#define WS2812_RED_ONLY()    /* G - 0x00 */ WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO;  \
                             /* R - 0x40 */ WS2812_ZERO; WS2812_ZERO; WS2812_ONE; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO;   \
                             /* B - 0x00 */ WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO

#define WS2812_BLUE_ONLY()   /* G - 0x00 */ WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO;  \
                             /* R - 0x00 */ WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO;  \
                             /* B - 0x40 */ WS2812_ZERO; WS2812_ZERO; WS2812_ONE; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO; WS2812_ZERO

//=============================================================================
// Prototypes
//=============================================================================
void main(void);
void main_c1(void);
void blink_led(void);
void systicktimer_1us_base(void);
void systicktimer_1ms_base(void);

extern void Mcu_StartCore1(void);
extern uint32_t get_core_id(void);
extern void enable_irq(uint32_t mask);
extern void set_cpu_private_timer(uint32_t timer_id, uint32_t ticks);
extern void Mcu_StartCoProcessorRiscV(void);

//=============================================================================
// Globals
//=============================================================================
volatile uint64_t SysTickTimer1usBase = 0;
volatile uint64_t SysTickTimer1msBase = 0;

//-----------------------------------------------------------------------------------------
/// \brief  main function for core 0
///
/// \param  void
///
/// \return void
//-----------------------------------------------------------------------------------------
void main(void)
{
  printf("Hello from core %d\r\n", get_core_id());

  GPIO->OUT.reg |= CORE0_LED;

  /* enable timers interrupt on core 0 */
  enable_irq((1UL << 16) | (1UL << 15) | (1UL << 6) );

  /* start the systick timer (1us base)*/
  set_cpu_private_timer(0, 80);

  /* start the systick timer (1ms base)*/
  set_cpu_private_timer(2, 80000);

  /* start the core 1*/
  Mcu_StartCore1();

#ifdef COPROCESSOR_ENABLED
  /* start the co-processor RISC-V */
  Mcu_StartCoProcessorRiscV();
#endif

  /* set the private cpu timer1 for core 0 */
  set_cpu_private_timer(1, LED_BLINK_FREQ_1HZ);

  for(;;);
}

//-----------------------------------------------------------------------------------------
/// \brief  main function for core1
///
/// \param  void
///
/// \return void
//-----------------------------------------------------------------------------------------
void main_c1(void)
{
  printf("Hello from core %d\r\n", get_core_id());

  GPIO->OUT.reg |= CORE1_LED;

  /* enable timer1 interrupt on core 1 */
  enable_irq((uint32_t)(1UL << 15));

  /* set the private cpu timer1 for core 1 */
  set_cpu_private_timer(1, LED_BLINK_FREQ_1HZ);

  for(;;);
}
//-----------------------------------------------------------------------------------------
/// \brief  
///
/// \param 
///
/// \return 
//-----------------------------------------------------------------------------------------
void systicktimer_1us_base(void)
{
  SysTickTimer1usBase++;
  set_cpu_private_timer(0, 80);
}

//-----------------------------------------------------------------------------------------
/// \brief  
///
/// \param 
///
/// \return 
//-----------------------------------------------------------------------------------------
void systicktimer_1ms_base(void)
{
  SysTickTimer1msBase++;
  set_cpu_private_timer(2, 80000);
}

//-----------------------------------------------------------------------------------------
/// \brief  
///
/// \param 
///
/// \return 
//-----------------------------------------------------------------------------------------
void blink_led(void)
{
#ifdef WS2812_ENABLED
  static uint32_t color = 0;
#endif

  /* reload the private timer1 */
  set_cpu_private_timer(1, LED_BLINK_FREQ_1HZ);
  
  /* toggle the leds */
  if(get_core_id())
  {
    GPIO->OUT.reg ^= CORE1_LED;

#ifdef WS2812_ENABLED
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
#endif
  }
  else
  {
    GPIO->OUT.reg ^= CORE0_LED;
  }
}
