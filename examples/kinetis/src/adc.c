#include "adc.h"
#include "sim.h"

static ADC ADC_list[] =
{
    {
            &SIM_SCGC6, SIM_SCGC6_ADC0, &ADC0_SC1A, &ADC0_SC1B, &ADC0_CFG1, &ADC0_CFG2, &ADC0_RA, &ADC0_RB,
            &ADC0_SC2, &ADC0_SC3, ADC0_IRQn, NULL
    },

};

void ADC0_IRQHandler()
{
	ADC* adc = &ADC_list[0];
	if (adc->handler)
	{
		adc->handler(adc);
	}
}

ADC* ADC_GetInstance(ADC_N n)
{
    return &ADC_list[(uint8_t)n];
}


void ADC_EnableClock(ADC* adc, BOOL enable)
{
	   SIM_Enable (adc->SIM_enableReg, adc->SIM_enableMask, enable);
}

ADC_Channel ADC_GetChannel(ADC* adc, ADC_CH_N chN)
{
    ADC_Channel ch;
    ch.adc = adc;

    switch (chN)
    {
    	case ADC_CH_SE0:
    	case ADC_CH_SE1:
    	case ADC_CH_SE2:
    	case ADC_CH_SE3:
    	case ADC_CH_SE4A:
    	case ADC_CH_SE5A:
    	case ADC_CH_SE6A:
    	case ADC_CH_SE7A:
    	case ADC_CH_SE8:
    	case ADC_CH_SE9:
    	case ADC_CH_SE11:
    	case ADC_CH_SE12:
    	case ADC_CH_SE13:
    	case ADC_CH_SE14:
    	case ADC_CH_SE15:
    	case ADC_CH_SE23:
    	case ADC_CH_TEMP:
    	case ADC_CH_VREFSH:
    	case ADC_CH_VREFSL:
    	case ADC_CH_BANDGAP:
    	{
    		ch.ADC_SC1 = adc->ADC_SC1A;
    		ch.ADC_R = adc->ADC_RA;
    		ch.ch_no = (uint8_t) chN;
    		ch.is_A = TRUE;
    		break;
    	}
    	case ADC_CH_SE4B:
    	case ADC_CH_SE5B:
    	case ADC_CH_SE6B:
    	case ADC_CH_SE7B:
    	{
    		ch.ADC_SC1 = adc->ADC_SC1B;
    		ch.ADC_R = adc->ADC_RB;
    		ch.ch_no = -((uint8_t)chN);
    		ch.is_A = FALSE;
    		break;
    	}
    };

    return ch;
}

BOOL ADC_IsConversionComplete (ADC_Channel* ch)
{
	return *ch->ADC_SC1 & ADC_SC1_COCO;
}

void ADC_EnableInterrupt(ADC_Channel* ch, BOOL enable)
{
	if (enable)
	{
		*ch->ADC_SC1 |= ADC_SC1_AIEN;
		NVIC_SetEnable(ch->adc->irq);
	}
	else
	{
		*ch->ADC_SC1 &= ~ADC_SC1_AIEN;
		NVIC_ClearEnable(ch->adc->irq);
	}
}

void ADC_SetInterruptHandler(ADC* adc, void(*handler)(ADC*))
{
	adc->handler = handler;
}

void ADC_SetChannel(ADC_Channel* ch)
{
	*ch->ADC_SC1 &= ~ADC_SC1_ADCH_CLEARMASK;
	*ch->ADC_SC1 |= ch->ch_no;
	if (ch->is_A)
	{
		*ch->adc->ADC_CFG2 &= ~ADC_CFG2_MUXSEL;
	}
	else
	{
		*ch->adc->ADC_CFG2 |= ADC_CFG2_MUXSEL;
	}
}

void ADC_SetClock_Div1(ADC* adc)
{
	*adc->ADC_CFG1 &= ~ADC_CFG1_ADIV_CLEARMASK;
	*adc->ADC_CFG1 |= ADC_CFG1_ADIV_DIV1;
}
void ADC_SetClock_Div2(ADC* adc)
{
	*adc->ADC_CFG1 &= ~ADC_CFG1_ADIV_CLEARMASK;
	*adc->ADC_CFG1 |= ADC_CFG1_ADIV_DIV2;
}
void ADC_SetClock_Div4(ADC* adc)
{
	*adc->ADC_CFG1 &= ~ADC_CFG1_ADIV_CLEARMASK;
	*adc->ADC_CFG1 |= ADC_CFG1_ADIV_DIV4;
}
void ADC_SetClock_Div8(ADC* adc)
{
	*adc->ADC_CFG1 &= ~ADC_CFG1_ADIV_CLEARMASK;
	*adc->ADC_CFG1 |= ADC_CFG1_ADIV_DIV8;
}
void ADC_SetMode_8bit(ADC* adc)
{
	*adc->ADC_CFG1 &= ~ADC_CFG1_MODE_CLEARMASK;
	*adc->ADC_CFG1 |= ADC_CFG1_MODE_8BIT;
}
void ADC_SetMode_10bit(ADC* adc)
{
	*adc->ADC_CFG1 &= ~ADC_CFG1_MODE_CLEARMASK;
	*adc->ADC_CFG1 |= ADC_CFG1_MODE_10BIT;
}
void ADC_SetMode_12bit(ADC* adc)
{
	*adc->ADC_CFG1 &= ~ADC_CFG1_MODE_CLEARMASK;
	*adc->ADC_CFG1 |= ADC_CFG1_MODE_12BIT;
}

void ADC_SetClockSource_Bus(ADC* adc)
{
	*adc->ADC_CFG1 &= ~ADC_CFG1_ADICLK_CLEARMASK;
	*adc->ADC_CFG1 |= ADC_CFG1_ADICLK_BUS;
}
void ADC_SetClockSource_BusDiv2(ADC* adc)
{
	*adc->ADC_CFG1 &= ~ADC_CFG1_ADICLK_CLEARMASK;
	*adc->ADC_CFG1 |= ADC_CFG1_ADICLK_BUS_DIV2;
}
void ADC_SetClockSource_Alt(ADC* adc)
{
	*adc->ADC_CFG1 &= ~ADC_CFG1_ADICLK_CLEARMASK;
	*adc->ADC_CFG1 |= ADC_CFG1_ADICLK_ALTCLK;
}
void ADC_SetClockSource_Async(ADC* adc)
{
	*adc->ADC_CFG1 &= ~ADC_CFG1_ADICLK_CLEARMASK;
	*adc->ADC_CFG1 |= ADC_CFG1_ADICLK_ADACK;
}

void ADC_SetHighSpeedConversion(ADC* adc, BOOL highSpeed)
{
	if (highSpeed)
	{
		*adc->ADC_CFG2 |= ADC_CFG2_ADHSC;
	}
	else
	{
		*adc->ADC_CFG2 &= ~ADC_CFG2_ADHSC;
	}
}

void ADC_SetLongConversion_20(ADC* adc)
{
	*adc->ADC_CFG1 |= ADC_CFG1_ADLSMP;
	*adc->ADC_CFG2 &= ~ADC_CFG2_ADLSTS_CLEARMASK;
	*adc->ADC_CFG2 |= ADC_CFG2_ADLSTS_20;
}
void ADC_SetLongConversion_12(ADC* adc)
{
	*adc->ADC_CFG1 |= ADC_CFG1_ADLSMP;
	*adc->ADC_CFG2 &= ~ADC_CFG2_ADLSTS_CLEARMASK;
	*adc->ADC_CFG2 |= ADC_CFG2_ADLSTS_12;
}

void ADC_SetLongConversion_6(ADC* adc)
{
	*adc->ADC_CFG1 |= ADC_CFG1_ADLSMP;
	*adc->ADC_CFG2 &= ~ADC_CFG2_ADLSTS_CLEARMASK;
	*adc->ADC_CFG2 |= ADC_CFG2_ADLSTS_6;
}

void ADC_SetLongConversion_2(ADC* adc)
{
	*adc->ADC_CFG1 |= ADC_CFG1_ADLSMP;
	*adc->ADC_CFG2 &= ~ADC_CFG2_ADLSTS_CLEARMASK;
	*adc->ADC_CFG2 |= ADC_CFG2_ADLSTS_2;
}

void ADC_SetShortConversion(ADC* adc)
{
	*adc->ADC_CFG1 &= ~ADC_CFG1_ADLSMP;
}

void ADC_EnableHardwareTrigger(ADC* adc, BOOL hardware)
{
	if (hardware)
	{
		*adc->ADC_SC2 |= ADC_SC2_ADTRG;
	}
	else
	{
		*adc->ADC_SC2 &= ~ADC_SC2_ADTRG;
	}
}

void ADC_EnableDMA(ADC* adc, BOOL enable)
{
	if (enable)
	{
		*adc->ADC_SC2 |= ADC_SC2_DMAEN;
	}
	else
	{
		*adc->ADC_SC2 &= ~ADC_SC2_DMAEN;
	}
}

void ADC_SetHardwareAveraging_0(ADC* adc)
{
	*adc->ADC_SC3 &= ~ADC_SC3_AVGE;
}
void ADC_SetHardwareAveraging_4(ADC* adc)
{
	*adc->ADC_SC3 |= ADC_SC3_AVGE;
	*adc->ADC_SC3 &= ~ADC_SC3_AVGS_CLEARMASK;
	*adc->ADC_SC3 |= ADC_SC3_AVGS_4;
}
void ADC_SetHardwareAveraging_8(ADC* adc)
{
	*adc->ADC_SC3 |= ADC_SC3_AVGE;
	*adc->ADC_SC3 &= ~ADC_SC3_AVGS_CLEARMASK;
	*adc->ADC_SC3 |= ADC_SC3_AVGS_8;
}
void ADC_SetHardwareAveraging_16(ADC* adc)
{
	*adc->ADC_SC3 |= ADC_SC3_AVGE;
	*adc->ADC_SC3 &= ~ADC_SC3_AVGS_CLEARMASK;
	*adc->ADC_SC3 |= ADC_SC3_AVGS_16;
}
void ADC_SetHardwareAveraging_32(ADC* adc)
{
	*adc->ADC_SC3 |= ADC_SC3_AVGE;
	*adc->ADC_SC3 &= ~ADC_SC3_AVGS_CLEARMASK;
	*adc->ADC_SC3 |= ADC_SC3_AVGS_32;
}

BOOL ADC_EnableCalibration(ADC* adc)
{
	*adc->ADC_CFG2 &= ~ADC_CFG2_MUXSEL;
	// *adc->ADC_SC3 |= ADC_SC3_CALF;
	*adc->ADC_SC3 |= ADC_SC3_CAL;
	BOOL rc = FALSE;
	while (1)
	{
		if(*adc->ADC_SC1A & ADC_SC1_COCO)
		{
			rc = TRUE;
			break;
		}
		else if (*adc->ADC_SC3 & ADC_SC3_CALF)
		{
			*adc->ADC_SC3 |= ADC_SC3_CALF;
			rc = FALSE;
			break;
		}
	}

	return rc;

}

void ADC_EnableContinuousConversion(ADC* adc, BOOL enable)
{
	if (enable)
	{
		*adc->ADC_SC3 |= ADC_SC3_ADCO;
	}
	else
	{
		*adc->ADC_SC3 &= ~ADC_SC3_ADCO;
		*adc->ADC_SC1A |= ADC_SC1_ADCH_DISABLE;
		*adc->ADC_SC1B |= ADC_SC1_ADCH_DISABLE;
	}
}


uint16_t ADC_SingleConversion(ADC_Channel* ch)
{
	while (*ch->adc->ADC_SC2 & ADC_SC2_ADACT)
		;

	*ch->ADC_SC1 |= ADC_SC1_COCO;

	while (!(*ch->ADC_SC1 & ADC_SC1_COCO))
		;

	return *ch->ADC_R;
}
