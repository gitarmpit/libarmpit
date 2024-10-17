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
void testRX();
void testTX(int baudRate);
void testDMA_TX(int baudRate);




static void run(void) {

  if (!LL_RCC_IsEnabledRTC()) {
    initRTC();
  }

  if (LL_PWR_IsActiveFlag_SB() && LL_PWR_IsActiveFlag_WU()) {
    LL_PWR_ClearFlag_WU();
    LL_PWR_ClearFlag_SB();
    if (LL_RTC_IsActiveFlag_ALRA(RTC)) {
      clearAlarmA();
      beep_alarmA();
    } 
    else if (LL_RTC_IsActiveFlag_ALRB(RTC)) {
      clearAlarmB();
      beep_alarmB();
    } 
    else  {
      LL_PWR_DisableWakeUpPin(LL_PWR_WAKEUP_PIN1);    // A0
      disableAlarmA();
      disableAlarmB();
      beep_handshake_ok();
      delay_ms(1000);
      RunSession();
      enableAlarmA();
      enableAlarmB();
    }
  }

  LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN1);    // A0
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
        disableAlarmA();
        disableAlarmB();
        RunSession();
        enableAlarmA();
        enableAlarmB();
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
  LL_PWR_DisableWakeUpPin(LL_PWR_WAKEUP_PIN1);    // A0
  GPIO_PIN p = GPIO_GetPin("A0");
  GPIO_Setup_In_Pulldown(&p);

  lowPowerRun(LL_RCC_MSIRANGE_0);
  // testDMA_TX(1200);
  testTX(1200);

  beep_init();
  run();

  while(1)
    ;
}


