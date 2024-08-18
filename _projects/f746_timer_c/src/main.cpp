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
static uint32_t cnt;
static uint32_t st;
static void Timer_Callback(void*) {
  //GPIO_TogglePin(led);
  ++cnt;
  if (cnt >= 1000000) {
    st = SysTick_GetTick();
    TIM_DisableTimer(TIM1);
  }
}

static void testTimer() {
  GPIO_PIN pin1 = GPIO_GetPin("B13");    // LED: B8
  GPIO_Setup_OutPP(&pin1);
  led = &pin1;

  GPIO_SetPin(&pin1);
  TIM_SetupCounterTIM1(1, Timer_Callback, NULL);
  TIM_DisableTimer(TIM1);
  TIM_EnableTimer(TIM1);
  //SysTick_Delay(1000);
  //printf ("%d\n", cnt);

  while (1) {
    SysTick_Delay(1000);
    printf("%d\n", st);
  }
}

int main(void) {
  System_Config();
  BOOL enableBypass = FALSE;
  SystemClock_Config_HSE(enableBypass);
  // SystemClock_Config_HSI();

  testTimer();

  while (1) {
    SysTick_Delay(1000);
    printf("%d\n", st);
  }
}
