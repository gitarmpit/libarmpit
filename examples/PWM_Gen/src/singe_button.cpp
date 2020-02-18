#include "adc_cpp.h"
#include "gpio_cpp.h"
#include "RCC_Helper.h"
#include "debug.h"
#include "GPIO_Helper.h"
#include "timer_cpp.h"
#include <math.h>
#include "button_handler/button_handler.h"


class MyButtonHandler3 : public ButtonHandler
{
private:
	TIMER_Channel* _pwmChannel;
	int _period;
    const int ranges[5] = {100000, 10000, 1000, 100, 10};
    int _current = 0;
    int _offset = 0;

    void update()
	{
		int p = ranges[_current] + _offset;
		_pwmChannel->SetupPWM1(p, p/2);
	}

public:
    MyButtonHandler3 (TIMER* timer, TIMER_Channel* pwmChannel) : ButtonHandler (timer),
	_pwmChannel(pwmChannel)
    {
    	update();
    }

    void setOffset (int offset)
    {
    	update();
    }

    virtual void OnStateChange(Button* button)
    {
    }

    virtual void OnClick(Button* button)
    {
    	_current = ++_current % 5;
    	update();
    }

    virtual void OnDoubleClick(Button* button)
    {
    	//_current = ++_current % 3;
    	//update();
    }
};


void SingleButton()
{
    GPIOA::GetInstance()->EnablePeripheralClock(true);
    TIMER_With_Channels* timer = TIM1::GetInstance();
    GPIOA::GetInstance()->SetupGPIO_OutAltPP(GPIO_PIN10);
    int channel = 3;

    timer->EnablePeripheralClock(true);
    TIMER_Channel* ch = timer->GetChannel(channel);
    ch->EnableChannel(true);
    timer->EnableCounter(true);
    ch->SetupPWM1(0, 0);

    TIMER* tim3 = TIM3::GetInstance();
    MyButtonHandler3 buttonHandler (tim3, ch);
    GPIO_PIN* pin = GPIO_Helper::GetPin("A7");
    Button b1 (pin, 1);
    buttonHandler.AddButton (&b1);

    while(1)
    	;

    GPIO_PIN* adcPin0 = GPIO_Helper::GetPin("A3");
    adcPin0->SetupGPIO_InAnalog();

    ADC1* adc1 = ADC1::GetInstance();
    adc1->Enable(true);
    adc1->AddConversion (ADC_IN3, ADC_SMP_SLOW);
    volatile int val0 = -1;

    while (1)
    {
      	int val = 0;
      	for (int i = 0; i < 32; ++i)
      	{
      		val += adc1->SingleConversion();
      	}

      	val /= 32;

        if (val0 == -1 || abs((int)(val - val0)) > 0)
        {
        	buttonHandler.setOffset(val);
        }

        val0 = val;
        delay_us(1000);
    }

}

