#include "button_handler/button_handler.h"
#include "gpio_helper.h"
#include "mcg.h"
#include "sim.h"

class MyButtonHandler : public ButtonHandler
{
private:
	GPIO_PIN* _ledPin;
public:
    MyButtonHandler (PIT* pit, GPIO_PIN* ledPin) : ButtonHandler (pit)
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
	SIM_Enable_PIT(TRUE);

	PIT* pit = PIT_GetInstance(PIT0);
	GPIO_PIN ledPin = GPIO_Helper_GetPin("D7");
	GPIO_SetupOut(&ledPin);
	GPIO_TogglePin(&ledPin);
	MyButtonHandler bh (pit, &ledPin);
	bh.SetUpdateIntervalUs(1000);
	bh.SetSettleTimeUs(5000);
    GPIO_PIN pin = GPIO_Helper_GetPin("A12");
    Button b1 (&pin, 1);
    bh.AddButton (&b1);
    bh.Init(true);
    while(1)
    	;
}
