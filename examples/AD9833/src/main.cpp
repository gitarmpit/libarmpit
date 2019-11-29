#include "GPIO_Helper.h"
#include "RCC_Helper.h"
#include "ad9833.h"
#include "debug.h"
#include "5110/Adafruit_Nokia5110.h"
#include "afio.h"

static void test_lcd()
{
    RCC_EnableAFIO(TRUE);
    AFIO_Remap_SWJ_CFG(false, true, true);

    //Too fast won't work, check clock and baud rate
    SPI* spi = GPIO_Helper::SetupSPI(SPI1_PB_3_4_5, true, false, false, SPI_BAUD_RATE_16);

    GPIO_PIN* rstPin = GPIO_Helper::GetPin("B9");
    rstPin->SetupGPIO_OutPP();
    rstPin->SetSpeedHigh();

    GPIO_PIN* dcPin = GPIO_Helper::GetPin("B7");
    dcPin->SetupGPIO_OutPP();
    dcPin->SetSpeedHigh();

    GPIO_PIN* ssPin = GPIO_Helper::GetPin("B6");
    ssPin->SetupGPIO_OutPP();
    ssPin->SetSpeedHigh();


    Adafruit_Nokia5110 display(spi, dcPin, rstPin, ssPin);
    display.Init(0xbc);
    //display.setRotation(2);

    display.write(L"123");
    display.display();
    //double f = 4.12345;
    //display.printf(0, 1, 2, "%8.5f", f);
    while(1);

}

int main()
{
    RCC_EnableHSI_64Mhz_AHB_32Mhz_APB1_16MHz_APB2_16MHz();

    test_lcd();

    //RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
    //false true worked a few times properly
    SPI* spi = GPIO_Helper::SetupSPI(SPI1_PA_5_6_7, true, false, true, SPI_BAUD_RATE_32);
    GPIO_PIN* ssPin =  GPIOA::GetInstance()->GetPin(GPIO_PIN4);
     //GPIO_Helper::GetPin("A4");
    ssPin->SetupGPIO_OutPP();


    AD9833 ad9833 (spi, ssPin, 16000000);
    ad9833.SetFrequency(REG0, 50000);
    delay(5000);
    ad9833.Reset();
    //ad9833.SetWaveform(REG0, TRIANGLE_WAVE);
    while(1)
        ;
}
