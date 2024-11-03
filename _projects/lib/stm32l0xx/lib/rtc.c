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


static void getBKP(STM32_ALARM* a, uint8_t alarmNo) {
  uint32_t tmpreg = (alarmNo == 0) ? RTC->BKP0R : RTC->BKP1R;
  a->wkOnly = (tmpreg >> BKP_WKONLY_POS) & BKP_SETTING_MASK; 
  a->period = (tmpreg >> BKP_PERIOD_POS) & BKP_SETTING_MASK;  
  a->skipFirst = (tmpreg >> BKP_SKIP_POS) & BKP_SETTING_MASK; 
  a->alarmTune = (tmpreg >> BKP_TUNE_POS) & BKP_SETTING_MASK; 
}

void getAlarmA (STM32_ALARM* a) {

  uint32_t tmpreg = (uint32_t)(RTC->ALRMAR);
  a->hour       = (uint8_t) __LL_RTC_CONVERT_BCD2BIN (((tmpreg & (RTC_ALRMAR_HT  | RTC_ALRMAR_HU))  >> RTC_ALRMAR_HU_Pos));
  a->minute     = (uint8_t) __LL_RTC_CONVERT_BCD2BIN (((tmpreg & (RTC_ALRMAR_MNT | RTC_ALRMAR_MNU)) >> RTC_ALRMAR_MNU_Pos));
  a->second     = (uint8_t) __LL_RTC_CONVERT_BCD2BIN (( tmpreg & (RTC_ALRMAR_ST  | RTC_ALRMAR_SU)));
  a->day        = (uint8_t) __LL_RTC_CONVERT_BCD2BIN (((tmpreg & (RTC_ALRMAR_DT  | RTC_ALRMAR_DU))  >> RTC_ALRMAR_DU_Pos));
  a->isWeekDay  = (uint32_t) (tmpreg & RTC_ALRMAR_WDSEL);

  uint32_t mask = (uint32_t) (tmpreg & LL_RTC_ALMA_MASK_ALL);
  uint32_t mask2 = LL_RTC_ALMA_GetMask(RTC);
  if ((mask & LL_RTC_ALMA_MASK_SECONDS) == LL_RTC_ALMA_MASK_SECONDS) {
    a->second = -1;
  }
  if ((mask & LL_RTC_ALMA_MASK_MINUTES) == LL_RTC_ALMA_MASK_MINUTES) {
    a->minute = -1;
  }
  if ((mask & LL_RTC_ALMA_MASK_HOURS) == LL_RTC_ALMA_MASK_HOURS) {
    a->hour = -1;
  }
  if ((mask & LL_RTC_ALMA_MASK_DATEWEEKDAY) == LL_RTC_ALMA_MASK_DATEWEEKDAY) {
    a->day = -1;
  }

  getBKP(a, 0);
}

void getAlarmB (STM32_ALARM* a) {

  uint32_t tmpreg = (uint32_t)(RTC->ALRMBR);
  a->hour       = (uint8_t) __LL_RTC_CONVERT_BCD2BIN (((tmpreg & (RTC_ALRMBR_HT  | RTC_ALRMBR_HU))  >> RTC_ALRMBR_HU_Pos));
  a->minute     = (uint8_t) __LL_RTC_CONVERT_BCD2BIN (((tmpreg & (RTC_ALRMBR_MNT | RTC_ALRMBR_MNU)) >> RTC_ALRMBR_MNU_Pos));
  a->second     = (uint8_t) __LL_RTC_CONVERT_BCD2BIN (( tmpreg & (RTC_ALRMBR_ST  | RTC_ALRMBR_SU)));
  a->day        = (uint8_t) __LL_RTC_CONVERT_BCD2BIN (((tmpreg & (RTC_ALRMBR_DT  | RTC_ALRMBR_DU))  >> RTC_ALRMBR_DU_Pos));
  a->isWeekDay  = (uint32_t) (tmpreg & RTC_ALRMBR_WDSEL);

  uint32_t mask = (uint32_t) (tmpreg & LL_RTC_ALMB_MASK_ALL);
  uint32_t mask2 = LL_RTC_ALMB_GetMask(RTC);
  if ((mask & LL_RTC_ALMB_MASK_SECONDS) == LL_RTC_ALMB_MASK_SECONDS) {
    a->second = -1;
  }
  if ((mask & LL_RTC_ALMB_MASK_MINUTES) == LL_RTC_ALMB_MASK_MINUTES) {
    a->minute = -1;
  }
  if ((mask & LL_RTC_ALMB_MASK_HOURS) == LL_RTC_ALMB_MASK_HOURS) {
    a->hour = -1;
  }
  if ((mask & LL_RTC_ALMB_MASK_DATEWEEKDAY) == LL_RTC_ALMB_MASK_DATEWEEKDAY) {
    a->day = -1;
  }

  getBKP(a, 1);

}

static void setBKP(STM32_ALARM* a, uint8_t alarmNo) {

  uint32_t tmpreg = 0;

  tmpreg |= (a->period & BKP_SETTING_MASK) << BKP_PERIOD_POS; 
  tmpreg |= (a->skipFirst & BKP_SETTING_MASK) << BKP_SKIP_POS;  
  tmpreg |= (a->alarmTune & BKP_SETTING_MASK) << BKP_TUNE_POS;       
  tmpreg |= (a->wkOnly & BKP_SETTING_MASK) << BKP_WKONLY_POS;

  if (alarmNo == 0)
    RTC->BKP0R = tmpreg;
  else
    RTC->BKP1R = tmpreg;
}

void setAlarmA (STM32_ALARM* a) {

  LL_PWR_EnableBkUpAccess();
  LL_RTC_DisableWriteProtection(RTC);

  uint32_t wdsel = 0;
  if (a->isWeekDay) {
    wdsel = RTC_ALRMBR_WDSEL;
  }

  uint32_t mask = LL_RTC_ALMA_MASK_ALL;
  if (a->hour != -1) {
    mask &= ~LL_RTC_ALMA_MASK_HOURS;
  }
  if (a->minute != -1) {
    mask &= ~LL_RTC_ALMA_MASK_MINUTES;
  }
  if (a->second != -1) {
    mask &= ~LL_RTC_ALMA_MASK_SECONDS;
  }
  if (a->day != -1) {
    mask &= ~LL_RTC_ALMA_MASK_DATEWEEKDAY;
  }

  uint32_t  tmpreg = 
              (((uint32_t)__LL_RTC_CONVERT_BIN2BCD(a->hour)   << RTC_ALRMAR_HU_Pos)  | \
              ((uint32_t)__LL_RTC_CONVERT_BIN2BCD(a->minute) << RTC_ALRMAR_MNU_Pos) | \
              ((uint32_t)__LL_RTC_CONVERT_BIN2BCD(a->second))                       | \
              ((uint32_t)__LL_RTC_CONVERT_BIN2BCD(a->day)  << RTC_ALRMAR_DU_Pos)  | \
              ((uint32_t)wdsel)                                     | \
              ((uint32_t)mask));

  RTC->ALRMAR = (uint32_t)tmpreg; 

  setBKP(a, 0);

  LL_RTC_EnableWriteProtection(RTC);
}

void setAlarmB (STM32_ALARM* a) {

  LL_PWR_EnableBkUpAccess();
  LL_RTC_DisableWriteProtection(RTC);

  uint32_t wdsel = 0;
  if (a->isWeekDay) {
    wdsel = RTC_ALRMBR_WDSEL;
  }

  uint32_t mask = LL_RTC_ALMB_MASK_ALL;
  if (a->hour != -1) {
    mask &= ~LL_RTC_ALMB_MASK_HOURS;
  }
  if (a->minute != -1) {
    mask &= ~LL_RTC_ALMB_MASK_MINUTES;
  }
  if (a->second != -1) {
    mask &= ~LL_RTC_ALMB_MASK_SECONDS;
  }
  if (a->day != -1) {
    mask &= ~LL_RTC_ALMB_MASK_DATEWEEKDAY;
  }

  uint32_t  tmpreg = 
              (((uint32_t)__LL_RTC_CONVERT_BIN2BCD(a->hour)   << RTC_ALRMBR_HU_Pos)  | \
              ((uint32_t)__LL_RTC_CONVERT_BIN2BCD(a->minute) << RTC_ALRMBR_MNU_Pos) | \
              ((uint32_t)__LL_RTC_CONVERT_BIN2BCD(a->second))                       | \
              ((uint32_t)__LL_RTC_CONVERT_BIN2BCD(a->day)  << RTC_ALRMBR_DU_Pos)  | \
              ((uint32_t)wdsel)                                     | \
              ((uint32_t)mask));

  RTC->ALRMBR = (uint32_t)tmpreg; 

  setBKP(a, 1);

  LL_RTC_EnableWriteProtection(RTC);
}

static uint8_t getBKP_value(int8_t alarmNo, int8_t pos) {
  if (alarmNo == 0) {
    return (RTC->BKP0R >> pos) & BKP_SETTING_MASK; 
  }
  else {
    return (RTC->BKP1R >> pos) & BKP_SETTING_MASK; 
  }
}

static void setBKP_value (int8_t alarmNo, int8_t pos, int8_t value) {
  if (alarmNo == 0) {
    RTC->BKP0R &= ~ (BKP_SETTING_MASK << pos);
    RTC->BKP0R |= (value & BKP_SETTING_MASK) << pos;
  }
  else {
    RTC->BKP1R &= ~ (BKP_SETTING_MASK << pos);
    RTC->BKP1R |= (value & BKP_SETTING_MASK) << pos;
  }
}

static uint8_t getBKP_skip(int8_t alarmNo) {
  return getBKP_value(alarmNo, BKP_SKIP_POS);
}

static uint8_t getBKP_period_cnt(int8_t alarmNo) {
  return getBKP_value(alarmNo, BKP_PERIOD_CNT_POS);
}

static uint8_t getBKP_period(int8_t alarmNo) {
  return getBKP_value(alarmNo, BKP_PERIOD_POS);
}

static void setBKP_skip (int8_t alarmNo, int skip) {
  setBKP_value(alarmNo, BKP_SKIP_POS, skip);
}


static void setBKP_period_cnt (int8_t alarmNo, int skip) {
  setBKP_value(alarmNo, BKP_PERIOD_CNT_POS, skip);
}


BOOL isSkipAlarm (int8_t alarmNo) {

  // Check initial skip
  uint8_t skip = getBKP_skip(alarmNo);
  if (skip > 0) {
    setBKP_skip(alarmNo, --skip);
    return TRUE;
  }


  uint8_t period_cnt = getBKP_period_cnt(alarmNo);
  if (period_cnt > 0) {
    setBKP_period_cnt(alarmNo, --period_cnt);
    return TRUE;
  }

  uint8_t period = getBKP_period(alarmNo);
  if (period > 0) {
    setBKP_period_cnt(alarmNo, period);
  }

  return FALSE;
}

int8_t getAlarmTuneNo(int8_t alarmNo) {
  return getBKP_value(alarmNo, BKP_TUNE_POS);
}


