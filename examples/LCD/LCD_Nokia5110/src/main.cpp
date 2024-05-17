#include "RCC_Helper.h"
#include "GPIO_Helper.h"
#include "spi.h"
#include "gpio_cpp.h"
#include <stdio.h>
#include "5110/Adafruit_Nokia5110.h"
#include "adc_cpp.h"
#include "fp.h"
#include "fp2.h"
#include "spi_cpp.h"
#include "usart_cpp.h"
#include "debug.h"

//#include "ARLRDBD47_40.h"  try it for 4-5 digits?

//maybe
//#include "gil46_40.h"
//#include "ltype46_40.h"
//#include "lucon46_40.h"  //same as ltype?
//#include "micross46_40.h"
//#include "micross34_40.h"  //best?
//#include "tahoma32_40.h"   //also very good , 34 fits but '1' is odd (thin), use 36 with reduced spacing
//#include "tahomabd32_40.h"  //bold, ok
//#include "msyi34_40.h" //good, very light, smaller than others use 44 or 46 instead of 32 (42 looks weird)
//#include "segoeui32_40.h"  //light  32 or 34  or 36

//#include "verdana35_40.h"  //good
//#include "verdana36_40.h"  //good, but 6 digits only

//fnt
//#include "fixedsys16.h"


//good small ones
//bitmap fnt
//#include "small.h"  //very good


//#include "tahoma20_40.h" 16-20
//#include "msyi20_40.h"  //20 is smaller that the default font;  some sizes produces unequal character sizes
                          //23-24 ok-ish, same size as the default font but not kind ugly
                          //27,31,35,38 ok

//#include "fnt/dos12.h"
//#include "fnt/term12.h"
#include "ttf/consola12.h"

static void test_display()
{
    delay(10);
    GPIOA* portA = GPIOA::GetInstance();
    GPIOB* portB = GPIOB::GetInstance();
    portA->EnablePeripheralClock(true);
    portB->EnablePeripheralClock(true);

    //Too fast won't work, check clock and baud rate
    SPI* lcdSpi = GPIO_Helper::SetupSPI(SPI1_PA_5_6_7, true, false, false, SPI_BAUD_RATE_16);

    GPIO_PIN* rstPin = portB->GetPin(GPIO_PIN2);
    rstPin->SetupGPIO_OutPP();
    rstPin->SetSpeedHigh();

    GPIO_PIN* dcPin = portB->GetPin(GPIO_PIN1);
    dcPin->SetupGPIO_OutPP();
    dcPin->SetSpeedHigh();

    GPIO_PIN* ssPin = portA->GetPin(GPIO_PIN4);
    ssPin->SetupGPIO_OutPP();
    ssPin->SetSpeedHigh();


    Adafruit_Nokia5110 lcd(lcdSpi, dcPin, rstPin, ssPin);
    lcd.Init(0xbc);
    lcd.setTextWrap(false);
    lcd.clearDisplay();
    lcd.display();
    //lcd.setRotation(2); //upside down
    lcd.setTextColor(1, 0);
    lcd.setFont(&consola12);
    lcd.printf("ABC0123456789\n");
#if 0

    lcd.display();
#endif

    //lcd.printf ("1'234'567\n");
    //lcd.setFont(&ARLRDBD47_40);
    //lcd.setYadvance(12);
    //lcd.setXadvance(5);
    //lcd.setFont(0);
    //lcd.printf (0, 1, "1,234,567\n");
    //lcd.printf ("1,234,567\n");
    //lcd.printf ("2,234,567");
    //lcd.printf (0, 2, "1,234,567");

    //lcd.setCursor(1, 1);
    //lcd.printf ("1'234'567");
    //lcd.printf ("123456");
    //lcd.drawBmp(0, 0, consolas_4bit_2);

    while(1)
    {
    }

}



//Vref = 3.3 * adc_res / 4095;  3.3*1480/4095= 1.193

//Vcc = Vref / adc_res * 4095

//3.77/4095*3131*6.87/4.87 
//Vbat = Vcc/4095 * adc_res  * R1 / R2

/*
2.78
3.10
3.42
3.75  0.325


3131  2.88-2.89     3.76-3.77   1.183/2.88
3147  2.67
3171  2.45
3195  2.25 
*/

class ADC_IRQ_JHandler_Test: public ADC_IRQ_Handler
{
public:

    virtual void Handle(ADC* adc)
    {
        static volatile uint32_t d1;
        static volatile uint32_t d2;

        d1 = adc->GetJData1();
        d2 = adc->GetJData2();

    }
};


//static void bat(Adafruit_Nokia5110* display)
//{
//    GPIOA* gpioA = GPIOA::GetInstance();
//    gpioA->EnablePeripheralClock(true);
//    gpioA->SetupGPIO_InAnalog(GPIO_PIN2);
//    ADC1* adc1 = ADC1::GetInstance();
//    adc1->Enable(true);
//    adc1->EnableTempVref(true);
//    adc1->AddJConversion (ADC_VREFINT, ADC_SMP_SLOWEST);
//    adc1->AddJConversion (ADC_IN2, ADC_SMP_SLOWEST);
//    char buf[8];
//    volatile uint32_t adc_vref, adc_r1;
//
//    float schottky_drop = 0.326;
//    while (1)
//    {
//        display->clearDisplay();
//        adc1->SingleJConversion();
//        adc_r1 = adc1->GetJData2();
//        adc_vref = adc1->GetJData1();
//
//        //volatile float v = 1.196 / adc_res * 4095;
//        //fix16_to_str (fix16_from_float(v), buf, 3);
//
//        //display->drawString(buf, 0, 0, 2);
//        //display->display();
//        delay(100);
//    }
//
//
//}
//
//static uint8_t avg (uint8_t* buf, uint8_t cnt)
//{
//    uint32_t total = 0;
//    for (uint8_t i = 0; i < cnt; ++i)
//    {
//        total += buf[i];
//    }
//    return total / cnt;
//}
//
//static void test_proximity()
//{
//    delay(10);
//    GPIOA* portA = GPIOA::GetInstance();
//    GPIOB* portB = GPIOB::GetInstance();
//    portA->EnablePeripheralClock(true);
//    portB->EnablePeripheralClock(true);
//
//    //Too fast won't work, check clock and baud rate
//    SPI* lcdSpi = GPIO_Helper::SetupSPI(SPI1_PA_5_6_7, true, false, false, SPI_BAUD_RATE_8);
//
//    GPIO_PIN* rstPin = portB->GetPin(GPIO_PIN2);
//    rstPin->SetupGPIO_OutPP();
//    rstPin->SetSpeedHigh();
//
//    GPIO_PIN* dcPin = portB->GetPin(GPIO_PIN1);
//    dcPin->SetupGPIO_OutPP();
//    dcPin->SetSpeedHigh();
//
//    GPIO_PIN* ssPin = portA->GetPin(GPIO_PIN4);
//    ssPin->SetupGPIO_OutPP();
//    ssPin->SetSpeedHigh();
//
//
//    Adafruit_Nokia5110 lcd(lcdSpi, dcPin, rstPin, ssPin);
//    lcd.Init(0xbc);
//    lcd.clearDisplay();
//    lcd.display();
//
//    GPIOB::GetInstance()->EnablePeripheralClock(true);
//
//
//    SPI* sonarSpi = GPIO_Helper::SetupSPI(SPI2_PB_13_14_15, true, false, false, SPI_BAUD_RATE_16);
//
//    lcd.write ("Warming up...", 0, 0);
//    lcd.display();
//    delay(3000);
//    sonarSpi->TransmitByte(0x1);
//    sonarSpi->TransmitByte(0x4);
//    sonarSpi->TransmitByte(0x5);
//
//    char buf[64];
//    uint8_t fifo[100];
//    uint8_t i = 0;
//    volatile uint8_t rec;
//
////    for (uint8_t i = 0; i < 100; ++i)
////    {
////        fifo[i] = spi2->TransmitByte(0x0);
////        delay(50);
////    }
//
//    while(1)
//    {
//        lcd.clearDisplay();
//        //sprintf (buf, "Master tx: %d", b);
//        //display.write(0, 1, buf);
//
//        //fifo [i++ % 100] = spi2->TransmitByte(0x0);
//        //rec = avg(fifo, 100);
//        rec = sonarSpi->TransmitByte(0x0);
//        lcd.printf(1, 0, 4, "%d", buf);
//        //++b;
//        lcd.display();
//        //delay(10);
//    }
//
//
//
//    while(1)
//        ;
//}
//
//static void test_uart()
//{
//    uint32_t clockSpeed = 9600;
//    USART* usart = GPIO_Helper::SetupUSART(USART1_PA_9_10, clockSpeed);
//
//    while(1)
//    {
//        while (!usart->IsRXNE())
//            ;
//
//        {
//            if (usart->Receive() == 23)
//            {
//                usart->SendByte(47);
//            }
//        }
//
//    }
//
//}


//static void test_uart2()
//{
//    delay(10);
//    GPIOA* portA = GPIOA::GetInstance();
//    GPIOB* portB = GPIOB::GetInstance();
//    portA->EnablePeripheralClock(true);
//    portB->EnablePeripheralClock(true);
//
//    //Too fast won't work, check clock and baud rate
//    SPI* lcdSpi = GPIO_Helper::SetupSPI(SPI1_PA_5_6_7, true, false, false, SPI_BAUD_RATE_16);
//
//    GPIO_PIN* rstPin = portB->GetPin(GPIO_PIN2);
//    rstPin->SetupGPIO_OutPP();
//    rstPin->SetSpeedHigh();
//
//    GPIO_PIN* dcPin = portB->GetPin(GPIO_PIN1);
//    dcPin->SetupGPIO_OutPP();
//    dcPin->SetSpeedHigh();
//
//    GPIO_PIN* ssPin = portA->GetPin(GPIO_PIN4);
//    ssPin->SetupGPIO_OutPP();
//    ssPin->SetSpeedHigh();
//
//
//    Adafruit_Nokia5110 lcd(lcdSpi, dcPin, rstPin, ssPin);
//    lcd.Init(0xbc);
//    //bat(&display);
//
//    lcd.write("123", 0, 0, 6);
//    lcd.write("123", 1, 1, 6);
//    lcd.display();
//
//    uint32_t clockSpeed = 9600;
//    USART* usart = GPIO_Helper::SetupUSART(USART1_PA_9_10, clockSpeed);
//
//    while(1)
//    {
//        while (!usart->IsRXNE())
//            ;
//
//        {
//            if (usart->Receive() == 23)
//            {
//                usart->SendByte(47);
//                lcd.clearDisplay();
//                lcd.write("Rx", 1, 1, 4);int Adafruit_GFX::drawChar2(unsigned int uniCode, int size)

//                lcd.display();
//            }
//        }
//
//    }
//
//
//
//    while(1)
//        ;
//
//    //0xb4, 0x14 default
//    //0xb1 - lighter 0xbf - darker
//
//    //red ones
//    //display.Init(0xac);
//
//    //display.Init();
//
//    //new blue
//    //display.Init(0xbc);
//
//}
//

#ifdef SEMIHOSTING

extern "C" void initialise_monitor_handles(void);

#endif

int main()
{
#if defined(STM32F1)
    RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
#elif defined (STM32F407xx)
    RCC_EnableHSI_168Mhz();
#elif defined (STM32F411xE) || defined(STM32F2)
    RCC_EnableHSI_100Mhz();
#elif defined(STM32F401xC)
    RCC_EnableHSI_84Mhz();
#endif

#ifdef SEMIHOSTING
    initialise_monitor_handles();
    printf ("test semihosting\n");
 #endif
    test_display();
}

