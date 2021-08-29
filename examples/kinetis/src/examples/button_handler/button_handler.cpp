#include "button_handler.h"

void ButtonHandleInterrupt(void* ctx);

ButtonHandler::ButtonHandler(PIT* timer)
{
    _timer = timer;
	_totalButtons = 0;
	_timerHookInterval = 0;
	_lastTimerHookTime = 0;
	_n_retries = 0;
    _timer_update_interval_us = DEFAULT_TIMER_UPDATE_INTERVAL_US;
    _settle_time_us = DEFAULT_SETTLE_TIME_US;
}

void ButtonHandler::Init(bool initialize_timer)
{
    systick_enable(TRUE);

    _n_retries = _settle_time_us / _timer_update_interval_us;

    for (int i = 0; i < _totalButtons; ++i)
    {
    	_buttons[i]._n_retries = _n_retries;
    }

    PIT_SetInterruptHandler(_timer, ButtonHandleInterrupt);
    _timer->ctx = this;

	if (initialize_timer)
    {
		PIT_EnableClock(TRUE);
    	PIT_SetPeriod_us(_timer, _timer_update_interval_us);
    	PIT_EnableInterrupt(_timer, TRUE);
    	PIT_EnableTimer(_timer, TRUE);
    }

	_lastTimerHookTime = millis();
}

void ButtonHandler::SetUpdateIntervalUs (uint32_t us)
{
    _timer_update_interval_us = us;
}

void ButtonHandler::SetSettleTimeUs (uint32_t us)
{
    _settle_time_us = us;
    if (_settle_time_us < _timer_update_interval_us)
    {
    	_settle_time_us = _timer_update_interval_us;
    }
}

void ButtonHandler::AddButton(Button* button)
{
   if (_totalButtons < MAX_BUTTONS) 
   {
        _buttons[_totalButtons++] = *button;
   }
}

Button* ButtonHandler::GetButton(uint8_t id)
{
    Button* b = 0;
    for (int i = 0; i < _totalButtons; ++i)
    {
        if (_buttons[i].GetId() == id)
        {
            return &_buttons[i];
        }
    }

    return b;
}


void ButtonHandleInterrupt(void* ctx)
{
	ButtonHandler* bh = (ButtonHandler*)ctx;

	bool callTimerHook =
			(bh->_timerHookInterval && (millis() - bh->_lastTimerHookTime) > bh->_timerHookInterval);

	for (int i = 0; i < bh->_totalButtons; ++i)
	{

		if (bh->_buttons[i].HasButtonStateChanged())
		{
			bh->OnStateChange(&bh->_buttons[i]);
			bh->_buttons[i]._durDown = 0;
			if (bh->_buttons[i].IsUp())
			{
				bh->OnButtonUp(&bh->_buttons[i]);
				if ((millis() - bh->_buttons[i]._lastClick) <= bh->DOUBLE_CLICK_INTERVAL_MS)
				{
					bh->OnDoubleClick(&bh->_buttons[i]);
				}
				bh->OnClick(&bh->_buttons[i]);
				bh->_buttons[i]._lastClick = millis();
				bh->_buttons[i]._lastDown = 0;
			}
			else
			{
				bh->OnButtonDown(&bh->_buttons[i]);
				bh->_buttons[i]._lastDown = millis();
			}
		}
		else if (bh->_buttons[i].IsDown())
		{
			bh->_buttons[i]._durDown = millis() - bh->_buttons[i]._lastDown;
		}
		if (callTimerHook)
		{
			bh->TimerHook(&bh->_buttons[i]);
		}
	}
	if (callTimerHook)
	{
		bh->_lastTimerHookTime = millis();
	}

}


