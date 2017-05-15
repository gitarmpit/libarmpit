#include "systick.h"
#include "_func.h"
#include "_pins.h"
#include "GPIO_Helper.h"

static void systick_handler(void)
{
	static GPIO_PIN* green = GPIO_Helper::GetPin(LED_G_PIN);
    green->Toggle();
}


void systick_test()
{
    uint32_t ms = 1000;

    static volatile uint32_t clock = RCC_GetAHB_CLK();
    static volatile uint32_t count = (uint64_t) clock * (uint64_t) ms / 1000llu;
    SystickEnable(count / 8, true, true, systick_handler);

    while(1)
    	;
}

