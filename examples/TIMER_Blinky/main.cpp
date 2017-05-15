/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2015 igorS
 *  
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


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
