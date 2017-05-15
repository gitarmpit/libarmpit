#include "cfg_number.h"



void CfgNumber::Init(int value, int min, int max, int lowInc, int highInc)
{
    _value = value;
    _min = min;
    _max = max;
    _lowInc = lowInc;
    _highInc = highInc;
}

void CfgNumber::Init(float value, int min, int max, int lowInc, int highInc)
{
	Init ((int)(value*10.0), min, max, lowInc, highInc);
}


void CfgNumber::IncrementLow()
{
    _value += _lowInc;
    if (_value > _max)
    {
        _value -= _max;
    }
}

void CfgNumber::DecrementLow()
{
    _value -= _lowInc;
    if (_value < 0)
    {
        _value += _max;
    }
}


void CfgNumber::IncrementHigh()
{
    _value += _highInc;
    if (_value > _max)
    {
        _value -= _max;
    }
}

void CfgNumber::DecrementHigh()
{
    _value -= _highInc;
    if (_value < 0)
    {
        _value += _max;
    }
}


void CfgNumber::GetValue(int* v)
{
	*v = _value;
}

void CfgNumber::GetValue(float* v)
{
	*v = _value / 10.0;
}

