#include "peripheral.h"

void EnablePeripheralClock(Peripheral* per, BOOL enable)
{
    if (enable) 
    {
        *per->_resetReg &= ~per->_bit;
        *per->_enableReg |= per->_bit;
    }
    else 
    {
        *per->_resetReg |= per->_bit;
        *per->_enableReg &= ~per->_bit;
    }
}

BOOL IsClockEnabled(Peripheral* per)
{
    return (*per->_enableReg & per->_bit);
}

