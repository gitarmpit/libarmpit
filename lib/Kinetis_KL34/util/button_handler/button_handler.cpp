#include "button_handler.h"

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

    PIT_SetInterruptHandler(_timer, HandleInterrupt);
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

void ButtonHandler::HandleInterrupt()
{
	bool callTimerHook =
			(_timerHookInterval && (millis() - _lastTimerHookTime) > _timerHookInterval);

	for (int i = 0; i < _totalButtons; ++i)
	{

		if (_buttons[i].HasButtonStateChanged())
		{
			OnStateChange(&_buttons[i]);
			_buttons[i]._durDown = 0;
			if (_buttons[i].IsUp())
			{
				OnButtonUp(&_buttons[i]);
				if ((millis() - _buttons[i]._lastClick) <= DOUBLE_CLICK_INTERVAL_MS)
				{
					OnDoubleClick(&_buttons[i]);
				}
				OnClick(&_buttons[i]);
				_buttons[i]._lastClick = millis();
				_buttons[i]._lastDown = 0;
			}
			else
			{
				OnButtonDown(&_buttons[i]);
				_buttons[i]._lastDown = millis();
			}
		}
		else if (_buttons[i].IsDown())
		{
			_buttons[i]._durDown = millis() - _buttons[i]._lastDown;
		}
		if (callTimerHook)
		{
			TimerHook(&_buttons[i]);
		}
	}
	if (callTimerHook)
	{
		_lastTimerHookTime = millis();
	}
}

void ButtonHandler::HandleInterrupt(void* ctx)
{
	((ButtonHandler*)ctx)->HandleInterrupt();
}


