#ifndef _MAIN_BUTTON_HANDLER_H
#define _MAIN_BUTTON_HANDLER_H

#include "button_handler/button_handler.h"
#include "GPIO_Helper.h"
#include "9163/TFT_ILI9163C.h"
#include "_pins.h"
#include <string.h>
#include "buzzer.h"
#include "cfg.h"

class ScreenView;

class MainButtonHandler : public ButtonHandler
{
private:
    Buzzer*       _buzzer;
    TIMER*       _t;
    I2C*         _i2c;

    TFT_ILI9163C* _lcd;
    int8_t        _idx;
    int8_t        _max_idx;
    ScreenView*   _screen;

public:

    MainButtonHandler(TIMER* t, TFT_ILI9163C* lcd, I2C* i2c, Buzzer* buzzer = 0);

    void StopTimer();
    void StartTimer();

    virtual void OnClick(Button* button);
    void Run();
    virtual void  TimerHook(Button* button);

    I2C* GetI2C () { return _i2c; }

    void SetActiveScreen (ScreenView* screen);

    Buzzer*  getBuzzer() { return _buzzer; }

};


#endif
