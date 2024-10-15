#include "gpio.h"
#include "stm32l0xx_ll_bus.h"
#include "stm32l0xx_ll_exti.h"
#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_rcc.h"
#include "stm32l0xx_ll_rtc.h"
#include "systick.h"
#include <stdio.h>

static void GPIO_floating() {
  LL_GPIO_InitTypeDef gpio_initstruct = {LL_GPIO_PIN_ALL, LL_GPIO_MODE_ANALOG, 
                                         LL_GPIO_SPEED_FREQ_HIGH, LL_GPIO_OUTPUT_PUSHPULL};

  LL_APB2_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA |
                           LL_IOP_GRP1_PERIPH_GPIOB |
                           LL_IOP_GRP1_PERIPH_GPIOC |
                           LL_IOP_GRP1_PERIPH_GPIOD);

  LL_GPIO_Init(GPIOA, &gpio_initstruct);
  LL_GPIO_Init(GPIOB, &gpio_initstruct);
  LL_GPIO_Init(GPIOC, &gpio_initstruct);
  LL_GPIO_Init(GPIOD, &gpio_initstruct);

  LL_APB2_GRP1_DisableClock(LL_IOP_GRP1_PERIPH_GPIOA |
                           LL_IOP_GRP1_PERIPH_GPIOB |
                           LL_IOP_GRP1_PERIPH_GPIOC |
                           LL_IOP_GRP1_PERIPH_GPIOD);
}



void testMCO(void) {

  LL_RCC_LSI_Enable();
  while (LL_RCC_LSI_IsReady() != 1)
     ;

  GPIO_PIN mco = GPIO_GetPin("A8");
  GPIO_Setup_OutAltPP (&mco, 0);

  LL_RCC_ConfigMCO(LL_RCC_MCO1SOURCE_LSI, LL_RCC_MCO1_DIV_1);
}


extern "C" void RTC_IRQHandler(void) {
  static int i; 
  ++i;
  printf ("%d: alarm\n", SysTick_GetTick());
  if (RTC->ISR & RTC_ISR_ALRAF) {
    RTC->ISR &= ~RTC_ISR_ALRAF;
  }
  if (EXTI->PR & EXTI_PR_PR17) {
    EXTI->PR &= EXTI_PR_PR17;
  }
}

void testRTCAlarm(void) {

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

  LL_RTC_SetSynchPrescaler(RTC, 322);

  LL_RTC_EnableShadowRegBypass(RTC);

  LL_RTC_ClearFlag_ALRA(RTC);
  
  LL_RTC_ALMA_SetMinute (RTC, 1);
  
  LL_RTC_ALMA_Enable(RTC);
  LL_RTC_EnableIT_ALRA(RTC);
  
  LL_RTC_ExitInitMode(RTC);


  LL_RTC_EnableWriteProtection(RTC);

  LL_EXTI_EnableIT_0_31 (LL_EXTI_LINE_17);
  
  LL_EXTI_EnableEvent_0_31 (LL_EXTI_LINE_17);
  LL_EXTI_EnableRisingTrig_0_31 (LL_EXTI_LINE_17);

  NVIC_SetPriority(RTC_IRQn, 0x0F);
  NVIC_EnableIRQ(RTC_IRQn);


  while(1) {
    SysTick_Delay(10);
  }
    
}

void setAlarmA(void) {
  LL_PWR_EnableBkUpAccess();
  LL_RTC_DisableWriteProtection(RTC);

  // Run every minute
  // LL_RTC_ALMA_SetSecond (RTC, 0x5);
  // LL_RTC_ALMA_SetMask(RTC, LL_RTC_ALMA_MASK_MINUTES | LL_RTC_ALMA_MASK_HOURS | LL_RTC_ALMA_MASK_DATEWEEKDAY);

  // Run every hour
  LL_RTC_ALMA_SetMinute (RTC, 0x1);
  LL_RTC_ALMA_SetSecond (RTC, 0x10);
  LL_RTC_ALMA_SetMask(RTC, LL_RTC_ALMA_MASK_HOURS | LL_RTC_ALMA_MASK_DATEWEEKDAY);
  
  // Run every day when hour = 12
  //LL_RTC_ALMA_SetMask(RTC, LL_RTC_ALMA_MASK_SECONDS | LL_RTC_ALMA_MASK_MINUTES | LL_RTC_ALMA_MASK_DATEWEEKDAY);
  //LL_RTC_ALMA_SetHour (RTC, __LL_RTC_CONVERT_BIN2BCD(9));

  LL_RTC_EnableWriteProtection(RTC);
}


void RTC_Calib(void) {
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

  //LL_RTC_SetAsynchPrescaler(RTC, 128);
  LL_RTC_SetSynchPrescaler(RTC, 322); //41230

  LL_RTC_EnableShadowRegBypass(RTC);
  
  // Calibration out

  // 1 Hz
  //RTC->CR |= RTC_CR_COSEL | RTC_CR_COE;

  // 512 Hz
  RTC->CR |= RTC_CR_COE;
  
  
  // B14
  //RTC->OR |= RTC_OR_OUT_RMP;

  LL_RTC_ExitInitMode(RTC);
  LL_RTC_EnableWriteProtection(RTC);


  while(1) {
  }
}


static void test_delay(void) {
  GPIO_PIN p = GPIO_GetPin("B12");
  GPIO_Setup_OutPP(&p);
  while (1) {
    GPIO_TogglePin(&p);
    //delay_ms(1);
  }
}


