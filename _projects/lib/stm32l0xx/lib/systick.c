#include "systick.h"
#include "stm32l0xx_ll_rcc.h"

uint8_t Systick_Tick_ms = 1; // default to 1ms
__IO uint32_t uwTick = 0;

void SysTick_Handler(void)
{
    SysTick_IncTick();
}

void SysTick_Init(void)
{
    LL_RCC_ClocksTypeDef RCC_Clocks;
    LL_RCC_GetSystemClocksFreq(&RCC_Clocks);
    SysTick_Config (RCC_Clocks.HCLK_Frequency / 1000 * Systick_Tick_ms); 
    NVIC_SetPriority(SysTick_IRQn, 0);
}

void SysTick_IncTick(void) 
{
  ++uwTick;
}

uint32_t SysTick_GetTick(void)
{
  return uwTick * Systick_Tick_ms;
}

void SysTick_Delay(uint32_t delay_ms)
{
  uint32_t tickstart = SysTick_GetTick();
  uint32_t wait = delay_ms;

  //++wait;

  while ((SysTick_GetTick() - tickstart) < wait)
  {
  }
}