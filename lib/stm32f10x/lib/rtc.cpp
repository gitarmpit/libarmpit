

#include "rtc_cpp.h"

#include "nvic.h"
#include "pwr.h"
#include "bkp.h"
#include "exti_cpp.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef USE_RTC

void RTCAlarm_IRQHandler(void)
{
    //  if (EXTI17::GetInstance()->IsPending())
    //  {
    //      EXTI17::GetInstance()->ClearPending();
    //  }
    //  if (PWR::GetInstance()->IsWUF())
    //  {
    //      PWR::GetInstance()->ClearWUF();
    //  }
    RTC::GetInstance()->HandleInterrupt();
}

void RTC_IRQHandler(void)
{
    RTC::GetInstance()->HandleInterrupt();
}

#endif

#ifdef __cplusplus
}
#endif

void RTC::HandleInterrupt ()
{
    if (IsOverrun() && _overrunHandler)
    {
        ClearOverrun();
        _overrunHandler->HandleInterrupt(true, false, false);
    }

    if (IsAlarm() && _alarmHandler)
    {
        ClearAlarm();
        _alarmHandler->HandleInterrupt(false, true, false);
    }

    if (IsSecond() && _secondHandler)
    {
        ClearSecond();
        _secondHandler->HandleInterrupt(false, false, true);
    }
}

bool RTC::IsOverrun()
{
    return (RTC_CRL & RTC_CRL_OWF);
}
bool RTC::IsAlarm()
{
    return (RTC_CRL & RTC_CRL_ALRF);
}
bool RTC::IsSecond()
{
    return (RTC_CRL & RTC_CRL_SECF);
}


void RTC::ClearOverrun()
{
    EnterConfigurationMode();
    RTC_CRL &= ~RTC_CRL_OWF;
    ExitConfigurationMode();
}
void RTC::ClearAlarm()
{
    EnterConfigurationMode();
    RTC_CRL &= ~RTC_CRL_ALRF;
    ExitConfigurationMode();
}

void RTC::ClearSecond()
{
    EnterConfigurationMode();
    RTC_CRL &= ~RTC_CRL_SECF;
    ExitConfigurationMode();
}



void RTC::SetClockSourceHSE_DIV128()
{
    RCC_SetRTCClockSourceHSE_DIV128();
}
void RTC::SetClockSourceLSE()
{
    RCC_SetRTCClockSourceLSE();
}
void RTC::SetClockSourceLSI()
{
    RCC_SetRTCClockSourceLSI();
}

void RTC::EnableClock(bool enable)
{
    RCC_EnableRTC(enable);
}

void RTC::WaitRSF()
{
    RTC_CRL &= ~(RTC_CRL_RSF);
    //wait for the RSF bit before the first read
    while ((RTC_CRL & (RTC_CRL_RSF)) == 0)
        ;


}

void RTC::EnableOverrunInterrupt (bool enable, RTC_Interrupt_Handler* overrunHandler)
{
    EnterConfigurationMode();
    if (enable)
    {
        RTC_CRH |= RTC_CRH_OWIE;
        _overrunHandler = overrunHandler;
        NVIC_SetEnable(IRQn_RTC);
    }
    else
    {
        RTC_CRH &= ~RTC_CRH_OWIE;
    }
    ExitConfigurationMode();
}

void RTC::EnableAlarmInterrupt (bool enable, RTC_Interrupt_Handler* alarmHandler)
{
    EnterConfigurationMode();
    if (enable)
    {
        RTC_CRH |= RTC_CRH_ALRIE;
        _alarmHandler = alarmHandler;
        NVIC_SetEnable(IRQn_RTCAlarm);
        EXTI17::GetInstance()->EnableInterruptRisingEdge(true);
    }
    else
    {
        RTC_CRH &= ~RTC_CRH_ALRIE;
        NVIC_ClearEnable(IRQn_RTCAlarm);
        EXTI17::GetInstance()->EnableInterruptRisingEdge(false);
    }
    ExitConfigurationMode();
}

void RTC::EnableSecondInterrupt (bool enable, RTC_Interrupt_Handler* secondHandler)
{
    EnterConfigurationMode();
    if (enable)
    {
        RTC_CRH |= RTC_CRH_SECIE;
        _secondHandler = secondHandler;
        NVIC_SetEnable(IRQn_RTC);
    }
    else
    {
        RTC_CRH &= ~RTC_CRH_SECIE;
    }
    ExitConfigurationMode();
}

void RTC::EnterConfigurationMode()
{
    //To write in the RTC_PRL, RTC_CNT, RTC_ALR registers, the peripheral must enter
    //Configuration Mode. This is done by setting the CNF bit in the RTC_CRL register.

    //1. Poll RTOFF, wait until its value goes to �1�
    while ((RTC_CRL & RTC_CRL_RTOFF) == 0)
        ;

    //2.  Set the CNF bit to enter configuration mode
    RTC_CRL     |= RTC_CRL_CNF;

}
void RTC::ExitConfigurationMode()
{
    //4.  Clear the CNF bit to exit configuration mode
    RTC_CRL     &= ~RTC_CRL_CNF;

    //5.  Poll RTOFF, wait until its value goes to �1� to check the end of the write operation
    while ((RTC_CRL & RTC_CRL_RTOFF) == 0)
        ;

}

void RTC::SetPrescaler (uint32_t val)
{
    EnterConfigurationMode();
    RTC_PRLH = (val>>16) & 0xf;
    RTC_PRLL = val & 0xffff;
    ExitConfigurationMode();
}


void RTC::SetCounter (uint32_t val)
{
    EnterConfigurationMode();
    RTC_CNTH = val>>16;
    RTC_CNTL = val & 0xffff;
    ExitConfigurationMode();
}

uint32_t RTC::GetCounter()
{
    return RTC_CNT;
}

void RTC::SetAlarm (uint32_t val)
{
    EnterConfigurationMode();
    RTC_ALRH = val>>16;
    RTC_ALRL = val & 0xffff;
    ExitConfigurationMode();

}
void RTC::SetPrescaler_ms (uint32_t ms)
{
    uint32_t clockFreqHZ = RCC_GetRTC_CLK();
    uint32_t prescVal = clockFreqHZ * ms / 1000;
    SetPrescaler (prescVal - 1);
}
void RTC::SetAlarm_ms (uint32_t ms)
{
    uint32_t clockFreqHZ = RCC_GetRTC_CLK();
    uint32_t msLengh =  (RTC_PRL + 1) * 1000 / clockFreqHZ;
    SetAlarm (ms/msLengh);
}

void RTC::OutputAlarmPulse (bool enable)
{
    BKP::GetInstance()->OutputAlarmPulse(enable);
}
void RTC::OutputSecondPulse (bool enable)
{
    BKP::GetInstance()->OutputSecondPulse(enable);
}

