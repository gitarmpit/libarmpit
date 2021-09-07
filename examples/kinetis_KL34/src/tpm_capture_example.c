#include "tpm_capture.h"
#include "mcg.h"
#include "sim.h"
#include "tpm.h"
#include "gpio.h"
#include "gpio_helper.h"

#pragma GCC diagnostic ignored "-Wunused-function"

static void generatePWM()
{
    //InitClock_FBI_Slow();
    //MCG_Enable_MCGIRCLK(TRUE);
    //SIM_Select_TPMSRC_MCGIRCLK();


    GPIO_PORT* portA = GPIO_GetInstance(PORTA);
    GPIO_EnableClock(portA, TRUE);
    GPIO_PIN a5 = GPIO_GetPin(portA, GPIO_PIN5);
    GPIO_SetAF(&a5, 3);

    TPM* tpm = TPM_GetInstance(TPM0_INSTANCE);
    TPM_EnableClock(tpm, TRUE);
    int period_us = 100;
    int duty_us = 33;
    TPM_Channel_t* ch = TPM_GetChannel(tpm, 2);
    TPM_Channel_SetupPWM(ch, period_us, duty_us);
    TPM_EnableCounter(tpm, TRUE);
}

static void generatePWM2()
{
    TPM_Channel_t* ch = GPIO_Helper_SetupTPM_Channel_t(TPM0_CH2_A5);
    int period_us = 61;
    int duty_us = 33;
    TPM_Channel_SetupPWM(ch, period_us, duty_us);
    TPM_EnableCounter(ch->tpm, TRUE);
}


void test_tpm_capture()
{
    InitClock_FEI_48Mhz_Bus_24Mhz();
    SIM_Select_TPMSRC_MCGFLLCLK();
    TPM_FREQ = CORE_FREQ;
    SIM_Select_FLL();

    generatePWM2();

    TPM_Capture_Context ctx;
    TPM_Channel_t* ch = GPIO_Helper_SetupTPM_Channel_t(TPM1_CH0_A12);
    GPIO_PIN a12 = GPIO_Helper_GetPin("A12");

    TPM_Capture_Setup(&ctx, ch, &a12, 200);
    TPM_EnableCounter(ch->tpm, TRUE);

    while(1)
    {
        uint32_t period, duty;
        TPM_Capture_GetValue(&ctx, &period, &duty);
        _printf ("period: %lu, duty: %lu\n", period, duty);
        delay_ms(1000);
    }

}

static void generatePWM3(TPM_Pins pin, int period_us, int duty_us)
{
    TPM_Channel_t* ch = GPIO_Helper_SetupTPM_Channel_t(pin);
    TPM_EnableCounter(ch->tpm, FALSE);
    TPM_Channel_SetupPWM(ch, period_us, duty_us);
    TPM_EnableCounter(ch->tpm, TRUE);
}


void test_tpm_capture2()
{
    InitClock_FEI_48Mhz_Bus_24Mhz();
    SIM_Select_TPMSRC_MCGFLLCLK();
    TPM_FREQ = CORE_FREQ;
    SIM_Select_FLL();

    // PWM
    generatePWM3 (TPM1_CH0_A12, 10, 6);
    generatePWM3 (TPM1_CH1_A13, 10, 5);

    TPM_Capture_Context ctx1;
    GPIO_PIN d0 = GPIO_Helper_GetPin("D0");
    TPM_Channel_t* ch_in1 = GPIO_Helper_SetupTPM_Channel_t(TPM0_CH0_D0);
    TPM_Capture_Setup(&ctx1, ch_in1, &d0, 200);


    TPM_Capture_Context ctx2;
    TPM_Channel_t* ch_in2 = GPIO_Helper_SetupTPM_Channel_t(TPM0_CH2_D2);
    GPIO_PIN d2 = GPIO_Helper_GetPin("D2");
    TPM_Capture_Setup(&ctx2, ch_in2, &d2, 200);

    TPM_EnableCounter(ch_in1->tpm, TRUE);

    while(1)
    {
        uint32_t period, duty;
        TPM_Capture_GetValue(&ctx1, &period, &duty);
        _printf ("period1: %lu, duty1: %lu\n", period, duty);

        TPM_Capture_GetValue(&ctx2, &period, &duty);
        _printf ("period2: %lu, duty2: %lu\n", period, duty);

        delay_ms(1000);
    }

}
