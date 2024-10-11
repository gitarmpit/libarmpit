#include "stm32l0xx_ll_rcc.h"

#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_rtc.h"
#include "rtc.h"

void initRTC(void) {

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


void disableAlarm(void) {

  LL_PWR_EnableBkUpAccess();
  LL_RTC_DisableWriteProtection(RTC);
  LL_RTC_DisableIT_ALRA(RTC);
  LL_RTC_ALMA_Disable(RTC);

  LL_RTC_ClearFlag_ALRA(RTC);
  while (!LL_RTC_IsActiveFlag_ALRAW(RTC))
    ;

  LL_RTC_EnableWriteProtection(RTC);
}

void enableAlarm(void) {
  LL_PWR_EnableBkUpAccess();
  LL_RTC_DisableWriteProtection(RTC);
  LL_RTC_ALMA_Enable(RTC);
  LL_RTC_EnableIT_ALRA(RTC);
  LL_RTC_EnableWriteProtection(RTC);
}

void setAlarm(void) {
  LL_PWR_EnableBkUpAccess();
  LL_RTC_DisableWriteProtection(RTC);

  // Run every minute
  //LL_RTC_ALMA_SetSecond (RTC, 0x10);
  //LL_RTC_ALMA_SetMask(RTC, LL_RTC_ALMA_MASK_MINUTES | LL_RTC_ALMA_MASK_HOURS | LL_RTC_ALMA_MASK_DATEWEEKDAY);

  // Run every hour
  LL_RTC_ALMA_SetMinute (RTC, 0x1);
  LL_RTC_ALMA_SetSecond (RTC, 0x1);
  LL_RTC_ALMA_SetMask(RTC, LL_RTC_ALMA_MASK_HOURS | LL_RTC_ALMA_MASK_DATEWEEKDAY);
  
  // Run every day when hour = 12
  //LL_RTC_ALMA_SetMask(RTC, LL_RTC_ALMA_MASK_SECONDS | LL_RTC_ALMA_MASK_MINUTES | LL_RTC_ALMA_MASK_DATEWEEKDAY);
  //LL_RTC_ALMA_SetHour (RTC, __LL_RTC_CONVERT_BIN2BCD(9));

  LL_RTC_EnableWriteProtection(RTC);
}


void setRTCTime (const STM32_TIME* t) {
  LL_PWR_EnableBkUpAccess();
  LL_RTC_DisableWriteProtection(RTC);
  LL_RTC_EnterInitMode(RTC);

  LL_RTC_TIME_Config(RTC, LL_RTC_TIME_FORMAT_PM, 
    __LL_RTC_CONVERT_BIN2BCD(t->hour), 
    __LL_RTC_CONVERT_BIN2BCD(t->minute), 
    __LL_RTC_CONVERT_BIN2BCD(t->second));

  LL_RTC_DATE_Config(RTC, 
    __LL_RTC_CONVERT_BIN2BCD(t->dow), 
    __LL_RTC_CONVERT_BIN2BCD(t->day), 
    __LL_RTC_CONVERT_BIN2BCD(t->month), 
    __LL_RTC_CONVERT_BIN2BCD(t->year - 2000));

  LL_RTC_ExitInitMode(RTC);
  LL_RTC_EnableWriteProtection(RTC);
}

static uint8_t RTC_Bcd2ToByte(uint8_t number)
{
  uint32_t tens = 0U;
  tens = (((uint32_t)number & 0xF0U) >> 4U) * 10U;
  return (uint8_t)(tens + ((uint32_t)number & 0x0FU));
}


void getRTCTime (STM32_TIME* t) {

  uint32_t tmpreg = (uint32_t)(RTC->TR & RTC_TR_RESERVED_MASK);
  t->hour      = (uint8_t) __LL_RTC_CONVERT_BCD2BIN (((tmpreg & (RTC_TR_HT  | RTC_TR_HU))  >> RTC_TR_HU_Pos));
  t->minute    = (uint8_t) __LL_RTC_CONVERT_BCD2BIN (((tmpreg & (RTC_TR_MNT | RTC_TR_MNU)) >> RTC_TR_MNU_Pos));
  t->second    = (uint8_t) __LL_RTC_CONVERT_BCD2BIN (( tmpreg & (RTC_TR_ST  | RTC_TR_SU)));

  tmpreg = (uint32_t)(RTC->DR & RTC_DR_RESERVED_MASK);

  t->year    = 2000 + (uint8_t) __LL_RTC_CONVERT_BCD2BIN (((tmpreg & (RTC_DR_YT | RTC_DR_YU)) >> RTC_DR_YU_Pos));
  t->month   = (uint8_t) __LL_RTC_CONVERT_BCD2BIN (((tmpreg & (RTC_DR_MT | RTC_DR_MU)) >> RTC_DR_MU_Pos));
  t->day     = (uint8_t) __LL_RTC_CONVERT_BCD2BIN ((tmpreg & (RTC_DR_DT | RTC_DR_DU)));
  t->dow     = (uint8_t) __LL_RTC_CONVERT_BCD2BIN (((tmpreg & (RTC_DR_WDU))            >> RTC_DR_WDU_Pos));

}
