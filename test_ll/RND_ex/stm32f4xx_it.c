# 39 "stm32f4xx_it.c"
# 66 "stm32f4xx_it.c"
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

    LL_EXTI_ClearFlag_0_31(USER_BUTTON_EXTI_LINE);


    UserButton_Callback();
  }
}
