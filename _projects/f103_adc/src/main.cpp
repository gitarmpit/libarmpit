#include <stdio.h>
#include "config.h"
#include "gpio.h"
#include "stm32f1xx_ll_adc.h"

static GPIO_PIN* led;



int main(void) {

  System_Config();
  SystemClock_Config_HSE();

  uint32_t ch = __LL_ADC_DECIMAL_NB_TO_CHANNEL(1);
  uint32_t ch2 = LL_ADC_CHANNEL_1; 

  while (1)
    ;
}
