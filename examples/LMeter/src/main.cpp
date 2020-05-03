#include "RCC_Helper.h"
#include "GPIO_Helper.h"
#include "gpio_cpp.h"
#include <stdio.h>
#include "9341/Adafruit_ILI9341.h"
#include "debug.h"
#include "system_time.h"
#include "Fonts2/7seg_4.h"
#include "Fonts2/consola18.h"
//#include "rote-gefahr-320x222.cpp"
#include "himalaya60.h"
#include <stdio.h>      /* printf */
#include <stdarg.h>
#include "Fonts2/FreeMono18pt7b.h"

#if 0
static void speed_test(Adafruit_ILI9341* display)
{
    display->setRotation(1);
    display->setBgColor(ILI9341_CYAN);
    display->setTextColor(ILI9341_BLACK);

    systick_enable(TRUE);
    uint32_t t0 = millis();
    for (int i = 0; i < 10; ++i)
    {
        display->fillScreen(ILI9341_CYAN);
    }
    volatile uint32_t d = millis() - t0;
    ++d;

    display->printf (0, 0, 4, "millis: %d\n", millis() - t0);
    while(1)
        ;

}
static void speed_test2(Adafruit_ILI9341* display)
{
    systick_enable(TRUE);
    display->setRotation(1);
    display->setBgColor(ILI9341_CYAN);
    display->setTextColor(ILI9341_BLACK);
    display->setTextSize(4);

    float f = 6.3;
    while (1)
    {
        uint32_t t0 = millis();
        display->printf (1, 0, 4, "test1 %7.4f", f);
        display->printf (" %7.4f", f);
        display->printf (1, 1, 4, "test2 %7.4f", f);
        display->printf (" %7.4f", f);
        display->printf (1, 2, 4, "test3 %7.4f", f);
        display->printf (" %7.4f", f);
        display->printf (1, 3, 4, "test4 %7.4f", f);
        display->printf (" %7.4f", f);
        display->printf (1, 4, 4, "test5 %7.4f", f);
        display->printf (" %7.4f", f);
        display->printf (1, 5, 4, "test6 %7.4f", f);
        display->printf (" %7.4f", f);
        display->printf (1, 6, 4, "test7 %7.4f", f);
        display->printf (" %7.4f", f);
        display->printf (1, 7, 4, "test8 %7.4f", f);
        display->printf (" %7.4f", f);
        display->printf (1, 8, 4, "test9 %7.4f", f);
        display->printf (" %7.4f", f);
        f += 0.01;
        volatile uint32_t d = millis() - t0;
        ++d; //138 ms with fast SPI and O2
        delay(300);
    }

}

static void speed_test3(Adafruit_ILI9341* display)
{
    display->setRotation(1);
    while(1)
    {
        display->fillScreen(ILI9341_CYAN);
        delay(500);
        display->fillScreen(ILI9341_BLACK);
        delay(500);
    }


}
static void speed_test4(Adafruit_ILI9341* display)
{
    systick_enable(TRUE);
    display->setRotation(1);
    display->setBgColor(ILI9341_CYAN);
    display->setTextColor(ILI9341_BLACK);
    display->setTextSize(4);

    uint32_t t0 = millis();

    for (int i = 0; i < 10; ++i)
    {
        display->setCursor(0, 0);
        display->write("ABCDEFGH1234");
    }

    volatile uint32_t t = millis() - t0;

    ++t;


    while(1)
        ;

}

#endif


// default
void testDefaultFont()
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

    display.fillScreen(ILI9341_BLACK);
    //display.clearScreen();

    //speed_test4(&display);

    //display.display();
    //while(1);

   //  display.setBgColor(ILI9341_BLACK);
    display.setTextColor(ILI9341_WHITE);
    display.setRotation(1);
    // display.setTextSize(7);
    display.setCursor(4, 3);
    display.write(L"11:46");
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
    display.setRotation(1);
    // display.setBgColor(ILI9341_BLACK);
    display.setTextColor(ILI9341_WHITE);
    display.fillScreen(ILI9341_BLACK);

    // display.setFont(&DSEG7ModernMini_BoldItalic_ttf);
    //display.setFont(&consola_ttf);
    display.setFont(&FreeMono18pt7b);
    //display.setCursor(0, 0);
    display.printf(1,1, "test3: %d", 123);
    display.printf(1,2, "ABC: %d", 012345);

    while(1)
    	;

    //display.write (L"11:32");
    //display.drawBmp(0, 0, rote_gefahr_320x222);

    //display.printf(0, 0, "Test, TEST: %d", 123);


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
