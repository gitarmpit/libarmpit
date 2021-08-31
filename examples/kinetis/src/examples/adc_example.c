#include "adc.h"
#include "gpio_helper.h"
#include "pmc.h"
#include "system_time.h"
#include "mcg.h"
//#include "lcd/ee_printf.h"

static void test_adc_1()
{
	InitClock_FEI_48Mhz_Bus_24Mhz();
	ADC_Channel ch = GPIO_Helper_SetupADC(ADC0, ADC_CH_SE14);
	ADC* adc = ch.adc;
	ADC_SetClockSource_Bus(adc);
	//ADC_SetClock_Div4(adc);
	//ADC_SetHardwareAveraging_32(adc);
	//ADC_SetLongConversion_20(adc);
	//ADC_SetHighSpeedConversion(adc, TRUE);
	BOOL rc = ADC_EnableCalibration(adc);
	if (!rc)
	{
		while(1);
	}

	ADC_SetMode_12bit(adc);

	//ADC_EnableInterrupt(&ch, TRUE);
	//ADC_EnableContinuousConversion(adc, TRUE);
	ADC_SetChannel(&ch);
	uint16_t res = 12345;
	while(1)
	{
		res = ADC_SingleConversion(&ch);
		printf ("%d\n", res);
		delay_ms(1000);
	}
}

static void printDouble(double v, int decimalDigits)
{
  int i = 1;
  int intPart, fractPart;
  for (;decimalDigits!=0; i*=10, decimalDigits--);
  intPart = (int)v;
  fractPart = (int)((v-(double)(int)v)*i);
  if(fractPart < 0) fractPart *= -1;
  printf("%i.%i", intPart, fractPart);
}

static void test_adc_bg()
{
	PMC_EnableBGB(TRUE);
	ADC_Channel ch = GPIO_Helper_SetupADC(ADC0, ADC_CH_BANDGAP);
	ADC* adc = ch.adc;
	ADC_SetClockSource_BusDiv2(adc);
	ADC_SetClock_Div4(adc);
	ADC_SetHardwareAveraging_32(adc);
	ADC_SetLongConversion_20(adc);
	ADC_SetHighSpeedConversion(adc, TRUE);
	BOOL rc = ADC_EnableCalibration(adc);


	if (!rc)
	{
		while(1);
	}

	ADC_SetMode_12bit(adc);

	ADC_SetChannel(&ch);

	//BG = 1.01433105469 V at 3.3V1.01433105469
	int16_t ref3_3 = 1259;

	uint16_t res = 0;
	while(1)
	{
		res = ADC_SingleConversion(&ch);
		//ee_sprintf(buf, "ref: %d, VCC: %f\n", res, 3.3 * ref3_3 / res);
		// printf ("ref: %d, VCC: %f\n", res, 3.3 * ref3_3 / res);
		printf ("ref: %d, VCC: ", res);
		printDouble(3.3 * ref3_3 / res, 3);
		printf ("\n");
		//printf ("%s\n", buf);
		delay_ms(1000);
	}
}


static void speed_test()
{
	InitClock_FEI_48Mhz_Bus_24Mhz();

	ADC_Channel ch = GPIO_Helper_SetupADC(ADC0, ADC_CH_SE14);
	ADC* adc = ch.adc;
	ADC_SetClockSource_Bus(adc);
	//ADC_SetClock_Div4(adc);
	//ADC_SetHardwareAveraging_32(adc);
	//ADC_SetLongConversion_20(adc);
	// ADC_SetHighSpeedConversion(adc, TRUE);
	BOOL rc = ADC_EnableCalibration(adc);
	if (!rc)
	{
		while(1);
	}

	ADC_SetMode_12bit(adc);

	ADC_SetChannel(&ch);
	uint16_t res;
	systick_enable(TRUE);
	uint32_t t0 = micros();

	for (int i = 0; i < 1000000; ++i)
	{
		//res = ADC_SingleConversion(&ch);

		*ch.ADC_SC1 |= ADC_SC1_COCO;

		while (!(*ch.ADC_SC1 & ADC_SC1_COCO))
			;

	}

	uint32_t t1 = micros();
	printf ("1mil conversions: %d ms, 1 conv, usec:", (t1-t0)/1000);
	printDouble((t1-t0) / 1000000., 3);
	printf ("\n");
	while(1);
}


void test_adc()
{
	test_adc_1();
	//speed_test();
}
