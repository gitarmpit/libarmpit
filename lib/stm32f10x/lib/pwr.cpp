


#include "pwr.h"

#include "system_control.h"
#include "rcc.h"


void PWR::EnableBKPWriteAccess(bool enable)
{
    if (enable)
    {
        PWR_CR |= PWR_CR_DBP;
    }
    else
    {
        PWR_CR &= ~PWR_CR_DBP;
    }
}

void PWR::Sleep(bool isWFI, bool sleepOnExit)
{
    SCB_SetSleepDeep(false);
    SCB_SetSleepOnExit(sleepOnExit);
    if (isWFI)
        __asm ("WFI");
    else
        __asm ("WFE");

}

void PWR::Stop(bool isLowPower, bool isWFI, bool sleepOnExit)
{
    PWR_CR &= ~PWR_CR_PDDS;
    if (isLowPower)
    {
        PWR_CR |= PWR_CR_LPDS;
    }
    else
    {
        PWR_CR &= ~PWR_CR_LPDS;
    }
    SCB_SetSleepDeep(false);
    SCB_SetSleepOnExit(sleepOnExit);

    if (isWFI)
        __asm ("WFI");
    else
        __asm ("WFE");

}

void PWR::Standby(bool isWFI, bool enableWakeupPin, bool sleepOnExit)
{
    PWR_CR |= PWR_CR_PDDS;
    PWR_CSR &= ~PWR_CSR_WUF;

    EnableWakeupPin(enableWakeupPin);

    SCB_SetSleepDeep(true);
    SCB_SetSleepOnExit(sleepOnExit);
    if (isWFI)
        __asm ("WFI");
    else
        __asm ("WFE");

}

void PWR::EnableWakeupPin(bool enable)
{
    if (enable)
        PWR_CSR |= PWR_CSR_EWUP;
    else
        PWR_CSR &= ~PWR_CSR_EWUP;
}

bool PWR::IsSBF()
{
    return PWR_CSR & PWR_CSR_SBF;
}

void PWR::ClearSBF()
{
    PWR_CR |= PWR_CR_CSBF;
}

bool PWR::IsWUF()
{
    return PWR_CSR & PWR_CSR_WUF;
}

void PWR::ClearWUF()
{
    PWR_CR |= PWR_CR_CWUF;
}

