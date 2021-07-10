#include "osc.h"


void OSC_Enable_OSERCCLK(BOOL enable)
{
	if (enable)
	{
		OSC_CR |= OSC_CR_ERCLKEN;
	}
	else
	{
		OSC_CR &= ~OSC_CR_ERCLKEN;
	}
}


void OSC_OSERCCLK_StopEnable(BOOL enable)
{
	if (enable)
	{
		OSC_CR |= OSC_CR_EREFSTEN;
	}
	else
	{
		OSC_CR &= ~OSC_CR_EREFSTEN;
	}
}
