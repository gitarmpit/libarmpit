#include "tpm_capture_cpp.h"
#include "gpio_helper.h"
#include "mcg.h"
#include "sim.h"

static void generatePWM3(TPM_Pins pin, int period_us, int duty_us)
{
    TPM_Channel* ch = GPIO_Helper_SetupTPM_Channel(pin);
    TPM_Base* tpm = ch->GetTPM();
    tpm->EnableCounter(FALSE);
    ch->SetupPWM(period_us, duty_us);
    tpm->EnableCounter(TRUE);
}


void test_tpm_capture_cpp1()
{
    InitClock_FEI_48Mhz_Bus_24Mhz();
    SIM_Select_TPMSRC_MCGFLLCLK();
    TPM_FREQ = CORE_FREQ;
    SIM_Select_FLL();

    // PWM
    generatePWM3 (TPM0_CH2_A5, 6313, 3821);

    GPIO_PIN a12 = GPIO_Helper_GetPin("A12");
    TPM_Channel* ch_in1 = GPIO_Helper_SetupTPM_Channel(TPM1_CH0_A12);

    TPM_PWM_Capture cap (ch_in1, &a12);
    cap.SetUp(10000);
    ch_in1->GetTPM()->EnableCounter(true);

    while(1)
    {
        uint32_t period, duty;
        cap.GetValue(period, duty);
        _printf ("period1: %lu, duty1: %lu\n", period, duty);

        //TPM_Capture_GetValue(&ctx2, &period, &duty);
        //_printf ("period2: %lu, duty2: %lu\n", period, duty);

        delay_ms(1000);
    }


}
