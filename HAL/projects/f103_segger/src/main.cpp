#include "but.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_system.h"

#include "button.h"
#include "gpio_helper.h"
#include "main.h"
#include "systick_helper.h"
#include "timer_helper.h"
#include <stdio.h>

// #define __HAL_RCC_DMA1_CLK_DISABLE()      (RCC->AHBENR &= ~(RCC_AHBENR_DMA1EN))

void SystemClock_Config(void);
void SystemClock_Config_HSI(void);

static GPIO_PIN* led;

static int cnt = 0;
static BOOL UP = FALSE;
static void Timer_Callback(void*) {
  GPIO_TogglePin(led);
}

static void Timer_Callback2(void* ctx) {
  TIM_Channel* ch = (TIM_Channel*)ctx;
  if (UP && cnt <= 70) {
    TIM_UpdateDs(ch, cnt++);
    if (cnt == 70) {
      UP = FALSE;
    }
  }
  else if (!UP && cnt >= 10) {
    TIM_UpdateDs(ch, cnt--);
    if (cnt == 10) {
      UP = TRUE;
    }
  }
}


static void testPWM3() {
  GPIO_PIN pin1 = GPIO_GetPin("B0");    // TIM3 channel 3
  GPIO_Setup_OutAltPP(&pin1);
  TIM_Channel ch = TIM_SetupPWM_TIM3(3, 80, 20);

  TIM_UpdateDs(&ch, 40);

  //TIM_SetupPWM_OnPin("B5", 400, 200);
  while (1)
    ;
}

static void testUpdateDs() {
  GPIO_PIN pin1 = GPIO_GetPin("B0");    // TIM3 channel 3
  GPIO_Setup_OutAltPP(&pin1);
  TIM_Channel ch = TIM_SetupPWM_TIM3(3, 80, 10);
  SysTick_Delay(2000);
  UP = TRUE;
  cnt = 10;
  TIM_SetupCounterTIM1(100000, Timer_Callback2, &ch);

  while (1)
    ;
}

static void testPWM2() {
  GPIO_PIN pin1 = GPIO_GetPin("A0");    // TIM2 channel 1
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM2(1, 180, 40);
  //TIM_SetupPWM_OnPin("B11", 800, 600);
  while (1)
    ;
}

static void testPWM1() {
  // GPIO_PIN pin1 = GPIO_GetPin("A11");    // TIM1 channel 4
  // GPIO_Setup_OutAltPP(&pin1);

  GPIO_PIN pin1 = GPIO_GetPin("A10");
  GPIO_Setup_OutPP(&pin1);
  GPIO_SetPin(&pin1);
  GPIO_ResetPin(&pin1);
    
  GPIO_Setup_OutAltPP(&pin1);
  TIM_SetupPWM_TIM1(3, 80, 20);

  // TIM_SetupPWM_TIM1(4, 80, 60);
  //TIM_SetupPWM_OnPin("A10", 8, 2);
  while (1)
    ;
}

static void testTimer() {
  GPIO_PIN pin1 = GPIO_GetPin("B0");
  GPIO_Setup_OutPP(&pin1);
  led = &pin1;

  GPIO_SetPin(&pin1);
  TIM_SetupCounterTIM1(100, Timer_Callback, NULL);
  while (1)
    ;
}

static void testButton() {
  GPIO_PIN sw = GPIO_GetPin("A7");
  // GPIO_Setup_In_Pullup(&sw);
  Button b(&sw, 1);
  while (1) {
    SysTick_Delay(100);
    // printf ("%d\n", GPIO_IsSet(&sw));
    printf("%d\n", b.IsPinSet());
  }
}

static void testButton2() {
  GPIO_PIN sw = GPIO_GetPin("A7");
  // GPIO_Setup_In_Pullup(&sw);
  button_ctx button = Button_Init(&sw, 1);

  while (1) {
    SysTick_Delay(100);
    // printf ("%d\n", GPIO_IsSet(&sw));
    printf("%d\n", Button_IsPinSet(&button));
  }
}


static void testBlinky() {
  GPIO_PIN pin1 = GPIO_GetPin("B0");
  GPIO_Setup_OutPP(&pin1);
  while (1) {
    printf("%d\n", SysTick_GetTick());
    GPIO_TogglePin(&pin1);
    SysTick_Delay(1000);
  }
}

int main(void) {
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));

  NVIC_SetPriority(RCC_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(RCC_IRQn);

  /** DISABLE: JTAG-DP Disabled and SW-DP Disabled
   */
  // LL_GPIO_AF_DisableRemap_SWJ();

  LL_GPIO_AF_Remap_SWJ_NONJTRST();
  LL_GPIO_AF_Remap_SWJ_NOJTAG();

  SystemClock_Config_HSI();

  //testBlinky();
  testPWM1();
  // testTimer();

  //testUpdateDs();
  //testButton2();

  while (1)
    ;
}

void SystemClock_Config(void) {
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2)
    ;

  LL_RCC_HSE_Enable();

  while (LL_RCC_HSE_IsReady() != 1)
    ;

  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);
  LL_RCC_PLL_Enable();

  while (LL_RCC_PLL_IsReady() != 1)
    ;

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  // LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_8);
  // LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_16);

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
    ;

  SystemCoreClockUpdate();
  SysTick_Init();
}


void SystemClock_Config_HSI(void)
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

  // LL_Init1msTick(64000000);

  // LL_SetSystemCoreClock(64000000);

  SystemCoreClockUpdate();
  SysTick_Init();
  LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSRC_PCLK2_DIV_8);
}

void Error_Handler(void) {
  __disable_irq();
  while (1) {
  }
}
