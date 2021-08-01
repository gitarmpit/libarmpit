#include "tpm.h"
#include "sim.h"

static void TPM_CalculateTimerValues (uint32_t us, uint16_t* count, uint16_t* presc);
static TPM TPM_list[] =
{
    { &SIM_SCGC6, SIM_SCGC6_TPM0, &TPM0_SC, &TPM0_CNT, &TPM0_MOD, &TPM0_STATUS, &TPM0_CONF, TPM0_CnCS_BASE, TPM0_CnV_BASE, TPM0_IRQn, NULL },
    { &SIM_SCGC6, SIM_SCGC6_TPM1, &TPM1_SC, &TPM1_CNT, &TPM1_MOD, &TPM1_STATUS, &TPM1_CONF, TPM1_CnCS_BASE, TPM1_CnV_BASE, TPM1_IRQn, NULL },
    { &SIM_SCGC6, SIM_SCGC6_TPM2, &TPM2_SC, &TPM2_CNT, &TPM2_MOD, &TPM2_STATUS, &TPM2_CONF, TPM2_CnCS_BASE, TPM2_CnV_BASE, TPM2_IRQn, NULL },
};

void TPM0_IRQHandler()
{
	TPM* tpm = &TPM_list[0];
	if (TPM_IsTOF(tpm))
	{
		TPM_ClearTOF(tpm);
		if (tpm->handler != NULL)
		{
			tpm->handler(tpm);
		}
	}
	for (uint8_t ch = 0; ch < 6; ++ch)
	{

	}
}

void TPM1_IRQHandler()
{
	TPM* tpm = &TPM_list[1];
	TPM_ClearTOF(tpm);
	if (tpm->handler != NULL)
	{
		tpm->handler(tpm);
	}
}

void TPM2_IRQHandler()
{
	TPM* tpm = &TPM_list[2];
	TPM_ClearTOF(tpm);
	if (tpm->handler != NULL)
	{
		tpm->handler(tpm);
	}
}


TPM* TPM_GetInstance(TPM_N n)
{
    return &TPM_list[(uint8_t)n];
}

TPM_Channel TPM_GetChannel(TPM* tpm, uint8_t channel)
{
	if (channel > 5)
	{
		while(1)
			;
	}

	TPM_Channel ch;
	ch.tpm = tpm;
	ch.TPM_CnSC = (volatile uint32_t*)(tpm->TPM_CnCS_BASE + channel*8);
	ch.TPM_CnV = (volatile uint32_t*)(tpm->TPM_CnV_BASE + channel*8);
	return ch;

}

void TPM_EnableClock (TPM* tpm, BOOL enable)
{
    SIM_Enable (tpm->SIM_enableReg, tpm->SIM_enableMask, enable);
}

void TPM_EnableDMA(TPM* tpm, BOOL enable)
{
	if (enable)
	{
		*tpm->TPM_SC |= TPM_SC_DMA;
	}
	else
	{
		*tpm->TPM_SC &= ~TPM_SC_DMA;
	}
}

// Time Overflow Flag
void TPM_ClearTOF(TPM* tpm)
{
	*tpm->TPM_SC |= TPM_SC_TOF;
}

BOOL TPM_IsTOF(TPM* tpm)
{
	return (*tpm->TPM_SC & TPM_SC_TOF);
}

void TPM_SetInterruptHandler(TPM* tpm, void (*handler)(TPM*))
{
	tpm->handler = handler;
}

void TPM_EnableInterrupt(TPM* tpm, BOOL enable)
{
	if (enable)
	{
		*tpm->TPM_SC |= TPM_SC_TOIE;
		NVIC_SetEnable(tpm->irq);
	}
	else
	{
		*tpm->TPM_SC &= ~TPM_SC_TOIE;
		NVIC_ClearEnable(tpm->irq);
	}
}

void TPM_EnableCounter(TPM* tpm, BOOL enable)
{
	*tpm->TPM_SC &= ~TPM_SC_CMOD_CLEARMASK;
	if (enable)
	{
		*tpm->TPM_SC |= TPM_SC_CMOD_ON;
	}
}

void TPM_SetPrescaler(TPM* tpm, TPM_PRESC presc)
{
	*tpm->TPM_SC &= ~TPM_SC_PS_CLEARMASK;
	*tpm->TPM_SC |= (uint8_t)presc;
}

void TPM_ClearCounter (TPM* tpm)
{
	*tpm->TPM_CNT = 0;
}

uint16_t TPM_GetCounterValue (TPM* tpm)
{
	return (uint16_t)*tpm->TPM_CNT;
}

void TPM_SetModulo(TPM* tpm, uint16_t mod)
{
	*tpm->TPM_MOD = mod;
}

static void TPM_CalculateTimerValues (uint32_t us, uint16_t* count, uint16_t* presc)
{
    uint32_t tpmClk = TPM_FREQ;
    uint64_t count32 = (uint64_t)tpmClk  * (uint64_t)us / 1000000llu;

    uint16_t max = 0xffff;

    *count = 0;
    *presc = 0;

    if (count32 == 0)
    {
        return;
    }


    while (count32 >= max && *presc < 7)
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

void TPM_Channel_SetupPWM(TPM_Channel* ch, uint32_t period_us, uint32_t duty_us)
{
    uint16_t period_count = 0;
    uint16_t presc = 0;

    TPM_CalculateTimerValues (period_us, &period_count, &presc);

    uint16_t duty_count = (uint16_t)((float)period_count * ((float)duty_us / (float)period_us));
	TPM_SetPrescaler(ch->tpm, (TPM_PRESC)presc);
	*ch->tpm->TPM_MOD = period_count;
	*ch->TPM_CnV = duty_count;
	TPM_Channel_EnablePWM(ch);
}

static uint8_t presc_table[] = { 1,2,4,8,16,32,64,128 };
void TPM_Channel_UpdatePWMDuty(TPM_Channel* ch, uint32_t duty_us)
{
    //if (_timer->IsCenterAlignedModeEnabled())
    //{
    //    duty_us /= 2;
    //}

    uint32_t tpmClk = TPM_FREQ;
    uint8_t presc = *ch->tpm->TPM_SC & 0x7;
    uint8_t div = presc_table[presc];
    uint16_t duty_count = (uint64_t)duty_us * (uint64_t)tpmClk / 1000000ull / (uint64_t)div;

	*ch->TPM_CnV = duty_count;
}


void TPM_Channel_ClearCHF(TPM_Channel* ch)
{

}
BOOL TPM_Channel_IsCHF(TPM_Channel* ch)
{

}
void TPM_Channel_EnableDMA(TPM_Channel* ch, BOOL enable)
{

}
void TPM_Channel_EnablePWM(TPM_Channel* ch)
{
	*ch->TPM_CnSC &= ~(0xf<<2);
	*ch->TPM_CnSC |= (TPM_CnCS_MSB | TPM_CnCS_ELSB);
}

void TPM_Channel_SetUpdatePeriod_us(TPM* tpm, uint32_t period_us)
{
    uint16_t period_count = 0;
    uint16_t presc = 0;

    TPM_CalculateTimerValues (period_us, &period_count, &presc);
	TPM_SetPrescaler(tpm, (TPM_PRESC)presc);
	*tpm->TPM_MOD = period_count;
	*tpm->TPM_CNT = 0;
}

void TPM_Channel_SetUpdatePeriod_ms(TPM* tpm, uint32_t period_ms)
{
	TPM_Channel_SetUpdatePeriod_us(tpm, period_ms*1000);
}

