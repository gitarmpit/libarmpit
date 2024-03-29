#include "button.h"
#include <stdio.h>

Button::Button(GPIO_PIN* pin, uint8_t buttonId) : _pin(pin)
{
    _buttonId = buttonId;
   _lastClick = 0;

    Init();

}

Button::Button()
{
    _buttonId = 0;
    _pin = 0;
    _activeHigh = false;
    _lastClick = 0;
    _n_same_state = 0;
    _lastDown = 0;
    _durDown = 0;
    _lastReadValue = false;
    _currentState = false;
    _n_retries = 1;
}

void Button::Init()
{
    GPIO_SetupIn(_pin);
    GPIO_PullUp(_pin);
    _n_same_state = 0;
    _lastDown = 0;
    _durDown = 0;
    PullUp();
}

void Button::Init(GPIO_PIN* pin, uint8_t buttonId)
{
    _buttonId = buttonId;
    _pin = pin;
    Init();
}


void Button::PullDown()
{
	GPIO_PullDown(_pin);
    _currentState = IsPinSet();
    _activeHigh = true;
    _lastReadValue = _currentState;
}

void Button::PullUp()
{
    GPIO_PullUp(_pin);
    _currentState = IsPinSet();
    _activeHigh = false;
    _lastReadValue = _currentState;
}


bool Button::IsPinSet()
{
	return GPIO_IsSet(_pin);
}

bool Button::IsDown()
{
    return (_currentState == _activeHigh);
}


bool Button::IsUp()
{
    return !IsDown();
}

//Debouncing logic
//Returns true on state change (button is stable low or high)
bool Button::HasButtonStateChanged()
{
    bool currentValue = IsPinSet();
    bool stateChanged = false;

    if (currentValue != _currentState)
    {
        if (currentValue == _lastReadValue)
        {
            ++_n_same_state;
            if (_n_same_state  == _n_retries)
            {
                _currentState = !_currentState; //flip the state
                _n_same_state = 0;
                stateChanged = true;
            }
        }
        else
        {
            _n_same_state = 0;
        }
        _lastReadValue = currentValue;
    }

    return stateChanged;
}

