#include "mcg.h"

uint32_t CORE_FREQ = 20000000;

void MCG_Select_MCGOUTCLK_FLL_PLL()
{
	MCG_C1 &= ~MCG_C1_CLKS_CLEARMASK;
	MCG_C1 |= MCG_C1_CLKS_FLL_PLL;
}

void MCG_Select_MCGOUTCLK_IRC()
{
	MCG_C1 &= ~MCG_C1_CLKS_CLEARMASK;
	MCG_C1 |= MCG_C1_CLKS_IRC;
}

// 7. MSGOUTCLOCK = OSCCLOCK
void MCG_Select_MCGOUTCLK_OSCCLK()
{
	MCG_C1 &= ~MCG_C1_CLKS_CLEARMASK;
	MCG_C1 |= MCG_C1_CLKS_OSCCLK;
}

void MCG_Set_FRDIV(uint8_t val)
{
	if (val < 1 || val > 7)
	{
		while(1)
			;
	}

	MCG_C1 &= ~MCG_C1_FRDIV_CLEARMASK;
	MCG_C1 |= (val<<3);
}

void MCG_Set_FLL_IRC32K()
{
	MCG_C1 |= MCG_C1_IREFS;
}

void MCG_Set_FLL_OSCCLK()
{
	MCG_C1 &= ~MCG_C1_IREFS;
}

void MCG_Enable_MCGIRCLK (BOOL isOn)
{
	if (isOn)
		MCG_C1 |= MCG_C1_IRCLKEN;
	else
		MCG_C1 &= ~MCG_C1_IRCLKEN;
}

void  MCG_IRCStopEnable(BOOL isEnabled)
{
	if (isEnabled)
		MCG_C1 |= MCG_C1_REFSTEN;
	else
		MCG_C1 &= ~MCG_C1_REFSTEN;
}

void MCG_Set_FCFTRIM(BOOL isOn)
{
	if (isOn)
		MCG_C2 |= MCG_C2_FCFTRIM;
	else
		MCG_C2 &= ~MCG_C2_FCFTRIM;
}
void MCG_Set_OSC_Range_Low()
{
	MCG_C2 &= ~MCG_C2_RANGE0_CLEARMASK;
	MCG_C2 |= MCG_C2_RANGE0_LOW;
}
void MCG_Set_OSC_Range_High()
{
	MCG_C2 &= ~MCG_C2_RANGE0_CLEARMASK;
	MCG_C2 |= MCG_C2_RANGE0_HIGH;
}
void MCG_Set_OSC_Range_VeryHigh()
{
	MCG_C2 &= ~MCG_C2_RANGE0_CLEARMASK;
	MCG_C2 |= MCG_C2_RANGE0_VERYHIGH;
}
void MCG_Set_HighGain_OSC(BOOL isOn)
{
	if (isOn)
		MCG_C2 |= MCG_C2_HGO0;
	else
		MCG_C2 &= ~MCG_C2_HGO0;
}
void MCG_Set_ExternalClock_OSC()
{
	MCG_C2 |= MCG_C2_EREFS0;
}
void MCG_Set_ExternalClock_ERC()
{
	MCG_C2 &= ~MCG_C2_EREFS0;

}
void MCG_DisableFLLPLL_InLowPowerMode(BOOL isDisabled)
{
	if (isDisabled)
		MCG_C2 |= MCG_C2_LP;
	else
		MCG_C2 &= ~MCG_C2_LP;
}
void MCG_Set_IRC_Slow()
{
	MCG_C2 &= ~MCG_C2_IRCS;
}
void MCG_Set_IRC_Fast()
{
	MCG_C2 |= MCG_C2_IRCS;
}

void MCG_SetFLL_Freq_24(BOOL DMX32)
{
	MCG_C4 &= ~MCG_C4_DRST_DRS_CLEARMASK;
	MCG_C4 |= MCG_C4_DRST_DRS_24;
	if (DMX32)
		MCG_C4 |= MCG_C4_DMX32;
	else
		MCG_C4 &= ~MCG_C4_DMX32;

}
void MCG_SetFLL_Freq_48(BOOL DMX32)
{
	MCG_C4 &= ~MCG_C4_DRST_DRS_CLEARMASK;
	MCG_C4 |= MCG_C4_DRST_DRS_48;
	if (DMX32)
		MCG_C4 |= MCG_C4_DMX32;
	else
		MCG_C4 &= ~MCG_C4_DMX32;
}
void MCG_SetFLL_Freq_72(BOOL DMX32)
{
	MCG_C4 &= ~MCG_C4_DRST_DRS_CLEARMASK;
	MCG_C4 |= MCG_C4_DRST_DRS_72;
	if (DMX32)
		MCG_C4 |= MCG_C4_DMX32;
	else
		MCG_C4 &= ~MCG_C4_DMX32;
}
void MCG_SetFLL_Freq_96(BOOL DMX32)
{
	MCG_C4 &= ~MCG_C4_DRST_DRS_CLEARMASK;
	MCG_C4 |= MCG_C4_DRST_DRS_96;
	if (DMX32)
		MCG_C4 |= MCG_C4_DMX32;
	else
		MCG_C4 &= ~MCG_C4_DMX32;
}

void MCG_EnablePLLClock(BOOL enable)
{
	if (enable)
		MCG_C5 |= MCG_C5_PLLCLKEN0;
	else
		MCG_C5 &= ~MCG_C5_PLLCLKEN0;
}

void MCG_PLLStopEnable(BOOL enable)
{
	if (enable)
		MCG_C5 |= MCG_C5_PLLSTEN0;
	else
		MCG_C5 &= ~MCG_C5_PLLSTEN0;
}

void MCG_Set_PLL_DIV(uint8_t div)
{
	if (div < 1 || div > 25)
	{
		while(1)
			;
	}
	MCG_C5 &= ~MCG_C5_PRDIV0_CLEARMASK;
	MCG_C5 |= (div-1);
}

void MCG_Set_PLLS_PLL()
{
	MCG_C6 |= MCG_C6_PLLS;
}

void MCG_Set_PLLS_FLL()
{
	MCG_C6 &= ~MCG_C6_PLLS;
}

void MCG_Set_VCO_DIV(uint8_t mul)
{
	if (mul < 24 || mul > 55)
	{
		while(1)
			;
	}
	MCG_C6 &= ~MCG_C6_VDIV0_CLEARMASK;
	MCG_C6 |= (mul-24);
}


BOOL  MCG_Is_LOLS0()
{
	return MCG_S & MCG_S_LOLS0;
}
BOOL  MCG_Is_LOCK0()
{
	return MCG_S & MCG_S_LOCK0;
}
BOOL  MCG_Is_PLLST()
{
	return MCG_S & MCG_S_PLLST;
}
BOOL  MCG_Is_IREFST()
{
	return MCG_S & MCG_S_IREFST;
}
BOOL  MCG_Is_CLKST_FLL()
{
	return MCG_S & MCG_S_CLKST_FLL;
}
BOOL  MCG_Is_CLKST_IRC()
{
	return MCG_S & MCG_S_CLKST_IRC;
}
BOOL  MCG_Is_CLKST_OSCCLK()
{
	return MCG_S & MCG_S_CLKST_OSCCLK;
}
BOOL  MCG_Is_CLKST_PLL()
{
	return MCG_S & MCG_S_CLKST_PLL;
}
BOOL  MCG_Is_OSCINIT0()
{
	return MCG_S & MCG_S_OSCINIT0;
}
BOOL  MCG_Is_IRCST()
{
	return MCG_S & MCG_S_IRCST;
}


void MCG_Set_FCRDIV1()
{
	MCG_SC &= MCG_SC_FCRDIV_CLEARKMASK;
}
void MCG_Set_FCRDIV2()
{
	MCG_SC &= MCG_SC_FCRDIV_CLEARKMASK;
	MCG_SC |= (1<<MCG_SC_FCRDIV_OFF);
}
void MCG_Set_FCRDIV4()
{
	MCG_SC &= MCG_SC_FCRDIV_CLEARKMASK;
	MCG_SC |= (2<<MCG_SC_FCRDIV_OFF);
}
void MCG_Set_FCRDIV8()
{
	MCG_SC &= MCG_SC_FCRDIV_CLEARKMASK;
	MCG_SC |= (3<<MCG_SC_FCRDIV_OFF);
}
void MCG_Set_FCRDIV16()
{
	MCG_SC &= MCG_SC_FCRDIV_CLEARKMASK;
	MCG_SC |= (4<<MCG_SC_FCRDIV_OFF);
}
void MCG_Set_FCRDIV32()
{
	MCG_SC &= MCG_SC_FCRDIV_CLEARKMASK;
	MCG_SC |= (5<<MCG_SC_FCRDIV_OFF);
}
void MCG_Set_FCRDIV64()
{
	MCG_SC &= MCG_SC_FCRDIV_CLEARKMASK;
	MCG_SC |= (6<<MCG_SC_FCRDIV_OFF);
}
void MCG_Set_FCRDIV128()
{
	MCG_SC &= MCG_SC_FCRDIV_CLEARKMASK;
	MCG_SC |= (7<<MCG_SC_FCRDIV_OFF);
}


BOOL MCG_Is_PLL_Locked()
{
	return MCG_S & MCG_S_LOCK0;
}
