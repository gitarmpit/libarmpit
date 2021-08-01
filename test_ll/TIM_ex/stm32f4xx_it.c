#include "main.h"
#include "stm32f4xx_it.h"
extern TIM_HandleTypeDef TimHandle;

extern __IO uint32_t uwAuthorizeTimComEvent;
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
  HAL_IncTick();
  if (uwAuthorizeTimComEvent)
  {

    HAL_TIM_GenerateEvent(&TimHandle, TIM_EventSource_COM);
  }
}
void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&TimHandle);

}
