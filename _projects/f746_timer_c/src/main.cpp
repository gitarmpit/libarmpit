#include "gpio_helper.h"
#include "system_init.h"
#include "systick.h"
#include "timer_helper.h"
#include <stdio.h>

GPIO_PIN* led;

static void Timer_Callback(void*) {
  GPIO_TogglePin(led);
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


  while (1) {
    SysTick_Delay(1000);
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
