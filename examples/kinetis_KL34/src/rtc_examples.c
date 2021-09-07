#include "common.h"
#include "rtc.h"
#include "osc.h"
#include "mcg.h"
#include "sim.h"
#include "gpio.h"

static GPIO_PIN* g_pin;

#pragma GCC diagnostic ignored "-Wunused-function"

static void secondsHandler1()
{
	if (g_pin != NULL)
	{
		GPIO_TogglePin(g_pin);
	}
}

static void secondsHandler2()
{
	if (g_pin != NULL)
	{
		GPIO_TogglePin(g_pin);
	}
	RTC_EnableCounter(FALSE);
	RTC_TSR = 0;
	RTC_TPR = 32768-970;
	RTC_EnableCounter(TRUE);
}

static void alarmHandler()
{
	if (g_pin != NULL)
	{
		GPIO_TogglePin(g_pin);
	}
	RTC_EnableCounter(FALSE);
	RTC_TSR = 0;
	RTC_TPR = 0;
	RTC_TAR = 9;
	RTC_EnableCounter(TRUE);
}

static void test_clockout_RTC()
{
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(1);
	//MCG_SetFLL_Freq_48(TRUE);

	GPIO_PORT* portE = GPIO_GetInstance(PORTE);
	GPIO_EnableClock(portE, TRUE);
	GPIO_PIN e0 = GPIO_GetPin(portE, GPIO_PIN0);
	GPIO_SetAF(&e0, 4);
	OSC_Enable_OSERCCLK(TRUE);
	SIM_Select_RTCClock_OSC32KCLK();
	// SIM_Select_RTCClock_LPO();
	SIM_Enable_RTC(TRUE);
	RTC_CR |= RTC_CR_SC16P;
	RTC_EnableOSC(TRUE);
	RTC_IER = 0;
	RTC_EnableCounter(TRUE);
	RTC_EnableERCLK32K(TRUE);

	// 1Hz
	// SIM_Select_RTC_CLOCKOUT_RTC();

	// 32Khz
	//SIM_Select_RTC_CLOCKOUT_OSCERCLK();

	while(1)
		;

}

static void test_clockout_RTC_LPO()
{
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(1);
	//MCG_SetFLL_Freq_48(TRUE);

	GPIO_PORT* portE = GPIO_GetInstance(PORTE);
	GPIO_EnableClock(portE, TRUE);
	GPIO_PIN e0 = GPIO_GetPin(portE, GPIO_PIN0);
	GPIO_SetAF(&e0, 4);

	SIM_Select_RTCClock_LPO();

	SIM_Enable_RTC(TRUE);
	RTC_EnableOSC(TRUE);
	RTC_IER = 0;
	RTC_EnableCounter(TRUE);
	RTC_EnableERCLK32K(TRUE);

	SIM_Select_RTC_CLOCKOUT_RTC();

	while(1)
		;

}

static void test_clockout_RTC_LPO_1hz()
{
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(1);
	//MCG_SetFLL_Freq_48(TRUE);

	GPIO_PORT* portE = GPIO_GetInstance(PORTE);
	GPIO_EnableClock(portE, TRUE);
	GPIO_PIN e0 = GPIO_GetPin(portE, GPIO_PIN0);
	GPIO_SetupOut(&e0);
	GPIO_SetSlewRateFast(&e0);
	g_pin = &e0;

	SIM_Select_RTCClock_LPO();

	SIM_Enable_RTC(TRUE);
	RTC_EnableOSC(TRUE);
	RTC_SR = 0;
	RTC_IER = 0;
	RTC_TSR = 0;
	RTC_TPR = 32768-1000;
	RTC_TAR = 0;
	RTC_EnableSecondsInt(TRUE, secondsHandler2);
	RTC_EnableCounter(TRUE);

	SIM_Select_RTC_CLOCKOUT_RTC();

	while(1)
		;

}

static void test_RTC_alarm()
{
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(1);
	//MCG_SetFLL_Freq_48(TRUE);

	GPIO_PORT* portE = GPIO_GetInstance(PORTE);
	GPIO_EnableClock(portE, TRUE);
	GPIO_PIN e0 = GPIO_GetPin(portE, GPIO_PIN0);
	GPIO_SetupOut(&e0);
	GPIO_SetSlewRateFast(&e0);
	g_pin = &e0;

	//delay_ms(2);

	OSC_Enable_OSERCCLK(TRUE);
	SIM_Select_RTCClock_OSC32KCLK();
	// SIM_Select_RTCClock_LPO();

	SIM_Enable_RTC(TRUE);
	// RTC_CR |= RTC_CR_SC16P;
	RTC_SR = 0;
	RTC_IER = 0;
	RTC_TSR = 0;
	RTC_TPR = 0;
	RTC_TAR = 9;
	RTC_EnableOSC(TRUE);
	delay_ms(2);

	//RTC_EnableSecondsInt(TRUE, secondsHandler1);
	RTC_EnableAlarmInt(TRUE, alarmHandler);
	//delay_ms(2);

	RTC_EnableCounter(TRUE);

	while(1)
		;

}

// Feed 32k MCGIRCLK to RTCIN
static void test_RTCIN()
{
	SIM_SetOUTDIV1(1);
	SIM_SetOUTDIV4(2);
	MCG_Set_IRC_Slow();

	GPIO_PORT* portE = GPIO_GetInstance(PORTE);
	GPIO_EnableClock(portE, TRUE);
	GPIO_PIN e0 = GPIO_GetPin(portE, GPIO_PIN0);

	GPIO_SetupOut(&e0);
	GPIO_SetSlewRateFast(&e0);

	//RTC_CLKOUT
	//GPIO_SetAF(&e0, 4);

	g_pin = &e0;


	//CLKOUT
	GPIO_PORT* portC = GPIO_GetInstance(PORTC);
	GPIO_EnableClock(portC, TRUE);
	GPIO_PIN c3 = GPIO_GetPin(portC, GPIO_PIN3);
	GPIO_SetAF(&c3, 5);

	MCG_Enable_MCGIRCLK(TRUE);

	SIM_SelectCLKOUT_MCGIRCLK();

	//RTCCLK_IN
	GPIO_PIN c1 = GPIO_GetPin(portC, GPIO_PIN1);
	GPIO_SetAF(&c1, 1);

	SIM_Select_RTCClock_RTC_CLKIN();

	SIM_Enable_RTC(TRUE);
	// RTC_CR |= RTC_CR_SC16P;
	RTC_SR = 0;
	RTC_IER = 0;
	RTC_TSR = 0;
	RTC_TPR = 0;
	RTC_TAR = 9;
	RTC_EnableOSC(TRUE);
	delay_ms(2);

	//RTC_EnableSecondsInt(TRUE, secondsHandler1);
	RTC_EnableAlarmInt(TRUE, alarmHandler);

	RTC_EnableCounter(TRUE);
	// SIM_Select_RTC_CLOCKOUT_RTC();


	while(1)
		;
}

void test_RTC()
{
	while(1);
}
