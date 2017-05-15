#ifndef _ABU_VIEW_H
#define _ABU_VIEW_H

#include "ScreenView.h"
#include "i2c_cpp.h"

class AbuView : public ScreenView
{
private:
    ScreenView* _parent;
    I2C* _i2c;
public:
    AbuView (MainButtonHandler* bhandler, TFT_ILI9163C* lcd, ScreenView* parent);

    virtual void OnClick(Button* button);
    void Run();


};



#endif
