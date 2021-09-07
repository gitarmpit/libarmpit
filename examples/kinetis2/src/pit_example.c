#include "osc.h"
#include "mcg.h"
#include "sim.h"
#include "gpio.h"
#include "pit.h"
#include "sim.h"

static GPIO_PIN* g_pin;



static void PIT_InterruptHandler(void* ctx)
{
	UNUSED(ctx);
	if (g_pin != NULL)
	{
		GPIO_TogglePin(g_pin);
	}
}

#pragma GCC diagnostic ignored "-Wunused-function"
static void pit_test1()
{
	SIM_SetOUTDIV4(1);
#if 0
//32K xtal FPL 24Mhz
	OSC_CR |= OSC_CR_SC8P;
	MCG_Set_ExternalClock_OSC();
	MCG_Set_OSC_Range_Low();
	MCG_Set_FLL_OSCCLK();
	MCG_SetFLL_Freq_24(TRUE);
	CORE_FREQ = 24000000;
	BUS_FREQ = CORE_FREQ;
#else
	// default ~20Mhz
	CORE_FREQ = 20000000;
	BUS_FREQ = CORE_FREQ;
#if 1
	//32Khz
	OSC_CR |= OSC_CR_SC8P;
	MCG_Set_ExternalClock_OSC();
	MCG_Set_OSC_Range_Low();
	CORE_FREQ = 32768;
	BUS_FREQ = CORE_FREQ;
	MCG_Select_MCGOUTCLK_OSCCLK();
	delay_ms(20);
#endif
#endif

	GPIO_PORT* portE = GPIO_GetInstance(PORTE);
	GPIO_EnableClock(portE, TRUE);
	GPIO_PIN e0 = GPIO_GetPin(portE, GPIO_PIN0);
	GPIO_SetupOut(&e0);
	GPIO_SetSlewRateFast(&e0);
	GPIO_SetPin(&e0);
	g_pin = &e0;

	SIM_Enable_PIT(TRUE);
	PIT_EnableClock(TRUE);
	PIT* pit = PIT_GetInstance(PIT0);
	PIT_SetPeriod_us(pit, 125000);
	PIT_EnableFreeze(FALSE);
	pit->interrupt_handler = PIT_InterruptHandler;
	PIT_EnableInterrupt(pit, TRUE);
	PIT_EnableTimer(pit, TRUE);
	delay_ms(200);
	while(1)
		;
}

#pragma GCC diagnostic ignored "-Wunused-function"
static void pit_test2()
{
	//InitClock_FEI_48Mhz_Bus_24Mhz();

	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(8);

	InitClock_FEI_Common();

	MCG_SetFLL_Freq_48(TRUE);
	CORE_FREQ = 48000000;
	BUS_FREQ = 48000000 / 8;

	GPIO_PORT* portE = GPIO_GetInstance(PORTE);
	GPIO_EnableClock(portE, TRUE);
	GPIO_PIN e0 = GPIO_GetPin(portE, GPIO_PIN0);
	GPIO_SetupOut(&e0);
	GPIO_SetSlewRateFast(&e0);
	GPIO_SetPin(&e0);
	g_pin = &e0;

	SIM_Enable_PIT(TRUE);
	PIT_EnableClock(TRUE);
	PIT* pit = PIT_GetInstance(PIT0);
	PIT_SetPeriod_us(pit, 125000);
	PIT_EnableFreeze(FALSE);
	pit->interrupt_handler = PIT_InterruptHandler;
	PIT_EnableInterrupt(pit, TRUE);
	PIT_EnableTimer(pit, TRUE);
	delay_ms(200);
	while(1)
		;
}

#pragma GCC diagnostic pop
void test_pit()
{
	pit_test2();
}
