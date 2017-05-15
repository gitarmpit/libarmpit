#include "abu_view.h"
#include "bitmaps/abu_no_eyes_16.cpp"
#include "debug.h"
#include "motionapp_reader.h"


AbuView::AbuView(MainButtonHandler* bhandler, TFT_ILI9163C* lcd, ScreenView* parent) :
        ScreenView(bhandler, lcd, 0)
{
    _parent = parent;
    _i2c =  bhandler->GetI2C();
  }

void AbuView::OnClick(Button* button)
{
    Done();
    _bhandler->SetActiveScreen(_parent);
}

void AbuView::Run()
{
    _lcd->setBgColor(0);
    _lcd->setTextColor(YELLOW);

    MPU6050 mpu (_i2c);
    mpu.reset();
    delay(50);
    mpu.initialize();
    mpu.setFullScaleAccelRange(3);
    mpu.setDLPFMode(1);
    int16_t ax, ay;
    while (!_done)
    {
        _lcd->clearScreen();
        ax = mpu.getAccelerationX();
        ay = mpu.getAccelerationY();
        _lcd->printf(0, 0, "%d %d", ax/64, ay/64);
        delay(50);
        _lcd->display();
    }

    _done = false;
}

#if 0
void AbuView::Run()
{
    uint32_t i2cClockSpeed = 100000;
    static I2C* i2c = GPIO_Helper::SetupI2C(MPU6050_I2C, i2cClockSpeed);
//    i2c->SetErrorCallback(I2C_Error, i2c);


    int i = 0;
    float y, p, r;
    MotionAppReader read(i2c);
    read.Init();
    _lcd->setTextSize(4);
    int lcenterx = 55;
    int lcentery = 65;
    int rcenterx = 88;
    int rcentery = 63;
    while (!_done)
    {
        read.GetYPR(&y, &p, &r);
        if ((++i % 50) == 0)
        {
            int x = r/1.5;
            int y = p/1.5;
            if (fabs(x) > 4 )
            {
                x = x>0 ? 4 : -4;
            }
            if (fabs(y) > 4)
            {
                y =  y>0 ? 4 : -4;
            }


            _lcd->drawBmp2(0, 0, abu_no_eyes_16);

            _lcd->drawPixel(lcenterx + x, lcentery + y,  0);
            _lcd->drawCircle(lcenterx + x, lcentery + y, 1, 0);
            _lcd->drawCircle(lcenterx + x, lcentery + y, 2, 0);
            _lcd->drawPixel(rcenterx + x, rcentery + y,  0);
            _lcd->drawCircle(rcenterx + x, rcentery + y, 1, 0);
            _lcd->drawCircle(rcenterx + x, rcentery + y, 2, 0);

            //_lcd->fillScreen(WHITE);
            //_lcd->write ("            ", 0, 0);
            //_lcd->printf (0, 0, "%5.1f", p);
            _lcd->displayFast();
        }
    }
    read.Reset();
    _done = false;

}

#endif
