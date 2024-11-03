#include "UART.h"
#include "config.h"
#include "stm32l0xx_ll_rcc.h"

#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_rtc.h"
#include "stm32l0xx_ll_system.h"
#include "buzzer.h"
#include <stdio.h>
#include "stm32l0xx_ll_usart.h"
#include "systick.h"
#include "low_power.h"

void testMCO_LSE(void);
void RTC_Calib(void);
void RTC_Calib2(void);
void testRTCAlarm(void);

// 0.3 uA with Ultra low power
// 1.8 uA without Ultra low power
static void StandBy() {

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
static void lowPowerRun() {

  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
    ;

  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE3); // 1.2V

  while (LL_PWR_IsActiveFlag_VOS() != 0)
    ;

  LL_RCC_MSI_Enable();

  while(LL_RCC_MSI_IsReady() != 1)
    ;

  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_0); // 64 KHz
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

static void initRTC(void) {

  LL_PWR_EnableBkUpAccess();

  LL_RCC_ForceBackupDomainReset();
  LL_RCC_ReleaseBackupDomainReset();

  LL_RCC_LSE_Enable();
  while (LL_RCC_LSE_IsReady() != 1)
     ;

  // LL_RCC_LSE_SetDriveCapability(LL_RCC_LSEDRIVE_HIGH);
  LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);

  LL_RCC_EnableRTC();

  LL_RTC_DisableWriteProtection(RTC);
  LL_RTC_EnterInitMode(RTC);

  LL_RTC_ALMA_Disable(RTC);

  RTC->TR = 0;
  RTC->DR = 0;
  
  //LL_RTC_SetSynchPrescaler(RTC, 322); 

  LL_RTC_EnableShadowRegBypass(RTC);

  LL_RTC_ClearFlag_ALRA(RTC);
  while (!LL_RTC_IsActiveFlag_ALRAW(RTC))
    ;
  
  LL_RTC_ExitInitMode(RTC);
  LL_RTC_EnableWriteProtection(RTC);
}


static void setupRTCAlarm(void) {

  LL_PWR_EnableBkUpAccess();
  LL_RTC_DisableWriteProtection(RTC);

  LL_RTC_DisableIT_ALRA(RTC);
  LL_RTC_ALMA_Disable(RTC);

  LL_RTC_ClearFlag_ALRA(RTC);
  while (!LL_RTC_IsActiveFlag_ALRAW(RTC))
    ;
  
  //LL_RTC_ALMA_EnableWeekday(RTC);

  //LL_RTC_ALMA_SetSecond (RTC, __LL_RTC_CONVERT_BIN2BCD(10));
  //LL_RTC_ALMA_SetMinute (RTC, 1);
  //LL_RTC_ALMA_SetDay(RTC, 1);


  //RTC->ALRMAR = RTC_ALRMAR_MSK4 | RTC_ALRMAR_MSK3 | RTC_ALRMAR_MSK2 | RTC_ALRMAR_MSK1;

  // Run every minute
  //LL_RTC_ALMA_SetSecond (RTC, 0x10);
  //LL_RTC_ALMA_SetMask(RTC, LL_RTC_ALMA_MASK_MINUTES | LL_RTC_ALMA_MASK_HOURS | LL_RTC_ALMA_MASK_DATEWEEKDAY);


  // Run every hour
  LL_RTC_ALMA_SetMinute (RTC, 0x1);
  LL_RTC_ALMA_SetSecond (RTC, 0x1);
  //LL_RTC_ALMA_SetMask(RTC, LL_RTC_ALMA_MASK_SECONDS | LL_RTC_ALMA_MASK_HOURS | LL_RTC_ALMA_MASK_DATEWEEKDAY);
  LL_RTC_ALMA_SetMask(RTC, LL_RTC_ALMA_MASK_HOURS);
  
  // Run every day when hour = 12
  //LL_RTC_ALMA_SetMask(RTC, LL_RTC_ALMA_MASK_SECONDS | LL_RTC_ALMA_MASK_MINUTES | LL_RTC_ALMA_MASK_DATEWEEKDAY);
  //LL_RTC_ALMA_SetHour (RTC, __LL_RTC_CONVERT_BIN2BCD(9));
  
  LL_RTC_ALMA_Enable(RTC);
  LL_RTC_EnableIT_ALRA(RTC);
  
  LL_RTC_EnableWriteProtection(RTC);
}


static void run(void) {

  if (!LL_RCC_IsEnabledRTC()) {
    initRTC();
    setupRTCAlarm();
  }

  if (LL_PWR_IsActiveFlag_WU()) {
    LL_PWR_ClearFlag_WU();
    LL_PWR_EnableBkUpAccess();
    LL_RTC_DisableWriteProtection(RTC);
    LL_RTC_ClearFlag_ALRA(RTC);
  }

  if (LL_PWR_IsActiveFlag_SB()) {
    LL_PWR_ClearFlag_SB();

    Buzzer* buzzer = Buzzer::GetInstance();
    buzzer->Init(LPBuzzerTimer::GetInstance());
    buzzer->SetVolume(10);
    //buzzer->PlayTone(2000, 100);
    buzzer->PlayTune(barbie);
    buzzer->Stop();
  }
  StandBy();
}

static void testTX(int baudRate) {
  LL_USART_Disable(USART1);
  GPIO_PIN tx = GPIO_GetPin("A9");
  GPIO_Setup_OutAltPP(&tx, 4);

  // A10
  GPIO_PIN rx = GPIO_GetPin("A10");
  GPIO_Setup_OutAltPP(&rx, 4);

  UART_Comm uart(USART1);
  uart.init(baudRate);
  uint8_t b = 0x4f;
  while(1) {
    uart.sendByte(b++);
    delay_ms(1000);
  }
}

static void check_reset() {

  if (LL_RCC_IsActiveFlag_PINRST()) {
    LL_RCC_ClearResetFlags();
    LL_PWR_ClearFlag_WU();
    LL_PWR_ClearFlag_SB();
    if (LL_RCC_IsEnabledRTC()) {
      Buzzer* buzzer = Buzzer::GetInstance();
      buzzer->Init(LPBuzzerTimer::GetInstance());
      buzzer->SetVolume(10);
      buzzer->Beep(1000, 100);
      buzzer->Stop();
    }
  }

}

void testDMA_TX(int baudRate) {
  LL_USART_Disable(USART1);
  GPIO_PIN tx = GPIO_GetPin("A9");
  GPIO_Setup_OutAltPP(&tx, 4);

  // A10
  GPIO_PIN rx = GPIO_GetPin("A10");
  GPIO_Setup_OutAltPP(&rx, 4);

  UART_Comm uart(USART1);
  uart.init(baudRate);
  const static int n = 1400;
  uint8_t buf[n];
  for (int i = 0; i < n; ++i) {
    buf[i] = 11;
  }
  printf ("start\n");
  uart.startDMATX(buf, sizeof buf, true);
  printf ("done 1\n");

  for (int i = 0; i < n; ++i) {
    buf[i] = 12;
  }

  printf ("start\n");
  uart.startDMATX(buf, sizeof buf, true);
  printf ("done 2\n");

  while(1)
    ;

}



int main(void) {
  System_Config();
  lowPowerRun();
 
  //testTX(1200);
  //testDMA_TX(1200);
  run();

  while(1)
    ;
}


