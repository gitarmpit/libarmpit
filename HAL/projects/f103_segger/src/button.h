#ifndef _BUTTON_H
#define _BUTTON_H

#include "gpio_helper.h"

/* The default behavior is active low: button is configured as a weak pull-up
 * So to active it, the switch needs to be connected from the input port to ground
 * Then pushing the switch will set it low
 *
 */
class Button
{

friend class ButtonHandler;

private:

    GPIO_PIN* _pin;


    bool _activeHigh;
    bool _currentState;  //high or low
    bool _lastReadValue; //high or low

    uint8_t n_same_state;

    uint8_t _buttonId;
    uint32_t _lastDown;
    uint32_t _durDown;
    uint32_t _lastClick;

    void Init();


public:
    Button(GPIO_PIN* pin, uint8_t buttonId);
    Button();

    void Init(GPIO_PIN* pin, uint8_t buttonId);

    //Open: HIGH
    //Closed: LOW
    //This is the default
    void PullUp();

    //Open: LOW
    //Closed: HIGH
    void PullDown();


    bool IsDown();
    bool IsUp();

    GPIO_PIN* GetPin() const { return _pin; }

    bool IsPinSet();

    uint8_t GetId() { return _buttonId; }

    //timestamps
    uint32_t GetLastDown() { return _lastDown; }
    uint32_t GetDurDown() { return _durDown; }
};

#endif
