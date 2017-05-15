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
