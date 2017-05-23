#include "RCC_Helper.h"
#include "GPIO_Helper.h"
#include "gpio_cpp.h"
#include <stdio.h>
#include "9163/TFT_ILI9163C.h"
#include "debug.h"
//#include "fonts/consola12.h"
#include "fonts/consola18.h"
#include "goback1.cpp"
#include "consolas_16bit.cpp"
#include "consola28.h"

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

    display->setFont(&consola_ttf);
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





void test2()
{
    delay(10);
    GPIOA* portA = GPIOA::GetInstance();
    GPIOB* portB = GPIOB::GetInstance();
    portA->EnablePeripheralClock(true);
    portB->EnablePeripheralClock(true);

    SPI* spi = GPIO_Helper::SetupSPI(SPI1_PA_5_6_7, true, false, false, SPI_BAUD_RATE_2);

    GPIO_PIN* rstPin = portB->GetPin(GPIO_PIN1);
    rstPin->SetupGPIO_OutPP();
    rstPin->SetSpeedHigh();

    GPIO_PIN* dcPin = portB->GetPin(GPIO_PIN0);
    dcPin->SetupGPIO_OutPP();
    dcPin->SetSpeedHigh();

    GPIO_PIN* ssPin = portA->GetPin(GPIO_PIN4);
    ssPin->SetupGPIO_OutPP();
    ssPin->SetSpeedHigh();

    TFT_ILI9163C display(spi, dcPin, rstPin, ssPin);
    display.fillScreen(1);
    display.fillScreen(0x8080);
    display.setTextColor(0x2020);
    //display.write("Hello ILI9163 world!");
    display.printf(0, 0, "test");
    display.display(true);
    while(1)
        ;

}


void test()
{
    delay(10);
    GPIOA* portA = GPIOA::GetInstance();
    GPIOB* portB = GPIOB::GetInstance();
    portA->EnablePeripheralClock(true);
    portB->EnablePeripheralClock(true);

    SPI* spi = GPIO_Helper::SetupSPI(SPI1_PA_5_6_7, true, false, false, SPI_BAUD_RATE_2);
    
    GPIO_PIN* rstPin = portB->GetPin(GPIO_PIN2);
    rstPin->SetupGPIO_OutPP();
    rstPin->SetSpeedHigh();

    GPIO_PIN* dcPin = portB->GetPin(GPIO_PIN1);
    dcPin->SetupGPIO_OutPP();
    dcPin->SetSpeedHigh();

    GPIO_PIN* ssPin = portA->GetPin(GPIO_PIN4);
    ssPin->SetupGPIO_OutPP();
    ssPin->SetSpeedHigh();

    TFT_ILI9163C display(spi, dcPin, rstPin, ssPin);
    display.setRotation(3);
    display.fillScreen(0);
    display.setTextColor(COLOR565(255, 255, 255), 0);

    display.drawBmp(0, 10, consolas_16bit);
    display.setFont(&consola28);
    display.setYadvance(50);
    display.printf (0, 1, "123");
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

int main()
{
#if defined(STM32F1)
    RCC_EnableHSI_64Mhz_AHB_32Mhz_APB1_16MHz_APB2_16MHz();
#elif defined(STM32F2)
    RCC_EnableHSI_100Mhz();
#elif defined(STM32F4)
    RCC_EnableHSI_168Mhz();
#endif

    test();

}
