#include "cartman_view.h"
#include "bitmaps/cartman1_16.cpp"

#include "motionapp_reader.h"


CartmanView::CartmanView(MainButtonHandler* bhandler, TFT_ILI9163C* lcd, ScreenView* parent) :
        ScreenView(bhandler, lcd, 0)
{
    _parent = parent;
    _i2c =  bhandler->GetI2C();
  }

void CartmanView::OnClick(Button* button)
{
    Done();
    _bhandler->SetActiveScreen(_parent);
}

void CartmanView::Run()
{
//    i2c->SetErrorCallback(I2C_Error, i2c);

    _lcd->drawBmp(0, 0, cartman1_16);
    _lcd->display();

    int i = 0;
    float y, p, r;
    MotionAppReader read(_i2c);
    read.Init();
    _lcd->setTextSize(4);
    int lastx = 0;
    int lasty = 0;
    while (!_done)
    {
        read.GetYPR(&y, &p, &r);
        if ((++i % 50) == 0)
        {
            int x = r/1.5;
            int y = p/1.5;
            if (fabs(x) > 7 )
            {
                x = x>0 ? 7 : -7;
            }
            if (fabs(y) > 8)
            {
                y =  y>0 ? 8 : -8;
            }

            _lcd->drawPixel(54 + lastx, 47 + lasty,  WHITE);
            _lcd->drawCircle(54 + lastx, 47 + lasty, 1, WHITE);
            _lcd->drawCircle(54 + lastx, 47 + lasty, 2, WHITE);
            _lcd->drawPixel(76+lastx, 47+lasty,  WHITE);
            _lcd->drawCircle(76+lastx, 47+lasty, 1, WHITE);
            _lcd->drawCircle(76+lastx, 47+lasty, 2, WHITE);

            _lcd->drawPixel(54 + x, 47 + y,  0);
            _lcd->drawCircle(54 + x, 47 + y, 1, 0);
            _lcd->drawCircle(54 + x, 47 + y, 2, 0);
            _lcd->drawPixel(76+x, 47+y,  0);
            _lcd->drawCircle(76+x, 47+y, 1, 0);
            _lcd->drawCircle(76+x, 47+y, 2, 0);

            //_lcd->fillScreen(WHITE);
            //_lcd->write ("            ", 0, 0);
            //_lcd->printf (0, 0, "%5.1f", p);
            _lcd->displayFast();
            lastx = x;
            lasty = y;
        }
    }
    read.Reset();
    _done = false;

}

