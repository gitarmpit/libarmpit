#include "RCC_Helper.h"
#include "GPIO_Helper.h"
#include "gpio_cpp.h"
#include <stdio.h>
#include "9163/TFT_ILI9163C.h"
#include "debug.h"
#include "consola12.h"
#include "coure8.h"
#include "systick.h"
//#include "calibril10.h"
#include "lucon9.h"
#include "dos8.h"
//#include "Org_01.h"
#include "cour10.h"

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
static void test()
{
    //delay(10);
	initGPIO();

    TFT_ILI9163C display(spi, dcPin, rstPin, ssPin);
    display.setRotation(2);
    display.fillScreen(0);
    display.setTextColor(COLOR565(255, 255, 255), 0);

    //display.drawBmp(0, 10, consolas_16bit);
    //display.setFont(&tahoma20);
    display.setFont(&coure);
    //display.setFont(&consola);
    //display.setYadvance(50);
    display.printf (0, 0, "2/13/2020");
    display.display();
    while(1)
        ;

    //bat (&display);


    //display.write("Hello ILI9163 F2 world!");
    //display.display(true);
    char buf[8];
    uint32_t i = 0;
    while(1)
    {
        ++i;
        display.printf(0, 0,  "%d", i);
        display.printf(0, 1,  "%d", i);
        display.printf(0, 2,  "%d", i);
        display.printf(0, 3,  "%d", i);
        display.printf(0, 4,  "%d", i);

        display.printf(5, 0,  "%d", i);
        display.printf(5, 1,  "%d", i);
        display.printf(5, 2,  "%d", i);
        display.printf(5, 3,  "%d", i);
        display.printf(5, 4,  "%d", i);

        display.display();
    }
    while(1)
        ;

}


/*
LED      (BACKLIGHT)   3.3v
SCK      (SCLK)      pin 13
SDA      (MOSI)      pin 11
A0      (DC)         pin 9
RESET   (RESET)      pin 8
CS      (CS)         pin 10
GND      (GND)      GND
VCC      (VCC)      5v
*/


#include "adc_cpp.h"
#include "fp.h"

static void bat(TFT_ILI9163C* display)
{
    GPIOA* gpioA = GPIOA::GetInstance();
    gpioA->EnablePeripheralClock(true);
    ADC1* adc1 = ADC1::GetInstance();
    adc1->Enable(true);
    adc1->EnableTempVref(true);
    adc1->AddConversion (ADC_VREFINT, ADC_SMP_SLOWEST);
    volatile fp res1;
    char buf[8];
    const uint32_t adc_vint = 1650; //1650-1670 1.33 1.34
    volatile uint32_t adc_res;

    display->setTextColor(WHITE);
    while (1)
    {
        adc_res = adc1->SingleConversion();
        display->clearScreen();
        display->printf(0, 0,  "%d", adc_res);
        volatile float vref = 3.3 * (float)adc_res / 4095;
        display->printf(0, 1,  "%5.2f", vref);
        display->display();
        delay(500);
    }
}



static void test_flood() {

	initGPIO();

    TFT_ILI9163C lcd(spi, dcPin, rstPin, ssPin);

    systick_cfg();

    int cnt = 0;
    int _tick;
    while (tick < 1000*60) {
        lcd.fillScreen(0xf0f0); //f0f0
        lcd.display();
        //delay(12);
        //delay(6);
        //lcd.fillScreen(0x0f0f); //0f0f
        //lcd.displayFast();
        //delay(12);
        cnt += 2;
    }
    _tick = tick;

    lcd.fillScreen(BLUE);
    //lcd.setRotation(1);
    lcd.setFont(&lucon);
    lcd.printf(1, 1, "Tick: %d, Cnt: %d", _tick, cnt);
    lcd.printf(1, 2, "fps: %7.3f", (float)cnt/60.0);
    lcd.display();

    while(1)
        ;

    //F2 140Mhz: 160fps, 160Mhz: 180fps, 180Mhz: 203fps, 200Mhz: 226fps
}

static void test_font() {

	initGPIO();

    TFT_ILI9163C lcd(spi, dcPin, rstPin, ssPin);

    lcd.fillScreen(BLUE);
    //lcd.setRotation(1);
    lcd.setFont(&dos8);
    lcd.printf(0, 0, "dos8: TEST test 123");
    lcd.setFont(&lucon);
    lcd.printf(0, 1, "lucon: TEST test 123456");
    //lcd.setFont(&Org_01);
    //lcd.printf(0, 2, "Org_01: TEST test 123");  //weird square
    //lcd.setFont(&coure);
    //lcd.printf(0, 2, "coure: TEST test 123");  //cour8 13x8 too big
    lcd.setFont(&cour);
    lcd.printf(0, 3, "cour10: TEST test 123");

    lcd.display();

    while(1)
        ;

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
    //test_flood();
    test_font();
}
