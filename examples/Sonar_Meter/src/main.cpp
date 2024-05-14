#include "RCC_Helper.h"
#include "GPIO_Helper.h"
#include "gpio_cpp.h"
#include <stdio.h>
#include "5110/Adafruit_Nokia5110.h"
#include "exti_cpp.h"
#include "debug.h"
#include "PWM_Capture/PWM_Capture_EXTI.h"

static void run()
{
    delay(10);
    GPIOA* portA = GPIOA::GetInstance();
    GPIOB* portB = GPIOB::GetInstance();
    portA->EnablePeripheralClock(true);
    portB->EnablePeripheralClock(true);

    //Too fast won't work, check clock and baud rate
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


    Adafruit_Nokia5110 display(spi, dcPin, rstPin, ssPin);
    display.Init(0xbc);
    display.clearDisplay();
    display.display();

    GPIOB::GetInstance()->EnablePeripheralClock(true);


    display.write ("Warming up...", 0, 0);
    display.display();

    //////////////////////////////////////////////////////////////////
    //GPIO_PIN* inputPin = GPIOB::GetInstance()->GetPin(GPIO_PIN7);
    //inputPin->SetupGPIO_InPullDown();
    const char* inputPin = "B7";

    TIMER_With_Channels* outtimer;
    TIMER_Channel* out_ch;

    outtimer = TIM4::GetInstance();
    outtimer->EnablePeripheralClock(true);

    //trigger
    GPIO_PIN* outPin = GPIO_Helper::GetPin("B6"); //PWM output
    outPin->SetupGPIO_OutAltPP();

    out_ch = outtimer->GetChannel(1);
    out_ch->SetupPWM1(70000, 100);
    out_ch->EnableChannel(true);
    outtimer->EnableCounter(true);

    //RCC_EnableAFIO(TRUE);

    //AFIO_RemapEXTI(EXTI_LINE7, PORTB);

    PWM_Capture_EXTI reader(TIM3::GetInstance(), inputPin, 0);

    reader.Start();
    /////////////////////////////////

    volatile uint8_t rec;

    while(1)
    {
        display.clearDisplay();
        rec = reader.GetDuty_us() / 58;
        display.printf(1, 4, "%", rec);
        display.display();
        delay(10);
    }



    while(1)
        ;
}


int main()
{
    //Too fast won't work, check clock and baud rate
    //64Mhz = baud rate div 8 max
    //RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
    run();
}

