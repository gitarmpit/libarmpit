#include "exti.h"
#include "afio.h"
#include "gpio_cpp.h"
#include "timer_cpp.h"

static void generatePWM()
{
    GPIOA::GetInstance()->EnablePeripheralClock(true);
    GPIOA::GetInstance()->SetupGPIO_OutAltPP(GPIO_PIN0);
    TIM2* timer2 = TIM2::GetInstance();
    timer2->EnablePeripheralClock(true);
    TIMER_Channel* ch1 = timer2->GetChannel(1);
    ch1->SetupPWM1(1000000, 500000); //1 sec period, 50% duty
    ch1->EnableChannel(true);
    while (1)
        ;
}


class EXTI15_Handler2: public EXTI_IRQ_Handler
{
public:
    //toggles LED on pin25 every second
    void HandleInterrupt(EXTI* exti)
    {
        GPIOB::GetInstance()->TogglePin(GPIO_PIN12);
    }
};

static void exti_input_pwm_test()
{
    GPIOB::GetInstance()->EnablePeripheralClock(true);
    //LED on pin25
    GPIOB::GetInstance()->SetupGPIO_OutPP(GPIO_PIN12);

    //Output PWM signal with 1 sec period on pin10 
    //Connect pin10 to the EXTI input on pin28
    generatePWM();

    RCC::GetInstance()->EnableAFIO(true);
    
    //Remap LINE15 to PB15 : pin28
    AFIO_RemapEXTI(EXTI_LINE15, PORTB);

    
    EXTI15::GetInstance()->EnableInterruptRisingEdge(true);

    EXTI15_Handler2 handler;
    EXTI15::GetInstance()->SetIrqHandler(&handler);
    while (1)
    {
    }
}



int main()
{
    RCC* rcc = RCC::GetInstance();
    rcc->SetAHBPrescalerDiv4();
    rcc->SetAPB1PrescalerDiv1();
    rcc->SetAPB2PrescalerDiv1();
    rcc->SetADCPrescalerDiv2();
    rcc->EnableHSI(3);

    exti_input_pwm_test();
}
