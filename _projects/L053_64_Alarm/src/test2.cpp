#include "config.h"
#include "stm32l0xx_ll_rcc.h"

#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_rtc.h"
#include "stm32l0xx_ll_system.h"
#include "buzzer.h"
#include "systick.h"
#include <stdio.h>


// 0.3 uA with Ultra low power
static void StandBy() {

  LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);

  CLEAR_BIT(PWR->CR, PWR_CR_PVDE);  // the default

//#define LL_PWR_REGU_VOLTAGE_SCALE1         (PWR_CR_VOS_0)                   /*!< 1.8V (range 1) */
//#define LL_PWR_REGU_VOLTAGE_SCALE2         (PWR_CR_VOS_1)                   /*!< 1.5V (range 2) */
//#define LL_PWR_REGU_VOLTAGE_SCALE3         (PWR_CR_VOS_0 | PWR_CR_VOS_1)    /*!< 1.2V (range 3) */
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE3);

  // Set SLEEPDEEP bit of Cortex System Control Register
  // LL_LPM_EnableDeepSleep(); // same as below
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
  
  LL_PWR_EnableUltraLowPower();

  __WFI();
}

// 44uA  L053R8T6 64-pin
static void lowPowerRun() {

  // POR / PVD : PVD

  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
    ;

  // LL_PWR_REGU_VOLTAGE_SCALE1 : 1.8 V
  // LL_PWR_REGU_VOLTAGE_SCALE2 : 1.5 V
  // LL_PWR_REGU_VOLTAGE_SCALE3 : 1.2 V
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE3);

  while (LL_PWR_IsActiveFlag_VOS() != 0)
    ;

  LL_RCC_MSI_Enable();

  while(LL_RCC_MSI_IsReady() != 1)
    ;

//#define LL_RCC_MSIRANGE_0                  RCC_ICSCR_MSIRANGE_0  /*!< MSI = 65.536 KHz */
//#define LL_RCC_MSIRANGE_1                  RCC_ICSCR_MSIRANGE_1  /*!< MSI = 131.072 KHz*/
//#define LL_RCC_MSIRANGE_2                  RCC_ICSCR_MSIRANGE_2  /*!< MSI = 262.144 KHz */
//#define LL_RCC_MSIRANGE_3                  RCC_ICSCR_MSIRANGE_3  /*!< MSI = 524.288 KHz */
//#define LL_RCC_MSIRANGE_4                  RCC_ICSCR_MSIRANGE_4  /*!< MSI = 1.048 MHz */
//#define LL_RCC_MSIRANGE_5                  RCC_ICSCR_MSIRANGE_5  /*!< MSI = 2.097 MHz */
//#define LL_RCC_MSIRANGE_6                  RCC_ICSCR_MSIRANGE_6  /*!< MSI = 4.194 MHz */

  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_0);
  LL_RCC_MSI_SetCalibTrimming(0);

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSI);

  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_MSI)
    ;

  SystemCoreClockUpdate();


  /* Low-power deepsleep/Sleep/Low-power run
     When this bit is set, the regulator switches in low-power mode when the bit LPRUN is set

      The LPSDSR bit must be set before the LPRUN bit is set. 
      LPSDSR can be reset only when LPRUN bit=0.
  */
  PWR->CR |= PWR_CR_LPSDSR; 

  // Low-power run mode
  PWR->CR |= PWR_CR_LPRUN;

  // Turn PVD off (is it off by default?) 
  PWR->CR &= ~PWR_CR_PVDE;

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
  
  LL_RTC_SetSynchPrescaler(RTC, 322); 

  LL_RTC_EnableShadowRegBypass(RTC);

  LL_RTC_ClearFlag_ALRA(RTC);
  while (!LL_RTC_IsActiveFlag_ALRAW(RTC))
    ;

  LL_RTC_ALMA_Disable(RTC);
  
  int day = LL_RTC_DATE_GetDay(RTC);
  //LL_RTC_ALMA_SetMinute (RTC, 1);
  LL_RTC_ALMA_SetHour (RTC, 0x12);
  //LL_RTC_ALMA_SetDay(RTC, 1);
  //LL_RTC_ALMA_SetSecond (RTC, __LL_RTC_CONVERT_BIN2BCD(30));

  //LL_RTC_ALMA_SetMask(RTC, LL_RTC_ALMA_MASK_MINUTES);
  //LL_RTC_ALMA_SetMask(RTC, LL_RTC_ALMA_MASK_HOURS);
  //LL_RTC_ALMA_SetMask(RTC, LL_RTC_ALMA_MASK_SECONDS);

  //RTC->ALRMAR = RTC_ALRMAR_MSK4 | RTC_ALRMAR_MSK3 | RTC_ALRMAR_MSK2 | RTC_ALRMAR_MSK1;
  
  LL_RTC_ALMA_Enable(RTC);
  LL_RTC_EnableIT_ALRA(RTC);
  
  LL_RTC_ExitInitMode(RTC);
  LL_RTC_EnableWriteProtection(RTC);
}

static void run(void) {

  if (LL_PWR_IsActiveFlag_WU()) {
    LL_PWR_ClearFlag_WU();
  }

  // Check standby flag
  if (LL_PWR_IsActiveFlag_SB()) {
    LL_PWR_ClearFlag_SB();
    //CLEAR_BIT(PWR->CSR, PWR_CSR_EWUP1_Msk);
    Buzzer* buzzer = Buzzer::GetInstance();
    buzzer->Init(LPBuzzerTimer::GetInstance());
    buzzer->SetVolume(10);
    buzzer->Beep(2000, 100);
    //buzzer->PlayTune(barbie);
    //buzzer->PlayTune(mars_short);
    buzzer->Stop();
    for (int i = 0; i < 3000; ++i) {
    }
    LL_PWR_DisableWakeUpPin(LL_PWR_WAKEUP_PIN1);
  }
  LL_PWR_ClearFlag_WU();
  LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN1);    // A0
  setupRTCForStandby();
  StandBy();
}


