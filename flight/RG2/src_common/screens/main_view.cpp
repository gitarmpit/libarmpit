#include "main_view.h"
#include "rtc.h"
#include "settings_view.h"
#include "jboss_view.h"
#include "calib_view.h"
#include "cartman_view.h"
#include "abu_view.h"
#include "rc_view.h"
#include "debug.h"

#include "bitmaps/36/settings_36x36_16.cpp"
#include "bitmaps/36/jb_36x36_16.cpp"
#include "bitmaps/36/crosshair_36x36_16.cpp"
#include "bitmaps/36/log_icon_30x30_16.cpp"
#include "bitmaps/36/battery_36x36_16.cpp"
#include "bitmaps/36/compass_36x36_16.cpp"


MainView::MainView(MainButtonHandler* bhandler, TFT_ILI9163C* lcd)
: ScreenView(bhandler, lcd, 6)
{
    //i2c->SetErrorCallback (I2C_Error, i2c);

    AddChild(new SettingsView(bhandler, lcd, this));
    AddChild(new CalibView(bhandler, lcd, this));
    AddChild(new AbuView(bhandler, lcd, this));
    AddChild(new JBOSSView(bhandler, lcd, this));
    AddChild(new CartmanView(bhandler, lcd, this));
    AddChild(new RcView(bhandler, lcd, this));
}


void MainView::OnClick(Button* button)
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



void MainView::Run()
{
    _lcd->setTextSize(1);
    _lcd->setBgColor(WHITE);
    _lcd->setTextColor(COLOR565(0,0,16));
    _lcd->setXposInPixels(true);

    while (!_done)
    {
        _lcd->fillScreen(WHITE);
        _lcd->drawBmp(7, 23, settings_36x36_16);
        _lcd->drawBmp(47, 23, crosshair_36x36_16);
        _lcd->drawBmp(87, 23, battery_36x36_16);

        _lcd->drawBmp(7, 71, jb_36x36_16);
        _lcd->drawBmp(47, 71, compass_36x36_16);
        _lcd->drawBmp(90, 76, log_icon_30x30_16);

        static uint16_t year, month, day, hour, min, sec;
        static BOOL isAM;
        RTC_GetCalendar(&year, &month, &day, &hour, &min, &sec, &isAM);
        _lcd->setXposInPixels(true);
        //_lcd->printf (2, 2, "%d:%02d:%02d %s %d/%d/20%d", isAM ? hour : hour - 12, min, sec, isAM ? "AM" : "PM", month, day, year);
        _lcd->printf (2, 2, "%02d:%02d:%02d %d/%d/20%d", hour, min, sec, month, day, year);
        _lcd->setXposInPixels(false);
        int row = _idx / 3;
        int col = _idx % 3;
        _lcd->drawRect(col*41 + 5, row*49 + 21, 38, 38, COLOR565(0,0,16));

        _lcd->display();
        delay(100);
    }
    _done = false;
}

