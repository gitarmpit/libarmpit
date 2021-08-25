#include "tpm_cpp.h"
#include "sim.h"
#include "mcg.h"
#include "gpio_helper.h"

class MyIntHandler : public TPM_TOF_Handler
{
public:
	MyIntHandler (const char* cpin)
	{
		_pin = GPIO_Helper_GetPin(cpin);
		GPIO_SetupOut(&_pin);
	}
	void HandleInterrupt (TPM* tpm)
	{
		GPIO_TogglePin(&_pin);
	}
private:
	GPIO_PIN _pin;
};

void test_tpm_cpp()
{

	//SIM_Select_FLL();
	//SIM_Select_TPMSRC_MCGFLLCLK();
    //InitClock_FEI_24Mhz_Bus_24Mhz();

	InitClock_FBI_Slow();
	MCG_Enable_MCGIRCLK(TRUE);
	SIM_Select_TPMSRC_MCGIRCLK();
	TPM_FREQ = CORE_FREQ;

    MyIntHandler h ("E30");

	TPM_1* tpm1 = TPM_1::GetInstance();
	tpm1->SetInterruptHandler(&h);
	tpm1->EnableClock(TRUE);
	tpm1->SetTimerPeriod_ms(1000);
	tpm1->EnableInterrupt(TRUE);
	tpm1->EnableCounter(TRUE);
	while(1)
		;

}
