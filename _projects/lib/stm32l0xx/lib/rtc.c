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

  LL_RTC_ClearFlag_ALRB(RTC);
  while (!LL_RTC_IsActiveFlag_ALRBW(RTC))
    ;

  
  LL_RTC_ExitInitMode(RTC);
  LL_RTC_EnableWriteProtection(RTC);
}


void disableAlarmA(void) {

  LL_PWR_EnableBkUpAccess();
  LL_RTC_DisableWriteProtection(RTC);
  LL_RTC_DisableIT_ALRA(RTC);
  LL_RTC_ALMA_Disable(RTC);

  LL_RTC_ClearFlag_ALRA(RTC);
  while (!LL_RTC_IsActiveFlag_ALRAW(RTC))
    ;

  LL_RTC_EnableWriteProtection(RTC);
}

void disableAlarmB(void) {

  LL_PWR_EnableBkUpAccess();
  LL_RTC_DisableWriteProtection(RTC);
  LL_RTC_DisableIT_ALRB(RTC);
  LL_RTC_ALMB_Disable(RTC);

  LL_RTC_ClearFlag_ALRB(RTC);
  while (!LL_RTC_IsActiveFlag_ALRBW(RTC))
    ;

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

  LL_RTC_DisableIT_ALRB(RTC);
  LL_RTC_ALMB_Disable(RTC);

  LL_RTC_ClearFlag_ALRB(RTC);
  while (!LL_RTC_IsActiveFlag_ALRBW(RTC))
    ;


  LL_RTC_EnableWriteProtection(RTC);
}



void enableAlarmA(void) {
  LL_PWR_EnableBkUpAccess();
  LL_RTC_DisableWriteProtection(RTC);
  LL_RTC_ALMA_Enable(RTC);
  LL_RTC_EnableIT_ALRA(RTC);
  LL_RTC_EnableWriteProtection(RTC);
}

void enableAlarmB(void) {
  LL_PWR_EnableBkUpAccess();
  LL_RTC_DisableWriteProtection(RTC);
  LL_RTC_ALMB_Enable(RTC);
  LL_RTC_EnableIT_ALRB(RTC);
  LL_RTC_EnableWriteProtection(RTC);
}


void clearAlarmA(void) {
  LL_PWR_EnableBkUpAccess();
  LL_RTC_DisableWriteProtection(RTC);
  LL_RTC_ClearFlag_ALRA(RTC);
  LL_RTC_EnableWriteProtection(RTC);
}

void clearAlarmB(void) {
  LL_PWR_EnableBkUpAccess();
  LL_RTC_DisableWriteProtection(RTC);
  LL_RTC_ClearFlag_ALRB(RTC);
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


void getAlarmA (STM32_ALARM* t) {

  uint32_t tmpreg = (uint32_t)(RTC->ALRMAR);
  t->hour       = (uint8_t) __LL_RTC_CONVERT_BCD2BIN (((tmpreg & (RTC_ALRMAR_HT  | RTC_ALRMAR_HU))  >> RTC_ALRMAR_HU_Pos));
  t->minute     = (uint8_t) __LL_RTC_CONVERT_BCD2BIN (((tmpreg & (RTC_ALRMAR_MNT | RTC_ALRMAR_MNU)) >> RTC_ALRMAR_MNU_Pos));
  t->second     = (uint8_t) __LL_RTC_CONVERT_BCD2BIN (( tmpreg & (RTC_ALRMAR_ST  | RTC_ALRMAR_SU)));
  t->day        = (uint8_t) __LL_RTC_CONVERT_BCD2BIN (((tmpreg & (RTC_ALRMAR_DT  | RTC_ALRMAR_DU))  >> RTC_ALRMAR_DU_Pos));
  t->isWeekDay  = (uint32_t) (tmpreg & RTC_ALRMAR_WDSEL);

  uint32_t mask = (uint32_t) (tmpreg & LL_RTC_ALMA_MASK_ALL);
  uint32_t mask2 = LL_RTC_ALMA_GetMask(RTC);
  if ((mask & LL_RTC_ALMA_MASK_SECONDS) == LL_RTC_ALMA_MASK_SECONDS) {
    t->second = -1;
  }
  if ((mask & LL_RTC_ALMA_MASK_MINUTES) == LL_RTC_ALMA_MASK_MINUTES) {
    t->minute = -1;
  }
  if ((mask & LL_RTC_ALMA_MASK_HOURS) == LL_RTC_ALMA_MASK_HOURS) {
    t->hour = -1;
  }
  if ((mask & LL_RTC_ALMA_MASK_DATEWEEKDAY) == LL_RTC_ALMA_MASK_DATEWEEKDAY) {
    t->day = -1;
  }

}

void getAlarmB (STM32_ALARM* t) {

  uint32_t tmpreg = (uint32_t)(RTC->ALRMBR);
  t->hour       = (uint8_t) __LL_RTC_CONVERT_BCD2BIN (((tmpreg & (RTC_ALRMBR_HT  | RTC_ALRMBR_HU))  >> RTC_ALRMBR_HU_Pos));
  t->minute     = (uint8_t) __LL_RTC_CONVERT_BCD2BIN (((tmpreg & (RTC_ALRMBR_MNT | RTC_ALRMBR_MNU)) >> RTC_ALRMBR_MNU_Pos));
  t->second     = (uint8_t) __LL_RTC_CONVERT_BCD2BIN (( tmpreg & (RTC_ALRMBR_ST  | RTC_ALRMBR_SU)));
  t->day        = (uint8_t) __LL_RTC_CONVERT_BCD2BIN (((tmpreg & (RTC_ALRMBR_DT  | RTC_ALRMBR_DU))  >> RTC_ALRMBR_DU_Pos));
  t->isWeekDay  = (uint32_t) (tmpreg & RTC_ALRMBR_WDSEL);

  uint32_t mask = (uint32_t) (tmpreg & LL_RTC_ALMB_MASK_ALL);
  uint32_t mask2 = LL_RTC_ALMB_GetMask(RTC);
  if ((mask & LL_RTC_ALMB_MASK_SECONDS) == LL_RTC_ALMB_MASK_SECONDS) {
    t->second = -1;
  }
  if ((mask & LL_RTC_ALMB_MASK_MINUTES) == LL_RTC_ALMB_MASK_MINUTES) {
    t->minute = -1;
  }
  if ((mask & LL_RTC_ALMB_MASK_HOURS) == LL_RTC_ALMB_MASK_HOURS) {
    t->hour = -1;
  }
  if ((mask & LL_RTC_ALMB_MASK_DATEWEEKDAY) == LL_RTC_ALMB_MASK_DATEWEEKDAY) {
    t->day = -1;
  }

}


void setAlarmA (STM32_ALARM* t) {

  LL_PWR_EnableBkUpAccess();
  LL_RTC_DisableWriteProtection(RTC);

  uint32_t wdsel = 0;
  if (t->isWeekDay) {
    wdsel = RTC_ALRMBR_WDSEL;
  }

  uint32_t mask = LL_RTC_ALMA_MASK_ALL;
  if (t->hour != -1) {
    mask &= ~LL_RTC_ALMA_MASK_HOURS;
  }
  if (t->minute != -1) {
    mask &= ~LL_RTC_ALMA_MASK_MINUTES;
  }
  if (t->second != -1) {
    mask &= ~LL_RTC_ALMA_MASK_SECONDS;
  }
  if (t->day != -1) {
    mask &= ~LL_RTC_ALMA_MASK_DATEWEEKDAY;
  }

  uint32_t  tmpreg = 
              (((uint32_t)__LL_RTC_CONVERT_BIN2BCD(t->hour)   << RTC_ALRMAR_HU_Pos)  | \
              ((uint32_t)__LL_RTC_CONVERT_BIN2BCD(t->minute) << RTC_ALRMAR_MNU_Pos) | \
              ((uint32_t)__LL_RTC_CONVERT_BIN2BCD(t->second))                       | \
              ((uint32_t)__LL_RTC_CONVERT_BIN2BCD(t->day)  << RTC_ALRMAR_DU_Pos)  | \
              ((uint32_t)wdsel)                                     | \
              ((uint32_t)mask));

  RTC->ALRMAR = (uint32_t)tmpreg; 

  LL_RTC_EnableWriteProtection(RTC);
}

void setAlarmB (STM32_ALARM* t) {

  LL_PWR_EnableBkUpAccess();
  LL_RTC_DisableWriteProtection(RTC);

  uint32_t wdsel = 0;
  if (t->isWeekDay) {
    wdsel = RTC_ALRMBR_WDSEL;
  }

  uint32_t mask = LL_RTC_ALMB_MASK_ALL;
  if (t->hour != -1) {
    mask &= ~LL_RTC_ALMB_MASK_HOURS;
  }
  if (t->minute != -1) {
    mask &= ~LL_RTC_ALMB_MASK_MINUTES;
  }
  if (t->second != -1) {
    mask &= ~LL_RTC_ALMB_MASK_SECONDS;
  }
  if (t->day != -1) {
    mask &= ~LL_RTC_ALMB_MASK_DATEWEEKDAY;
  }

  uint32_t  tmpreg = 
              (((uint32_t)__LL_RTC_CONVERT_BIN2BCD(t->hour)   << RTC_ALRMBR_HU_Pos)  | \
              ((uint32_t)__LL_RTC_CONVERT_BIN2BCD(t->minute) << RTC_ALRMBR_MNU_Pos) | \
              ((uint32_t)__LL_RTC_CONVERT_BIN2BCD(t->second))                       | \
              ((uint32_t)__LL_RTC_CONVERT_BIN2BCD(t->day)  << RTC_ALRMBR_DU_Pos)  | \
              ((uint32_t)wdsel)                                     | \
              ((uint32_t)mask));

  RTC->ALRMBR = (uint32_t)tmpreg; 

  LL_RTC_EnableWriteProtection(RTC);
}