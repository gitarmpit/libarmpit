#include "tpm_capture.h"
#include "mcg.h"
#include "sim.h"
#include "tpm.h"
#include "gpio.h"
#include "gpio_helper.h"
#include <stdio.h>

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
