#include "timer_cpp.h"
#include "gpio_cpp.h"
#include "RCC_Helper.h"
#include "GPIO_Helper.h"
#include "debug.h"

static void generatePWM_to_ESC2()
{
    GPIOA::GetInstance()->EnablePeripheralClock(true);
    GPIOA::GetInstance()->SetupGPIO_OutAltPP(GPIO_PIN0);
    TIM2* timer2 = TIM2::GetInstance();
    timer2->EnablePeripheralClock(true);
    TIMER_Channel* ch1 = timer2->GetChannel(1);
    ch1->SetupPWM1(20000, 1000); 
    ch1->EnableChannel(true);
    timer2->EnableCounter(true);
    delay(1000);
    while(1)
    {
        ch1->SetupPWM1(20000, 2000); 
        delay(1000);
        ch1->SetupPWM1(20000, 500); 
        delay(1000);
    }
}

static void test_roboArm() 
{
    GPIOA::GetInstance()->EnablePeripheralClock(true);
    GPIOA::GetInstance()->SetupGPIO_OutAltPP(GPIO_PIN0);
    GPIOA::GetInstance()->SetupGPIO_OutAltPP(GPIO_PIN1);
    GPIOA::GetInstance()->SetupGPIO_OutAltPP(GPIO_PIN2);

    TIM2* timer2 = TIM2::GetInstance();
    timer2->EnablePeripheralClock(true);

    TIMER_Channel* pivot = timer2->GetChannel(1);
    pivot->EnableChannel(true);

    //TIMER_Channel* arm = timer2->GetChannel(2);
    //arm->EnableChannel(true);

    //TIMER_Channel* hand = timer2->GetChannel(2);
    //hand->EnableChannel(true);
    timer2->EnableCounter(true);

    while(1)
    {
        pivot->SetupPWM1(20000, 1300); 
        delay(600);
        pivot->SetupPWM1(20000, 1450); 
        delay(600);
        pivot->SetupPWM1(20000, 1612); 
        delay(600);
        pivot->SetupPWM1(20000, 1450); 
        delay(1000);
    }
    
    while(1);

}

static void generatePWM_to_ESC()
{
    GPIOA::GetInstance()->EnablePeripheralClock(true);
    GPIOA::GetInstance()->SetupGPIO_OutAltPP(GPIO_PIN0);
    GPIOA::GetInstance()->SetupGPIO_OutAltPP(GPIO_PIN1);
    TIM2* timer2 = TIM2::GetInstance();
    timer2->EnablePeripheralClock(true);
    TIMER_Channel* ch1 = timer2->GetChannel(1);
    ch1->EnableChannel(true);
    TIMER_Channel* ch2 = timer2->GetChannel(2);
    ch2->EnableChannel(true);
    timer2->EnableCounter(true);
    
    ch1->SetupPWM1(20000, 2000); 
    ch2->SetupPWM1(20000, 2000); 
    delay(2000);
    
    ch1->SetupPWM1(20000, 700); 
    ch2->SetupPWM1(20000, 700); 

    delay(2000);

    ch1->SetupPWM1(20000, 1000); 
    ch2->SetupPWM1(20000, 1000); 

    delay(2000);

    ch1->SetupPWM1(20000, 2000); 
    ch2->SetupPWM1(20000, 2000); 


    while(1);
    
    while (1) 
    {
        for (int i = 0; i <= 800; i += 20)
        {
            ch1->SetupPWM1(20000, 1200 + i); 
            ch2->SetupPWM1(20000, 1200 + i); 
            delay(4000);
        }

        delay(4000);

        for (int i = 0; i <= 800; i += 20)
        {
            ch1->SetupPWM1(20000, 2000 - i); 
            ch2->SetupPWM1(20000, 2000 - i); 
            delay(4000);
        }


    }
}


//Generates PWM on pin10
static void generatePWM()
{
    GPIOA::GetInstance()->EnablePeripheralClock(true);
    GPIOA::GetInstance()->SetupGPIO_OutAltPP(GPIO_PIN0);
    TIM2* timer2 = TIM2::GetInstance();
    timer2->EnablePeripheralClock(true);
    TIMER_Channel* ch1 = timer2->GetChannel(1);
    ch1->SetupPWM1(20000, 1000); 
    ch1->EnableChannel(true);
    timer2->EnableCounter(true);

    delay(1000);
    ch1->SetupPWM1(20000, 1500); 
    while (1)
        ;
}

static void generatePWM2() 
{
    TIMER_With_Channels* timer = TIM1::GetInstance();
    GPIO_Helper::SetupTIM1_PA_8_9_10_11();
    //GPIO_Helper::SetupTIM2_PA_0_1_2_3();
    TIMER_Channel* ch1 = timer->GetChannel(1);
    TIMER_Channel* ch2 = timer->GetChannel(2);
    TIMER_Channel* ch3 = timer->GetChannel(3);
    TIMER_Channel* ch4 = timer->GetChannel(4);
    ch1->SetupPWM1(20000, 500); 
    ch1->EnableChannel(true);
    ch2->SetupPWM1(20000, 1000); 
    ch2->EnableChannel(true);
    ch3->SetupPWM1(20000, 1500); 
    ch3->EnableChannel(true);
    ch4->SetupPWM1(20000, 2000); 
    ch4->EnableChannel(true);
    timer->EnableCounter(true);
    while (1)
        ;
}


class PWM_Capture_Handler1 : public PWM_Capture_Handler
{
public:
    
    //The measured period and duty in us are passed to the handler. 
    //If worked correctly, should be 10000 / 4000 to match generated PWM
    virtual void Handle (uint32_t period_us, uint32_t duty_us)
    {
        volatile static int i = 0;
        ++i;
    }
};


class Update_Handler: public TIMER_Interrupt_Handler
{
private:
    TIMER_Channel* _ch;
  bool _isOn;
  uint32_t _cnt;
  uint32_t _maxOn;
    uint32_t _maxOff;
  uint32_t _max;

public:
    Update_Handler(TIMER_Channel* ch) 
    {
        _isOn = true;
        _ch = ch;
        _cnt = 0;
        _maxOn = 220;
        _maxOff = 200;
        _max = _maxOn;
        
    }

    
    void HandleInterrupt(bool isUpdate, bool isTrigger)
    {
        if (isUpdate)
        {
                   if (++_cnt == _max) 
                     {
                         _cnt = 0;
                         _isOn = !_isOn;
                         _ch->EnableChannel(_isOn);
                         if (_isOn) 
                         {
                             _max = _maxOn;
                         }
                         else 
                         {
                             _max = _maxOff;
                         }
                     }
        }
    }
};




//Generates 38KHz PWM carrier freq ans some random test signals to control an IR receiver 
//like TSOP4838 used by TV remotes
//The IR receiver should be connected to pin10
//Can connect an LED to the IR receiver collector and watch it blink
static void control_ir_receiver()
{
    GPIOA::GetInstance()->EnablePeripheralClock(true);
        
    GPIOA::GetInstance()->SetupGPIO_InFloat(GPIO_PIN6);  //pin16 PWM input

    
    PWM_Capture_Handler1 handler;
    TIM3* timer3 = TIM3::GetInstance();
    timer3->EnablePeripheralClock(true);
    TIMER_Channel* ch_in = timer3->GetChannel(1);  //TIM3 channe 1 : pin16 is used to capture PWM

    uint32_t input_period_us = 800;  //tells how fast to run the input counter, needs to match roughly the expected period of incoming PWM 
    ch_in->CaptureInputPWM (input_period_us, &handler);


        GPIOA::GetInstance()->SetupGPIO_OutAltPP(GPIO_PIN0);
    TIM2* timer2 = TIM2::GetInstance();
    timer2->EnablePeripheralClock(true);
    TIMER_Channel* ch1 = timer2->GetChannel(1);
    ch1->SetupPWM1(26, 13); //38KHZ 50% duty
    ch1->EnableChannel(true);
    
    Update_Handler uhandler(ch1);
    timer2->AddInterruptHandler(&uhandler);
    timer2->EnableUpdateInterrupt(true);   //enable timer update interrupt
        timer2->EnableNVICInterrupt(true);
    
    ch1->EnableChannel(true);
    uint32_t i = 0;
    

    /*
    while (1)
    {
        ch1->EnableChannel(true);
        timer2->EnableCounter(true);
        delay(60);
        timer2->EnableCounter(false);
        ch1->EnableChannel(false);
        delay(60);
        if (++i % 2 == 0)
        {
            delay(60);
        }            
        
    }
    */
        
    timer2->EnableCounter(true);
        while(1)
            ;
        
    while (1)
    {
        ch1->EnableChannel(true);
        delay(10);
        ch1->EnableChannel(false);
        delay(20);
        
    }


    
}




int main()
{
#if defined (STM32F1)   
    //RCC_EnableHSI_72Mhz_AHB_72Mhz_APB1_36MHz_APB2_72MHz();
    RCC_EnableHSI(true);
#else
    RCC_EnableHSI_100Mhz();
#endif  

    //generatePWM();

    generatePWM2();
    
    //generatePWM_to_ESC();
    //test_roboArm();
    control_ir_receiver();

}
