#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_utils.h"

#include "gpio_helper.h"
#include "timer_helper.h"
#include "main.h"
#include "test.h"
#include <stdio.h>

// #define __HAL_RCC_DMA1_CLK_DISABLE()      (RCC->AHBENR &= ~(RCC_AHBENR_DMA1EN))


void SystemClock_Config(void);

static GPIO_PIN* led;

void Timer_Callback()
{
    LL_GPIO_TogglePin(led->GPIOx, led->Pin);
}


int main(void)
{
  printf ("test");

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

  NVIC_SetPriority(RCC_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(RCC_IRQn);

  /** DISABLE: JTAG-DP Disabled and SW-DP Disabled
  */
  // LL_GPIO_AF_DisableRemap_SWJ();

  SystemClock_Config();

  GPIO_PIN pin1 = GPIO_GetPin("B8");
  GPIO_Setup_OutPP(&pin1);

  led = &pin1;
  SystemCoreClockUpdate();

  LL_RCC_ClocksTypeDef RCC_Clocks;
  LL_RCC_GetSystemClocksFreq(&RCC_Clocks);

  uint32_t psc = __LL_TIM_CALC_PSC(RCC_Clocks.PCLK1_Frequency, 500000);
  uint32_t ar = __LL_TIM_CALC_ARR(RCC_Clocks.PCLK1_Frequency, psc, 10);

  /*
  uint32_t freq = SystemCoreClock;
  freq = SystemCoreClock;

  LL_RCC_ClocksTypeDef RCC_Clocks;
  LL_RCC_GetSystemClocksFreq(&RCC_Clocks);

  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

  LL_TIM_SetCounterMode(TIM2, LL_TIM_COUNTERMODE_UP);

  uint32_t psc = __LL_TIM_CALC_PSC(RCC_Clocks.PCLK1_Frequency, 10000);
  LL_TIM_SetPrescaler(TIM2, psc);

  uint32_t ar = __LL_TIM_CALC_ARR(RCC_Clocks.PCLK1_Frequency, LL_TIM_GetPrescaler(TIM2), 1);
  LL_TIM_SetAutoReload(TIM2, ar);

  LL_TIM_EnableIT_UPDATE(TIM2);

  NVIC_SetPriority(TIM2_IRQn, 0);
  NVIC_EnableIRQ(TIM2_IRQn);

  LL_TIM_EnableCounter(TIM2);
  */
  TIM_SetupCounterTIM2(1000000);

  //LL_TIM_GenerateEvent_UPDATE(TIM2);


  /*
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_USART2_Init();
  */

  testFunc();

  while (1)
  {
	  //LL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
      //LL_GPIO_TogglePin(pin1.GPIOx, pin1.Pin);
      LL_mDelay(1000);
  }
}

void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2)
  {
  }

  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_HSI_Enable();

  while(LL_RCC_HSI_IsReady() != 1)
      ;


  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_16);
  LL_RCC_PLL_Enable();

  while(LL_RCC_PLL_IsReady() != 1)
  {
  }

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
      ;

  LL_Init1msTick(64000000);

  // LL_SetSystemCoreClock(64000000);

  LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSRC_PCLK2_DIV_8);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

