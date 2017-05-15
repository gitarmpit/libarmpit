#ifndef _RC_VIEW_H
#define _RC_VIEW_H

#include "ScreenView.h"
#include "i2c_cpp.h"

class RcView : public ScreenView
{
private:
    ScreenView* _parent;
public:
    RcView (MainButtonHandler* bhandler, TFT_ILI9163C* lcd, ScreenView* parent);

    virtual void OnClick(Button* button);
    void Run();


};



#endif
