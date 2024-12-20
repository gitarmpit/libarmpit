#include "config.h"
#include "stm32l0xx_ll_rcc.h"

#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_system.h"
#include <stdio.h>

bool testReceiveTime();

// 54uA  L053C8T6 48-pin
static void lowPowerRun(uint32_t range) {

  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
    ;

  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1); // 1.2V

  while (LL_PWR_IsActiveFlag_VOS() != 0)
    ;

  LL_RCC_MSI_Enable();

  while(LL_RCC_MSI_IsReady() != 1)
    ;

  LL_RCC_MSI_SetRange(range); // 2% errors with 1200 baud

  //LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_1); // 0% errors with 2400 baud

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


int main(void) {
  System_Config();

  //SystemClock_Config_MSI();
  //SystemClock_Config_HSI_32();
  SystemCoreClockUpdate();
  lowPowerRun(LL_RCC_MSIRANGE_0);

  testReceiveTime();

  while(1)
    ;
}


