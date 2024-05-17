#include "bkp.h"
#include "pwr.h"
#include "rtc_cpp.h"
#include "gpio_cpp.h"
#include "exti_cpp.h"
#include "systick.h"
#include "RCC_Helper.h"
#include "debug.h"

class TamperHandler: public BKP_TamperInterrupt_Handler
{
public:
    virtual void HandleInterrupt(bool isEvent)
    {

    }
};

static void systick_handler(void)
{
    GPIOB::GetInstance()->TogglePin(GPIO_PIN12);
}

class PowerModeTester: public RTC_Interrupt_Handler
{

private:

    GPIO_PORT *gpioPort;
    PWR *pwr;
    BKP *bkp;
    RTC *rtc;
    EXTI17 *exti17;

    void StartClock()
    {
        RCC_EnableHSI_24Mhz();
    }

    void InitSystick(uint32_t ms)
    {
        uint32_t clock = RCC_GetSystemClock();
        uint32_t count = clock * ms / 1000 * 8;
        SystickEnable(count, true, true, systick_handler);
    }

    void InitGPIO()
    {
        gpioPort->EnablePeripheralClock(true);
        gpioPort->SetupGPIO_OutPP(GPIO_PIN12);
        gpioPort->SetupGPIO_OutPP(GPIO_PIN14);
        gpioPort->SetupGPIO_OutPP(GPIO_PIN13);
    }

    void InitRTC(uint32_t second, uint32_t alarm)
    {
        RCC_EnableLSI();
        rtc->SetClockSourceLSI();
        rtc->SetPrescaler_ms(second);
        rtc->SetAlarm_ms(alarm);
        rtc->EnableClock(true);
        if (alarm != 0)
        {
            rtc->EnableAlarmInterrupt(true, this);
        }
    }

public:

    PowerModeTester()
    {
        gpioPort = GPIOB::GetInstance();
        pwr = PWR::GetInstance();
        bkp = BKP::GetInstance();
        rtc = RTC::GetInstance();
        exti17 = EXTI17::GetInstance();
    }

    void TestStop()
    {
        StartClock();
        InitGPIO();

        pwr->EnablePeripheralClock(true);
        bkp->EnablePeripheralClock(true);
        pwr->EnableBKPWriteAccess(true);

        InitSystick(200);

        InitRTC(1000, 10000);

        EXTI17::GetInstance()->EnableInterruptRisingEdge(true);
        while (1)
        {
            rtc->SetCounter(0);
            delay(2000);
            pwr->Stop(true, true);
            //clock will be restarted in the alarm int handler
            delay(1000);
            gpioPort->ResetPin(GPIO_PIN14);
            gpioPort->ResetPin(GPIO_PIN13);
        }
    }

    void TestStandBy()
    {
        StartClock();
        InitGPIO();

        pwr->EnablePeripheralClock(true);
        bkp->EnablePeripheralClock(true);
        pwr->EnableBKPWriteAccess(true);

        InitSystick(200);

        InitRTC(1000, 10000);

        if (pwr->IsSBF())
        {
            gpioPort->SetPin(GPIO_PIN14);
            pwr->ClearSBF();
        }
        delay(2000);
        pwr->Standby(true, true);
    }

    void TestSleep()
    {
        StartClock();
        InitGPIO();

        pwr->EnablePeripheralClock(true);
        bkp->EnablePeripheralClock(true);
        pwr->EnableBKPWriteAccess(true);

        InitSystick(200);

        InitRTC(10000, 0);
        rtc->EnableSecondInterrupt(true);

        while (1)
        {
            delay(2000);
            gpioPort->ResetPin(GPIO_PIN14);
            pwr->Sleep(true);
            if (pwr->IsWUF())
            {
                pwr->ClearWUF();
                gpioPort->SetPin(GPIO_PIN14);
            }
        }
    }

    void HandleInterrupt(bool isOverrun, bool isAlarm, bool isSecond)
    {
        if (isAlarm && pwr->IsWUF())
        {
            pwr->ClearWUF();
            gpioPort->SetPin(GPIO_PIN14);
            StartClock();
        }
        if (exti17->IsPending())
        {
            gpioPort->SetPin(GPIO_PIN13);
            exti17->ClearPending();
        }
    }

};

void bkp_pwr_test()
{
    PWR *pwr = PWR::GetInstance();
    pwr->EnablePeripheralClock(true);

    BKP *bkp = BKP::GetInstance();
    bkp->EnablePeripheralClock(true);

    pwr->EnableBKPWriteAccess(true);

    bkp->EnableTamperPin(true);

    TamperHandler tamperHandler;
    bkp->EnableTamperInterrupt(true, &tamperHandler);

    bkp->WriteBackupData(0xbeef, 1);
    uint16_t d = bkp->ReadBackupData(1);

    bkp->WriteBackupData(0xbeef, 42);
    d = bkp->ReadBackupData(42);

    bkp->ResetBackupDomain();
    d = bkp->ReadBackupData(42);

    bkp->WriteBackupData(0xbeef, 2);
    d = bkp->ReadBackupData(2);
    //trigger tamper here
    d = bkp->ReadBackupData(2);

    bkp->EnableTamperPin(false);
    bkp->EnableTamperPin(true);
    bkp->WriteBackupData(0xbeef, 3);
    d = bkp->ReadBackupData(3);

}

int main()
{
    //Clocks initialized inside the test
    bkp_pwr_test();
}
