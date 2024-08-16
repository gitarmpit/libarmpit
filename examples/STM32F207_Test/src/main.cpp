#include "timer_cpp.h"
#include "gpio_cpp.h"
#include "RCC_Helper.h"
#include "GPIO_Helper.h"
#include <stdio.h>
#include "debug.h"


static void generatePWM(uint32_t period_us, uint32_t duty_us)
{
    TIMER_With_Channels* outtimer = TIM2::GetInstance();
    outtimer->EnablePeripheralClock(true);

    GPIO_PIN* outPin = GPIOB::GetInstance()->GetPin(GPIO_PIN15); //PWM output
#ifdef STM32F4
    outPin->SetAF(outtimer->GetAf());
#endif
    outPin->SetupGPIO_OutAltPP();

    TIMER_Channel* out_ch = outtimer->GetChannel(2);
    out_ch->SetupPWM1(period_us, duty_us);
    out_ch->EnableChannel(true);
    outtimer->EnableCounter(true);
}

static void test_manual_pwm()
{
    GPIOB::GetInstance()->EnablePeripheralClock(true);
    GPIO_PIN* outPin = GPIOB::GetInstance()->GetPin(GPIO_PIN15); //PWM output
    outPin->SetupGPIO_OutPP();

    while(1)
    {
        outPin->Set();
        delay_us(100);
        outPin->Reset();
        delay_us(100);

    }
}

// extern "C" void initialise_monitor_handles(void);

int main()
{
//#ifdef SEMIHOSTING
//    initialise_monitor_handles();
//#endif

    RCC_EnableHSI_100Mhz();
    RCC_SetAHBPrescalerDiv2();
    //RCC_SetAPB1PrescalerDiv16();
    Debug_EnableCYCCNT(true);

    GPIO_PIN* green = GPIO_Helper::GetPin("B11");
    green->SetupGPIO_OutPP();
    green->Set();

    GPIO_PIN* red = GPIO_Helper::GetPin("B10");
    red->SetupGPIO_OutPP();
    red->Reset();

    while(1)
    {
    	green->Toggle();
    	delay(1000);
    }


    /*
    green->Set();
    red->Set();

    green->Reset();
    green->Set();

    red->Reset();
    red->Set();

    green->Reset();
    red->Reset();
	*/

    GPIO_PIN* rst = GPIO_Helper::GetPin("B8");
    rst->SetupGPIO_OutPP();
    rst->Set();

    while(1)
    	;

}
