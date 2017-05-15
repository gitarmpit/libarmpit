#include "button_ctrl.h"


Button_Ctrl::Button_Ctrl(MainLoop* mainLoop)
{
    _mainLoop = mainLoop;
}

void Button_Ctrl::Init()
{
    //29
    Button lstickButton(GPIOA::GetInstance()->GetPin(GPIO_PIN8), LSTICK_BUTTON);
    _mainLoop->AddButton(&lstickButton);
    //30
    Button rstickButton(GPIOA::GetInstance()->GetPin(GPIO_PIN9), RSTICK_BUTTON);
    _mainLoop->AddButton(&rstickButton);
    //31
    Button backButton (GPIOA::GetInstance()->GetPin(GPIO_PIN10), BACK_BUTTON);
    _mainLoop->AddButton(&backButton);
    //32
    Button startButton (GPIOA::GetInstance()->GetPin(GPIO_PIN11), START_BUTTON);
    _mainLoop->AddButton(&startButton);
    //33
    Button dpadUpButton(GPIOA::GetInstance()->GetPin(GPIO_PIN12), DPAD_UP_BUTTON);
    _mainLoop->AddButton(&dpadUpButton);
    //38
    Button dpadRightButton(GPIOA::GetInstance()->GetPin(GPIO_PIN15), DPAD_RIGHT_BUTTON);
    _mainLoop->AddButton(&dpadRightButton);

    //39, 41 : SPI1 for the LCD

    //3
    Button dpadDownButton(GPIOC::GetInstance()->GetPin(GPIO_PIN14), DPAD_DOWN_BUTTON);
    _mainLoop->AddButton(&dpadDownButton);
    //40
    Button dpadLeftButton(GPIOB::GetInstance()->GetPin(GPIO_PIN4), DPAD_LEFT_BUTTON);
    _mainLoop->AddButton(&dpadLeftButton);

}

