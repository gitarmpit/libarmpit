#ifndef _PID_VIEW_H
#define _PID_VIEW_H
#include "ScreenView.h"
#include "cfg_model.h"

class PidView : public ScreenView
{
private:
	bool _flashing;
	CfgModel       _cfgModel;
	CFG*           _cfg;
	ScreenView*    _parent;

public:
	PidView (MainButtonHandler* bhandler, TFT_ILI9163C* lcd, ScreenView* parent);

    virtual void OnClick(Button* button);
    void Run();
    virtual void TimerHook(Button* button);


};



#endif
