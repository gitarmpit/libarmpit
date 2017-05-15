
#include "timer_cpp.h"
#include "gpio_cpp.h"
#include "RCC_helper.h"
#include "GPIO_Helper.h"
#include <stdio.h>

//comment this to feed your own input instead of the test PWM signal
#define GENERATE_TEST_PWM


static void generatePWM(TIMER_With_Channels* timer)
{
    TIMER_Channel* ch1 = timer->GetChannel(1);
    ch1->SetupPWM1(31323, 187);
    ch1->EnableChannel(true);
    timer->EnableCounter(true);

}


class PWM_Capture_Handler1 : public PWM_Capture_Handler
{
public:
    
    //The measured period and duty in us are passed to the handler. 
    virtual void Handle (uint32_t period_us, uint32_t duty_us)
    {
#ifdef SEMIHOSTING
        printf ("period: %lu, duty: %lu\n", period_us, duty_us);
#endif
    }
};





static void test_pwm_capture() 
{
    
    GPIOA::GetInstance()->EnablePeripheralClock(true);

    GPIO_PIN* outPin = GPIOA::GetInstance()->GetPin(GPIO_PIN0); //PWM output
    outPin->SetupGPIO_OutAltPP();

    //Input pin:  A6 TIM3 Ch1
    GPIO_PIN* inputPin = GPIOA::GetInstance()->GetPin(GPIO_PIN6);

#ifdef STM32F4
    outPin->SetAF(AF1); //A0: output, timer2 ch1

    inputPin->SetAF(AF2); //A6: input, timer3 ch1
    inputPin->SetupGPIO_OutAltPP();

    //second channel
    GPIOA::GetInstance()->SetAF(AF2, GPIO_PIN7);

    
#else
    inputPin->SetupGPIO_InFloat();  //pin16 input
#endif


    //output
    TIMER_With_Channels* outTimer = TIM2::GetInstance();
    outTimer->EnablePeripheralClock(true);

    generatePWM(outTimer);
    
    PWM_Capture_Handler1 handler;
    TIMER_With_SlaveMode* inTimer = TIM3::GetInstance();
    inTimer->EnablePeripheralClock(true);


    //TIMER_Channel* ch_in = inTimer->GetChannel(1);  //TIM3 channe 1 : pin16 is used to capture PWM

    uint32_t input_period_us = 50000;  //tells how fast to run the input counter,
    //needs to match roughly but slightly higher than the expected period of incoming PWM
    inTimer->CaptureInputPWM (input_period_us, &handler);

    while(1);
}

extern "C" void initialise_monitor_handles(void);

int main()
{
    RCC_EnableHSI_64Mhz();
    //RCC_EnableHSI_24Mhz();
    //RCC_EnableHSI(true);
    RCC_SetAHBPrescalerDiv2();

#ifdef SEMIHOSTING
    initialise_monitor_handles();
#endif

    test_pwm_capture();
}
