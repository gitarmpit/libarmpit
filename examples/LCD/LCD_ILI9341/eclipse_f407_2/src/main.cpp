#include "RCC_Helper.h"
#include "GPIO_Helper.h"
#include "gpio_cpp.h"
#include <stdio.h>
#include "Adafruit_ILI9341.h"
#include "debug.h"
#include "system_time.h"
#include "fonts/FreeMono24pt7b.h"
#include "fonts/FreeMonoBold18pt7b.h"
#include "fonts/FreeSans18pt7b.h"
#include "fonts/FreeMonoBoldOblique18pt7b.h"  //italic
#include "fonts/Org_01.h"
#include "fonts/FreeSerif24pt7b.h"
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

    //display.fillScreen(ILI9341_CYAN);
    //display.clearScreen();
    //display.display();
    //while(1);

    //display.setBgColor();
    display.setTextColor(ILI9341_BLACK, ILI9341_RED);
    display.setRotation(1);
    display.setTextSize(1);
    display.fillScreen(ILI9341_RED);

    //display.setTextColor(ILI9340_BLACK, ILI9340_WHITE);

    //display.invertDisplay(true);
    //display.drawFastHLine(0, 0, 100, 10);

    //display.setCursor(0, 0);

    //display.write("Hello ILI9341 world!");


    //display.write("ABCabc-12345");
    display.setCursor(0, 24);
    //display.setFont(&FreeSans18pt7b);
    //display.setFont(&FreeSerif24pt7b);
    display.setFont(&FreeMonoBold18pt7b);
    display.write("ABCabc-123.O");
    //display.drawChar(0, 30, 'A', ILI9341_RED, 0, 1);

    //display.display();


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
