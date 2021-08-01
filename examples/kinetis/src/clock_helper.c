#include "sim.h"
#include "mcg.h"
#include "osc.h"

static void InitClock_FEI_Common()
{
	MCG_Set_FLL_IRC32K();
	MCG_Set_PLLS_FLL();

	while (!MCG_Is_IREFST())
		;

	while (MCG_Is_PLLST())
		;

	while (!MCG_Is_CLKST_FLL())
		;

}

void InitClock_FEI_24Mhz_Bus_24Mhz()
{
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(1);

	InitClock_FEI_Common();

	MCG_SetFLL_Freq_24(TRUE);
	CORE_FREQ = 24000000;
	BUS_FREQ = 24000000;
}

void InitClock_FEI_48Mhz_Bus_24Mhz()
{
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(2);

	InitClock_FEI_Common();

	MCG_SetFLL_Freq_48(TRUE);
	CORE_FREQ = 48000000;
	BUS_FREQ = 24000000;
}

void InitClock_FEI_72Mhz_Bus_24Mhz()
{
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(3);

	InitClock_FEI_Common();

	MCG_SetFLL_Freq_72(TRUE);
	CORE_FREQ = 72000000;
	BUS_FREQ = 24000000;
}

void InitClock_FEI_72Mhz_Bus_36Mhz()
{
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(2);

	InitClock_FEI_Common();

	MCG_SetFLL_Freq_72(TRUE);
	CORE_FREQ = 72000000;
	BUS_FREQ = 36000000;
}

void InitClock_FEI_96Mhz_Bus_24Mhz()
{
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(4);

	InitClock_FEI_Common();

	MCG_SetFLL_Freq_96(TRUE);
	CORE_FREQ = 96000000;
	BUS_FREQ = 24000000;
}

void InitClock_FEI_96Mhz_Bus_32Mhz()
{
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(3);

	InitClock_FEI_Common();

	MCG_SetFLL_Freq_96(TRUE);
	CORE_FREQ = 96000000;
	BUS_FREQ = 32000000;
}

static void InitOSC_Common(BOOL high)
{
	// C2 range / gain
	if (high)
	{
		MCG_Set_OSC_Range_High();
		MCG_Set_HighGain_OSC(TRUE);
	}
	else
	{
		OSC_CR |= OSC_CR_SC8P;
		MCG_Set_OSC_Range_Low();
		MCG_Set_HighGain_OSC(FALSE);
	}

	//C2 EREFS0
	MCG_Set_ExternalClock_OSC();

}

static void InitClock_FEE_Common(uint8_t frdiv, BOOL high)
{
	InitOSC_Common(high);

	//C1 CLKS
	MCG_Select_MCGOUTCLK_FLL_PLL();

	MCG_Set_FRDIV(frdiv);

	//C1 IREFS
	MCG_Set_FLL_OSCCLK();

	MCG_Set_PLLS_FLL();

	while (!MCG_Is_OSCINIT0())
		;

	while (MCG_Is_IREFST())
		;

	while (MCG_Is_PLLST())
		;

	while (!MCG_Is_CLKST_FLL())
		;

}

void InitClock_FEE_24Mhz_Bus_24Mhz(uint8_t frdiv, BOOL high)
{
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(1);

	InitClock_FEE_Common(frdiv, high);

	//C4 DRST, DMX32
	MCG_SetFLL_Freq_24(TRUE);
	CORE_FREQ = 24000000;
	BUS_FREQ = 24000000;
}

void InitClock_FEE_48Mhz_Bus_24Mhz(uint8_t frdiv, BOOL high)
{
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(2);

	InitClock_FEE_Common(frdiv, high);

	//C4 DRST, DMX32
	MCG_SetFLL_Freq_48(TRUE);


	CORE_FREQ = 48000000;
	BUS_FREQ = 24000000;
}

void InitClock_FEE_72Mhz_Bus_24Mhz(uint8_t frdiv, BOOL high)
{
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(3);
	InitClock_FEE_Common(frdiv, high);

	//C4 DRST, DMX32
	MCG_SetFLL_Freq_72(TRUE);

	CORE_FREQ = 72000000;
	BUS_FREQ = 24000000;
}

void InitClock_FEE_72Mhz_Bus_36Mhz(uint8_t frdiv, BOOL high)
{
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(2);
	InitClock_FEE_Common(frdiv, high);

	//C4 DRST, DMX32
	MCG_SetFLL_Freq_72(TRUE);

	CORE_FREQ = 72000000;
	BUS_FREQ = 24000000;
}

void InitClock_FEE_96Mhz_Bus_24Mhz(uint8_t frdiv, BOOL high)
{
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(4);
	InitClock_FEE_Common(frdiv, high);

	//C4 DRST, DMX32
	MCG_SetFLL_Freq_96(TRUE);

	CORE_FREQ = 96000000;
	BUS_FREQ = 24000000;
}

void InitClock_FEE_96Mhz_Bus_32Mhz(uint8_t frdiv, BOOL high)
{
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(3);
	InitClock_FEE_Common(frdiv, high);

	//C4 DRST, DMX32
	MCG_SetFLL_Freq_96(TRUE);

	CORE_FREQ = 96000000;
	BUS_FREQ = 24000000;
}

//4Mhz
void InitClock_FBI_Fast()
{
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(1);
	MCG_Set_FCRDIV1();
	MCG_Set_IRC_Fast();
	MCG_Select_MCGOUTCLK_IRC();

	while (!MCG_Is_IRCST())
		;

	while (!MCG_Is_CLKST_IRC())
		;

	CORE_FREQ = 4000000;
	BUS_FREQ = 4000000;
}

//32 K
void InitClock_FBI_Slow()
{
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(1);
	MCG_Set_IRC_Slow();
	MCG_Select_MCGOUTCLK_IRC();
	MCG_DisableFLLPLL_InLowPowerMode(FALSE);

	while (MCG_Is_IRCST())
		;

	while (!MCG_Is_CLKST_IRC())
		;

	CORE_FREQ = 32768;
	BUS_FREQ = 32768;
}

//Bypassed low power internal, 32K
void InitClock_BLPI()
{
	InitClock_FBI_Slow();
	MCG_DisableFLLPLL_InLowPowerMode(TRUE);
}

// External XTAL
void InitClock_FBE(BOOL high)
{
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(1);

	InitOSC_Common(high);

	//C1 CLKS
	MCG_Select_MCGOUTCLK_OSCCLK();

	while (!MCG_Is_OSCINIT0())
		;

	while (!MCG_Is_CLKST_OSCCLK())
		;

	CORE_FREQ = XTAL_FREQ;
	BUS_FREQ = CORE_FREQ;
}

void InitClock_BLPE(BOOL high)
{
	InitClock_FBE(high);
	MCG_DisableFLLPLL_InLowPowerMode(TRUE);
}

void InitClock_PBE	(uint8_t frdiv, uint8_t pll_div, uint8_t pll_mul)
{

	InitOSC_Common(TRUE);

	//C1 CLKS
	MCG_Select_MCGOUTCLK_OSCCLK();

	//C1 FRDIV
	MCG_Set_FRDIV(frdiv);

	//C1 IREFS
	MCG_Set_FLL_OSCCLK();

	while (!MCG_Is_OSCINIT0())
		;

	while (MCG_Is_IREFST())
		;

	while (!MCG_Is_CLKST_OSCCLK())
		;

	//C5 PRDIV
	MCG_Set_PLL_DIV(pll_div);

	//C2 LP
	MCG_DisableFLLPLL_InLowPowerMode(FALSE);

	//C6 PLLS
	MCG_Set_PLLS_PLL();

	//C6 VDIV0
	MCG_Set_VCO_MUL(pll_mul);

	while (!MCG_Is_PLLST())
		;

	while (!MCG_Is_LOCK0())
		;

}

void InitClock_PEE(uint8_t frdiv, uint8_t pll_div, uint8_t pll_mul)
{
	InitClock_PBE(frdiv, pll_div, pll_mul);
	//C1 CLKS
	MCG_Select_MCGOUTCLK_FLL_PLL();
	while (!MCG_Is_CLKST_PLL())
		;
}

void InitClock_PEE_48Mhz_Bus_24Mhz()
{
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(2);
	InitClock_PEE(7, 2, 24);
}

