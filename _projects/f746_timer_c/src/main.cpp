#include "gpio.h"
#include "config.h"
#include "systick.h"
#include "timer.h"
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
  TIM_SetupCounter(TIM1, 40, Timer_Callback, NULL);


  while (1) {
    SysTick_Delay(1000);
  }
}

static void testPWM1() {
  
  GPIO_PIN a8 = GPIO_GetPin("A8");
  GPIO_Setup_OutAltPP(&a8);
  GPIO_SetAF(&a8, 1);

  TIM_Channel ch = TIM_SetupPWM(TIM1, 1, 2, 1);
  TIM_UpdatePeriodDs_ns(&ch, 32, 15);
}


static void testPWM3() {
  
  GPIO_PIN b0 = GPIO_GetPin("B0");
  GPIO_Setup_OutAltPP(&b0);
  GPIO_SetAF(&b0, 2);

  TIM_Channel ch = TIM_SetupPWM(TIM3, 3, 400, 200);
}


int main(void) {
  System_Config();
  BOOL enableBypass = FALSE;
  SystemClock_Config_HSE(enableBypass);
  // SystemClock_Config_HSI();


  testPWM1();

  //testTimer();

  while (1) {
  }
}
