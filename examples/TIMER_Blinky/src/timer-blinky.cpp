#include "timer_cpp.h"
#include "gpio_cpp.h"

class Update_Handler: public TIMER_Interrupt_Handler
{
public:

    //blink LED on pin25 every second  and count seconds
    void HandleInterrupt(bool isUpdate, bool isTrigger)
    {
        //update flag is cleared automatically before entering the handler, 
        //so no need to clear any flags here
        if (isUpdate)
        {
            volatile static uint8_t i = 0;
            ++i;

            GPIOB::GetInstance()->TogglePin(GPIO_PIN12);
        }
    }
};

void test_counter()
{
    GPIOB::GetInstance()->EnablePeripheralClock(true);
    GPIOB::GetInstance()->SetupGPIO_OutPP(GPIO_PIN12);  //pin25

    Update_Handler handler;
    TIM2* t = TIM2::GetInstance();
    t->EnablePeripheralClock(true);
    t->EnableNVICInterrupt(true);     //enable global system interrupts (core)
    t->AddInterruptHandler(&handler);
    t->EnableUpdateInterrupt(true);   //enable timer update interrupt
    t->SetCounterValue(0);            //optional
    t->SetUpdatePeriod_us(1000000);   //1 second 
    t->EnableCounter(true);           //start counting

    while (1)
        ;

}

