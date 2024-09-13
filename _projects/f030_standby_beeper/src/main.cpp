#include "config.h"

#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_tim.h"
#include "stm32f0xx_ll_pwr.h"
#include "stm32f0xx_ll_rtc.h"
#include "stm32f0xx_ll_exti.h"
#include "stm32f0xx_ll_system.h"

#include "systick.h"

//#include "stm32f0xx.h"
#include <stdio.h>
#include "gpio.h"
#include "timer_cpp.h"

void SystemClock_Config(void);

static void GPIO_floating(void) {
  LL_GPIO_InitTypeDef gpio_initstruct = {LL_GPIO_PIN_ALL, LL_GPIO_MODE_ANALOG, 
                                         LL_GPIO_SPEED_FREQ_HIGH, LL_GPIO_OUTPUT_PUSHPULL};

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA |
                           LL_AHB1_GRP1_PERIPH_GPIOB);

  LL_GPIO_Init(GPIOA, &gpio_initstruct);
  LL_GPIO_Init(GPIOB, &gpio_initstruct);
  //LL_GPIO_Init(GPIOC, &gpio_initstruct);
  //LL_GPIO_Init(GPIOD, &gpio_initstruct);

  LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOA |
                           LL_AHB1_GRP1_PERIPH_GPIOB);
}


// 1.9 uA:  VDDA_MONITOR is off in option bytes: AA 55 DF 20   (DF)
// 3.0 uA:  VDDA_MONITOR on  (FF)
static void StandBy(void) {

  LL_PWR_ClearFlag_WU();
  LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
  GPIO_floating();  // doesn't make any difference

  __WFI();
}


static void setupRTCForStandby(void) {

  LL_PWR_EnableBkUpAccess();
  LL_RCC_LSI_Enable();
  while (LL_RCC_LSI_IsReady() != 1)
     ;

  LL_RCC_ForceBackupDomainReset();
  LL_RCC_ReleaseBackupDomainReset();
  LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);

  LL_RCC_EnableRTC();

  LL_RTC_DisableWriteProtection(RTC);
  LL_RTC_EnterInitMode(RTC);
  RTC->TR = 0;
  RTC->DR = 0;
  
  LL_RTC_SetSynchPrescaler(RTC, 310); //39810

  LL_RTC_EnableShadowRegBypass(RTC);

  LL_RTC_ClearFlag_ALRA(RTC);
  while (!LL_RTC_IsActiveFlag_ALRAW(RTC))
    ;
  
  //LL_RTC_ALMA_SetMinute (RTC, 1);
  //LL_RTC_ALMA_SetHour (RTC, 1);
  LL_RTC_ALMA_SetSecond (RTC, __LL_RTC_CONVERT_BIN2BCD(3));

  //LL_RTC_ALMA_SetMask(RTC, LL_RTC_ALMA_MASK_MINUTES);
  //LL_RTC_ALMA_SetMask(RTC, LL_RTC_ALMA_MASK_HOURS);
  //LL_RTC_ALMA_SetMask(RTC, LL_RTC_ALMA_MASK_SECONDS);

  //RTC->ALRMAR = RTC_ALRMAR_MSK4 | RTC_ALRMAR_MSK3 | RTC_ALRMAR_MSK2 | RTC_ALRMAR_MSK1;
  
  LL_RTC_ALMA_Enable(RTC);
  LL_RTC_EnableIT_ALRA(RTC);
  
  LL_RTC_ExitInitMode(RTC);
  LL_RTC_EnableWriteProtection(RTC);
}


static void beep(void) {
  GPIO_PIN pin = GPIO_GetPin("A6");
  GPIO_Setup_OutAltPP(&pin);
  GPIO_SetAF(&pin, 1);
  TIMER* t = TIMER3::GetInstance();
  TIM_Channel ch = t->SetupPWM(1, 500, 400);
  SysTick_Delay(30000);
  t->DisableCounter();
}


static void run(void) {
  // Check standby flag
  if (LL_PWR_IsActiveFlag_SB()) {
      LL_PWR_ClearFlag_SB();
      CLEAR_BIT(PWR->CSR, PWR_CSR_EWUP1_Msk);
      beep();
  }
  setupRTCForStandby();
  StandBy();
}


static void SystemClock_Config_HSI(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  }
  LL_RCC_HSI_Enable();

  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  LL_RCC_HSI_SetCalibTrimming(16);

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_16);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {

  }

}


int main(void)
{
  System_Config();
  //SystemClock_Config();
  SystemClock_Config_HSI();
  SystemCoreClockUpdate();
  SysTick_Init();

  run();

  while (1)
    ;
}

