#include "adc_cpp.h"
#include "gpio_cpp.h"
#include "RCC_Helper.h"
#include "debug.h"
#include "GPIO_Helper.h"
#include "timer_cpp.h"
#include <math.h>
#include "button_handler/button_handler.h"

static int _scale;
static int  range[] = { 2, 200, 2000, 10000};

class MyButtonHandler4: public ButtonHandler {
private:
	const int max_scale = 2;

public:
	MyButtonHandler4(TIMER* timer) :
			ButtonHandler(timer) {
		_scale = max_scale;
	}

	virtual void OnStateChange(Button* button) {
	}

	virtual void OnClick(Button* button) {
		--_scale;
		if (_scale < 0)
		{
			_scale = max_scale;
		}
	}

	virtual void OnDoubleClick(Button* button) {
	}
};

//two ADCs continuous
void TwoADCs2() {

	TIMER* tim3 = TIM3::GetInstance();
	MyButtonHandler4 buttonHandler(tim3);
	GPIO_PIN* pin = GPIO_Helper::GetPin("A7");
	Button b1(pin, 1);
	buttonHandler.AddButton(&b1);
	////////////////////////////////////////////////////////////

	GPIOA::GetInstance()->EnablePeripheralClock(true);
	TIMER_With_Channels* timer = TIM1::GetInstance();
	GPIOA::GetInstance()->SetupGPIO_OutAltPP(GPIO_PIN10);
	int channel = 3;

	timer->EnablePeripheralClock(true);
	TIMER_Channel* ch = timer->GetChannel(channel);
	ch->EnableChannel(true);
	timer->EnableCounter(true);
	ch->SetupPWM1(0, 0);

	////////////////////////////////////////////

	GPIO_PIN* adcPin0 = GPIO_Helper::GetPin("A3");
	adcPin0->SetupGPIO_InAnalog();

	GPIO_PIN* adcPin1 = GPIO_Helper::GetPin("A4");
	adcPin1->SetupGPIO_InAnalog();

	ADC1* adc1 = ADC1::GetInstance();
	adc1->Enable(true);
	volatile int val = -1, val0 = -1;
	volatile int val2 = -1, val2_0 = -1;
	int period0 = 0, period;

	while (1) {
		adc1->ClearSequence();
		adc1->AddConversion(ADC_IN3, ADC_SMP_SLOW);

		val = 0;
		for (int i = 0; i < 32; ++i) {
			val += adc1->SingleConversion();
		}

		val /= 32;

		adc1->ClearSequence();
		adc1->AddConversion(ADC_IN4, ADC_SMP_SLOW);

		val2 = 0;
		for (int i = 0; i < 32; ++i) {
			val2 += adc1->SingleConversion();
		}

		val2 /= 32;

		//int period_us = 2 + (int) (((float) (val + val2)) / 81.92);
		//int period_us = 2 + (int) (((float) (val + val2)) / 40.96);


		if (_scale == 0)
		{
			period = 2 + (int) (((float) (val + val2)) * (200 - 2) / 8192);
		}
		else if (_scale == 1)
		{
			period = 200 + (int) (((float) (val + val2)) * (2000 - 200) / 8192);
		}
		else if (_scale == 2)
		{
			period = 2000 + (int) (((float) (val + val2)) * (100000-2000) / 8192);
		}



		// (100000-2000) / 4096) = 23.92578125
		//period = 2000 + (float)val * 23.92578125 + val2; //val2/171.196081633;
		//period = 200 + (float)val * 2.392578125 + val2/40.96; //val2/171.196081633;


		if (period != period0)
		{
			ch->UpdatePWM(period, period / 2);
		}

		period0 = period;
		val0 = val;
		val2_0 = val2;
		delay_us(1000);
	}

}
