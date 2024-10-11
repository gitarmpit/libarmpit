#include "config.h"
#include "stm32l0xx_ll_rcc.h"

#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_rtc.h"
#include <stdio.h>
#include  "low_power.h"
#include "rtc.h"
#include "beep.h"
#include "gpio.h"

void RunSession();


static void setupRTCAlarm(void) {

  disableAlarm();
  setAlarm();
  enableAlarm();
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
    if (LL_RTC_IsActiveFlag_ALRA(RTC)) {
      beep_alarm();
    }
  }

  standby();
}


static void check_reset(void) {

  if (LL_RCC_IsActiveFlag_PINRST()) {

    LL_RCC_ClearResetFlags();
    LL_PWR_ClearFlag_WU();
    LL_PWR_ClearFlag_SB();

    if (LL_RCC_IsEnabledRTC()) {
      LL_PWR_EnableBkUpAccess();
      int cnt = RTC->BKP0R;
      if (cnt == 2) {
        RTC->BKP0R = 0;
        disableAlarm();
        RunSession();
        enableAlarm();
      } 
      else {
        ++cnt;
        RTC->BKP0R = cnt;
      }
    }
  }
}


int main(void) {
  System_Config();

  lowPowerRun(LL_RCC_MSIRANGE_0);

  beep_init();

  check_reset();
  run();

  while(1)
    ;
}


