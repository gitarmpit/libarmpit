#include "rotary_encoder_handler.h"
#include <stdio.h>

void RotaryHandleInterrupt(void* ctx);

RotaryHandler::RotaryHandler(PIT* timer, Button* A, GPIO_PIN* B)
{
	_A = A;
	_B = B;
    _timer = timer;
    _n_retries = 0;
    _timer_update_interval_us = DEFAULT_TIMER_UPDATE_INTERVAL_US;
    _settle_time_us = DEFAULT_SETTLE_TIME_US;
    _state = 0;
}

void RotaryHandler::Init(bool initialize_timer)
{
    systick_enable(TRUE);

    _n_retries = _settle_time_us / _timer_update_interval_us;

    _A->SetRetries(_n_retries);
    GPIO_SetupIn(_B);

    //UpdateState();
	//printf ("initial state: %x\n", _state);

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

void RotaryHandler::SetSettleTimeUs (uint32_t us)
{
    _settle_time_us = us;
    if (_settle_time_us < _timer_update_interval_us)
    {
        _settle_time_us = _timer_update_interval_us;
    }
}

void RotaryHandler::UpdateState()
{
	_state <<= 2;
	bool stateA = _A->IsUp();
	bool stateB = GPIO_IsSet(_B);
	stateB ? SETBIT(_state, 0) : CLRBIT (_state, 0);
	stateA ? SETBIT(_state, 1) : CLRBIT (_state, 1);
	printf ("A: %d, B: %d\n", stateA, stateB);
}

void RotaryHandleInterrupt(void* ctx)
{
	RotaryHandler* bh = (RotaryHandler*)ctx;

	if (bh->_A->HasButtonStateChanged())
	{
		//bh->UpdateState();
		//printf ("state: %d\n", bh->_state & 0xf);
		//int8_t dir = bh->_table[bh->_state & 0xf];
		bool b_isSet = *bh->_B->port->GPIO_PDIR & bh->_B->pinMask;
		bool a_isSet = bh->_A->IsUp();
		int8_t dir = (a_isSet == b_isSet) ? -1 : 1;
		if (dir)
		{
			bh->OnStateChange(dir);
		}
	}

}
