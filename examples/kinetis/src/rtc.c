#include "rtc.h"
#include "nvic.h"

static void (*g_secondsHandler)(void);
static void (*g_alarmHandler)(void);

void RTC_Seconds_IRQHandler()
{
	if (g_secondsHandler != NULL)
	{
		g_secondsHandler();
	}
}

void RTC_IRQHandler()
{
	if (g_alarmHandler != NULL)
	{
		g_alarmHandler();
	}
}

void RTC_EnableOSC(BOOL enable)
{
	if (enable)
	{
		RTC_CR |= RTC_CR_OSCE;
	}
	else
	{
		RTC_CR &= ~RTC_CR_OSCE;
	}
}

void RTC_EnableCounter(BOOL enable)
{
	if (enable)
	{
		RTC_SR |= RTC_SR_TCE;
	}
	else
	{
		RTC_SR &= ~RTC_SR_TCE;
	}
}

void RTC_EnableERCLK32K(BOOL enable)
{
	if (enable)
	{
		RTC_CR |= RTC_CR_CLKO;
	}
	else
	{
		RTC_CR &= ~RTC_CR_CLKO;
	}
}

void RTC_SWReset()
{
	RTC_CR |= RTC_CR_SWR;
}

void RTC_EnableSecondsInt(BOOL enable, void (*handler)(void))
{
	if (enable)
	{
		g_secondsHandler = handler;
		RTC_IER |= RTC_IER_TSIE;
		NVIC_SetEnable(RTC_Seconds_IRQn);
	}
	else
	{
		NVIC_ClearEnable(RTC_Seconds_IRQn);
		RTC_IER &= ~RTC_IER_TSIE;
	}
}

void RTC_EnableAlarmInt(BOOL enable, void (*handler)(void))
{
	if (enable)
	{
		g_alarmHandler = handler;
		RTC_IER |= RTC_IER_TAIE;
		NVIC_SetEnable(RTC_IRQn);
	}
	else
	{
		NVIC_ClearEnable(RTC_IRQn);
		RTC_IER &= ~RTC_IER_TSIE;
	}

}





