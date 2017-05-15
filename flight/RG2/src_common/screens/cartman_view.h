#ifndef _CARTMAN_VIEW_H
#define _CARTMAN_VIEW_H

#include "ScreenView.h"
#include "i2c_cpp.h"

class CartmanView : public ScreenView
{
private:
    ScreenView* _parent;
    I2C* _i2c;
public:
    CartmanView (MainButtonHandler* bhandler, TFT_ILI9163C* lcd, ScreenView* parent);

    virtual void OnClick(Button* button);
    void Run();


};



#endif
