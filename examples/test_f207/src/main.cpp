#include "RCC_Helper.h"
#include "GPIO_Helper.h"
#include "gpio_cpp.h"
#include <stdio.h>
#include "debug.h"
#include "systick.h"

volatile int tick = 0;
static void systick_handler(void) {
    ++tick;
}

static void systick_cfg() {
    uint32_t ms = 1;
    static volatile uint32_t clock = RCC_GetAHB_CLK();
    static volatile uint32_t count = (uint64_t) clock * (uint64_t) ms / 1000llu;
    SystickEnable(count / 8, true, true, systick_handler);
}


SPI* spi;
GPIO_PIN *rstPin, *dcPin, *ssPin;

//F2 board pin header
static void initGPIO()
{
    GPIOA* portA = GPIOA::GetInstance();
    GPIOB* portB = GPIOB::GetInstance();
    portA->EnablePeripheralClock(true);
    portB->EnablePeripheralClock(true);

    spi = GPIO_Helper::SetupSPI(SPI1_PA_5_6_7, true, false, false, SPI_BAUD_RATE_2);

    rstPin = portB->GetPin(GPIO_PIN2);
    rstPin->SetupGPIO_OutPP();
    rstPin->SetSpeedHigh();

    dcPin = portB->GetPin(GPIO_PIN1);
    dcPin->SetupGPIO_OutPP();
    dcPin->SetSpeedHigh();

    ssPin = portA->GetPin(GPIO_PIN4);
    ssPin->SetupGPIO_OutPP();
    ssPin->SetSpeedHigh();

}

int main()
{
#if defined(STM32F1)
    RCC_EnableHSI_64Mhz_AHB_32Mhz_APB1_16MHz_APB2_16MHz();
#elif defined(STM32F2)
    //RCC_EnableHSI_100Mhz();
    FLASH_EnableDCache();
    FLASH_EnableICache();
    FLASH_EnablePrefetchBuffer();

    FLASH_SetWaitState(4);

    RCC_EnableHSI(TRUE);
    uint8_t pllm = 8;
    uint16_t plln = 400;  //200Mhz
    uint16_t pllp = 4;
    RCC_EnablePLL(pllm, plln, pllp);


#elif defined(STM32F4)
    RCC_EnableHSI_168Mhz();
#endif

    Debug_EnableCYCCNT(true);
    while(1)
    	;
}
