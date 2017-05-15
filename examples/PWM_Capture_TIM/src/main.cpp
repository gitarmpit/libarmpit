
#include "timer_cpp.h"
#include "gpio_cpp.h"
#include "RCC_Helper.h"
#include "GPIO_Helper.h"
#include <stdio.h>
#include "debug.h"
#include "PWM_Capture/PWM_Capture_TIM.h"


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

class My_Timeout_Handler : public PWM_Timeout_Handler
{
public:
    virtual void Handle_Timeout()
    {
#ifdef SEMIHOSTING
        printf ("timeout\n");
#endif
    }
};


static void test_input_capture2()
{
    GPIOA::GetInstance()->EnablePeripheralClock(true);

    //output
    //TIMER_With_Channels* outTimer = generatePWM();
    generatePWM(100000, 20);

    GPIOE::GetInstance()->EnablePeripheralClock(true);

    TIMER_With_Channels* timer = TIM2::GetInstance();
    timer->EnablePeripheralClock(true);
    timer->EnableNVICInterrupt(true);

    timer->SetUpdatePeriod_us(200000);
    timer->EnableAutoPreload(true);

    //timer->EnableUpdateInterrupt(true);
    timer->SetCounterValue(0);

    const char* inputPin = "e5";

    My_Timeout_Handler timeout_handler;
    PWM_Capture_TIM handler1 (timer, 1, inputPin, &timeout_handler);
    handler1.Start();

    timer->EnableCounter(true);

    volatile uint32_t period[128], duty[128] = {0};
    for (uint8_t i = 0; i < 1280; ++i)
    {
        duty[i] = (uint32_t)((float)handler1.GetDuty_us()/58.0);
        //period[i] = i; //handler1.GetPeriod_us();
        //printf ("%d\n", duty[i]);
        delay(5);
    }

    timer->EnableCounter(false);
    timer->EnableNVICInterrupt(false);

    while(1)
    {
        delay(50);
    }

}


static void test_input_capture() 
{
      GPIOA::GetInstance()->EnablePeripheralClock(true);

        //output
        //TIMER_With_Channels* outTimer = generatePWM();

      generatePWM(18, 6);


    //input
    TIMER_With_Channels* timer = TIM2::GetInstance();
    timer->EnablePeripheralClock(true);
    timer->EnableNVICInterrupt(true);
    timer->SetUpdatePeriod_us(50000);
    timer->EnableAutoPreload(true);
    timer->EnableUpdateInterrupt(true);
    timer->SetCounterValue(0);

//    GPIO_PIN* inputPin1 = GPIOA::GetInstance()->GetPin(GPIO_PIN0);
//    inputPin1->SetAF(AF1);
//    inputPin1->SetupGPIO_OutAltPP();
//    GPIO_PIN* inputPin2 = GPIOA::GetInstance()->GetPin(GPIO_PIN1);
//    inputPin2->SetAF(AF1);
//    inputPin2->SetupGPIO_OutAltPP();
//    GPIO_PIN* inputPin3 = GPIOA::GetInstance()->GetPin(GPIO_PIN2);
//    inputPin3->SetAF(AF1);
//    inputPin3->SetupGPIO_OutAltPP();

    const char* inputPin1 = "a0";
    const char* inputPin2 = "a1";
    const char* inputPin3 = "a2";


    TIMER_Channel* ch_in1 = timer->GetChannel(1);
    TIMER_Channel* ch_in2 = timer->GetChannel(2);
    TIMER_Channel* ch_in3 = timer->GetChannel(3);

    My_Timeout_Handler timeout_handler;

    PWM_Capture_TIM handler1 (timer, 1, inputPin1, &timeout_handler);
    handler1.Start();

    PWM_Capture_TIM handler2 (timer, 2, inputPin2, &timeout_handler);
    handler2.Start();

    PWM_Capture_TIM handler3 (timer, 3, inputPin3, &timeout_handler);
    handler3.Start();

    timer->EnableCounter(true);

    volatile uint32_t period1, duty1;
    volatile uint32_t period2, duty2;
    volatile uint32_t period3, duty3;
    while(1)
    {
        duty1 = handler1.GetDuty_us();
        period1 = handler1.GetPeriod_us();
        duty2 = handler2.GetDuty_us();
        period2 = handler2.GetPeriod_us();
        duty3 = handler3.GetDuty_us();
        period3 = handler3.GetPeriod_us();


//#ifdef SEMIHOSTING
//        printf ("%d %d %d\n", duty1, duty2, duty3);
//#endif
        delay(500);
    }

    //Simulate timeout: stop output timer after 5 sec
//    delay(5000);
//    outTimer->EnableCounter(false);
//
//    while(1);
}


extern "C" void initialise_monitor_handles(void);


int main()
{
//#ifdef SEMIHOSTING
//    initialise_monitor_handles();
//#endif

#ifdef STM32F4
    RCC_EnableHSI_168Mhz();
#else
    RCC_EnableHSI_64Mhz_AHB_32Mhz_APB1_16MHz_APB2_16MHz();
#endif
    RCC_SetAHBPrescalerDiv2();
    //RCC_SetAPB1PrescalerDiv16();

    test_input_capture2();
    test_manual_pwm();
}
