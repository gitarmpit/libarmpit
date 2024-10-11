#include "stm32l0xx_ll_rcc.h"

#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_system.h"
#include <stdio.h>

// 0.3 uA with Ultra low power
// 1.8 uA without Ultra low power
void standby(void) {

  LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);
  CLEAR_BIT(PWR->CR, PWR_CR_PVDE);  // the default
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE3);
  while (LL_PWR_IsActiveFlag_VOS() != 0)
    ;
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
  LL_PWR_EnableUltraLowPower();
  __WFI();
}

// 54uA  L053C8T6 48-pin
void lowPowerRun(uint32_t range) {

  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
    ;

  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE3); // 1.2V

  while (LL_PWR_IsActiveFlag_VOS() != 0)
    ;

  LL_RCC_MSI_Enable();

  while(LL_RCC_MSI_IsReady() != 1)
    ;

  LL_RCC_MSI_SetRange(range); // 64 KHz
  LL_RCC_MSI_SetCalibTrimming(0);

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSI);

  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_MSI)
    ;


  PWR->CR |= PWR_CR_LPSDSR;  // Low-power deepsleep/Sleep/Low-power run
  PWR->CR |= PWR_CR_LPRUN; // Low-power run mode
  PWR->CR &= ~PWR_CR_PVDE;   // Turn PVD off (is it off by default?) 

  SystemCoreClockUpdate();
}

