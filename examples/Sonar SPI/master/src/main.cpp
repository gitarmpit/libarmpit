#include "RCC_Helper.h"
#include "GPIO_Helper.h"
#include "spi.h"
#include "gpio_cpp.h"
#include <stdio.h>
#include "spi_cpp.h"
#include "5110/Adafruit_PCD8544.h"
#include "debug.h"


static void run_spi_master()
{
    delay(10);
    GPIOA* portA = GPIOA::GetInstance();
    GPIOB* portB = GPIOB::GetInstance();
    portA->EnablePeripheralClock(true);
    portB->EnablePeripheralClock(true);

    //LCD
    SPI* spi = GPIO_Helper::SetupSPI(SPI1_PA_5_6_7, true, false, false, SPI_BAUD_RATE_8);

    GPIO_PIN* rstPin = portB->GetPin(GPIO_PIN1);
    rstPin->SetupGPIO_OutPP();
    rstPin->SetSpeedHigh();

    GPIO_PIN* dcPin = portB->GetPin(GPIO_PIN0);
    dcPin->SetupGPIO_OutPP();
    dcPin->SetSpeedHigh();

    GPIO_PIN* ssPin = portA->GetPin(GPIO_PIN4);
    ssPin->SetupGPIO_OutPP();
    ssPin->SetSpeedHigh();


    Adafruit_PCD8544 display(spi, dcPin, rstPin, ssPin);
    display.Init(0xbc);
    display.setRotation(2);

    display.write("124", 0, 0, 2);
    display.display();



    //UltraSonic
    SPI* spi2 = GPIO_Helper::SetupSPI(SPI2_PB_13_14_15, true, false, false, SPI_BAUD_RATE_2);

    delay(3000);
    spi2->TransmitByte(0x5);
//    delay(100);
    spi2->TransmitByte(0x1);
    //delay(50);
    spi2->TransmitByte(0x4);

    uint8_t fifo[100];
    uint8_t i = 0;
    volatile uint8_t rec;

//    for (uint8_t i = 0; i < 100; ++i)
//    {
//        fifo[i] = spi2->TransmitByte(0x0);
//        delay(50);
//    }

    while(1)
    {
        //display.write(0, 1, buf);

        rec = spi2->TransmitByte(0x0);
        display.clearDisplay();
        display.printf (0, 0, 4, "%d", rec);
        display.display();
        delay(20);
    }



    while (1)
        ;
    }

int main()
{
    RCC_EnableHSI_168Mhz();
    run_spi_master();
}

