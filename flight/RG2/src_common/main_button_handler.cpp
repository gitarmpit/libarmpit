#include "main_button_handler.h"
#include "screens/main_view.h"
#include "screens/ScreenView.h"
#include "debug.h"

MainButtonHandler::MainButtonHandler(TIMER* t, TFT_ILI9163C* lcd, I2C* i2c, Buzzer* buzzer)
 : ButtonHandler(t)
{
    _idx = 0;
    _i2c = i2c;
    _max_idx = 0;
    _t = t;
    _buzzer = buzzer;
    _lcd = lcd;
    _lcd->setBgColor(0);
    _lcd->setTextColor(WHITE);
    _lcd->setTextSize(4);
    _screen = new MainView (this, lcd);

    SetTimerHookInterval(25);
}


void MainButtonHandler::OnClick(Button* button)
{
    if (_buzzer)
    {
        _buzzer->PlayTone(500, 4);
    }

    _screen->OnClick(button);

}

void MainButtonHandler::Run()
{
    Button b1 (GPIO_Helper::GetPin(BUTTON_UP), ScreenView::BUTTON1);
    AddButton(&b1);

    Button b2 (GPIO_Helper::GetPin(BUTTON_DOWN), ScreenView::BUTTON2);
    AddButton(&b2);

    while(1)
    {
        _screen->Run();
        delay(10);
    }
}

void  MainButtonHandler::TimerHook(Button* button)
{
    _screen->TimerHook(button);
}

void MainButtonHandler::SetActiveScreen (ScreenView* screen)
{
    _screen = screen;
}


