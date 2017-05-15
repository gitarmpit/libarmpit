#ifndef _BUTTON_CTRL_H
#define _BUTTON_CTRL_H

#include "main_loop.h"

#define BACK_BUTTON  0
#define START_BUTTON 1
#define DPAD_UP_BUTTON 2
#define DPAD_DOWN_BUTTON 3
#define DPAD_LEFT_BUTTON 4
#define DPAD_RIGHT_BUTTON 5
#define LSTICK_BUTTON 6
#define RSTICK_BUTTON 7


class Button_Ctrl
{
private:
    MainLoop* _mainLoop;
public:
    Button_Ctrl (MainLoop* mainLoop);
    void Init();
};

#endif
