#include "osc.h"
#include "mcg.h"
#include "sim.h"
#include "gpio.h"


static void SetupClkoutPin()
{
	GPIO_PORT* portC = GPIO_GetInstance(PORTC);
	GPIO_EnableClock(portC, TRUE);
	GPIO_PIN c3 = GPIO_GetPin(portC, GPIO_PIN3);
	GPIO_SetAF(&c3, 5);
}

void TestClockout1()
{
	SetupClkoutPin();
	InitClock_FEI_24Mhz_Bus_24Mhz();
	SIM_SelectCLKOUT_BUS();
	while(1)
		;
}

void TestClockout2()
{
	SetupClkoutPin();
	SIM_SelectCLKOUT_BUS();
	delay_ms(5000);
	InitClock_FEE_24Mhz_Bus_24Mhz(0, FALSE);
	//InitClock_FEE_96Mhz_Bus_24Mhz(0, FALSE);
	delay_ms(5000);
	InitClock_FEI_48Mhz_Bus_24Mhz();
	while(1)
		;
}

void TestClockout3()
{
	SetupClkoutPin();
	InitClock_FBI_Fast();
	//InitClock_FBI_Slow();
	//InitClock_BLPI();
	//InitClock_BLPE(FALSE);
	//InitClock_PEE_48Mhz_Bus_24Mhz();
	SIM_SelectCLKOUT_BUS();
	while(1)
		;
}



void test_clockout_PLL()
{
	SetupClkoutPin();


	SIM_SelectCLKOUT_BUS();
	while(1)
		;

}


// MCGIRCLK / LPO / OSCERCLK
void test_clockout()
{
	OSC_CR |= OSC_CR_SC8P;
	MCG_Set_ExternalClock_OSC();
	MCG_Set_OSC_Range_Low();

	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(1);
	//MCG_SetFLL_Freq_48(TRUE);

	MCG_Set_FCRDIV1();
	MCG_Set_IRC_Fast();
	MCG_Select_MCGOUTCLK_IRC();
	MCG_Enable_MCGIRCLK(TRUE);

	OSC_Enable_OSERCCLK(TRUE);

	GPIO_PORT* portC = GPIO_GetInstance(PORTC);
	GPIO_EnableClock(portC, TRUE);
	GPIO_PIN c3 = GPIO_GetPin(portC, GPIO_PIN3);
	GPIO_SetAF(&c3, 5);

	//4 Mhz
	//SIM_SelectCLKOUT_MCGIRCLK();

	// 1Khz
	//SIM_SelectCLKOUT_LPO();

	// 32Khz
	SIM_SelectCLKOUT_OSCERCLK();


	while(1)
		;
}

