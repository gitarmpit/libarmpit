#include "lptmr.h"
#include "gpio.h"
#include "mcg.h"

static GPIO_PIN* g_pin;

void handler()
{
	if (g_pin != NULL)
	{
        GPIO_TogglePin(g_pin);
	}

}

static GPIO_PIN setup_pin()
{
    GPIO_PORT* portE = GPIO_GetInstance(PORTE);
    GPIO_EnableClock(portE, TRUE);
    GPIO_PIN e0 = GPIO_GetPin(portE, GPIO_PIN0);
    GPIO_SetupOut(&e0);
    GPIO_SetSlewRateFast(&e0);
    return e0;
}

// LPO
void test_lptmr()
{
	InitClock_FEI_24Mhz_Bus_24Mhz();

	GPIO_PIN pin = setup_pin();
    g_pin = &pin;

	LPTMPR_EnableClock(TRUE);
	LPTMPR_SelectMode_TimeCounter();
	LPTMR_SelectClock_LPO();
	LPTMR_ClearCounter();
	LPTMR_SetTimer_ms(10);
	LPTMPR_SetInterruptHandler(handler);
	LPTMPR_EnableCounter(TRUE);
	LPTMPR_EnableInterrupt(TRUE);
	while(1)
		;
}


// MCGIRCLK 4Mhz
void test_lptmr2()
{
	InitClock_FEI_24Mhz_Bus_24Mhz();

	GPIO_PIN pin = setup_pin();
    g_pin = &pin;

    MCG_Set_FCRDIV1();
    MCG_Set_IRC_Fast(); // 4Mhz
    MCG_Enable_MCGIRCLK(TRUE);

	LPTMPR_EnableClock(TRUE);
	LPTMPR_SelectMode_TimeCounter();
	LPTMR_SelectClock_MCGIRCLK(4000000);
	LPTMR_ClearCounter();
	LPTMR_SetTimer_hours(24);
	LPTMR_SetTimer_ms(10000);

	LPTMPR_SetInterruptHandler(handler);
	LPTMPR_EnableCounter(TRUE);
	LPTMPR_EnableInterrupt(TRUE);
	while(1)
		;
}

// MCGIRCLK 32K / 2 / 16 = 1KHz = 1ms
void test_lptmr3()
{
	InitClock_FEI_24Mhz_Bus_24Mhz();

	GPIO_PIN pin = setup_pin();
    g_pin = &pin;

    MCG_Set_IRC_Slow();
    MCG_Enable_MCGIRCLK(TRUE);

	LPTMPR_EnableClock(TRUE);
	LPTMPR_SelectMode_TimeCounter();
	LPTMR_SelectClock_MCGIRCLK(32768);
	uint16_t cnt = LPTMR_GetCounterValue();
	LPTMR_ClearCounter();
	cnt = LPTMR_GetCounterValue();
	LPTMPR_SetCMR(15);
	LPTMR_SetPrescaler(LPTMR_DIV2);
	uint16_t psr = (uint16_t)LPTMR_PSR;
	LPTMPR_SetInterruptHandler(handler);
	LPTMPR_EnableCounter(TRUE);
	LPTMPR_EnableInterrupt(TRUE);
	while(1)
	{
		cnt = LPTMR_GetCounterValue();
	}
}

// MCGIRCLK 32K
void test_lptmr4()
{
	InitClock_FEI_24Mhz_Bus_24Mhz();

	GPIO_PIN pin = setup_pin();
    g_pin = &pin;

    MCG_Set_IRC_Slow(); //32K
    MCG_Enable_MCGIRCLK(TRUE);

	LPTMPR_EnableClock(TRUE);
	LPTMPR_SelectMode_TimeCounter();
	LPTMR_SelectClock_MCGIRCLK(32768);
	LPTMR_ClearCounter();

	LPTMR_SetTimer_ms(10000);

	LPTMPR_SetInterruptHandler(handler);
	LPTMPR_EnableCounter(TRUE);
	LPTMPR_EnableInterrupt(TRUE);
	while(1)
		;
}

