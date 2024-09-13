#include "config.h"
#include "stm32l0xx_ll_bus.h"
#include "stm32l0xx_ll_gpio.h"
#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_rcc.h"
#include "stm32l0xx_ll_system.h"
#include "systick.h"


void System_Config(void) 
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriority(SysTick_IRQn, 3);

  // Select which pad is connected to VREFINT_ADC
  // LL_SYSCFG_VREFINT_SetConnection(LL_SYSCFG_VREFINT_CONNECT_IO1);

}

void System_EnableRTC(void) 
{
  LL_RCC_LSI_Enable();

  while(LL_RCC_LSI_IsReady() != 1)
    ;

  LL_PWR_EnableBkUpAccess();
  if(LL_RCC_GetRTCClockSource() != LL_RCC_RTC_CLKSOURCE_LSI)
  {
    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();
    LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
  }

  LL_RCC_EnableRTC();
}

void SystemClock_Config_MSI(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
    ;

  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);

  while (LL_PWR_IsActiveFlag_VOS() != 0)
    ;

  LL_RCC_MSI_Enable();

  while(LL_RCC_MSI_IsReady() != 1)
    ;

  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_5);
  LL_RCC_MSI_SetCalibTrimming(0);

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSI);

  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_MSI)
    ;

  SystemCoreClockUpdate();
  //SysTick_Init();

}
