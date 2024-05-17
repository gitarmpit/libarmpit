#include "rcc.h"
#include "systick.h"
#include "gpio_cpp.h"
#include "RCC_helper.h"

//toggle LED every second
static void systick_handler(void)
{
    GPIOD::GetInstance()->TogglePin(GPIO_PIN12);
}

static void systick_test()
{
    uint32_t ms = 1000;
    GPIOD::GetInstance()->EnablePeripheralClock(true);
    GPIOD::GetInstance()->SetupGPIO_OutPP(GPIO_PIN12);  

    static volatile uint32_t clock = RCC_GetAHB_CLK();
    static volatile uint32_t count = (uint64_t) clock * (uint64_t) ms / 1000llu;
    SystickEnable(count / 8, true, true, systick_handler);
    while (1)
    {
    }
}


int main()
{
#ifdef STM32F1
    RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
#else
    RCC_EnableHSI_168Mhz();
#endif
    systick_test();
}
