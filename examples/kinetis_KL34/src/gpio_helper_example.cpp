#include "gpio_helper.h"
#include "mcg.h"
#include "sim.h"

void test_gpio_helper_pwm()
{
	InitClock_FEI_24Mhz_Bus_24Mhz();
	SIM_Select_TPMSRC_MCGFLLCLK();
	TPM_FREQ = CORE_FREQ;
	SIM_Select_FLL();

	TPM_Channel_t* ch = GPIO_Helper_SetupTPM_Channel_t(TPM0_CH0_D0);
	TPM_Channel_t* ch1 = GPIO_Helper_SetupTPM_Channel_t(TPM0_CH1_D1);
	TPM_Channel_t* ch2 = GPIO_Helper_SetupTPM_Channel_t(TPM0_CH2_D2);
	TPM_Channel_t* ch3 = GPIO_Helper_SetupTPM_Channel_t(TPM0_CH3_D3);
	TPM_Channel_t* ch4 = GPIO_Helper_SetupTPM_Channel_t(TPM0_CH4_D4);
	TPM_Channel_t* ch5 = GPIO_Helper_SetupTPM_Channel_t(TPM0_CH5_D5);

	TPM* tpm = ch->tpm;
	TPM_EnableClock(tpm, TRUE);
	int period_ms = 200;
	int duty_ms = 100;
	TPM_Channel_SetupPWM(ch, period_ms * 1000, duty_ms * 1000);
	TPM_Channel_SetupPWM(ch1, period_ms * 1000, duty_ms * 1000);
	TPM_Channel_SetupPWM(ch2, period_ms * 1000, duty_ms * 1000);
	TPM_Channel_SetupPWM(ch3, period_ms * 1000, duty_ms * 1000);
	TPM_Channel_SetupPWM(ch4, period_ms * 1000, duty_ms * 1000);
	TPM_Channel_SetupPWM(ch5, period_ms * 1000, duty_ms * 1000);
	TPM_EnableCounter(tpm, TRUE);
	while(1);
}

void test_gpio_helper_pwm2()
{
	InitClock_FEI_24Mhz_Bus_24Mhz();
	SIM_Select_TPMSRC_MCGFLLCLK();
	TPM_FREQ = CORE_FREQ;
	SIM_Select_FLL();

	TPM_Channel_t* ch0 = GPIO_Helper_SetupTPM_Channel_t(TPM0_CH0_C1);
	GPIO_Helper_SetupTPM_Channel_t(TPM0_CH0_E24);

	TPM_Channel_t* ch1 = GPIO_Helper_SetupTPM_Channel_t(TPM0_CH1_C2);
	GPIO_Helper_SetupTPM_Channel_t(TPM0_CH1_E25);

	TPM_Channel_t* ch2 = GPIO_Helper_SetupTPM_Channel_t(TPM0_CH2_C3);
	GPIO_Helper_SetupTPM_Channel_t(TPM0_CH2_A5);
	GPIO_Helper_SetupTPM_Channel_t(TPM0_CH2_E29);
	GPIO_Helper_SetupTPM_Channel_t(TPM0_CH2_D2);

	TPM_Channel_t* ch3 = GPIO_Helper_SetupTPM_Channel_t(TPM0_CH3_C4);
	GPIO_Helper_SetupTPM_Channel_t(TPM0_CH3_D3);
	GPIO_Helper_SetupTPM_Channel_t(TPM0_CH3_E30);

	TPM_Channel_t* ch4 = GPIO_Helper_SetupTPM_Channel_t(TPM0_CH4_E31);
	GPIO_Helper_SetupTPM_Channel_t(TPM0_CH4_D4);


	TPM* tpm = ch0->tpm;
	TPM_EnableClock(tpm, TRUE);
	int period_ms = 200;
	int duty_ms = 100;
	TPM_Channel_SetupPWM(ch0, period_ms * 1000, duty_ms * 1000);
	TPM_Channel_SetupPWM(ch1, period_ms * 1000, duty_ms * 1000);
	TPM_Channel_SetupPWM(ch2, period_ms * 1000, duty_ms * 1000);
	TPM_Channel_SetupPWM(ch3, period_ms * 1000, duty_ms * 1000);
	TPM_Channel_SetupPWM(ch4, period_ms * 1000, duty_ms * 1000);
	TPM_EnableCounter(tpm, TRUE);
	while(1);
}


// 4MB clock
void test_gpio_helper_pwm3()
{
	InitClock_FBI_Fast();
	SIM_Select_TPMSRC_MCGIRCLK();
	TPM_FREQ = CORE_FREQ;
	MCG_Enable_MCGIRCLK(TRUE);

	//InitClock_FEI_24Mhz_Bus_24Mhz();
	//SIM_Select_TPMSRC_MCGFLLCLK();
	//TPM_FREQ = CORE_FREQ;
	//SIM_Select_FLL();


	TPM_Channel_t* ch0 = GPIO_Helper_SetupTPM_Channel_t(TPM1_CH0_A12);
	GPIO_Helper_SetupTPM_Channel_t(TPM1_CH0_B0);
	GPIO_Helper_SetupTPM_Channel_t(TPM1_CH0_E20);

	TPM_Channel_t* ch1 = GPIO_Helper_SetupTPM_Channel_t(TPM1_CH1_A13);
	GPIO_Helper_SetupTPM_Channel_t(TPM1_CH1_B1);
	GPIO_Helper_SetupTPM_Channel_t(TPM1_CH1_E21);

	TPM* tpm = ch0->tpm;
	TPM_EnableClock(tpm, TRUE);
	int period_ms = 2000;
	int duty_ms = 1000;
	TPM_Channel_SetupPWM(ch0, period_ms * 1000, duty_ms * 1000);
	duty_ms = 500;
	TPM_Channel_SetupPWM(ch1, period_ms * 1000, duty_ms * 1000);
	TPM_EnableCounter(tpm, TRUE);
	while(1);
}

// 4MB clock
void test_gpio_helper_pwm4()
{
	InitClock_FBI_Fast();
	SIM_Select_TPMSRC_MCGIRCLK();
	TPM_FREQ = CORE_FREQ;
	MCG_Enable_MCGIRCLK(TRUE);
	delay_ms(1);

	TPM_Channel_t* ch0 = GPIO_Helper_SetupTPM_Channel_t(TPM2_CH0_A1);
	GPIO_Helper_SetupTPM_Channel_t(TPM2_CH0_B18);
	GPIO_Helper_SetupTPM_Channel_t(TPM2_CH0_B2);
	GPIO_Helper_SetupTPM_Channel_t(TPM2_CH0_E22);

	TPM_Channel_t* ch1 = GPIO_Helper_SetupTPM_Channel_t(TPM2_CH1_A2);
	GPIO_Helper_SetupTPM_Channel_t(TPM2_CH1_B19);
	GPIO_Helper_SetupTPM_Channel_t(TPM2_CH1_B3);
	GPIO_Helper_SetupTPM_Channel_t(TPM2_CH1_E23);

	TPM* tpm = ch0->tpm;
	TPM_EnableClock(tpm, TRUE);
	int period_ms = 2000;
	int duty_ms = 1000;
	TPM_Channel_SetupPWM(ch0, period_ms * 1000, duty_ms * 1000);
	duty_ms = 200;
	TPM_Channel_SetupPWM(ch1, period_ms * 1000, duty_ms * 1000);
	TPM_EnableCounter(tpm, TRUE);
	while(1);
}
