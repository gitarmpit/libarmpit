//#include "stm32f4xx_it.h"
void NMI_Handler(void)
{
}
void HardFault_Handler(void)
{

  while (1)
  {
  }
}
void MemManage_Handler(void)
{
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  while (1)
  {
  }
}
void UsageFault_Handler(void)
{
  while (1)
  {
  }
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
void USER_BUTTON_IRQHANDLER(void)
{
  if(LL_EXTI_IsActiveFlag_0_31(USER_BUTTON_EXTI_LINE) != RESET)
  {
    UserButton_Callback();
    LL_EXTI_ClearFlag_0_31(USER_BUTTON_EXTI_LINE);
  }
}

void TIM6_DAC_IRQHandler(void)
{
  if(LL_DAC_IsActiveFlag_DMAUDR1(DAC1) != 0)
  {
    LL_DAC_ClearFlag_DMAUDR1(DAC1);
    DacUnderrunError_Callback();
  }
}

void DMA1_Stream5_IRQHandler(void)
{
  if(LL_DMA_IsActiveFlag_TE7(DMA1) == 1)
  {
    LL_DMA_ClearFlag_TE7(DMA1);
    DacDmaTransferError_Callback();
  }
}
