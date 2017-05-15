#include "RCC_Helper.h"
#include "GPIO_Helper.h"
//#include "ssd1306.h"
#include "spi.h"
#include "gpio_cpp.h"
#include <stdio.h>
#include "SSD1306/Adafruit_SSD1306.h"
#include "debug.h"



static SPI*  g_spi;
static GPIO_PIN* g_rstPin;
static GPIO_PIN* g_dcPin;
static I2C*   g_i2c;
 static void test3()
 {
        GPIOA* portA = GPIOA::GetInstance();
        GPIOB* portB = GPIOB::GetInstance();
        portA->EnablePeripheralClock(true);
        portB->EnablePeripheralClock(true);

        g_spi = GPIO_Helper::SetupSPI(SPI2_PB_13_14_15, true, false, false, SPI_BAUD_RATE_2);
        g_rstPin = portB->GetPin(GPIO_PIN12);
        g_rstPin->SetupGPIO_OutPP();
        g_dcPin = portB->GetPin(GPIO_PIN7);
        g_dcPin->SetupGPIO_OutPP();
 }
 
static void test2() 
{
    GPIOA* portA = GPIOA::GetInstance();
    GPIOB* portB = GPIOB::GetInstance();
    portA->EnablePeripheralClock(true);
    portB->EnablePeripheralClock(true);

    g_spi = GPIO_Helper::SetupSPI(SPI2_PB_13_14_15, true, false, false, SPI_BAUD_RATE_2);
    g_rstPin = portB->GetPin(GPIO_PIN12);
    g_rstPin->SetupGPIO_OutPP();
    g_dcPin = portB->GetPin(GPIO_PIN7);
    g_dcPin->SetupGPIO_OutPP();

    
    //D0: clk  D1: MOSI
    
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
//    LED_P8x16Str(20, 6, buf);
//    }
}
 
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

static void test_i2c_ada()
{
    GPIOA* portA = GPIOA::GetInstance();
    GPIOB* portB = GPIOB::GetInstance();
    portA->EnablePeripheralClock(true);
    portB->EnablePeripheralClock(true);

   I2C* i2c = GPIO_Helper::SetupI2C(I2C1_PB_6_7, 600000);
   Adafruit_SSD1306 display (i2c);
   display.setBgColor(0);
   display.setTextColor(1);
   display.clearDisplay();
   display.display();
   display.write("Hello, World!");
   display.display();

   while(1)
       ;
}

//static void test_i2c_2()
//{
//    GPIOA* portA = GPIOA::GetInstance();
//    GPIOB* portB = GPIOB::GetInstance();
//    portA->EnablePeripheralClock(true);
//    portB->EnablePeripheralClock(true);
//
//   I2C* i2c = GPIO_Helper::SetupI2C(I2C1_PB_6_7, 100000);
//   SSD1306 display (i2c);
//   display.ClearScreen();
//   display.WriteLine(0, 0, "Test1234567890 1234567890");
//
//   while(1)
//       ;
//}



static void test()
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

int main()
{
#if defined(STM32F1)
    RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
#elif defined (STM32F407xx)
    RCC_EnableHSI_168Mhz();
#elif defined (STM32F411xE) || defined(STM32F2)
    RCC_EnableHSI_100Mhz();
#endif

    test_i2c_ada();

}
