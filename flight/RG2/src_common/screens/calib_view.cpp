#include "calib_view.h"
#include "motionapp_reader.h"
#include <math.h>
#include "debug.h"

void test_mpu(TFT_ILI9163C* lcd);
void mpu_done();


CalibView::CalibView(MainButtonHandler* bhandler, TFT_ILI9163C* lcd, ScreenView* parent)
: ScreenView(bhandler, lcd, 6)
{
    _i2c =  bhandler->GetI2C();
    _parent = parent;
    //AddChild()
}

void CalibView::OnClick(Button* button)
{
    Done();
    _bhandler->SetActiveScreen(_parent);
}


void CalibView::Run()
{
    _lcd->setBgColor(0);
    _lcd->setTextColor(CYAN);
    _lcd->setTextSize(2);
    _lcd->clearScreen();
    _lcd->setTextColor(YELLOW);
    _lcd->printf (0, 0, "Calibration mode");

    _lcd->setTextColor(CYAN);
    _lcd->printf (0, 2, "Initializing sensors...");
    _lcd->display();
//    _lcd->drawEllipse(_lcd->width()/2, _lcd->height()/2, 13, 12, CYAN);



    int i = 0;
    float y, p, r;
    MotionAppReader read (_i2c);
    read.Init();
    char buf[64];
    float y0 = 0;
    bool stabilized = false;
    int stable_count = 0;
    bool buzzer_on = false;
    uint32_t avg_t = 0;
    bool bulleye = false;
    int16_t circle_color;
    while (!_done)
    {
        uint32_t t0 = micros();
        read.GetYPR(&y, &p, &r);
        uint32_t t1 = micros();
        avg_t += (t1 - t0);
        if (fabs(y - y0) < 0.01)
        {
            ++stable_count;
        }
        else
        {
            stable_count = 0;
        }
        y0 = y;
        if ((++i % 50) == 0)
        {
            if (buzzer_on)
            {
                _bhandler->getBuzzer()->Stop();
                buzzer_on = false;
            }
            if (fabs(p) < 0.9 && fabs(r) < 0.9)
            {
                circle_color = RED;
                if (!bulleye)
                {
                    bulleye = true;
                    _bhandler->getBuzzer()->PlayTone (2000);
                    buzzer_on = true;
                }
            }
            else
            {
                circle_color = YELLOW;
                bulleye = false;
            }
            _lcd->clearScreen();
            _lcd->setTextColor(YELLOW);
            _lcd->printf (0, 0, "Calibration mode");
            _lcd->drawFastHLine(0, 14, 128, YELLOW);
            _lcd->setTextColor(CYAN);
            _lcd->printf (0, 1, "p: %4.2f", p);
            _lcd->printf (0, 2, "r: %4.2f", r);
            _lcd->printf (10, 1, "y: %4.2f", y);
            _lcd->printf (10, 2, "bull: %d", bulleye);
//            if (stable_count > 10000)
//            {
//                _lcd->setTextColor(YELLOW);
//                _lcd->write("yaw stabilized", 0, 3);
//                _lcd->setTextColor(CYAN);
//            }
//            _lcd->printf (0, 5, "telapsed: %u", t1);
//            _lcd->printf (0, 6, "avg: %u", avg_t / i);

            //draw_hairline(lcd);
            _lcd->drawFastHLine(0, 64, 128, CYAN);
            _lcd->drawFastVLine(64, 14, 128, CYAN);
            _lcd->drawEllipse(64 + (int)(r*5.), 64 + (int)(p*5.), 13, 12, circle_color);
            if (bulleye)
            {
                _lcd->drawEllipse(64 + (int)(r*5.), 64 + (int)(p*5.), 12, 11, circle_color);
            }
            _lcd->drawPixel(64 + (int)(r*5.), 64 + (int)(p*5.), circle_color);
            _lcd->displayFast();
            //LED_Indicators::GetInstance()->ToggleRGB();
        }

   }
    _done = false;
   read.Reset();
   delay(100);
}
