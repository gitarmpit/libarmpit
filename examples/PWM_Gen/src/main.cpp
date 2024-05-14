#include "adc_cpp.h"
#include "gpio_cpp.h"
#include "RCC_Helper.h"
#include "debug.h"
#include "GPIO_Helper.h"
#include "timer_cpp.h"
#include <math.h>
#include "button_handler/button_handler.h"

void TwoADCs2();
void SingleButton();


static TIMER_Channel* ch = 0;

/*
static void Timer_Ch_Setup()
{
    TIMER_With_Channels* t4 = GPIO_Helper::SetupTimer(TIM4_PB_6_7_8_9);
    uint32_t period_us = 1000;
    uint32_t duty_us = 500;
    t4->EnableUpdateInterrupt(true);
    t4->EnableNVICInterrupt(true);

    //B9 46
    TIMER_Channel* ch = t4->GetChannel(4);
    ch->SetupPWM1(period_us, duty_us);
    ch->EnableChannel(true);

    t4->EnableCounter(true);
    while(1)
    	;

}
*/

static void PWM2()
{
    GPIOB::GetInstance()->EnablePeripheralClock(true);
    GPIOB::GetInstance()->SetupGPIO_OutAltPP(GPIO_PIN9);
    TIMER_With_Channels* timer4 = TIM4::GetInstance();
    timer4->EnablePeripheralClock(true);
    ch = timer4->GetChannel(4);
    // ch4->SetupPWM1(20000, 1000);
    ch->EnableChannel(true);
    timer4->EnableCounter(true);
    ch->SetupPWM1(0, 0);
}

#if 0
static void TestSingleConversion() 
{
#if defined(STM32F1)
	RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
#elif defined(STM32F4)
	RCC_EnableHSI_168Mhz();
    RCC_SetAPB2PrescalerDiv8();
#endif

    //Timer_Ch_Setup();
    PWM2();

    GPIO_PIN* adcPin = GPIO_Helper::GetPin("A0");
    adcPin->SetupGPIO_InAnalog();

    //GPIO_PIN* outPin = GPIO_Helper::GetPin("B1");
    //outPin->SetupGPIO_OutPP();
    //outPin->Reset();

    ADC1* adc1 = ADC1::GetInstance();
    adc1->Enable(true);
    //adc1->SetPrescalerDIV8();
    adc1->AddConversion (ADC_IN0, ADC_SMP_FAST);
    volatile uint32_t val, val0 = 0;
    while (1) 
    {
        val = adc1->SingleConversion();
        if (val == 0)
        	val = 1;
        if (abs((int)(val - val0)) > 1)
        {
        //ch4->UpdatePWMDuty(res1);
        	ch->UpdatePWM(val*10, val*5);
        }
        val0 = val;
        /*
        if (res1 < 2000)
        {
            outPin->Set();
        }
        else
        {
            outPin->Reset();
        }
        */
        delay_us(500);
    }

}
#endif


class MyButtonHandler1 : public ButtonHandler
{
private:
	TIMER_Channel* _pwmChannel;
	int _period_us;
	int step = 10000;
	const int _max = 100000;

	void update()
	{
		_pwmChannel->SetupPWM1(_period_us, _period_us/2);
		if (_period_us <= 10)
		{
			step = 1;
		}
		else if (_period_us <= 100)
		{
		    step = 10;
		}
		else if (_period_us <= 1000)
		{
			step = 100;
		}
		else if (_period_us <= 10000)
		{
			step = 1000;
		}
		else
		{
			step = 10000;
		}
	}

public:
    MyButtonHandler1 (TIMER* timer, TIMER_Channel* pwmChannel) : ButtonHandler (timer),
	_pwmChannel(pwmChannel)
    {
    	_period_us = _max;
    	update();
    }


    virtual void OnStateChange(Button* button)
    {
    }

    virtual void OnClick(Button* button)
    {
    	_period_us -= step;
    	if (_period_us < 2)
    	{
    		_period_us = 2;
    	}
    	update();
    }

    virtual void OnDoubleClick(Button* button)
    {
    	_period_us += step;
    	if (_period_us > _max)
    	{
    		_period_us = _max;
    	}
    	update();
    }
};


static int filter(int new_data)
{
  static int data[8] = { 0,0,0,0,0,0,0,0 };
  static int oldiest_idx = 0;
  static int  sum = 0;

  sum = sum - data[oldiest_idx] + new_data;

  data[oldiest_idx] = new_data;

  //p_oldiest_ptr = (p_oldiest_ptr==7) ? 0 :(p_oldiest_ptr++);
  oldiest_idx = (++oldiest_idx) % 8;

  return(sum>>3);
}

static void run()
{
	/*
	GPIO_PIN* p = GPIO_Helper::GetPin("A9");
	p->SetupGPIO_OutPP();
	p->Reset();
	delay(2000);
	p->Set();
	while(1)
		;
	 */

	//PWM
    //GPIOB::GetInstance()->EnablePeripheralClock(true);
    //GPIOB::GetInstance()->SetupGPIO_OutAltPP(GPIO_PIN9);
    //TIMER_With_Channels* timer = TIM4::GetInstance();
    //int channel = 4;

	//TIMER_With_Channels* timer = GPIO_Helper::SetupTimer(TIM1_PA_8_9_10_11);

    GPIOA::GetInstance()->EnablePeripheralClock(true);
    TIMER_With_Channels* timer = TIM1::GetInstance();
    GPIOA::GetInstance()->SetupGPIO_OutAltPP(GPIO_PIN10);
    int channel = 3;

    timer->EnablePeripheralClock(true);
    TIMER_Channel* ch = timer->GetChannel(channel);
    ch->EnableChannel(true);
    timer->EnableCounter(true);


    TIMER* tim3 = TIM3::GetInstance();
    MyButtonHandler1 buttonHandler (tim3, ch);
    //GPIO_PORT* pinPort = GPIOB::GetInstance();
    //pinPort->EnablePeripheralClock(true);
    //GPIO_PIN*  pin = pinPort->GetPin(GPIO_PIN5);
    GPIO_PIN* pin = GPIO_Helper::GetPin("A7");
    Button b1 (pin, 1);
    buttonHandler.AddButton (&b1);
    while(1)
    	;
}

#if 1
static void run2()
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

    ////////////////////////////////////////////

    GPIO_PIN* adcPin0 = GPIO_Helper::GetPin("A3");
    adcPin0->SetupGPIO_InAnalog();

    GPIO_PIN* adcPin1 = GPIO_Helper::GetPin("A4");
    adcPin1->SetupGPIO_InAnalog();

    ADC1* adc1 = ADC1::GetInstance();
    adc1->Enable(true);
    adc1->AddConversion (ADC_IN4, ADC_SMP_SLOW);
    //adc1->SetPrescalerDIV8();
    volatile int val = -1, val0 = -1;
    volatile int val2, val2_0 = 0;
    volatile int total = 0;
    volatile int changed = 0;
    volatile int max_change = 0;
    float a = 0.1;

    //for (int i = 0; i < 128; ++i)
    //{
    // 	val = val * (1 - a) + a * adc1->SingleConversion();
    //}

    while (1)
    {
        //adc1->ClearSequence();
        //adc1->AddConversion (ADC_IN1, ADC_SMP_SLOW);
      	//val = adc1->SingleConversion() / 16;
      	++total;

      	// val = val * (1 - a) + a * adc1->SingleConversion();


      	val = 0;
      	for (int i = 0; i < 32; ++i)
      	{
      		val += adc1->SingleConversion();
      		//delay_us(300);
      	}

      	val /= 32;


      	//int period_us = val*64 + 2;
      	int period_us = val/256 + 2;

        //adc1->ClearSequence();
        //adc1->AddConversion (ADC_IN1, ADC_SMP_SLOW);
        //val2 = (adc1->SingleConversion() >> 8);

        // if (abs((int)(val - val0)) > 1)
      	if (val0 != -1 && abs((int)(val - val0)) > max_change)
      	{
      		max_change = abs((int)(val - val0));
      	}

        //if (val != val0)
        if (val0 == -1 || abs((int)(val - val0)) > 2)
        {
        //ch4->UpdatePWMDuty(res1);
        	ch->UpdatePWM(period_us, period_us/2);
        	++changed;
        }

        val0 = val;
        val2_0 = val2;
        /*
        if (res1 < 2000)
        {
            outPin->Set();
        }
        else
        {
            outPin->Reset();
        }
        */
        delay_us(1000);
    }

}
#endif

static void run3()
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

    ////////////////////////////////////////////

    GPIO_PIN* adcPin0 = GPIO_Helper::GetPin("A3");
    adcPin0->SetupGPIO_InAnalog();

    GPIO_PIN* adcPin1 = GPIO_Helper::GetPin("A4");
    adcPin1->SetupGPIO_InAnalog();

    ADC1* adc1 = ADC1::GetInstance();
    adc1->Enable(true);
    //adc1->AddConversion (ADC_IN4, ADC_SMP_SLOW);
    volatile int val = -1, val0 = -1;
    volatile int val2 = -1, val2_0 = -1;
    volatile int total = 0;
    volatile int changed = 0;
    volatile int max_change = 0;

    while (1)
    {
#ifdef STM32F1
        adc1->ClearSequence();
#endif
        adc1->AddConversion (ADC_IN3, ADC_SMP_SLOW);
      	++total;

      	val = 0;
      	for (int i = 0; i < 32; ++i)
      	{
      		val += adc1->SingleConversion();
      		//delay_us(300);
      	}

      	val /= 32;

#ifdef STM32F1
        adc1->ClearSequence();
#endif
        adc1->AddConversion (ADC_IN4, ADC_SMP_SLOW);

      	val2 = 0;
      	for (int i = 0; i < 32; ++i)
      	{
      		val2 += adc1->SingleConversion();
      		//delay_us(300);
      	}

      	val2 /= 32;

      	int period_us = 2 + (val + val2)*10;


      	//int period_us = val*64 + 2;

        //adc1->ClearSequence();
        //adc1->AddConversion (ADC_IN1, ADC_SMP_SLOW);
        //val2 = (adc1->SingleConversion() >> 8);

        // if (abs((int)(val - val0)) > 1)
      	if (val0 != -1 && abs((int)(val - val0)) > max_change)
      	{
      		max_change = abs((int)(val - val0));
      	}

        //if (val != val0)
        if (val0 == -1 || abs((int)(val - val0)) > 0 || abs((int)(val2 - val2_0)) > 0)
        {
        //ch4->UpdatePWMDuty(res1);
        	ch->UpdatePWM(period_us, period_us/2);
        	++changed;
        }

        val0 = val;
        val2_0 = val2;
        delay_us(1000);
    }

}
int main()
{
#if defined(STM32F1)
	RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
#elif defined(STM32F4)
	RCC_EnableHSI_168Mhz();
    RCC_SetAPB2PrescalerDiv8();
#endif

    Debug_EnableCYCCNT(true);

    TwoADCs2();
    //SingleButton();
    return 0;
}
