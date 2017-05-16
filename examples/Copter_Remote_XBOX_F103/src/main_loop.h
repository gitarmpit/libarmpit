#ifndef _MAIN_LOOP_H
#define _MAIN_LOOP_H

#include "button_handler.h"
#include "adc_reader.h"
#include "5110/Adafruit_Nokia5110.h"
#include "RF22/si4432.h"
#include "backlight_handler.h"

class MainLoop : public ButtonHandler, public ADC_Button_Handler
{
private:

    uint8_t _clicks;
    ADC_Reader _adc_reader;
    Adafruit_Nokia5110*   _display;
    //LCD5110*  _display;
    Si4432*    _radio;
    TIMER_Channel*  _lcdBL;
    TIMER_Channel*  _sound;
    TIMER_Channel*  _redLED;
    TIMER_Channel*  _greenLED;
    uint32_t        _blDuty;
    //TIMER*          _blTimer;

    BacklightHandler  _backLight;

    void Click();
    void Click2();
    void Intro();

public:

    MainLoop(Si4432* radio, Adafruit_Nokia5110* display, TIMER_Channel* lcdBL,
            TIMER_Channel* sound, TIMER_Channel* redLED, TIMER_Channel* greenLED);

    void Run();

    virtual void OnStateChange(Button* button);
    virtual void OnClick(Button* button);

    virtual void Analog_Clicked(ANALOG_BUTTON button);

};



#endif
