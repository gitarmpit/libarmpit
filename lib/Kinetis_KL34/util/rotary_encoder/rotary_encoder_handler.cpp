#include "rotary_encoder_handler.h"
#include <stdio.h>


RotaryHandler::RotaryHandler(PIT* timer, GPIO_PIN* A, GPIO_PIN* B)
{
	_A = A;
	_B = B;
    _timer = timer;
    _timer_update_interval_us = DEFAULT_TIMER_UPDATE_INTERVAL_US;
}

void RotaryHandler::Init(bool initialize_timer)
{
    systick_enable(TRUE);

    GPIO_SetupIn(_A);
    GPIO_SetupIn(_B);

    PIT_SetInterruptHandler(_timer, RotaryHandleInterrupt);
    _timer->ctx = this;

    if (initialize_timer)
    {
        PIT_EnableClock(TRUE);
        PIT_SetPeriod_us(_timer, _timer_update_interval_us);
        PIT_EnableInterrupt(_timer, TRUE);
        PIT_EnableTimer(_timer, TRUE);
    }


}

void RotaryHandler::SetUpdateIntervalUs (uint32_t us)
{
    _timer_update_interval_us = us;
}

void  RotaryHandler::ProcessInterrupt()
{
	_state <<= 2;
	if (*_A->port->GPIO_PDIR & _A->pinMask) _state |= 0x02;
	if (*_B->port->GPIO_PDIR & _B->pinMask) _state |= 0x01;
	_state &= 0x0f;

	int8_t dir = 0;
	if  (_table[_state])
	{
		_store <<= 4;
		_store |= _state;
		if ((_store&0xff) == 0x2b) dir = 1;
		if ((_store&0xff) == 0x17) dir = -1;
	}

	if (dir != 0)
		OnStateChange(dir);
}

void RotaryHandler::RotaryHandleInterrupt(void* ctx)
{
	((RotaryHandler*)ctx)->ProcessInterrupt();

}
