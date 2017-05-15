#include "RCC_Helper.h"
#include "GPIO_Helper.h"
#include "spi.h"
#include "gpio_cpp.h"
#include <stdio.h>
#include "SSD1306/Adafruit_SSD1306.h"
#include "debug.h"
 
//static void test_i2c()
//{
//    GPIOA* portA = GPIOA::GetInstance();
//    GPIOB* portB = GPIOB::GetInstance();
//    portA->EnablePeripheralClock(true);
//    portB->EnablePeripheralClock(true);
//
//    g_i2c = GPIO_Helper::SetupI2C1_PB_6_7(100000);
//
//    LED_Init();
//    LED_P8x16Str(23,0,"welcome to");
//    LED_P8x16Str(40,2,"Smart");
//    LED_P8x16Str(20,4,"Prototyping");
//
//    uint32_t i = 0;
//    char buf[32];
//    while(1)
//    {
//        sprintf (buf, "%d", i++);
//        //delay(1);
//        LED_P8x16Str(20, 6, buf);
//    }
//}

static void  test_slave_spi()
{
    GPIOA* portA = GPIOA::GetInstance();
    GPIOB* portB = GPIOB::GetInstance();
    portA->EnablePeripheralClock(true);
    portB->EnablePeripheralClock(true);

    I2C* i2c = GPIO_Helper::SetupI2C(I2C1_PB_8_9, 600000);
   Adafruit_SSD1306 display (i2c);
   display.setTextColor(1);
   display.clearDisplay();
   display.display();
   //display.setCursor(0, 0);
   //display.write(0, 0, "Hello, F407 World1");
   display.display();

   SPI* spi = GPIO_Helper::SetupSPI(SPI1_PA_5_6_7, false, false, false, SPI_BAUD_RATE_16);
   //spi->SendByte(0x13);
   uint8_t rcv, snd= 0x7e;
   while(1)
   {
       display.clearDisplay();
       display.printf(0, 1, "Slave tx: %d", snd);
       rcv = spi->TransmitByte(snd);
       display.printf(0, 2, "Slave rx: %d", rcv);
       display.display();
       snd = rcv+1;
   }
}


static void test_i2c_ada()
{
    GPIOA* portA = GPIOA::GetInstance();
    GPIOB* portB = GPIOB::GetInstance();
    portA->EnablePeripheralClock(true);
    portB->EnablePeripheralClock(true);

   I2C* i2c = GPIO_Helper::SetupI2C(I2C1_PB_8_9, 600000);
   Adafruit_SSD1306 display (i2c);
   display.setTextColor(1);
   display.clearDisplay();
   display.display();
   display.write("Hello, STM32F407 World!");
   display.display();

   while(1)
       ;
}




static void test_spi()
{
    delay(10);
    GPIOA* portA = GPIOA::GetInstance();
    GPIOB* portB = GPIOB::GetInstance();
    portA->EnablePeripheralClock(true);
    portB->EnablePeripheralClock(true);

    SPI* spi = GPIO_Helper::SetupSPI(SPI2_PB_13_14_15, true, false, false, SPI_BAUD_RATE_2);
    GPIO_PIN* rstPin = portB->GetPin(GPIO_PIN12);
    rstPin->SetupGPIO_OutPP();
    rstPin->SetSpeedHigh();
    GPIO_PIN* dcPin = portB->GetPin(GPIO_PIN7);
    dcPin->SetupGPIO_OutPP();
    dcPin->SetSpeedHigh();

//    SPI* spi = GPIO_Helper::SetupSPI1_MasterMode_PA_5_6_7(false, false, SPI_BAUD_RATE_2);
//    GPIO_PIN* rstPin = portB->GetPin(GPIO_PIN1);
//    rstPin->SetupGPIO_OutPP();
//    GPIO_PIN* dcPin = portB->GetPin(GPIO_PIN0);
//    dcPin->SetupGPIO_OutPP();

//    Adafruit_SSD1306 display (spi, rstPin, dcPin);
//    display.Init();

//    SSD1306 display (spi, rstPin, dcPin);
//    char buf[256];
//    //sprintf (buf, "%d", 12345);
//    display.WriteLine (0, 0, "12345678901234567890");
//    display.WriteLine (0, 10, "22345678901234567890");
//    display.WriteLine (0, 20, "32345678901234567890");
//    display.WriteLine (0, 30, "42345678901234567890");
//    display.WriteLine (0, 40, "52345678901234567890");
//    display.WriteLine (0, 50, "62345678901234567890");

    //display.WriteLine (0, 0, buf);
    while(1)
        ;

}

int main2()
{
    //RCC_EnableHSI_168Mhz();
    //test_i2c_ada();
    test_slave_spi();

}
