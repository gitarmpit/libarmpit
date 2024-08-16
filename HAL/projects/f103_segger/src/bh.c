#include "bh.h"
#include "systick_helper.h"
#include "timer_helper.h"

#define MAX_BUTTONS 32

// if defined, single click will not be triggered immeditiately
// we'll wait for another click in which case it will treated as a double click
// if no click within the double click interval, single click event will fire
// slows things down!
// #define SINGLE_DOUBLE_CLICK_MUTUALLY_EXCLUSIVE

#define DEFAULT_TIMER_UPDATE_INTERVAL_US 2000
#define DEFAULT_SETTLE_TIME_US DEFAULT_TIMER_UPDATE_INTERVAL_US * 2

// Windows default is 500ms
#define DOUBLE_CLICK_INTERVAL_MS 300

#define SINGLE_CLICK_INTERVAL_MS 1500

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

static BOOL HasButtonStateChanged(button_ctx* b);

static void handle_interrupt(void* ctx) {
  BOOL callTimerHook = (_timerHookInterval && (SysTick_GetTick() - _lastTimerHookTime) > _timerHookInterval);

  for (int i = 0; i < _totalButtons; ++i) {
    if (HasButtonStateChanged(&_buttons[i])) {

      if (_on_state_chage) 
        _on_state_chage(&_buttons[i]);
    
      _buttons[i].durDown = 0;
      if (Button_IsUp(&_buttons[i])) {

        if (_on_up)
          _on_up(&_buttons[i]);
  
        if (_on_double_click && (SysTick_GetTick() - _buttons[i].lastClick) <= DOUBLE_CLICK_INTERVAL_MS) {
          _on_double_click(&_buttons[i]);
          //_buttons[i]._lastClick = 0;
        }
        if (_on_click) {
          _on_click(&_buttons[i]);
        }
        _buttons[i].lastClick = SysTick_GetTick();
        _buttons[i].lastDown = 0;
      } else if (_on_down) {
        _on_down(&_buttons[i]);
        _buttons[i].lastDown = SysTick_GetTick();
      }
    } else if (Button_IsDown(&_buttons[i])) {
      _buttons[i].durDown = SysTick_GetTick() - _buttons[i].lastDown;
    }

    if (callTimerHook && _timer_hook_handler) {
      _timer_hook_handler(&_buttons[i]);
    }
  }
  if (callTimerHook) {
    _lastTimerHookTime = SysTick_GetTick();
  }
}

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

void BH_Init(TIM_TypeDef* tim) {
  timer_update_interval_us = DEFAULT_TIMER_UPDATE_INTERVAL_US;
  settle_time_us           = DEFAULT_SETTLE_TIME_US;
  n_retries                = settle_time_us / timer_update_interval_us;
  _totalButtons            = 0;
  _timerHookInterval       = 0;
  _lastTimerHookTime       = SysTick_GetTick();

  TIM_SetupCounter(tim, timer_update_interval_us, handle_interrupt, 0);
}

void BH_AddButton(button_ctx* button) {
  if (_totalButtons < MAX_BUTTONS) {
    _buttons[_totalButtons++] = *button;
  }
}

button_ctx* BH_GetButton(uint8_t id) {
  for (int i = 0; i < _totalButtons; ++i) {
    if (_buttons[i].buttonId == id) {
      return &_buttons[i];
    }
  }

  return 0;
}

// Debouncing logic
// Returns true on state change (button is stable low or high)
static BOOL HasButtonStateChanged(button_ctx* b) {
  BOOL currentValue = GPIO_IsSet(b->pin);
  BOOL stateChanged = FALSE;

  if (currentValue != b->currentState) {
    if (currentValue == b->lastReadValue) {
      ++b->n_same_state;
      if (b->n_same_state == n_retries) {
        b->currentState = !b->currentState;    // flip the state
        b->n_same_state = 0;
        stateChanged    = TRUE;
        //               ++_nStateChanges;  //debugging only
      }
    } else {
      b->n_same_state = 0;
    }
    b->lastReadValue = currentValue;
  }

  return stateChanged;
}

#if 0






void ButtonHandler::SetUpdateIntervalUs(uint32_t us) {
  timer_update_interval_us = us;
  n_retries                = settle_time_us / timer_update_interval_us;
  _timer->SetUpdatePeriod_us(timer_update_interval_us);
}

void ButtonHandler::SetSettleTimeUs(uint32_t us) {
  settle_time_us = us;
  n_retries      = settle_time_us / timer_update_interval_us;
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

}
#endif

#endif
