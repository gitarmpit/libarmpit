#include "exti.h"
#include "afio.h"
#include "gpio_cpp.h"
#include "rcc_cpp.h"

class EXTI15_Handler: public EXTI_IRQ_Handler
{
public:
    //toggles LED on button click
    void HandleInterrupt(EXTI* exti)
    {
        exti->EnableInterruptFallingEdge(false);
        GPIOB::GetInstance()->TogglePin(GPIO_PIN12);
        RCC::GetInstance()->Delay_ms(100);
        exti->EnableInterruptFallingEdge(true);
    }
};

static void exti_button_test()
{
    GPIOB::GetInstance()->EnablePeripheralClock(true);
    //LED on pin25
    GPIOB::GetInstance()->SetupGPIO_OutPP(GPIO_PIN12);

    //button on pin28
    GPIOB::GetInstance()->SetupGPIO_InPUPD(GPIO_PIN15);
    GPIOB::GetInstance()->SetPin(GPIO_PIN15);

    RCC::GetInstance()->EnableAFIO(true);
    
    //Remap LINE15 to PB15 : pin28
    AFIO_RemapEXTI(EXTI_LINE15, PORTB);

    
    EXTI15::GetInstance()->EnableInterruptFallingEdge(true);
    //EXTI15::GetInstance()->EnableInterruptRisingEdge(true);

    EXTI15_Handler handler;
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

    exti_button_test();
}
