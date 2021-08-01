#ifndef PERIPHERAL_H 
#define PERIPHERAL_H

#include "common.h"


typedef struct _Peripheral
{
    volatile uint32_t*  _enableReg;
    volatile uint32_t*  _resetReg;
    uint32_t   _bit;
} Peripheral;

void EnablePeripheralClock(Peripheral* per, BOOL enable);
BOOL IsClockEnabled(Peripheral* per);

#endif
