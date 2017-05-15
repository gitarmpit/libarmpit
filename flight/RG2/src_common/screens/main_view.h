#ifndef _MAIN_VIEW_H
#define _MAIN_VIEW_H
#include "ScreenView.h"

class MainView : public ScreenView
{
public:
	MainView (MainButtonHandler* bhandler, TFT_ILI9163C* lcd);

    virtual void OnClick(Button* button);
    void Run();


};



#endif
