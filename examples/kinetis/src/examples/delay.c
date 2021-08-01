#include "gpio.h"
#include "mcg.h"
#include "osc.h"

void test_delay()
{
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(4);
	MCG_Set_ExternalClock_OSC();
	MCG_Set_OSC_Range_Low();
	MCG_Set_FLL_OSCCLK();
	MCG_SetFLL_Freq_96(TRUE);
	MCG_Select_MCGOUTCLK_FLL_PLL();

	CORE_FREQ = 96000000;

	GPIO_PORT* portD = GPIO_GetInstance(PORTD);
	GPIO_EnableClock(portD, TRUE);
	GPIO_PIN d5 = GPIO_GetPin(portD, GPIO_PIN5);
	GPIO_SetupOut(&d5);
	GPIO_SetSlewRateFast(&d5);

	while(1)
	{
		GPIO_TogglePin(&d5);
		delay_ms(2000);
	}

}


// 32K clock
void test_delay2()
{
	MCG_Set_FCRDIV1();
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(1);
	OSC_CR |= OSC_CR_SC8P;
	MCG_Set_OSC_Range_Low();
	MCG_Set_ExternalClock_OSC();
	MCG_Select_MCGOUTCLK_OSCCLK();

	GPIO_PORT* portD = GPIO_GetInstance(PORTD);
	GPIO_EnableClock(portD, TRUE);
	GPIO_PIN d5 = GPIO_GetPin(portD, GPIO_PIN5);
	GPIO_SetupOut(&d5);
	GPIO_SetSlewRateFast(&d5);
	CORE_FREQ = 32768;

	while(1)
	{
		GPIO_TogglePin(&d5);
		delay_ms(1000);
	}

}
