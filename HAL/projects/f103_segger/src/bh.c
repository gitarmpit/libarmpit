#include "bh.h"
#define MAX_BUTTONS 32

// if defined, single click will not be triggered immeditiately
// we'll wait for another click in which case it will treated as a double click
// if no click within the double click interval, single click event will fire
// slows things down!
// #define SINGLE_DOUBLE_CLICK_MUTUALLY_EXCLUSIVE

#define DEFAULT_TIMER_UPDATE_INTERVAL_US 2000
#define DEFAULT_SETTLE_TIME_US  DEFAULT_TIMER_UPDATE_INTERVAL_US * 2;

// Windows default is 500ms
#define DOUBLE_CLICK_INTERVAL_MS = 300;
#define SINGLE_CLICK_INTERVAL_MS = 1500;

static uint32_t timer_update_interval_us;
static uint32_t settle_time_us;
static uint32_t n_retries;

static TIM_TypeDef* _timer;

button_ctx _buttons[MAX_BUTTONS];

uint8_t _totalButtons;
uint32_t _timerHookInterval;
uint32_t _lastTimerHookTime;

button_handler _on_state_chage;
button_handler _on_up;
button_handler _on_down;
button_handler _on_click;
button_handler _on_double_click;
button_handler _timer_hook_handler;


void BH_SetOnStateChangeHandler(button_handler on_state_chage) {
  _on_state_chage = on_state_chage;
}

void BH_SetOnButtonUpHandler(button_handler on_up) {
  _on_up = on_up;
}

void BH_SetOnButtonDownHandler(button_handler on_down) {
  _on_down = on_down;
}

void BH_SetOnClickHandler(button_handler on_click) {
  _on_click = on_click;
}

void BH_SetOnDoubleClickHandler(button_handler on_double_click) {
  _on_double_click = on_double_click;
}

void BH_SetTimerHookHandler(button_handler timer_hook_handler) {
  _timer_hook_handler = timer_hook_handler;
}



#if 0



ButtonHandler::ButtonHandler(TIMER* timer, bool initialize_timer) {
  _timer = timer;
  Init(initialize_timer);
}

void ButtonHandler::Init(bool initialize_timer) {
  systick_enable(TRUE);

  timer_update_interval_us = DEFAULT_TIMER_UPDATE_INTERVAL_US;
  settle_time_us           = DEFAULT_SETTLE_TIME_US;
  n_retries                = settle_time_us / timer_update_interval_us;

  if (initialize_timer) {
    _timer->EnablePeripheralClock(true);
    _timer->SetUpdatePeriod_us(timer_update_interval_us);
    _timer->EnableUpdateInterrupt(true);
    _timer->EnableNVICInterrupt(true);
    _timer->EnableCounter(true);
  }
  _totalButtons      = 0;
  _timerHookInterval = 0;
  _timer->AddInterruptHandler(this);

  _lastTimerHookTime = millis();
}

void ButtonHandler::SetUpdateIntervalUs(uint32_t us) {
  timer_update_interval_us = us;
  n_retries                = settle_time_us / timer_update_interval_us;
  _timer->SetUpdatePeriod_us(timer_update_interval_us);
}

void ButtonHandler::SetSettleTimeUs(uint32_t us) {
  settle_time_us = us;
  n_retries      = settle_time_us / timer_update_interval_us;
}

void ButtonHandler::AddButton(Button* button) {
  if (_totalButtons < MAX_BUTTONS) {
    _buttons[_totalButtons++] = *button;
  }
}

Button* ButtonHandler::GetButton(uint8_t id) {
  Button* b = 0;
  for (int i = 0; i < _totalButtons; ++i) {
    if (_buttons[i].GetId() == id) {
      return &_buttons[i];
    }
  }

  return b;
}

#ifdef SINGLE_DOUBLE_CLICK_MUTUALLY_EXCLUSIVE
void ButtonHandler::HandleInterrupt(bool isUpdate, bool isTrigger) {
  UNUSED(isTrigger);

  if (isUpdate) {
    bool callTimerHook = (_timerHookInterval && (millis() - _lastTimerHookTime) > _timerHookInterval);

    for (int i = 0; i < _totalButtons; ++i) {
      if (HasButtonStateChanged(&_buttons[i])) {
        OnStateChange(&_buttons[i]);
        _buttons[i]._durDown = 0;
        if (_buttons[i].IsUp()) {
          OnButtonUp(&_buttons[i]);
          if ((millis() - _buttons[i]._lastClick) <= DOUBLE_CLICK_INTERVAL_MS) {
            OnDoubleClick(&_buttons[i]);
            _buttons[i]._lastClick = 0;
          } else {
            _buttons[i]._lastClick = millis();
          }
          _buttons[i]._lastDown = 0;
        } else {
          OnButtonDown(&_buttons[i]);
          _buttons[i]._lastDown = millis();
        }
      } else if (_buttons[i].IsDown()) {
        _buttons[i]._durDown = millis() - _buttons[i]._lastDown;
      } else if (_buttons[i]._lastClick != 0 && (millis() - _buttons[i]._lastClick) >= DOUBLE_CLICK_INTERVAL_MS) {
        OnClick(&_buttons[i]);
        _buttons[i]._lastClick = 0;
      }
      if (callTimerHook) {
        TimerHook(&_buttons[i]);
      }
    }
    if (callTimerHook) {
      _lastTimerHookTime = millis();
    }
  }
}
#else
void ButtonHandler::HandleInterrupt(bool isUpdate, bool isTrigger) {
  UNUSED(isTrigger);

  if (isUpdate) {
    bool callTimerHook = (_timerHookInterval && (millis() - _lastTimerHookTime) > _timerHookInterval);

    for (int i = 0; i < _totalButtons; ++i) {
      if (HasButtonStateChanged(&_buttons[i])) {
        OnStateChange(&_buttons[i]);
        _buttons[i]._durDown = 0;
        if (_buttons[i].IsUp()) {
          OnButtonUp(&_buttons[i]);
          if ((millis() - _buttons[i]._lastClick) <= DOUBLE_CLICK_INTERVAL_MS) {
            OnDoubleClick(&_buttons[i]);
            //_buttons[i]._lastClick = 0;
          }
          {
            OnClick(&_buttons[i]);
            _buttons[i]._lastClick = millis();
          }
          _buttons[i]._lastDown = 0;
        } else {
          OnButtonDown(&_buttons[i]);
          _buttons[i]._lastDown = millis();
        }
      } else if (_buttons[i].IsDown()) {
        _buttons[i]._durDown = millis() - _buttons[i]._lastDown;
      }
      if (callTimerHook) {
        TimerHook(&_buttons[i]);
      }
    }
    if (callTimerHook) {
      _lastTimerHookTime = millis();
    }
  }
}
#endif

// Debouncing logic
// Returns true on state change (button is stable low or high)
bool ButtonHandler::HasButtonStateChanged(Button* b) {
  bool currentValue = b->IsPinSet();
  bool stateChanged = false;

  if (currentValue != b->_currentState) {
    if (currentValue == b->_lastReadValue) {
      ++b->n_same_state;
      if (b->n_same_state == n_retries) {
        b->_currentState = !b->_currentState;    // flip the state
        b->n_same_state  = 0;
        stateChanged     = true;
        //               ++_nStateChanges;  //debugging only
      }
    } else {
      b->n_same_state = 0;
    }
    b->_lastReadValue = currentValue;
  }

  return stateChanged;
}

#endif
