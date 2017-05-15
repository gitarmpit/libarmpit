#include "settings_view.h"
#include "pid_view.h"
#include "debug.h"

#include "bitmaps/36/pid1_36x36_16.cpp"
#include "bitmaps/36/goback1.cpp"
#include "bitmaps/36/wifi2_36x36_16.cpp"
#include "bitmaps/36/micro36.cpp"
#include "bitmaps/36/bluetooth36.cpp"
#include "bitmaps/36/sound36.cpp"



SettingsView::SettingsView(MainButtonHandler* bhandler, TFT_ILI9163C* lcd, ScreenView* parent)
: ScreenView(bhandler, lcd, 6)
{
    AddChild(parent);
    AddChild(new PidView(bhandler, lcd, this));
    AddChild(0);
    AddChild(0);
    AddChild(0);
    AddChild(0);
    AddChild(0);
}


void SettingsView::OnClick(Button* button)
{
    if (button->GetId() == BUTTON2)
    {
        ++_idx;
        if (_idx > _nitems-1)
        {
            _idx = 0;
        }
    }
    else
    {
        if (_children[_idx] != 0)
        {
            Done();
            _bhandler->SetActiveScreen(_children[_idx]);
        }

//            if (_idx == 0)
//            {
//                _max_idx = 5;
//                _level = SETTINGS;
//            }
//            else if (_idx == 1)
//            {
//                _level = CALIB;
//            }
//            else if (_idx == 3)
//            {
//                _idx = 0;
//                _level = JBOSS;
//            }
    }
}



void SettingsView::Run()
{
    int8_t i = 0;
    _lcd->setTextColor(COLOR565(15,31,26));
    _lcd->setBgColor(WHITE);

    _idx = 0;
    while (!_done)
    {
        _lcd->fillScreen(WHITE);
        _lcd->write ("Settings: ", 0, 0, 4);
        if (_idx  != 0 || i < 3)
        {
            _lcd->drawBmp(7, 33, goback1);
        }
        if (_idx  != 1 || i < 3)
        {
            _lcd->drawBmp(47, 33, pid1_36x36_16);
        }
        if (_idx  != 2 || i < 3)
        {
            _lcd->drawBmp(87, 33, bluetooth36);
        }
        if (_idx  != 3 || i < 3)
        {
            _lcd->drawBmp(7, 81, micro36);
        }
        if (_idx  != 4 || i < 3)
        {
            _lcd->drawBmp(47, 81, wifi2_36x36_16);
        }
        if (_idx  != 5 || i < 3)
        {
            _lcd->drawBmp(87, 81, sound36);
        }

        if (++i == 5)
        {
            i = 0;
        }
        _lcd->display();
        delay(100);
    }

    _done = false;
}

