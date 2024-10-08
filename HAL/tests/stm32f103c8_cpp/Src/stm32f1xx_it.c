#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_it.h"

void NMI_Handler(void)
{
  while (1);
}

void HardFault_Handler(void)
{
  while (1);
}

void MemManage_Handler(void)
{
  while (1);
}

void BusFault_Handler(void)
{
  while (1);
}

void UsageFault_Handler(void)
{
  while (1);
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
}

void RCC_IRQHandler(void)
{
}

void ADC1_2_IRQHandler(void)
{
}

void TIM2_IRQHandler(void)
{
    if(LL_TIM_IsActiveFlag_UPDATE(TIM2) == 1)
    {
      LL_TIM_ClearFlag_UPDATE(TIM2);
      Timer_Callback();
    }
}

void I2C1_ER_IRQHandler(void)
{
}

void SPI1_IRQHandler(void)
{
}

void USART1_IRQHandler(void)
{
}

