#include "osc.h"
#include "mcg.h"
#include "sim.h"
#include "gpio.h"

void test_clockout_FLL_OSC32()
{
	OSC_CR |= OSC_CR_SC8P;
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(2);
	// MCG_Set_IRC_Fast();
	// MCG_Select_MCGOUTCLK_IRC();

	//Only if OSC is Range1 (4Mhz+)
	//MCG_Set_FRDIV(4);


	MCG_Set_ExternalClock_OSC();
	MCG_Set_OSC_Range_Low();
	MCG_Set_FLL_OSCCLK();
	MCG_SetFLL_Freq_48(TRUE);

	MCG_Select_MCGOUTCLK_FLL_PLL();

	GPIO_PORT* portC = GPIO_GetInstance(PORTC);
	GPIO_EnableClock(portC, TRUE);
	GPIO_PIN c3 = GPIO_GetPin(portC, GPIO_PIN3);
	GPIO_SetAF(&c3, 5);

	SIM_SelectCLKOUT_BUS();
	while(1)
		;

}

void test_clockout_PLL()
{
	OSC_CR |= OSC_CR_SC8P;
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(2);

	MCG_Select_MCGOUTCLK_FLL_PLL();


	 MCG_Set_ExternalClock_OSC();
	 MCG_Set_OSC_Range_Low();

	 MCG_EnablePLLClock(TRUE);
	 MCG_Set_PLL_DIV(1);
	 MCG_Set_VCO_DIV(25);

	GPIO_PORT* portC = GPIO_GetInstance(PORTC);
	GPIO_EnableClock(portC, TRUE);
	GPIO_PIN c3 = GPIO_GetPin(portC, GPIO_PIN3);
	GPIO_SetAF(&c3, 5);

	SIM_SelectCLKOUT_BUS();
	while(1)
		;

}

// 7. MSGOUTCLOCK = OSCCLOCK
void test_clockout_OSCCLOCK()
{
	MCG_Set_FCRDIV1();
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(1);
	GPIO_PORT* portC = GPIO_GetInstance(PORTC);
	GPIO_EnableClock(portC, TRUE);
	GPIO_PIN c3 = GPIO_GetPin(portC, GPIO_PIN3);
	GPIO_SetAF(&c3, 5);
	OSC_CR |= OSC_CR_SC8P;

	MCG_Set_ExternalClock_OSC();
	MCG_Select_MCGOUTCLK_OSCCLK();
	// 32K
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

