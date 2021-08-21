#include "osc.h"
#include "mcg.h"
#include "sim.h"
#include "gpio.h"
#include "tpm.h"

static GPIO_PIN* g_pin;


void tpm_handler(TPM* tpm)
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

//OSC32K clock
void test_tpm()
{
	GPIO_PIN pin = setup_pin();
    g_pin = &pin;

	OSC_CR |= OSC_CR_SC8P;
	MCG_Set_ExternalClock_OSC();
	MCG_Set_OSC_Range_Low();
	OSC_Enable_OSERCCLK(TRUE);

	SIM_Select_TPMSRC_OSCERCLK();

	TPM* tpm = TPM_GetInstance(TPM0);
	tpm->TOF_handler = tpm_handler;
	TPM_EnableClock(tpm, TRUE);
	*tpm->TPM_MOD = 32767;

	TPM_EnableInterrupt(tpm, TRUE);
	TPM_EnableCounter(tpm, TRUE);
	while(1)
		;
}

//24Mhz External FLL clock
void test_tpm2()
{
	SIM_Select_FLL();
	SIM_Select_TPMSRC_MCGFLLCLK();

	GPIO_PIN pin = setup_pin();
    g_pin = &pin;

    InitClock_FEE_24Mhz_Bus_24Mhz(0, FALSE);

	TPM* tpm = TPM_GetInstance(TPM0);
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
void test_tpm_timer()
{
	SIM_Select_FLL();
	SIM_Select_TPMSRC_MCGFLLCLK();

	GPIO_PIN pin = setup_pin();
    g_pin = &pin;

	InitClock_FEI_24Mhz_Bus_24Mhz();

	TPM_FREQ = CORE_FREQ;

	TPM* tpm = TPM_GetInstance(TPM0);
	tpm->TOF_handler = tpm_handler;
	TPM_EnableClock(tpm, TRUE);

	TPM_Channel_SetUpdatePeriod_us(tpm, 1000);

	TPM_EnableInterrupt(tpm, TRUE);
	TPM_EnableCounter(tpm, TRUE);
	while(1)
		;

}

//Internal 32K
void test_tpm_timer2()
{
	SIM_Select_TPMSRC_MCGIRCLK();

	GPIO_PIN pin = setup_pin();
    g_pin = &pin;

	MCG_Enable_MCGIRCLK(TRUE);
	InitClock_FBI_Slow();
	//delay_ms(1000);

	TPM_FREQ = CORE_FREQ;

	TPM* tpm = TPM_GetInstance(TPM0);
	tpm->TOF_handler = tpm_handler;
	TPM_EnableClock(tpm, TRUE);
	// delay_ms(100);

	TPM_Channel_SetUpdatePeriod_ms(tpm, 1000);
	TPM_EnableInterrupt(tpm, TRUE);
	TPM_EnableCounter(tpm, TRUE);

	while(1)
		;

}



void test_pwm()
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


	TPM* tpm = TPM_GetInstance(TPM0);
	tpm->TOF_handler = tpm_handler;
	TPM_EnableClock(tpm, TRUE);
	int period_us = 2000;
	int duty_us = 1000;
	//*tpm->TPM_MOD = 48000;
	TPM_SetPrescaler(tpm, TPM_DIV8);
	//uint16_t cnt = period_us * BUS_FREQ / 8 / 1000000;
	uint16_t cnt = (uint64_t)period_us * TPM_FREQ / 1000000;
	*tpm->TPM_MOD = cnt;

	TPM_Channel ch = TPM_GetChannel(tpm, 5);
	*ch.TPM_CnV = cnt/2;
	TPM_Channel_EnablePWM(&ch);
	TPM_EnableCounter(tpm, TRUE);
	while(1)
	{
		delay_ms(2);
		//*ch.TPM_CnV += 500;
	}
		;

}

// FLL, external OSC
void test_pwm2()
{
	SIM_Select_FLL();
	SIM_SelectTPMClock_MCGFLLCLK();

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

	TPM* tpm = TPM_GetInstance(TPM0);
	tpm->TOF_handler = tpm_handler;
	TPM_EnableClock(tpm, TRUE);
	int period_us = 500000; //500ms but it will get capped at 350ms being the max at this clock speed (24Mhz)
	int duty_us = 150000;
	TPM_Channel ch = TPM_GetChannel(tpm, 5);
	TPM_Channel_SetupPWM(&ch, period_us, duty_us);
	TPM_EnableCounter(tpm, TRUE);
	delay_ms(50000);
	TPM_Channel_UpdatePWMDuty(&ch, 100000);
	while(1)
	{
	}
		;

}

static void generatePWM()
{
	//InitClock_FBI_Slow();
	//MCG_Enable_MCGIRCLK(TRUE);
	//SIM_Select_TPMSRC_MCGIRCLK();


    GPIO_PORT* portA = GPIO_GetInstance(PORTA);
    GPIO_EnableClock(portA, TRUE);
    GPIO_PIN a5 = GPIO_GetPin(portA, GPIO_PIN5);
    GPIO_SetAF(&a5, 3);

	TPM* tpm = TPM_GetInstance(TPM0);
	TPM_EnableClock(tpm, TRUE);
	int period_us = 50;
	int duty_us = 25;
	TPM_Channel ch = TPM_GetChannel(tpm, 2);
	TPM_Channel_SetupPWM(&ch, period_us, duty_us);
	TPM_EnableCounter(tpm, TRUE);
}

uint16_t presc_array[] = {1,2,4,8,16,32,64,128};
uint32_t g_val;
uint32_t vals[1000];
int n = 0;
uint16_t g_mod = 65535;
uint16_t g_presc = 1;
static void tmp_capture_handler(uint32_t val)
{
	int32_t diff = val - g_val;
	if (diff < 0)
	{
		diff += g_mod;
	}
	uint64_t val_us =  diff * presc_array[g_presc] * 1000000llu / CORE_FREQ;
	if (n < 1000)
	{
		vals[n++] = val_us;
	}
	g_val = val;
	// printf ("%u\n", diff);
}

void test_tpm_capture()
{
	InitClock_FEI_24Mhz_Bus_24Mhz();
	SIM_Select_TPMSRC_MCGFLLCLK();
	TPM_FREQ = CORE_FREQ;
	SIM_Select_FLL();

	generatePWM();

    GPIO_PORT* portA = GPIO_GetInstance(PORTA);
    GPIO_EnableClock(portA, TRUE);
    GPIO_PIN a13 = GPIO_GetPin(portA, GPIO_PIN13);
    GPIO_SetAF(&a13, 3);

	TPM* tpm = TPM_GetInstance(TPM1);
	tpm->CHF_handler = tmp_capture_handler;
	TPM_EnableClock(tpm, TRUE);
	uint16_t mod, presc;
	TPM_CalculateTimerValues(200, &mod, &presc);
	*tpm->TPM_CNT = 0;
	TPM_SetModulo(tpm, mod);
	g_mod = mod;
	g_presc = presc;
	TPM_SetPrescaler(tpm, presc);
	TPM_Channel ch = TPM_GetChannel(tpm, 1);
	TPM_Channel_EnableInterrupt(&ch, TRUE);
	TPM_Channel_SetupInputCaptureEitherEdge(&ch);
	TPM_EnableCounter(tpm, TRUE);
	while(1)
		;
}
