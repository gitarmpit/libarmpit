#include "RCC_Helper.h"
#include "GPIO_Helper.h"
#include "gpio_cpp.h"
#include <stdio.h>
#include "9341/Adafruit_ILI9341.h"
#include "debug.h"
#include "system_time.h"
#include "systick.h"
#include "rote-gefahr-16.cpp"
#include "consola12.h"

SPI* spi;
GPIO_PIN *rstPin, *dcPin, *ssPin;

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

static void initGPIO()
{
    spi = GPIO_Helper::SetupSPI(SPI1_PA_5_6_7, true, false, false, SPI_BAUD_RATE_2);

    rstPin = GPIO_Helper::GetPin("C4");
    rstPin->SetupGPIO_OutPP();
    rstPin->SetSpeedHigh();

    dcPin = GPIO_Helper::GetPin("C5");
    dcPin->SetupGPIO_OutPP();
    dcPin->SetSpeedHigh();

    ssPin = GPIO_Helper::GetPin("A4");
    ssPin->SetupGPIO_OutPP();
    ssPin->SetSpeedHigh();

}


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
        display->printf (1, 0, "test1 %7.4f", f);
        display->printf (" %7.4f", f);
        display->printf (1, 1, "test2 %7.4f", f);
        display->printf (" %7.4f", f);
        display->printf (1, 2, "test3 %7.4f", f);
        display->printf (" %7.4f", f);
        display->printf (1, 3, "test4 %7.4f", f);
        display->printf (" %7.4f", f);
        display->printf (1, 4, "test5 %7.4f", f);
        display->printf (" %7.4f", f);
        display->printf (1, 5, "test6 %7.4f", f);
        display->printf (" %7.4f", f);
        display->printf (1, 6, "test7 %7.4f", f);
        display->printf (" %7.4f", f);
        display->printf (1, 7, "test8 %7.4f", f);
        display->printf (" %7.4f", f);
        display->printf (1, 8, "test9 %7.4f", f);
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
        display->write(L"ABCDEFGH1234");
    }

    volatile uint32_t t = millis() - t0;

    ++t;


    while(1)
        ;

}

static void test_pushColor() {

	delay(100);
    initGPIO();

    Adafruit_ILI9341 lcd(spi, dcPin, rstPin, ssPin);
    lcd.setRotation(1);

    lcd.pushColor(0x1234);
    systick_enable(TRUE);
    uint32_t t0 = millis();

    for (int x = 0; x < 320; ++x)
    {
    	for (int y = 0; y < 240; ++y)
    		lcd.drawPixel(x, y, ILI9341_GREENYELLOW);
    }

    volatile uint32_t d = millis() - t0;
    ++d;

    lcd.setTextColor(ILI9341_BLACK);
    lcd.setFont(&consola);
    lcd.printf (0, 3, "millis2: %d\n", millis() - t0);

    while(1)
        ;

}


static void test_font() {

	delay(100);
    initGPIO();

    Adafruit_ILI9341 lcd(spi, dcPin, rstPin, ssPin);
    lcd.setRotation(1);

    lcd.fillScreen(ILI9341_BLUE);
    lcd.setBgColor(ILI9341_BLUE);
    lcd.setTextColor(ILI9341_WHITE);

    //lcd.setTextColor(COLOR565(255, 255, 255), 0);

    lcd.setFont(&consola);
    lcd.printf(0, 0, "consola12: TEST test 123");
    lcd.setCursor(4, 3);
    //lcd.setTextSize(3);
    lcd.write(L"test");

    while(1)
        ;

}

static void test2()
{
    delay(10);
    initGPIO();

    Adafruit_ILI9341 lcd(spi, dcPin, rstPin, ssPin);

    lcd.fillScreen(ILI9341_BLACK);
    lcd.setBgColor(ILI9341_BLACK);
    lcd.setTextColor(ILI9341_WHITE);
    lcd.setRotation(1);
    lcd.setTextSize(7);
    lcd.setCursor(4, 3);
    lcd.write(L"11:45");
    while(1);
}

static void test()
{
    delay(100);

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

    display.setBgColor(ILI9341_BLACK);
    display.setTextColor(ILI9341_WHITE);
    display.setRotation(1);
    display.setTextSize(7);
    display.setCursor(4, 3);
    display.write(L"11:45");
    while(1);

    //display.setTextColor(ILI9340_BLACK, ILI9340_WHITE);

    //display.invertDisplay(true);
    //display.drawFastHLine(0, 0, 100, 10);

    display.setCursor(0, 0);

    //display.write("Hello ILI9341 world!");


    display.printf (0, 0, 4, "Test %f\n", 1.2345);
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
    test_pushColor();
}
