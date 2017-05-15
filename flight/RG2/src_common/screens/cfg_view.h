#ifndef _CFG_VIEW_H
#define _CFG_VIEW_H

#include "button_handler/button_handler.h"
#include "GPIO_Helper.h"
#include "9163/TFT_ILI9163C.h"
#include "_pins.h"
#include <string.h>
#include "buzzer.h"
#include "cfg.h"
#include "cfg_model.h"

#define UP 1
#define DOWN 2

//class CfgModel;

class CfgView : public ButtonHandler
{
private:
    Buzzer*       _buzzer;
    TIMER*       _t;

	TFT_ILI9163C* _lcd;
	bool          _done;
	int8_t       _idx;
	CFG*           _cfg;
	CfgModel*     _cfgModel;
	bool          _flashing;

public:
    CfgView(CfgModel* cfgModel, TIMER* t, TFT_ILI9163C* lcd, Buzzer* buzzer = 0);

    void StopTimer();

    virtual void OnClick(Button* button);
    virtual void OnDoubleClick(Button* button);

    virtual void TimerHook(Button* button);

	void Run();

	//void Publish(CFG* cfg);


	void Done() { _done = true; }



};



#endif
