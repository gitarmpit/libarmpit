#include "timer_cpp.h"
#include "gpio_cpp.h"
#include "rcc_helper.h"


volatile static uint32_t t1 = 0;
volatile static uint32_t t2 = 0;


class Update_Handler: public TIMER_Interrupt_Handler
{
public:

    //blink LED on pin22 every second  and count seconds
    virtual void HandleInterrupt(bool isUpdate, bool isTrigger)
    {
        //update flag is cleared automatically before entering the handler, 
        //so no need to clear any flags here
        if (isUpdate)
        {
            t1++;
            GPIOB::GetInstance()->TogglePin(GPIO_PIN11);
        }
    }
};



static void test_counter() 
{
    GPIOB::GetInstance()->EnablePeripheralClock(true);
    GPIOB::GetInstance()->SetupGPIO_OutPP(GPIO_PIN11);  //pin22
    GPIOB::GetInstance()->SetPin(GPIO_PIN11);

    Update_Handler handler;
    TIM2* t = TIM2::GetInstance();
    t->EnablePeripheralClock(true);
    t->EnableNVICInterrupt(true);     //enable global system interrupts (core)
    //NVIC_SetPriority (IRQn_TIM2, 15<<4);  //set lowest priority
    t->AddInterruptHandler(&handler);
    t->EnableUpdateInterrupt(true);   //enable timer update interrupt
    t->SetCounterValue(0);            //optional
    t->SetUpdatePeriod_us(1000000);   //1 second 
    t->EnableCounter(true);           //start counting

    GPIOB::GetInstance()->ResetPin(GPIO_PIN11);
   
    while(1)
        ;

}

int main()
{
 
    RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
    
    test_counter();
}
