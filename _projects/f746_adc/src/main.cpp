#include "config.h"
#include "gpio.h"
#include <stdio.h>
#include "adc.h"

GPIO_PIN* led;


int main(void) {
  System_Config();
  BOOL enableBypass = FALSE;
  SystemClock_Config_HSE(enableBypass);

  while (1) {
  }
}
