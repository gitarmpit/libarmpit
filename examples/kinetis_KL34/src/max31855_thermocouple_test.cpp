#include "i2c.h"
#include "mcg.h"
#include "gpio_helper.h"
#include "sim.h"
#include "system_time.h"
#include "pit.h"
#include "lcd/Adafruit_SSD1306.h"
#include "lcd/ee_printf.h"
#include "gpio_helper.h"
#include "button_handler/button_handler.h"
#include "tpm.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

Adafruit_SSD1306* SSD1306_GetInstance();

bool oven_started = false;

typedef struct _TC
{
	SPI* spi;
} TC;

static bool readTC(SPI* spi, GPIO_PIN& ss, int16_t& traw_ext, int16_t& traw_int)
{
	GPIO_ResetPin(&ss);
	uint8_t b = SPI_TransmitByte(spi, 0);
	traw_ext = b << 8;
	traw_ext |= SPI_TransmitByte(spi, 0);

	b = SPI_TransmitByte(spi, 0);
	traw_int = b << 8;
	traw_int |= SPI_TransmitByte(spi, 0);

	GPIO_SetPin(&ss);

	if ( (traw_ext & 0x1) || (traw_int & 0x1) || (traw_int & 0x2) || (traw_int & 0x4) )
	{
		return false;
	}

	traw_int >>= 4;
	traw_ext >>= 2;
	return true;
}

static SPI* InitTC_SPI(GPIO_PIN& ss)
{
    GPIO_PORT* portD = GPIO_GetInstance(PORTD);
    GPIO_EnableClock(portD, TRUE);

    ss = GPIO_GetPin(portD, GPIO_PIN0);
    GPIO_SetupOut(&ss);

    GPIO_PIN ck = GPIO_GetPin(portD, GPIO_PIN1);
    GPIO_SetAF(&ck, 2);

    GPIO_PIN mi = GPIO_GetPin(portD, GPIO_PIN3);
    GPIO_SetAF(&mi, 2);

    SPI* spi0 = SPI_GetInstance(SPI0);
	SPI_Initialize(spi0, TRUE, 2, SPI_DIV32, TRUE, TRUE, FALSE);
	return spi0;
}

static UART* InitUART()
{
    GPIO_PORT* portE = GPIO_GetInstance(PORTE);
    GPIO_EnableClock(portE, TRUE);
    GPIO_PIN e0 = GPIO_GetPin(portE, GPIO_PIN0);
    GPIO_SetAF(&e0, 3);
    GPIO_PIN e1 = GPIO_GetPin(portE, GPIO_PIN1);
    GPIO_SetAF(&e1, 3);

    UART* uart1 = UART_GetInstance(UART1);
    UART_Initialize(uart1, (uint32_t)UART_BAUD_115200, TRUE, TRUE);
    return uart1;
}

class ThermoButtonHandler : public ButtonHandler
{
public:
    ThermoButtonHandler (PIT* pit) : ButtonHandler (pit)
    {
    }


    virtual void OnStateChange(Button* button)
    {
    	UNUSED(button);
    }

    virtual void OnClick(Button* button)
    {
    	UNUSED(button);
    	oven_started = true;
    }

    virtual void OnDoubleClick(Button* button)
    {
    	UNUSED(button);
    }
};



static void test1()
{
	InitClock_FEI_48Mhz_Bus_24Mhz();


    GPIO_PORT* portD = GPIO_GetInstance(PORTD);
    GPIO_EnableClock(portD, TRUE);

    GPIO_PIN ss = GPIO_GetPin(portD, GPIO_PIN0);
    GPIO_SetupOut(&ss);

    GPIO_PIN ck = GPIO_GetPin(portD, GPIO_PIN1);
    GPIO_SetAF(&ck, 2);

    GPIO_PIN mi = GPIO_GetPin(portD, GPIO_PIN3);
    GPIO_SetAF(&mi, 2);

    //UART* uart = GPIO_Helper_SetupUART (UART0_PD_6_7, 115200, TRUE, TRUE);


    GPIO_PORT* portE = GPIO_GetInstance(PORTE);
    GPIO_EnableClock(portE, TRUE);
    GPIO_PIN e0 = GPIO_GetPin(portE, GPIO_PIN0);
    GPIO_SetAF(&e0, 3);

    UART* uart1 = UART_GetInstance(UART1);
    UART_Initialize(uart1, (uint32_t)UART_BAUD_115200, TRUE, TRUE);


	SPI* spi0 = SPI_GetInstance(SPI0);
	SPI_Initialize(spi0, TRUE, 2, SPI_DIV32, TRUE, TRUE, FALSE);

	Adafruit_SSD1306* lcd = SSD1306_GetInstance();
	lcd->setTextColor(1, 0);
	lcd->clearDisplay();
	//lcd->write(0,5, "Hello, World5! 55555");
	lcd->display();

	SIM_Enable_PIT(TRUE);
	PIT* pit = PIT_GetInstance(PIT0);
	ThermoButtonHandler bh (pit);
	bh.SetUpdateIntervalUs(1000);
	bh.SetSettleTimeUs(5000);
    GPIO_PIN pin = GPIO_Helper_GetPin("A13");
    Button b1 (&pin, 1);
    bh.AddButton (&b1);
    bh.Init(true);
	while(!oven_started)
	{
		delay_ms(1);
	}


	//warm-up, discard some initial garbage
	for (int i = 0; i < 10; ++i)
	{
		GPIO_ResetPin(&ss);
		SPI_TransmitByte(spi0, 0);
		SPI_TransmitByte(spi0, 0);
		SPI_TransmitByte(spi0, 0);
		SPI_TransmitByte(spi0, 0);
		GPIO_SetPin(&ss);
		delay_ms(10);
	}


	//lcd->printf(0,4, "Started");
	//lcd->display();

	// Signal to start logging
	UART_SendByte(uart1, 0xff);
	delay_ms(100);
	UART_SendByte(uart1, 0xff);
	delay_ms(100);
	UART_SendByte(uart1, 0xff);
	delay_ms(100);

	int16_t traw_ext, traw_int;
	while (1)
	{
		GPIO_ResetPin(&ss);
		uint8_t b = SPI_TransmitByte(spi0, 0);
		traw_ext = b << 8;
		traw_ext |= SPI_TransmitByte(spi0, 0);

		b = SPI_TransmitByte(spi0, 0);
		traw_int = b << 8;
		traw_int |= SPI_TransmitByte(spi0, 0);

		GPIO_SetPin(&ss);

		if ( (traw_ext & 0x1) || (traw_int & 0x1) || (traw_int & 0x2) || (traw_int & 0x4) )
		{
			continue;
		}


		traw_int >>= 4;
		traw_ext >>= 2;

	    char buf[256];
	    float t_int = traw_int * 0.0625;
	    float t_ext = traw_ext * 0.25;
	    ee_sprintf(buf, "%7.4f", t_int);
		lcd->printf(0,5, "%s\n", buf);
	    ee_sprintf(buf, "%7.4f", t_ext);

		lcd->printf(0,6, "%s\n", buf);

		UART_SendByte(uart1, (uint8_t)t_ext);

		lcd->display();
		delay_ms(1000);
	}

	while(1);
}

static void test2()
{
	InitClock_FEI_48Mhz_Bus_24Mhz();

	SIM_Select_FLL();
	SIM_Select_TPMSRC_MCGFLLCLK();
	TPM_FREQ = CORE_FREQ;
	MCG_Select_MCGOUTCLK_FLL_PLL();

    GPIO_PORT* portD = GPIO_GetInstance(PORTD);
    GPIO_EnableClock(portD, TRUE);

    GPIO_PIN ss = GPIO_GetPin(portD, GPIO_PIN0);
    GPIO_SetupOut(&ss);

    GPIO_PIN ck = GPIO_GetPin(portD, GPIO_PIN1);
    GPIO_SetAF(&ck, 2);

    GPIO_PIN mi = GPIO_GetPin(portD, GPIO_PIN3);
    GPIO_SetAF(&mi, 2);


    TPM_Channel_t* ch = GPIO_Helper_SetupTPM_Channel_t(TPM0_CH3_E30);
	TPM* tpm = TPM_GetInstance(TPM0_INSTANCE);
	TPM_EnableClock(tpm, TRUE);
	int period_us = 160000;
	int duty_us = 80000;
	TPM_Channel_SetupPWM(ch, period_us, duty_us);
	TPM_EnableCounter(tpm, TRUE);
	//TPM_Channel_UpdatePWMDuty(ch, 100000);


    GPIO_PORT* portE = GPIO_GetInstance(PORTE);
    GPIO_EnableClock(portE, TRUE);
    GPIO_PIN e0 = GPIO_GetPin(portE, GPIO_PIN0);
    GPIO_SetAF(&e0, 3);

    UART* uart1 = UART_GetInstance(UART1);
    UART_Initialize(uart1, (uint32_t)UART_BAUD_115200, TRUE, TRUE);


	SPI* spi0 = SPI_GetInstance(SPI0);
	SPI_Initialize(spi0, TRUE, 2, SPI_DIV32, TRUE, TRUE, FALSE);

	Adafruit_SSD1306* lcd = SSD1306_GetInstance();
	lcd->setTextColor(1, 0);
	lcd->clearDisplay();
	//lcd->write(0,5, "Hello, World5! 55555");
	lcd->display();

	SIM_Enable_PIT(TRUE);
	PIT* pit = PIT_GetInstance(PIT0);
	ThermoButtonHandler bh (pit);
	bh.SetUpdateIntervalUs(1000);
	bh.SetSettleTimeUs(5000);
    GPIO_PIN pin = GPIO_Helper_GetPin("A13");
    Button b1 (&pin, 1);
    bh.AddButton (&b1);
    bh.Init(true);
	while(!oven_started)
	{
		delay_ms(1);
	}

	//warm-up, discard some initial garbage
	for (int i = 0; i < 10; ++i)
	{
		GPIO_ResetPin(&ss);
		SPI_TransmitByte(spi0, 0);
		SPI_TransmitByte(spi0, 0);
		SPI_TransmitByte(spi0, 0);
		SPI_TransmitByte(spi0, 0);
		GPIO_SetPin(&ss);
		delay_ms(10);
	}


	//lcd->printf(0,4, "Started");
	//lcd->display();

	// Signal to start logging
	UART_SendByte(uart1, 0xff);
	delay_ms(100);
	UART_SendByte(uart1, 0xff);
	delay_ms(100);
	UART_SendByte(uart1, 0xff);
	delay_ms(100);

	int16_t traw_ext, traw_int;
	while (1)
	{
		GPIO_ResetPin(&ss);
		uint8_t b = SPI_TransmitByte(spi0, 0);
		traw_ext = b << 8;
		traw_ext |= SPI_TransmitByte(spi0, 0);

		b = SPI_TransmitByte(spi0, 0);
		traw_int = b << 8;
		traw_int |= SPI_TransmitByte(spi0, 0);

		GPIO_SetPin(&ss);

		if ( (traw_ext & 0x1) || (traw_int & 0x1) || (traw_int & 0x2) || (traw_int & 0x4) )
		{
			continue;
		}


		traw_int >>= 4;
		traw_ext >>= 2;

	    char buf[256];
	    float t_int = traw_int * 0.0625;
	    float t_ext = traw_ext * 0.25;
	    ee_sprintf(buf, "%7.4f", t_int);
		lcd->printf(0,5, "%s\n", buf);
	    ee_sprintf(buf, "%7.4f", t_ext);

		lcd->printf(0,6, "%s\n", buf);

		UART_SendByte(uart1, (uint8_t)t_ext);

		lcd->display();
		delay_ms(1000);
	}

	while(1);

}

class RelayButtonHandler : public ButtonHandler
{
private:
	GPIO_PIN* _relay_pin;

public:
    RelayButtonHandler (PIT* pit, GPIO_PIN* relay_pin) : ButtonHandler (pit)
    {
    	_relay_pin = relay_pin;
    }

    virtual void OnStateChange(Button* button)
    {
    	UNUSED(button);
    }

    virtual void OnClick(Button* button)
    {
    	UNUSED(button);
    	GPIO_TogglePin(_relay_pin);
    }

    virtual void OnDoubleClick(Button* button)
    {
    	UNUSED(button);
    }
};


// Test relay via on/off button
static void test3()
{
	InitClock_FEI_48Mhz_Bus_24Mhz();
	GPIO_PIN relay_pin = GPIO_Helper_GetPin("E30");
	GPIO_SetupOut(&relay_pin);
	GPIO_ResetPin(&relay_pin);

	SIM_Enable_PIT(TRUE);
	PIT* pit = PIT_GetInstance(PIT0);
	RelayButtonHandler bh (pit, &relay_pin);
	bh.SetUpdateIntervalUs(1000);
	bh.SetSettleTimeUs(5000);
    GPIO_PIN pin = GPIO_Helper_GetPin("A13");
    Button b1 (&pin, 1);
    bh.AddButton (&b1);
    bh.Init(true);

    while(1);

}

static ADC* adc;
static ADC_Channel adc_ch;

static void setup_adc()
{
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

}


static void pwm_gen1()
{
	setup_adc();

    uint16_t res;
    uint16_t ema_s = 0;
    float 	ema_a = 0.3;
    int start_freq = 100;
    int end_freq = start_freq*100;
    int val0 = 0;
    bool once = false;

    while(1)
    {
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
        if (!once || val0 != val)
        {
        	once = true;
        	//lcd->printf(0,5, "%4d %6dHz %5dus", val, freq, period_us);
        	//lcd->display();
        	//TPM_Channel_SetupPWM(tpm_ch, period_us, period_us/2);
        }
        delay_ms(10);
        val0 = val;
    }

	while(1)
		;
}

typedef struct _ADC_Ctx
{
    int16_t ema_s;
    float 	ema_a;
    int16_t val0;
    int32_t duty_min;
    int32_t duty_max;
    Adafruit_SSD1306* lcd;
    TPM_Channel_t* ch;
} ADC_CTX;

static void PitHandler_ADC(void* ctx)
{
	ADC_CTX* adc_ctx = (ADC_CTX*)ctx;

	int16_t raw = ADC_SingleConversion(&adc_ch);
	adc_ctx->ema_s = adc_ctx->ema_a*raw + (1.0-adc_ctx->ema_a)*(float)adc_ctx->ema_s;
	if (adc_ctx->ema_s < 5)
	{
		adc_ctx->ema_s = 5;
	}
	if (adc_ctx->ema_s > 4090)
	{
		adc_ctx->ema_s = 4090;
	}
	int16_t val = adc_ctx->ema_s - 5;
	int32_t duty_us = adc_ctx->duty_min + (float)val*(adc_ctx->duty_max - adc_ctx->duty_min)/4085;
	if (adc_ctx->val0 != val)
	{
		//TPM_Channel_SetupPWM(tpm_ch, period_us, period_us/2);
		adc_ctx->lcd->printf(0,5, "%7d", duty_us);
		adc_ctx->lcd->display();
		TPM_Channel_UpdatePWMDuty(adc_ctx->ch, duty_us);
	}
	adc_ctx->val0 = val;
}

// test relay via PWM
static void test4()
{

	InitClock_FEI_48Mhz_Bus_24Mhz();
	SIM_Select_FLL();
	SIM_Select_TPMSRC_MCGFLLCLK();
	TPM_FREQ = CORE_FREQ;
	MCG_Select_MCGOUTCLK_FLL_PLL();

	/*
	InitClock_FBI_Slow();
	MCG_Enable_MCGIRCLK(TRUE);
	SIM_Select_TPMSRC_MCGIRCLK();
	TPM_FREQ = CORE_FREQ;
	*/

	Adafruit_SSD1306* lcd = SSD1306_GetInstance();
	lcd->setTextColor(1, 0);
	lcd->clearDisplay();
	//lcd->write(0,5, "Hello, World5! 55555");
	lcd->display();

	// UART1
    GPIO_PORT* portE = GPIO_GetInstance(PORTE);
    GPIO_EnableClock(portE, TRUE);
    GPIO_PIN e0 = GPIO_GetPin(portE, GPIO_PIN0);
    GPIO_SetAF(&e0, 3);
    GPIO_PIN e1 = GPIO_GetPin(portE, GPIO_PIN1);
    GPIO_SetAF(&e1, 3);

    UART* uart1 = UART_GetInstance(UART1);
    UART_Initialize(uart1, (uint32_t)UART_BAUD_115200, TRUE, TRUE);
    /*
    int8_t i = 0;
    while (1)
    {
        uint8_t r = UART_ReceiveByte(uart1);
        lcd->printf(0,5, "%8x\n", r);
        lcd->display();
        delay_ms(1000);
        UART_SendByte(uart1, i++);
    }
*/
	setup_adc();


	int period_us = 128000;
	int duty_us = 17000;

	ADC_CTX ctx;
    ctx.ema_s = 0;
    ctx.ema_a = 0.3;
    ctx.duty_min = duty_us;
    ctx.duty_max = period_us;
    ctx.val0 = 0;
    ctx.lcd = lcd;

	SIM_Enable_PIT(TRUE);
	PIT_EnableClock(TRUE);
	PIT* pit = PIT_GetInstance(PIT0);
	PIT_SetPeriod_us(pit, 10000);
	pit->interrupt_handler = PitHandler_ADC;
	pit->ctx = &ctx;
	PIT_EnableInterrupt(pit, TRUE);
	PIT_EnableTimer(pit, TRUE);


	TPM_Channel_t* ch = GPIO_Helper_SetupTPM_Channel_t(TPM0_CH3_E30);
	TPM* tpm = TPM_GetInstance(TPM0_INSTANCE);
	TPM_EnableClock(tpm, TRUE);
	TPM_Channel_SetupPWM(ch, period_us, duty_us);
	TPM_EnableCounter(tpm, TRUE);
	ctx.ch = ch;


	// thermocouple
    GPIO_PORT* portD = GPIO_GetInstance(PORTD);
    GPIO_EnableClock(portD, TRUE);

    GPIO_PIN ss = GPIO_GetPin(portD, GPIO_PIN0);
    GPIO_SetupOut(&ss);

    GPIO_PIN ck = GPIO_GetPin(portD, GPIO_PIN1);
    GPIO_SetAF(&ck, 2);

    GPIO_PIN mi = GPIO_GetPin(portD, GPIO_PIN3);
    GPIO_SetAF(&mi, 2);

    SPI* spi0 = SPI_GetInstance(SPI0);
	SPI_Initialize(spi0, TRUE, 2, SPI_DIV32, TRUE, TRUE, FALSE);

	//warm-up, discard some initial garbage
	for (int i = 0; i < 10; ++i)
	{
		GPIO_ResetPin(&ss);
		SPI_TransmitByte(spi0, 0);
		SPI_TransmitByte(spi0, 0);
		SPI_TransmitByte(spi0, 0);
		SPI_TransmitByte(spi0, 0);
		GPIO_SetPin(&ss);
		delay_ms(10);
	}


	// Signal to start logging
	UART_SendByte(uart1, 0xff);
	delay_ms(100);
	UART_SendByte(uart1, 0xff);
	delay_ms(100);
	UART_SendByte(uart1, 0xff);
	delay_ms(100);

	int16_t traw_ext, traw_int;
	while (1)
	{
		GPIO_ResetPin(&ss);
		uint8_t b = SPI_TransmitByte(spi0, 0);
		traw_ext = b << 8;
		traw_ext |= SPI_TransmitByte(spi0, 0);

		b = SPI_TransmitByte(spi0, 0);
		traw_int = b << 8;
		traw_int |= SPI_TransmitByte(spi0, 0);

		GPIO_SetPin(&ss);

		if ( (traw_ext & 0x1) || (traw_int & 0x1) || (traw_int & 0x2) || (traw_int & 0x4) )
		{
			continue;
		}


		traw_int >>= 4;
		traw_ext >>= 2;

	    char buf[256];
	    float t_int = traw_int * 0.0625;
	    float t_ext = traw_ext * 0.25;
	    ee_sprintf(buf, "%7.4f", t_int);
		lcd->printf(0,6, "%s\n", buf);
	    ee_sprintf(buf, "%7.4f", t_ext);

		lcd->printf(0,7, "%s\n", buf);

		UART_SendByte(uart1, (uint8_t)t_ext);

		lcd->display();
		delay_ms(1000);
	}


    while(1);

}

// C/sec
static void test5()
{

	InitClock_FEI_48Mhz_Bus_24Mhz();
	SIM_Select_FLL();
	SIM_Select_TPMSRC_MCGFLLCLK();
	TPM_FREQ = CORE_FREQ;
	MCG_Select_MCGOUTCLK_FLL_PLL();

	Adafruit_SSD1306* lcd = SSD1306_GetInstance();
	lcd->setTextColor(1, 0);
	lcd->clearDisplay();
	lcd->display();

	UART* uart1 = InitUART();

    // PWM
	int period_us = 128000;
	int duty_us = 128000;
	TPM_Channel_t* ch = GPIO_Helper_SetupTPM_Channel_t(TPM0_CH3_E30);
	TPM* tpm = TPM_GetInstance(TPM0_INSTANCE);
	TPM_EnableClock(tpm, TRUE);
	TPM_Channel_SetupPWM(ch, period_us, duty_us);
	TPM_EnableCounter(tpm, TRUE);

	// thermocouple
	GPIO_PIN ss;
	SPI* spi = InitTC_SPI(ss);

	//warm-up, discard some initial garbage
	int16_t traw_ext, traw_int;
	for (int i = 0; i < 10; ++i)
	{
		readTC(spi, ss, traw_ext, traw_int);
		delay_ms(10);
	}

	while (1)
	{
		if (readTC(spi, ss, traw_ext, traw_int))
		{
			char buf[256];
			float t_int = traw_int * 0.0625;
			float t_ext = traw_ext * 0.25;
			ee_sprintf(buf, "%7.4f", t_int);
			lcd->printf(0,6, "%s\n", buf);
			ee_sprintf(buf, "%7.4f", t_ext);
			lcd->printf(0,7, "%s\n", buf);

			UART_SendByte(uart1, (uint8_t)(traw_ext & 0xff));
			UART_SendByte(uart1, (uint8_t)(traw_ext>>8 & 0xff));
			UART_SendByte(uart1, (uint8_t)(traw_ext>>16 & 0xff));
			UART_SendByte(uart1, (uint8_t)(traw_ext>>24 & 0xff));

			uint8_t ack = UART_ReceiveByte(uart1);
			if (ack != 0xfe)
			{
				lcd->printf(0,7, "ERR\n", buf);
				while(1);
			}

			lcd->display();
			delay_ms(1000);
		}
	}


    while(1);

}


void test_thermocouple()
{
    //test1();
    test4();
    while(1);
}
