#include "tpm_capture.h"
#include "mcg.h"
#include "sim.h"
#include "tpm.h"
#include "gpio.h"
#include "gpio_helper.h"

uint16_t presc_array[] = {1,2,4,8,16,32,64,128};
int32_t g_val = -1;
uint32_t vals[1000];
int n = 0;
uint16_t g_mod = 65535;
uint16_t g_presc = 1;
int16_t max = 0;
int32_t g_period;
int32_t g_duty;

// uint64_t val_us =  diff * presc_array[g_presc] * 1000000llu / TPM_FREQ;
//uint32_t val_us =  diff * 1000000 / TPM_FREQ; // *  1000000llu / TPM_FREQ;

TPM_Channel g_ch;

static void tmp_capture_handler(uint16_t val)
{
	if (g_val != -1)
	{
		int32_t diff = val - g_val;
		if (diff < 0)
		{
			diff += g_mod;
		}
		if (n < 20)
		{
			//vals[n++] = val_us;
			if (g_duty == 0)
			{
				g_duty = diff;
				vals[n++] = diff;
			}
			else
			{
				g_period = g_duty + diff;
				g_duty = 0;
				vals[n++] = g_period;
			}
			//++n;
			//if (diff > max)
			//	max = diff;
		}
		else
		{
			for (int i = 0; i < 20; ++i)
			{
				printf ("%u: %u\n", vals[i], vals[i] * presc_array[g_presc] * 1000000llu / TPM_FREQ);
			}
			//printf ("%d\n", max);
			while(1);
		}
	}
	else
	{
		TPM_Channel_SetupInputCaptureEitherEdge(&g_ch);
	}
	g_val = val;
	// printf ("%u\n", diff);
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
	int period_us = 100;
	int duty_us = 10;
	TPM_Channel ch = TPM_GetChannel(tpm, 2);
	TPM_Channel_SetupPWM(&ch, period_us, duty_us);
	TPM_EnableCounter(tpm, TRUE);
}

static void generatePWM2()
{
    TPM_Channel ch = GPIO_Helper_SetupTPM(TPM0_CH2_A5);
	int period_us = 100;
	int duty_us = 10;
	TPM_Channel_SetupPWM(&ch, period_us, duty_us);
	TPM_EnableCounter(ch.tpm, TRUE);
}

void test_tpm_capture()
{
	InitClock_FEI_48Mhz_Bus_24Mhz();
	SIM_Select_TPMSRC_MCGFLLCLK();
	TPM_FREQ = CORE_FREQ;
	SIM_Select_FLL();

	generatePWM2();

    GPIO_PORT* portA = GPIO_GetInstance(PORTA);
    GPIO_EnableClock(portA, TRUE);
    GPIO_PIN a13 = GPIO_GetPin(portA, GPIO_PIN12);
    GPIO_SetAF(&a13, 3);

	TPM* tpm = TPM_GetInstance(TPM1);
	tpm->CHF_handler2 = tmp_capture_handler;
	TPM_EnableClock(tpm, TRUE);
	uint16_t mod, presc;
	TPM_CalculateTimerValues(1000, &mod, &presc);
	*tpm->TPM_CNT = 0;
	TPM_SetModulo(tpm, mod);
	g_mod = mod;
	g_presc = presc;
	TPM_SetPrescaler(tpm, presc);
	g_ch = TPM_GetChannel(tpm, 0);
	TPM_Channel_EnableInterrupt(&g_ch, TRUE);
	TPM_Channel_SetupInputCaptureRisingEdge(&g_ch);
	TPM_EnableCounter(tpm, TRUE);
	while(1)
		;
}


void test_tpm_capture2()
{
	InitClock_FEI_48Mhz_Bus_24Mhz();
	SIM_Select_TPMSRC_MCGFLLCLK();
	TPM_FREQ = CORE_FREQ;
	SIM_Select_FLL();

	generatePWM();
	TPM_Capture_Context ctx;
	TPM_Channel ch = GPIO_Helper_SetupTPM(TPM1_CH0_A12);

	TPM_Capture_Setup(&ctx, &ch, 100);
	while(1)
	{
		uint32_t period, duty;
		TPM_Capture_GetValue(&ctx, &period, &duty);
		printf ("period: %d, duty: %d\n", period, duty);
		delay_ms(1000);
	}
}
