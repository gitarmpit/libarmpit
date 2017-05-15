#include "fp2.h"
#include "ee_printf.h"

static uint32_t to_fix (float val)
{
	volatile float fract;
    volatile int32_t ival;
    int16_t ifract;
    bool neg = false;

	fract = val - int32_t(val);
	ival = (int32_t)(val - fract);
	ifract = (int16_t)(fract*1000);
	if (ival < 0)
	{
		ival = -ival;
		ifract = -ifract;
		neg = true;
	}
//	return ((uint16_t)ifract<<16) | (ival & 0xffff);
	uint32_t res = (ival & 0x1fffff) | ((ifract & 0x3ff)<<21);
	if (neg)
	{
		res |=  0x80000000;
	}
	return res;

}

static float from_fix (uint32_t val)
{
    bool neg = false;
    if (val & 0x80000000)
    {
        val &= ~0x80000000;
        neg = true;
    }

	float f1 = (float)(val & 0x1fffff);
	float f2 = (float)((val>>21) & 0x3ff);
	if (neg)
	{
	    f1 = -f1;
	    f2 = -f2;
	}

	return f1 + f2/1000.;

}

