#include "RCC_Helper.h"
#include "GPIO_Helper.h"
#include "IRRemote/IRSend.h"
#include "timer_cpp.h"
#include "rcc.h"
#include "debug.h"

static void test()
{
    TIMER_With_Channels* timer = TIM3::GetInstance();
    timer->EnablePeripheralClock(true);
    //GPIOA::GetInstance()->EnablePeripheralClock(true);
    GPIOB::GetInstance()->EnablePeripheralClock(true);
    //GPIO_PIN* irSendPin = GPIOA::GetInstance()->GetPin(GPIO_PIN6);
    //GPIO_PIN* ch1 = GPIOA::GetInstance()->GetPin(GPIO_PIN7);
    GPIO_PIN* irSendPin = GPIOB::GetInstance()->GetPin(GPIO_PIN0);
    //GPIO_PIN* ch3 = GPIOB::GetInstance()->GetPin(GPIO_PIN1);
    irSendPin->SetupGPIO_OutAltPP();
    //ch1->SetupGPIO_OutAltPP();
    //ch2->SetupGPIO_OutAltPP();
    //ch3->SetupGPIO_OutAltPP();

    TIMER_Channel* ch = timer->GetChannel(3);
    ch->SetupPWM1(26, 13); //38KHZ 50% duty
    timer->EnableCounter(true);
    
    /*
    
     while (1)
    {
        ch->EnableChannel(true);
        delay(1);
        ch->EnableChannel(false);
        delay(20);
        
    }
    */
    
    IRSend irSend (timer->GetChannel(3));

    while(1)
    {
        irSend.SendSAMSUNG(0xE0E0D02F, 32);
        irSend.SendSAMSUNG(0xE0E0D02F, 32);
        irSend.SendSAMSUNG(0xE0E0D02F, 32);
        delay(20);
    }

}

int main()
{
#if defined (STM32F1)   
    RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
#else
    RCC_EnableHSI_168Mhz();
#endif  

    test();

}
