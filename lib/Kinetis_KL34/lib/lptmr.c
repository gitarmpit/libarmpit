#include "lptmr.h"
#include "sim.h"

static void (*g_handler)(void);

void LPTMR0_IRQHandler()
{
	LPTMPR_ClearTCF();
    if (g_handler != NULL)
    {
        g_handler();
    }
}

BOOL LPTMPR_IsTCF()
{
	return (LPTMR_CSR & LPTMR_CSR_TCF) == LPTMR_CSR_TCF;
}

void LPTMPR_ClearTCF()
{
	LPTMR_CSR |=  LPTMR_CSR_TCF;
}

void LPTMPR_EnableInterrupt(BOOL enable)
{
    if (enable)
    {
    	LPTMR_CSR |= LPTMR_CSR_TIE;
        NVIC_SetEnable(LPTMR0_IRQn);
    }
    else
    {
    	LPTMR_CSR &= ~LPTMR_CSR_TIE;
        NVIC_ClearEnable(LPTMR0_IRQn);
    }

}

void LPTMPR_PinSelect(uint8_t pin)
{
	if (pin > 3)
	{
		while(1)
			;
	}

	LPTMR_CSR &= ~LPTMR_CSR_TPS_CLEARMASK;
	LPTMR_CSR |= (pin << 4);
}

void LPTMPR_EnableClock (BOOL enable)
{
	SIM_Enable_LPTMR(enable);
}

void LPTMPR_SetInterruptHandler(void (*handler)())
{
    g_handler = handler;
}

void LPTMPR_SelectMode_TimeCounter()
{
	LPTMR_CSR &= ~LPTMR_CSR_TMS;
}

void LPTMPR_SelectMode_PulseCounter()
{
	LPTMR_CSR |= LPTMR_CSR_TMS;
}

void LPTMPR_EnableCounter(BOOL enable)
{
    if (enable)
    {
    	LPTMR_CSR |= LPTMR_CSR_TEN;
    }
    else
    {
    	LPTMR_CSR &= ~LPTMR_CSR_TEN;
    }
}

void LPTMPR_SetCMR(uint16_t cmr)
{
	LPTMR_CMR = cmr;
}

void LPTMR_SetPrescaler(LPTMR_PRESC presc)
{
	LPTMR_PSR &= ~LPTMR_PSR_PRESCALE_CLEARMASK;
	LPTMR_PSR |= (uint8_t)presc << 3;
}


// When reading the CNR, the bus clock must be at least two times faster than the rate at
// which the LPTMR counter is incrementing, otherwise incorrect data may be returned
uint16_t LPTMR_GetCounterValue ()
{
	LPTMR_CNR = 0;
    return LPTMR_CNR;
}

void LPTMR_SelectClock_MCGIRCLK(uint32_t clockFreq)
{
	LPTMR_PSR &= ~ LPTMR_PSR_PCS_CLEARMASK;
	LPTMR_PSR |= LPTMR_PSR_PCS_MCGIRCLK;
	LPTMR_FREQ = clockFreq;
}
void LPTMR_SelectClock_LPO()
{
	LPTMR_PSR &= ~ LPTMR_PSR_PCS_CLEARMASK;
	LPTMR_PSR |= LPTMR_PSR_PCS_LPO;
	LPTMR_FREQ = 1000;
}
void LPTMR_SelectClock_ERCLK32K(uint32_t clockFreq)
{
	LPTMR_PSR &= ~ LPTMR_PSR_PCS_CLEARMASK;
	LPTMR_PSR |= LPTMR_PSR_PCS_ERCLK32K;
	LPTMR_FREQ = clockFreq;
}
void LPTMR_SelectClock_OSCERCLK(uint32_t clockFreq)
{
	LPTMR_PSR &= ~ LPTMR_PSR_PCS_CLEARMASK;
	LPTMR_PSR |= LPTMR_PSR_PCS_OSCERCLK;
	LPTMR_FREQ = clockFreq;
}

static void LPTMR_CalculateTimerValues (uint32_t us, uint16_t* count, uint16_t* presc)
{
    uint32_t tpmClk = LPTMR_FREQ;
    uint64_t count32 = (uint64_t)tpmClk  * (uint64_t)us / 1000000llu / 2;

    uint16_t max = 0xffff;

    *count = 0;
    *presc = 0;

    if (count32 == 0)
    {
        return;
    }

    while (count32 >= max && *presc < 16)
    {
        count32 /= 2;
        (*presc)++;
    }

    if (count32 > max)
    {
    	count32 = max;
    }

    *count = (uint16_t)count32;
}

void LPTMR_SetTimer_ms (uint32_t ms)
{
	uint16_t cmr;
	uint16_t presc;
	LPTMR_CalculateTimerValues(ms * 1000, &cmr, &presc);
	LPTMPR_SetCMR(cmr);
	LPTMR_SetPrescaler((LPTMR_PRESC)presc);
}

void LPTMR_SetTimer_sec (uint32_t sec)
{
	LPTMR_SetTimer_ms(sec * 1000);
}

void LPTMR_SetTimer_hours (uint32_t hours)
{
	LPTMR_SetTimer_sec (hours * 3600);
}
