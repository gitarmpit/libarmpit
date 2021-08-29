#ifndef _BUTTON_HANDLER_H
#define _BUTTON_HANDLER_H

#include "pit.h"
#include "gpio.h"
#include "button.h"
#include "system_time.h"

/* The default behavior is active low: button is configured as a weak pull-up
 * So to activate it, the switch needs to be connected from the input port to ground
 * Then pushing the switch will set it low: the IsOn method will return true
 *
 */
class ButtonHandler
{
private:
    const static uint32_t DEFAULT_TIMER_UPDATE_INTERVAL_US = 1000;
    const static uint32_t  DEFAULT_SETTLE_TIME_US = 2000;

    //Windows default is 500ms
    const static uint32_t  DOUBLE_CLICK_INTERVAL_MS = 300;

    const static uint32_t  SINGLE_CLICK_INTERVAL_MS = 1500;

    uint32_t _timer_update_interval_us;
    uint32_t _settle_time_us;
    const static uint8_t MAX_BUTTONS = 32;

    PIT* _timer;

    Button    _buttons[MAX_BUTTONS];
    uint8_t   _totalButtons;
    uint32_t  _timerHookInterval;
    uint32_t  _lastTimerHookTime;
    uint32_t  _n_retries;

protected:
    ButtonHandler(PIT* timer);
    virtual ~ButtonHandler() {}

public:

    friend void ButtonHandleInterrupt(void* ctx);

    // Should be called after adding all buttons
    void Init(bool initialize_timer);

    void AddButton(Button* button);

    Button* GetButton(uint8_t ID);

    void SetUpdateIntervalUs (uint32_t us);
    void SetSettleTimeUs (uint32_t us);

    virtual void OnStateChange(Button* button) { UNUSED(button); }
    virtual void OnButtonUp(Button* button) { UNUSED(button); }
    virtual void OnButtonDown(Button* button) { UNUSED(button); }
    virtual void OnClick(Button* button) { UNUSED(button); }
    virtual void OnDoubleClick(Button* button) { UNUSED(button); }
    virtual void  TimerHook(Button* button) { UNUSED(button); }
    void SetTimerHookInterval (uint32_t ms) { _timerHookInterval = ms; }


};

#endif
