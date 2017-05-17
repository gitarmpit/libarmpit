#include "RCC_Helper.h"
#include "GPIO_Helper.h"
#include "gpio_cpp.h"
#include <stdio.h>
#include "9341/Adafruit_ILI9341.h"
#include "debug.h"
#include "system_time.h"

static void speed_test(Adafruit_ILI9341* display)
{
    systick_enable(TRUE);
    uint32_t t0 = millis();
    for (int i = 0; i < 16; ++i)
    {
        display->fillScreen(ILI9341_CYAN);
    }
    display->printf (0, 0, 4, "millis: %d\n", millis() - t0);
    while(1)
        ;

}

void test()
{
    delay(10);

    SPI* spi = GPIO_Helper::SetupSPI(SPI1_PA_5_6_7, true, false, false, SPI_BAUD_RATE_2);

    GPIO_PIN* rstPin = GPIO_Helper::GetPin("C4");
    rstPin->SetupGPIO_OutPP();
    rstPin->SetSpeedHigh();

    GPIO_PIN* dcPin = GPIO_Helper::GetPin("C5");
    dcPin->SetupGPIO_OutPP();
    dcPin->SetSpeedHigh();

    GPIO_PIN* ssPin = GPIO_Helper::GetPin("A4");
    ssPin->SetupGPIO_OutPP();
    ssPin->SetSpeedHigh();

    Adafruit_ILI9341 display(spi, dcPin, rstPin, ssPin);


    display.fillScreen(ILI9341_CYAN);
    display.display();
    display.setBgColor(ILI9341_CYAN);
    display.setTextColor(ILI9341_BLACK);
    display.setRotation(1);
    display.setTextSize(2);
    //display.setTextColor(ILI9340_BLACK, ILI9340_WHITE);

    //display.invertDisplay(true);
    //display.drawFastHLine(0, 0, 100, 10);

    display.setCursor(0, 0);

    //display.write("Hello ILI9341 world!");


    display.printf (0, 0, 4, "Test %f\n", 1.2345);
    display.display();


        //display.write("test", 0, 0, 2);
        //display.drawString("test", 0, 0, 4);
        //display.drawString("test", 0, 0, 6);
        //display.drawString("test", 0, 0, 7);

     while(1)
    	 ;

}

int main()
{
#ifdef STM32F1
    RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
#elif defined (STM32F4)
    RCC_EnableHSI_168Mhz();
#endif
    Debug_EnableCYCCNT (TRUE);
    test();

}
