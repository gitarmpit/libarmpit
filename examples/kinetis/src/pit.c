#include "pit.h"
#include "nvic.h"


static PIT PIT_list[] =
{
    {
      (volatile uint32_t*)(PIT_CVAL_BASE + 16 * 1),
      (volatile uint32_t*)(PIT_LVAL_BASE + 16 * 1),
	  (volatile uint32_t*)(PIT_TCTRL_BASE + 16 * 1),
	  (volatile uint32_t*)(PIT_TFLG_BASE + 16 * 1),
	  FALSE,
	  NULL,
	  NULL
    },
    {
      (volatile uint32_t*)(PIT_CVAL_BASE + 16 * 2),
      (volatile uint32_t*)(PIT_LVAL_BASE + 16 * 2),
	  (volatile uint32_t*)(PIT_TCTRL_BASE + 16 * 2),
	  (volatile uint32_t*)(PIT_TFLG_BASE + 16 * 2),
	  FALSE,
	  NULL,
	  NULL
    },
};

static uint8_t PIT_table_size;

void PIT_IRQHandler()
{
	if (PIT_table_size == 0)
	{
		PIT_table_size = sizeof(PIT_list) / sizeof (PIT);
	}

	for (int i = 0; i < PIT_table_size; ++i)
	{
		PIT* pit = &PIT_list[i];
		if (pit->enabled && PIT_IsInterrupt(pit))
		{
			PIT_ClearInterrupt(pit);
			if (pit->interrupt_handler != NULL)
			{
				pit->interrupt_handler(pit->ctx);
			}
		}
	}
}

PIT* PIT_GetInstance(PIT_N pitN)
{
	return &PIT_list[(uint8_t)pitN];
}

/*
PIT PIT_GetInstance(PIT_N pitN)
{
	PIT pit;
	pit.PIT_CVAL  =  (volatile uint32_t*)(PIT_CVAL_BASE + 16 * (int)pitN);
	pit.PIT_LVAL  =  (volatile uint32_t*)(PIT_LVAL_BASE + 16 * (int)pitN);
	pit.PIT_TCTRL =  (volatile uint32_t*)(PIT_TCTRL_BASE + 16 * (int)pitN);
	pit.PIT_TFLG  =  (volatile uint32_t*)(PIT_TFLG_BASE + 16 * (int)pitN);
	return pit;
}
*/

void PIT_EnableClock(BOOL enable)
{
	if (enable)
	{
		PIT_MCR &= ~PIT_MCR_MDIS;
	}
	else
	{
		PIT_MCR |= PIT_MCR_MDIS;
	}
}

void PIT_EnableFreeze(BOOL enable)
{
	if (enable)
	{
		PIT_MCR |= PIT_MCR_FRZ;
	}
	else
	{
		PIT_MCR &= ~PIT_MCR_FRZ;
	}
}

void     PIT_Chain(PIT* pit)
{
	*pit->PIT_TCTRL |= PIT_TCTRL_CHN;
}

void     PIT_SetPeriod_us(PIT* pit, uint32_t us)
{
	uint32_t val = (uint64_t)us * BUS_FREQ/1000000;
	*pit->PIT_LVAL = val;

}


void     PIT_SetLoadValue(PIT* pit, uint32_t val)
{
	*pit->PIT_LVAL = val;
}
uint32_t PIT_GetCurrentValue(PIT* pit)
{
	return *pit->PIT_CVAL;
}

void     PIT_SetInterruptHandler (PIT* pit, void (*interrupt_handler)(void*))
{
	pit->interrupt_handler = interrupt_handler;
}

void     PIT_EnableInterrupt(PIT* pit, BOOL enable)
{
	if (enable)
	{
		pit->enabled = TRUE;
		*pit->PIT_TCTRL |= PIT_TCTRL_TIE;
		NVIC_SetEnable(PIT_IRQn);
	}
	else
	{
		pit->enabled = FALSE;
		*pit->PIT_TCTRL &= ~PIT_TCTRL_TIE;
		NVIC_ClearEnable(PIT_IRQn);
	}
}
void     PIT_EnableTimer(PIT* pit, BOOL enable)
{
	if (enable)
	{
		*pit->PIT_TCTRL |= PIT_TCTRL_TEN;
	}
	else
	{
		*pit->PIT_TCTRL &= ~PIT_TCTRL_TEN;
	}

}
BOOL     PIT_IsInterrupt(PIT* pit)
{
	return *pit->PIT_TFLG & PIT_TFLG_TIF;
}
void     PIT_ClearInterrupt(PIT* pit)
{
	*pit->PIT_TFLG |= PIT_TFLG_TIF;
}

