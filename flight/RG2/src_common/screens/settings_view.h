#ifndef _SETTINGS_VIEW_H
#define _SETTINGS_VIEW_H
#include "ScreenView.h"

class SettingsView : public ScreenView
{
public:
	SettingsView (MainButtonHandler* bhandler, TFT_ILI9163C* lcd, ScreenView* parent);

    virtual void OnClick(Button* button);
    void Run();


};



#endif
