#include "gpio_helper.h"
#include "stm32f7xx_ll_gpio.h"
#include "system_init.h"
#include "systick.h"
#include "timer_helper.h"
#include <stdio.h>

#define OUT_PIN_Pin LL_GPIO_PIN_8
#define OUT_PIN_GPIO_Port GPIOB
#define IN_PIN_Pin LL_GPIO_PIN_9
#define IN_PIN_GPIO_Port GPIOB

GPIO_PIN* led;
static uint32_t cnt1;
static uint32_t cnt2;
static void Timer_Callback(void*) {
  //GPIO_TogglePin(led);
  printf ("tim1\n");
  if (cnt1 == 0) {
    //TIM_SetupCounter(TIM2, 1, 0, 0);
    //TIM_SetUpdatePeriod_ns(TIM2, 200);
  }
  else if (cnt1 == 1) {
    TIM_DisableTimer(TIM2);
    printf ("tim1 disabled\n");
  }
  ++cnt1;
}

static void Timer_Callback2(void*) {
    //TIM_DisableTimer(TIM2);
    //GPIO_TogglePin(led);
    ++cnt2;
}

static void testTimer() {
  GPIO_PIN pin1 = GPIO_GetPin("B13");    // LED: B8
  GPIO_Setup_OutPP(&pin1);
  led = &pin1;

  GPIO_SetPin(&pin1);
  
  //TIM_DisableTimer(TIM1);
  //TIM_EnableTimer(TIM1);

  //TIM_SetUpdatePeriod_ns(TIM2, 1000);
  TIM_SetupCounter(TIM1, 1000000, Timer_Callback, NULL);

  TIM_SetupCounter(TIM2, 1, 0, 0);
  TIM_SetUpdatePeriod_ns(TIM2, 500);


  //SysTick_Delay(1000);
  //printf ("%d\n", cnt);

  while (1) {
    SysTick_Delay(1000);
    printf("%d, %llu\n", cnt1, __cnt);
  }
}


int main(void) {
  System_Config();
  BOOL enableBypass = FALSE;
  SystemClock_Config_HSE(enableBypass);
  // SystemClock_Config_HSI();


  testTimer();

  while (1) {
  }
}
