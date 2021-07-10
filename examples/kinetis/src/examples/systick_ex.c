#include "osc.h"
#include "mcg.h"
#include "sim.h"
#include "gpio.h"
#include "systick.h"

static GPIO_PIN* g_pin;

void MySysTickHandler()
{
	if (g_pin != NULL)
	{
		GPIO_TogglePin(g_pin);
	}
}

void test_systick1()
{
	GPIO_PORT* portE = GPIO_GetInstance(PORTE);

#if 1
//32K xtal
	OSC_CR |= OSC_CR_SC8P;
	MCG_Set_ExternalClock_OSC();
	MCG_Set_OSC_Range_Low();
	MCG_Set_FLL_OSCCLK();
#endif

	SIM_SetOUTDIV4(3);
	MCG_SetFLL_Freq_96(TRUE);
	CORE_FREQ = 96000000;

	GPIO_EnableClock(portE, TRUE);
	GPIO_PIN e1 = GPIO_GetPin(portE, GPIO_PIN1);
	GPIO_SetupOut(&e1);
	GPIO_SetSlewRateFast(&e1);
	GPIO_SetPin(&e1);

	//SIM_SetOUTDIV1(4);
	//CORE_FREQ /= 4;

	g_pin = &e1;
	uint32_t us = 100;
	SystickEnable((uint64_t)us * CORE_FREQ/16/1000000, TRUE, TRUE, MySysTickHandler);
	while(1)
		;

}

// Fast IRC 4Mhz
void test_systick2()
{
	GPIO_PORT* portE = GPIO_GetInstance(PORTE);

	//Default 4Mhz
	CORE_FREQ = 4000000;
	// defaults to div2
	MCG_Set_FCRDIV1();
	MCG_Set_IRC_Fast();
	MCG_Select_MCGOUTCLK_IRC();

	GPIO_EnableClock(portE, TRUE);
	GPIO_PIN e1 = GPIO_GetPin(portE, GPIO_PIN1);
	GPIO_SetupOut(&e1);
	GPIO_SetSlewRateFast(&e1);
	GPIO_SetPin(&e1);

	//SIM_SetOUTDIV1(4);
	//CORE_FREQ /= 4;

	g_pin = &e1;
	uint32_t us = 1000;
	SystickEnable(us * CORE_FREQ/1000000, TRUE, FALSE, MySysTickHandler);
	while(1)
		;

}

// Slow IRC 32768
static void test_systick3()
{
	GPIO_PORT* portE = GPIO_GetInstance(PORTE);

	CORE_FREQ = 32768;
	MCG_Set_FCRDIV1();
	MCG_Set_IRC_Slow();
	MCG_Select_MCGOUTCLK_IRC();

	GPIO_EnableClock(portE, TRUE);

	GPIO_PIN e1 = GPIO_GetPin(portE, GPIO_PIN1);
	GPIO_SetupOut(&e1);
	GPIO_SetSlewRateFast(&e1);
	GPIO_SetPin(&e1);

	//SIM_SetOUTDIV1(4);
	//CORE_FREQ /= 4;

	g_pin = &e1;
	uint32_t us = 10000;
	SystickEnable(us * CORE_FREQ/1000000, TRUE, FALSE, MySysTickHandler);
	while(1)
		;

}

