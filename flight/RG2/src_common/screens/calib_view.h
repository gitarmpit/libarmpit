#ifndef _CALIB_VIEW_H
#define _CALIB_VIEW_H
#include "ScreenView.h"

class CalibView : public ScreenView
{
private:
    ScreenView* _parent;
    I2C*        _i2c;
public:
    CalibView (MainButtonHandler* bhandler, TFT_ILI9163C* lcd, ScreenView* parent);

    virtual void OnClick(Button* button);
    void Run();


};



#endif
