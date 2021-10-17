#include "i2c.h"
#include "gpio_helper.h"
#include "lcd/Adafruit_SSD1306.h"
#include "mcg.h"
#include "system_time.h"
#include "tpm.h"
#include "sim.h"
#include "adc.h"
#include <stdlib.h>
#include "button_handler/button_handler.h"
#include "gpio_helper.h"
#include "sim.h"

int range0 = 1;
int range = 1;

class MyButtonHandler2 : public ButtonHandler
{
public:

	MyButtonHandler2 (PIT* pit) : ButtonHandler (pit)
    {
    }

    virtual void OnStateChange(Button* button)
    {
    	UNUSED(button);
    }

    virtual void OnClick(Button* button)
    {
    	UNUSED(button);
    	++range;
    	if (range > 10)
    	{
    		range = 1;
    	}

    }

    virtual void OnDoubleClick(Button* button)
    {
    	UNUSED(button);
    }
};

Adafruit_SSD1306* SSD1306_GetInstance();

ADC* adc;
ADC_Channel adc_ch;
Adafruit_SSD1306* lcd;
TPM_Channel_t* tpm_ch;
MyButtonHandler2* bh;

static void setup()
{
	InitClock_FEI_48Mhz_Bus_24Mhz();
	lcd = SSD1306_GetInstance();
	lcd->clearDisplay();
	lcd->write(0,5, "TEST");

	SIM_Select_TPMSRC_MCGFLLCLK();
	TPM_FREQ = CORE_FREQ;

    tpm_ch = GPIO_Helper_SetupTPM_Channel_t(TPM0_CH3_E30);
	int period_us = 1000;
	int duty_us = 500;
	TPM_Channel_SetupPWM(tpm_ch, period_us, duty_us);
	TPM_EnableCounter(tpm_ch->tpm, TRUE);

	lcd->display();

    adc_ch = GPIO_Helper_SetupADC(ADC0, ADC_CH_SE14);
    adc = adc_ch.adc;
    ADC_SetClockSource_BusDiv2(adc);
    ADC_SetClock_Div4(adc);
    ADC_SetHardwareAveraging_32(adc);
    ADC_SetLongConversion_20(adc);
    ADC_SetHighSpeedConversion(adc, FALSE);
    BOOL rc = ADC_EnableCalibration(adc);

    if (!rc)
    {
        while(1);
    }

    ADC_SetMode_12bit(adc);

    ADC_SetChannel(&adc_ch);

    //
	SIM_Enable_PIT(TRUE);

	PIT* pit = PIT_GetInstance(PIT0);
	bh = new MyButtonHandler2(pit);
	bh->SetUpdateIntervalUs(1000);
	bh->SetSettleTimeUs(5000);
    GPIO_PIN pin = GPIO_Helper_GetPin("A12");
    Button b1 (&pin, 1);
    bh->AddButton (&b1);
    bh->Init(true);

}

static void pwm_gen2()
{
	setup();

    uint16_t res, res0 = 0;
    int start_freq = 1000;
    int val0 = 0;
    int count_changed = 0;
    int cmax = 0;
    while(1)
    {
        res = ADC_SingleConversion(&adc_ch);
        if (res < 5)
        {
        	res = 5;
        }
        if (res > 4090)
        {
        	res = 4090;
        }
        int16_t val = res - 5;
        int freq = start_freq + (float)val*99000/4085;
        int period_us = 1000000/freq;
        if (val0 != val)
        {
        	++count_changed;
        	if (count_changed > 3)
        	{
        		lcd->printf(0,5, "%-4d %-6d Hz %-6d us", val, freq, period_us);
        		lcd->printf(0,6, "cmax: %-4d", cmax);
        		lcd->display();
        		TPM_Channel_SetupPWM(tpm_ch, period_us, period_us/2);
        	}
        	if (count_changed > cmax)
        	{
        		cmax = count_changed;
        	}
        }
        else
        {
        	count_changed = 0;
        }
        delay_ms(10);
        res0 = res;
        val0 = val;
    }

	while(1)
		;
}

static void pwm_gen3()
{
	setup();

    int16_t res, res0 = 0;
    int16_t delta0 = 0;
    int start_freq = 1000;
    int count_changed = 0;
    bool once = true;
    while(1)
    {
        res = ADC_SingleConversion(&adc_ch);
        if (res < 5)
        {
        	// res = 5;
        }
        if (res > 4090)
        {
        	// res = 4090;
        }

    	int16_t delta = res - res0;
    	// check if it is changing in the same direction
    	//lcd->printf(0,4, "res0: %4d, res: %4d", res0, res);
    	//lcd->display();
        if (delta != 0 && ((delta ^ delta0) >= 0))
        {
        	++count_changed;
        }
        else
        {
        	count_changed = false;
        }

    	if (once || count_changed > 2)
        {
    		once = false;
        	int16_t val = res - 5;
        	int freq = start_freq + (float)val*99000/4085;
        	int period_us = 1000000/freq;
        	lcd->printf(0,5, "%-4d", res);
        	//lcd->printf(0,5, "%-4d %-6d Hz %-6d us", val, freq, period_us);
        	//lcd->printf(0,6, "cmax: %-4d", cmax);
        	lcd->display();
        	//TPM_Channel_SetupPWM(ch, period_us, period_us/2);
        }

        delay_ms(10);
        delta0 = delta;
        res0 = res;
    }

	while(1)
		;
}

// moving average
static void pwm_gen1()
{
	setup();

    uint16_t res, res0 = 0;
    uint16_t ema_s = 0;
    float 	ema_a = 0.3;
    int period0 = 0;
    int start_freq = 100 * range;
    int end_freq = start_freq*100;
    int val0 = 0;
    bool once = false;

    while(1)
    {
    	if (range0 != range)
    	{
    	    start_freq = 100 * range;
    	    end_freq = start_freq*100;
    	}
        res = ADC_SingleConversion(&adc_ch);
        ema_s = ema_a*res + (1.0-ema_a)*(float)ema_s;
        if (ema_s < 5)
        {
        	ema_s = 5;
        }
        if (ema_s > 4090)
        {
        	ema_s = 4090;
        }
        int16_t val = ema_s - 5;
        int freq = start_freq + (float)val*(end_freq-start_freq)/4085;
        int period_us = 1000000/freq;
        if (!once || val0 != val || range0 != range)
        {
        	once = true;
        	lcd->printf(0,5, "%4d %6dHz %5dus", val, freq, period_us);
        	lcd->printf(0,6, "range: %2d", range);
        	lcd->display();
        	TPM_Channel_SetupPWM(tpm_ch, period_us, period_us/2);
        	if (range0 != range)
        		range0 = range;
        }
        delay_ms(10);
        res0 = res;
        period0 = period_us;
        val0 = val;
    }

	while(1)
		;
}

void pwm_gen()
{
	pwm_gen1();
	while(1);
}
