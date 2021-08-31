#include "pmc.h"

BOOL PMC_Is_LVDF()
{
	return PMC_LVDSC1 & PMC_LVDSC1_LVDF;
}

void PMC_EnableInterrupt_LVD(BOOL enable)
{
	if (enable)
	{
		PMC_LVDSC1 |= PMC_LVDSC1_LVDIE;
	}
	else
	{
		PMC_LVDSC1 &= ~PMC_LVDSC1_LVDIE;
	}
}

void PMC_Clear_LVDF()
{
	PMC_LVDSC1 |= PMC_LVDSC1_LVDACK;
}

void PMC_EnableResetOnLVD(BOOL enable)
{
	if (enable)
	{
		PMC_LVDSC1 |= PMC_LVDSC1_LVDRE;
	}
	else
	{
		PMC_LVDSC1 &= ~PMC_LVDSC1_LVDRE;
	}

}

// 1.6V
void PMC_Select_LVDL()
{
	PMC_LVDSC1 &= ~PMC_LVDSC1_LVDV_CLEARMASK;
	PMC_LVDSC1 |= PMC_LVDSC1_LVDV_LVDL;
}

// 2.6V
void PMC_Select_LVDH()
{
	PMC_LVDSC1 &= ~PMC_LVDSC1_LVDV_CLEARMASK;
	PMC_LVDSC1 |= PMC_LVDSC1_LVDV_LVDH;
}

BOOL PMC_Is_LVWF()
{
	return PMC_LVDSC2 & PMC_LVDSC2_LVWF;
}

void PMC_EnableInterrupt_LVW(BOOL enable)
{
	if (enable)
	{
		PMC_LVDSC2 |= PMC_LVDSC2_LVWIE;
	}
	else
	{
		PMC_LVDSC2 &= ~PMC_LVDSC2_LVWIE;
	}
}

void PMC_Clear_LVWF()
{
	PMC_LVDSC2 &= ~PMC_LVDSC2_LVWV_CLEARMASK;
	PMC_LVDSC2 |= PMC_LVDSC2_LVWACK;
}

// 2.7V
void PMC_Select_LVWL()
{
	PMC_LVDSC2 &= ~PMC_LVDSC2_LVWV_CLEARMASK;
	PMC_LVDSC2 |= PMC_LVDSC2_LVWV_LOW;
}
// 2.8V
void PMC_Select_LVWM1()
{
	PMC_LVDSC2 &= ~PMC_LVDSC2_LVWV_CLEARMASK;
	PMC_LVDSC2 |= PMC_LVDSC2_LVWV_MID1;
}
// 2.9V
void PMC_Select_LVWM2()
{
	PMC_LVDSC2 &= ~PMC_LVDSC2_LVWV_CLEARMASK;
	PMC_LVDSC2 |= PMC_LVDSC2_LVWV_MID2;
}
// 3.0V
void PMC_Select_LVWH()
{
	PMC_LVDSC2 &= ~PMC_LVDSC2_LVWV_CLEARMASK;
	PMC_LVDSC2 |= PMC_LVDSC2_LVWV_HIGH;
}

void PMC_EnableBG(BOOL enable)
{
	if (enable)
	{
		PMC_REGSC |= PMC_REGSC_BGEN;
	}
	else
	{
		PMC_REGSC &= ~PMC_REGSC_BGEN;
	}
}

void PMC_EnableBGB(BOOL enable)
{
	if (enable)
	{
		PMC_REGSC |= PMC_REGSC_BGBE;
	}
	else
	{
		PMC_REGSC &= ~PMC_REGSC_BGBE;
	}

}

BOOL PMC_Is_ACKISO()
{
	return PMC_REGSC & PMC_REGSC_ACKISO;
}

void PMC_Clear_ACKISO()
{
	PMC_REGSC |= PMC_REGSC_ACKISO;
}



