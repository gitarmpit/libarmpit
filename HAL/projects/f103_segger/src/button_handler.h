#ifndef _BUTTON_HANDLER_H
#define _BUTTON_HANDLER_H

#if 0

#include "timer_cpp.h"
#include "gpio_cpp.h"
#include "button.h"
#include "system_time.h"

/* The default behavior is active low: button is configured as a weak pull-up
 * So to active it, the switch needs to be connected from the input port to ground
 * Then pushing the switch will set it low: the IsOn method will return true
 *
 */
class ButtonHandler : protected TIMER_Interrupt_Handler
{
private:
    const static uint32_t DEFAULT_TIMER_UPDATE_INTERVAL_US = 1000;
    const static uint32_t  DEFAULT_SETTLE_TIME_US = DEFAULT_TIMER_UPDATE_INTERVAL_US * 2;

    //Windows default is 500ms
    const static uint32_t  DOUBLE_CLICK_INTERVAL_MS = 300;

    const static uint32_t  SINGLE_CLICK_INTERVAL_MS = 1500;

    uint32_t timer_update_interval_us;
    uint32_t settle_time_us;
    uint32_t n_retries;
    const static uint8_t MAX_BUTTONS = 32;

    TIMER* _timer;


    Button  _buttons[MAX_BUTTONS];
    uint8_t  _totalButtons;
    uint32_t _timerHookInterval;
    uint32_t _lastTimerHookTime;


    void Init(bool initialize_timer);

    bool HasButtonStateChanged(Button* b);
    virtual void HandleInterrupt(bool isUpdate, bool isTrigger);
protected:
    ButtonHandler(TIMER* timer, bool initialize_timer = true);
    virtual ~ButtonHandler() {}

public:


    void AddButton(Button* button);

    Button* GetButton(uint8_t ID);

    void SetUpdateIntervalUs (uint32_t us);
    void SetSettleTimeUs (uint32_t us);

    virtual void OnStateChange(Button* button) {}
    virtual void OnButtonUp(Button* button) {}
    virtual void OnButtonDown(Button* button) {}
    virtual void OnClick(Button* button) {}
    virtual void OnDoubleClick(Button* button) {}
    virtual void  TimerHook(Button* button) {}
    void SetTimerHookInterval (uint32_t ms) { _timerHookInterval = ms; }


};

#endif

#endif
