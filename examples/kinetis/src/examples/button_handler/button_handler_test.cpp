#include "button_handler.h"
#include "gpio_helper.h"
#include "mcg.h"
#include "sim.h"

class MyButtonHandler : public ButtonHandler
{
private:
	GPIO_PIN* _ledPin;
public:
    MyButtonHandler (TPM_Base* tpm, GPIO_PIN* ledPin) : ButtonHandler (tpm)
    {
    	_ledPin = ledPin;
    }


    virtual void OnStateChange(Button* button)
    {
    	UNUSED(button);
    }

    virtual void OnClick(Button* button)
    {
    	UNUSED(button);
    	GPIO_TogglePin(_ledPin);
    }

    virtual void OnDoubleClick(Button* button)
    {
    	UNUSED(button);
    }
};


void test_button_handler()
{
	InitClock_FEI_48Mhz_Bus_24Mhz();
	SIM_Select_TPMSRC_MCGFLLCLK();
	TPM_FREQ = CORE_FREQ;
	SIM_Select_FLL();

	TPM2* tpm = TPM2::GetInstance();
	GPIO_PIN ledPin = GPIO_Helper_GetPin("D7");
	GPIO_SetupOut(&ledPin);
	GPIO_TogglePin(&ledPin);
	MyButtonHandler bh (tpm, &ledPin);
    GPIO_PIN pin = GPIO_Helper_GetPin("A12");
    Button b1 (&pin, 1);
    bh.AddButton (&b1);
    while(1)
    	;
}
