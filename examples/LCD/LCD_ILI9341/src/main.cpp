#include "RCC_Helper.h"
#include "GPIO_Helper.h"
#include "gpio_cpp.h"
#include <stdio.h>
#include "9341/Adafruit_ILI9340.h"
#include "debug.h"

void test()
{
    delay(10);
    GPIOA* portA = GPIOA::GetInstance();
    GPIOB* portB = GPIOB::GetInstance();
    portA->EnablePeripheralClock(true);
    portB->EnablePeripheralClock(true);

    SPI* spi = GPIO_Helper::SetupSPI(SPI2_PB_13_14_15, true, false, false, SPI_BAUD_RATE_2);

    GPIO_PIN* rstPin = portB->GetPin(GPIO_PIN0);
    rstPin->SetupGPIO_OutPP();
    rstPin->SetSpeedHigh();

    GPIO_PIN* dcPin = portB->GetPin(GPIO_PIN1);
    dcPin->SetupGPIO_OutPP();
    dcPin->SetSpeedHigh();

    GPIO_PIN* ssPin = portA->GetPin(GPIO_PIN4);
    ssPin->SetupGPIO_OutPP();
    ssPin->SetSpeedHigh();

    Adafruit_ILI9340 display(spi, dcPin, rstPin, ssPin);
    display.fillScreen(ILI9340_RED);
    //display.setTextColor(ILI9340_WHITE);
    display.setRotation(1);
    display.setTextSize(2);
    //display.setTextColor(ILI9340_BLACK, ILI9340_WHITE);

    //display.invertDisplay(true);
    //display.drawFastHLine(0, 0, 100, 10);

    while(1)
    {
        display.setCursor(0, 0);
        display.write("Hello ILI9341 world!");
        display.write("test", 0, 0, 2);
        //display.drawString("test", 0, 0, 4);
        //display.drawString("test", 0, 0, 6);
        //display.drawString("test", 0, 0, 7);

    }

}

int main()
{
#ifdef STM32F1
    RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
#elif defined (STM32F4)
    RCC_EnableHSI_100Mhz();
#endif

    test();

}
