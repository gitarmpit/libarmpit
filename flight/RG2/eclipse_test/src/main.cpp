#include "RCC_helper.h"
#include "_pins.h"
#include "GPIO_Helper.h"
#include "9163/TFT_ILI9163C.h"
#include "debug.h"
#include "system_time.h"
#include "i2c_cpp.h"
#include "LED_Indicators.h"
#include "adc_cpp.h"
#include "timer_cpp.h"
#include "buzzer.h"
#include "i2c_cpp.h"
#include "flash_cfg.h"
#include "_func.h"
#include "ee_printf.h"


const uint32_t d[] =
{ 0xdeadbeef, 0xbabebeda };



int main()
{
    RCC_EnableHSE_168Mhz();
    RCC_EnableHSI(false);


    while(1)
        ;
}


