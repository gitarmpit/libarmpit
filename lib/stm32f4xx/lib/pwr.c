#include "pwr.h"

#include "system_control.h"
#include "rcc.h"

void PWR_EnablePeripheralClock(BOOL enable)
{
    if (enable)
    {
        if (!RCC_IsEnabledPWR())
        {
            RCC_EnablePWR(TRUE);
        }
    }
    else
    {
        RCC_EnablePWR(FALSE);
    }
}

void PWR_DisableWriteProtection(BOOL disable)
{
    if (disable)
    {
        PWR_CR |= PWR_CR_DBP;
    }
    else
    {
        PWR_CR &= ~PWR_CR_DBP;
    }
}

void PWR_Sleep(BOOL isWFI, BOOL sleepOnExit)
{
    SCB_SetSleepDeep(FALSE);
    SCB_SetSleepOnExit(sleepOnExit);
    if (isWFI)
        __asm ("WFI");
    else
        __asm ("WFE");

}

void PWR_Stop(BOOL isLowPower, BOOL isWFI, BOOL sleepOnExit)
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
    SCB_SetSleepDeep(FALSE);
    SCB_SetSleepOnExit(sleepOnExit);

    if (isWFI)
        __asm ("WFI");
    else
        __asm ("WFE");

}

void PWR_Standby(BOOL isWFI, BOOL enableWakeupPin, BOOL sleepOnExit)
{
    PWR_CR |= PWR_CR_PDDS;
    PWR_CSR &= ~PWR_CSR_WUF;

    PWR_EnableWakeupPin(enableWakeupPin);

    SCB_SetSleepDeep(TRUE);
    SCB_SetSleepOnExit(sleepOnExit);
    if (isWFI)
        __asm ("WFI");
    else
        __asm ("WFE");

}

void PWR_EnableWakeupPin(BOOL enable)
{
    if (enable)
        PWR_CSR |= PWR_CSR_EWUP;
    else
        PWR_CSR &= ~PWR_CSR_EWUP;
}

BOOL PWR_IsSBF()
{
    return PWR_CSR & PWR_CSR_SBF;
}

void PWR_ClearSBF()
{
    PWR_CR |= PWR_CR_CSBF;
}

BOOL PWR_IsWUF()
{
    return PWR_CSR & PWR_CSR_WUF;
}

void PWR_ClearWUF()
{
    PWR_CR |= PWR_CR_CWUF;
}
