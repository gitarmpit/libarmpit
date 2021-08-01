#include "rtc_cpp.h"
#include "exti.h"
#include "pwr.h"
#include "bkp.h"
#include "gpio_cpp.h"

class My_RTC_Interrupt_Handler: public RTC_Interrupt_Handler
{

private:

    uint32_t alarmCount;
    uint32_t secondCount;

public:

    My_RTC_Interrupt_Handler()
    {
        alarmCount = 0;
        secondCount = 0;
    }

    void HandleInterrupt(bool isOverrun, bool isAlarm, bool isSecond)
    {
        static RTC* rtc = RTC::GetInstance();
        if (isAlarm)
        {
            ++alarmCount;
            rtc->SetCounter(0);
            rtc->SetAlarm_ms(2000);
            GPIOB::GetInstance()->TogglePin(GPIO_PIN13);
        }
        if (isSecond)
        {
            ++secondCount;
            GPIOB::GetInstance()->TogglePin(GPIO_PIN12);
        }
        if (isOverrun)
        {
            static volatile uint32_t tmp = 0;
        }
    }

};

static void rtc_test()
{

    GPIOB::GetInstance()->EnablePeripheralClock(true);
    GPIOB::GetInstance()->SetupGPIO_OutPP(GPIO_PIN12);  
    GPIOB::GetInstance()->SetupGPIO_OutPP(GPIO_PIN13);  


    My_RTC_Interrupt_Handler handler;
    RTC* rtc = RTC::GetInstance();
    rtc->SetClockSourceLSI();

    PWR::GetInstance()->EnablePeripheralClock(true);
    BKP::GetInstance()->EnablePeripheralClock(true);
    PWR::GetInstance()->EnableBKPWriteAccess(true);

    RCC::GetInstance()->EnableLSI();
    rtc->EnableClock(true);
    rtc->WaitRSF();
    rtc->SetCounter(0);
    rtc->SetPrescaler_ms(1000);
    rtc->SetAlarm_ms(2000);

    rtc->EnableAlarmInterrupt(true, &handler);
    rtc->EnableSecondInterrupt(true, &handler);
    rtc->EnableOverrunInterrupt(true, &handler);

    //rtc->OutputSecondPulse(true);
    //rtc->OutputAlarmdPulse(true);

    //EXTI17::GetInstance()->EnableInterruptFallingEdge(true);
    //EXTI17::GetInstance()->EnableInterruptRisingEdge(true);
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

    rtc_test();
}

