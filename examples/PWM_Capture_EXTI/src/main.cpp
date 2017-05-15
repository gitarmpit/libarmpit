#include "exti_cpp.h"
#include "timer_cpp.h"
#include "gpio_cpp.h"
#include "RCC_helper.h"
#include "GPIO_Helper.h"
#include "rcc.h"
#include <stdio.h>
#include "debug.h"
#include "PWM_Capture/PWM_Capture_EXTI.h"

static TIMER_With_Channels* generatePWM()
{
    GPIOA::GetInstance()->EnablePeripheralClock(true);

    TIMER_With_Channels* outTimer = TIM2::GetInstance();
    outTimer->EnablePeripheralClock(true);

    GPIO_PIN* outPin = GPIOA::GetInstance()->GetPin(GPIO_PIN0); //PWM output
#ifdef STM32F4
    outPin->SetAF(outTimer->GetAf()); //A0: output, timer2 ch1
#endif
    outPin->SetupGPIO_OutAltPP();

    TIMER_Channel* ch1 = outTimer->GetChannel(1);
    ch1->SetupPWM1(31323, 187);
    ch1->EnableChannel(true);
    outTimer->EnableCounter(true);
    return outTimer;

}

static void generatePWM2()
{
    GPIOB::GetInstance()->EnablePeripheralClock(true);

    //GPIO_PIN* outPin = GPIOB::GetInstance()->GetPin(GPIO_PIN15);
    //outPin->SetupGPIO_OutPP();
    //outPin->Reset();

    //TIMER_With_Channels* outtimer = TIM12::GetInstance();
    TIMER_With_Channels* outtimer = TIM3::GetInstance();
    outtimer->EnablePeripheralClock(true);

    GPIO_PIN* outPin = GPIOB::GetInstance()->GetPin(GPIO_PIN15); //PWM output
#ifdef STM32F4
    outPin->SetAF(outtimer->GetAf());
#endif
    outPin->SetupGPIO_OutAltPP();

    TIMER_Channel* out_ch = outtimer->GetChannel(2);
    out_ch->SetupPWM1(120, 100);
    out_ch->EnableChannel(true);
    outtimer->EnableCounter(true);
}


class My_Timeout_Handler : public PWM_Timeout_Handler
{
public:
    virtual void Handle_Timeout()
    {
        static volatile int i = 0;
        ++i;
    }
};

static void exti_input_pwm_test()
{
    GPIOA::GetInstance()->EnablePeripheralClock(true);

    //output
    //TIMER_With_Channels* outTimer = generatePWM();

    generatePWM2();

    
    GPIOE::GetInstance()->EnablePeripheralClock(true);
//    GPIO_PIN* inputPin = GPIOE::GetInstance()->GetPin(GPIO_PIN5);
//    inputPin->SetupGPIO_InPullDown();
//    SYSCFG_RemapEXTI(EXTI_LINE5, PORTE);
    const char* inputPin = "e5";
    //Timer is not connected to pins
    TIMER* timer_in = TIM4::GetInstance();
    My_Timeout_Handler timeout_handler;
    PWM_Capture_EXTI handler (timer_in, inputPin, &timeout_handler);
    EXTI5::GetInstance()->SetIrqHandler(&handler);

    handler.Start();
    delay(1000);
    volatile uint32_t period, duty;

    while(7)
    {
        duty = handler.GetDuty_us();
        period = handler.GetPeriod_us();
#ifdef SEMIHOSTING
        printf ("period: %d, duty: %d\n", period, duty);
#endif
        delay(500);
    }


    delay(5000);
    //stop PWM signal to simulate timeout
    //outTimer->EnableCounter(false);
    while (1)
        ;

}

extern "C" void initialise_monitor_handles(void);

int main()
{
#ifdef SEMIHOSTING
    initialise_monitor_handles();
#endif

#ifdef STM32F4
   RCC_EnableHSI_100Mhz();
#else
   RCC_EnableHSI_64Mhz_AHB_32Mhz_APB1_16MHz_APB2_16MHz();
#endif

    RCC_SetAHBPrescalerDiv2();
    //RCC_SetAPB1PrescalerDiv16();
    //initialise_monitor_handles();
    exti_input_pwm_test();
}
