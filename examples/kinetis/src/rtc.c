#include "rtc.h"

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
		RTC_TSR = 0;
		RTC_SR |= RTC_SR_TCE;
	}
	else
	{
		RTC_SR &= ~RTC_SR_TCE;
	}
}



