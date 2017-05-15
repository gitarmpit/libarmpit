#include "RCC_Helper.h"
#include "GPIO_Helper.h"
#include "spi.h"
#include "gpio_cpp.h"
#include <stdio.h>
#include "5110_2/lcd5110.h"
#include "debug.h"

static void test()
{
    delay(10);
    GPIOA* portA = GPIOA::GetInstance();
    GPIOB* portB = GPIOB::GetInstance();
    portA->EnablePeripheralClock(true);
    portB->EnablePeripheralClock(true);

    //Too fast won't work, check clock and baud rate
    SPI* spi = GPIO_Helper::SetupSPI1_MasterMode_PA_5_6_7(false, false, SPI_BAUD_RATE_8);

    GPIO_PIN* rstPin = portB->GetPin(GPIO_PIN2);
    rstPin->SetupGPIO_OutPP();
    rstPin->SetSpeedHigh();

    GPIO_PIN* dcPin = portB->GetPin(GPIO_PIN1);
    dcPin->SetupGPIO_OutPP();
    dcPin->SetSpeedHigh();

    GPIO_PIN* ssPin = portA->GetPin(GPIO_PIN4);
    ssPin->SetupGPIO_OutPP();
    ssPin->SetSpeedHigh();


    LCD5110 display(spi, dcPin, rstPin, ssPin, true);
    //0xb4, 0x14 default
    //0xb1 - lighter 0xbf - darker
    
    //red ones
    //display.Init(0xac);
    display.Init();
    
    //new blue
    //display.Init(0xbc);
    
    //old blue: default is OK
    
    display.Clear();


    uint32_t i = 0;
    char buf[32] = {0};
    while(6)
    {
        sprintf (buf, "%d", i++);
        
        display.GotoXY(0, 0);
        display.WriteLine(buf);
        display.GotoXY(0, 1);
        display.WriteLine(buf);
        display.GotoXY(0, 2);
        display.WriteLine(buf);
        display.GotoXY(0, 3);
        display.WriteLine(buf);
        display.GotoXY(0, 4);
        display.WriteLine(buf);
        display.GotoXY(0, 5);
        display.WriteLine(buf);

        
        display.GotoXY(8, 0);
        display.WriteLine(buf);
        display.GotoXY(8, 1);
        display.WriteLine(buf);
        display.GotoXY(8, 2);
        display.WriteLine(buf);
        display.GotoXY(8, 3);
        display.WriteLine(buf);
        display.GotoXY(8, 4);
        display.WriteLine(buf);
        display.GotoXY(8, 5);
        display.WriteLine(buf);
        
        
        delay(10);
    }


}

int main()
{
    //Too fast won't work, check clock and baud rate
    //64Mhz = baud rate div 8 max
    RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
    //FLASH_SetWaitState(2);
    //RCC_EnableHSI(TRUE);
    //RCC_EnablePLL(16);
    //RCC_SetAPB1PrescalerDiv2();

    test();

}
