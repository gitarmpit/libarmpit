#include "osc.h"
#include "mcg.h"
#include "sim.h"
#include "gpio.h"
#include "tpm.h"
#include "sim.h"
#include "gpio_helper.h"

static GPIO_PIN* g_pin;

#pragma GCC diagnostic ignored "-Wunused-function"

void tpm_handler(void* ctx)
{
	UNUSED(ctx);
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

//OSC32K clock
static void test_tpm1()
{
	GPIO_PIN pin = setup_pin();
    g_pin = &pin;

	OSC_CR |= OSC_CR_SC8P;
	MCG_Set_ExternalClock_OSC();
	MCG_Set_OSC_Range_Low();
	OSC_Enable_OSERCCLK(TRUE);

	SIM_Select_TPMSRC_OSCERCLK();

	TPM* tpm = TPM_GetInstance(TPM0_INSTANCE);
	tpm->TOF_handler = tpm_handler;
	TPM_EnableClock(tpm, TRUE);
	*tpm->TPM_MOD = 32767;

	TPM_EnableInterrupt(tpm, TRUE);
	TPM_EnableCounter(tpm, TRUE);
	while(1)
		;
}

//24Mhz External FLL clock
static void test_tpm2()
{
	SIM_Select_FLL();
	SIM_Select_TPMSRC_MCGFLLCLK();

	GPIO_PIN pin = setup_pin();
    g_pin = &pin;

    InitClock_FEE_24Mhz_Bus_24Mhz(0, FALSE);

	TPM* tpm = TPM_GetInstance(TPM0_INSTANCE);
	tpm->TOF_handler = tpm_handler;
	TPM_EnableClock(tpm, TRUE);
	*tpm->TPM_MOD = 48000;
	TPM_SetPrescaler(tpm, TPM_DIV8);
	TPM_EnableInterrupt(tpm, TRUE);
	TPM_EnableCounter(tpm, TRUE);
	while(1)
		;

}

//24Mhz internal FLL clock
static void test_tpm_timer()
{
	SIM_Select_FLL();
	SIM_Select_TPMSRC_MCGFLLCLK();

	GPIO_PIN pin = setup_pin();
    g_pin = &pin;

	InitClock_FEI_24Mhz_Bus_24Mhz();

	TPM_FREQ = CORE_FREQ;

	TPM* tpm = TPM_GetInstance(TPM0_INSTANCE);
	tpm->TOF_handler = tpm_handler;
	TPM_EnableClock(tpm, TRUE);

	TPM_Channel_SetUpdatePeriod_us(tpm, 1000);

	TPM_EnableInterrupt(tpm, TRUE);
	TPM_EnableCounter(tpm, TRUE);
	while(1)
		;

}

//Internal 32K
static void test_tpm_timer2()
{
	SIM_Select_TPMSRC_MCGIRCLK();

	GPIO_PIN pin = setup_pin();
    g_pin = &pin;

	MCG_Enable_MCGIRCLK(TRUE);
	InitClock_FBI_Slow();
	//delay_ms(1000);

	TPM_FREQ = CORE_FREQ;

	TPM* tpm = TPM_GetInstance(TPM0_INSTANCE);
	tpm->TOF_handler = tpm_handler;
	TPM_EnableClock(tpm, TRUE);
	// delay_ms(100);

	TPM_Channel_SetUpdatePeriod_ms(tpm, 1000);
	TPM_EnableInterrupt(tpm, TRUE);
	TPM_EnableCounter(tpm, TRUE);

	while(1)
		;

}

static void test_pwm()
{
	SIM_Select_FLL();
	SIM_Select_TPMSRC_MCGFLLCLK();

    GPIO_PORT* portD = GPIO_GetInstance(PORTD);
    GPIO_EnableClock(portD, TRUE);
    GPIO_PIN d5 = GPIO_GetPin(portD, GPIO_PIN5);
    GPIO_SetAF(&d5, 4);

	OSC_CR |= OSC_CR_SC8P;
	MCG_Set_ExternalClock_OSC();
	MCG_Set_OSC_Range_Low();
	MCG_Set_FLL_OSCCLK();
	MCG_SetFLL_Freq_24(TRUE);
	CORE_FREQ = 24000000;
	BUS_FREQ = CORE_FREQ / 2;
	TPM_FREQ = CORE_FREQ;

	MCG_Select_MCGOUTCLK_FLL_PLL();


	TPM* tpm = TPM_GetInstance(TPM0_INSTANCE);
	tpm->TOF_handler = tpm_handler;
	TPM_EnableClock(tpm, TRUE);
	int period_us = 2000;
	// int duty_us = 1000;
	//*tpm->TPM_MOD = 48000;
	TPM_SetPrescaler(tpm, TPM_DIV8);
	//uint16_t cnt = period_us * BUS_FREQ / 8 / 1000000;
	uint16_t cnt = (uint64_t)period_us * TPM_FREQ / 1000000;
	*tpm->TPM_MOD = cnt;

	TPM_Channel_t* ch = TPM_GetChannel(tpm, 5);
	*ch->TPM_CnV = cnt/2;
	TPM_Channel_EnablePWM(ch);
	TPM_EnableCounter(tpm, TRUE);
	while(1)
	{
		delay_ms(2);
		//*ch.TPM_CnV += 500;
	}
		;

}

// FLL, external OSC
static void test_pwm2()
{
	SIM_Select_FLL();
	SIM_Select_TPMSRC_MCGFLLCLK();

    GPIO_PORT* portD = GPIO_GetInstance(PORTD);
    GPIO_EnableClock(portD, TRUE);
    GPIO_PIN d5 = GPIO_GetPin(portD, GPIO_PIN5);
    GPIO_SetAF(&d5, 4);

	OSC_CR |= OSC_CR_SC8P;
	MCG_Set_ExternalClock_OSC();
	MCG_Set_OSC_Range_Low();
	MCG_Set_FLL_OSCCLK();
	MCG_SetFLL_Freq_24(TRUE);
	CORE_FREQ = 24000000;
	BUS_FREQ = CORE_FREQ / 2;
	TPM_FREQ = CORE_FREQ;

	MCG_Select_MCGOUTCLK_FLL_PLL();

	TPM* tpm = TPM_GetInstance(TPM0_INSTANCE);
	tpm->TOF_handler = tpm_handler;
	TPM_EnableClock(tpm, TRUE);
	int period_us = 500000; //500ms but it will get capped at 350ms being the max at this clock speed (24Mhz)
	int duty_us = 150000;
	TPM_Channel_t* ch = TPM_GetChannel(tpm, 5);
	TPM_Channel_SetupPWM(ch, period_us, duty_us);
	TPM_EnableCounter(tpm, TRUE);
	delay_ms(50000);
	TPM_Channel_UpdatePWMDuty(ch, 100000);
	while(1);
}

static void test_pwm3()
{
	InitClock_FBI_Slow();
	MCG_Enable_MCGIRCLK(TRUE);
	SIM_Select_TPMSRC_MCGIRCLK();
	TPM_FREQ = CORE_FREQ;

    TPM_Channel_t* ch = GPIO_Helper_SetupTPM_Channel_t(TPM0_CH3_E30);
	int period_us = 1000000;
	int duty_us = 500000;
	TPM_Channel_SetupPWM(ch, period_us, duty_us);
	TPM_EnableCounter(ch->tpm, TRUE);

	while(1);

}

static void test_pwm4()
{
	InitClock_FEI_48Mhz_Bus_24Mhz();
    SIM_Select_TPMSRC_MCGFLLCLK();
	SIM_Select_FLL();
	TPM_FREQ = CORE_FREQ;

    TPM_Channel_t* ch = GPIO_Helper_SetupTPM_Channel_t(TPM0_CH3_E30);
	int period_us = 1000;
	int duty_us = 500;
	TPM_Channel_SetupPWM(ch, period_us, duty_us);
	TPM_EnableCounter(ch->tpm, TRUE);

	while(1);

}


void test_tpm()
{
	test_pwm4();
	//test_tpm_timer();
	while(1);
}

