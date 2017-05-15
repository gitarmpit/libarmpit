#ifndef _SCREEN_VIEW_H
#define _SCREEN_VIEW_H
#include "9163/TFT_ILI9163C.h"
#include "button_handler/button_handler.h"
#include "main_button_handler.h"

class Button;

#define MAX_CHILDREN 32



class ScreenView
{
public:

	enum BUTTON_ID
	{
		BUTTON1,
		BUTTON2
	};


	ScreenView(MainButtonHandler* bhandler, TFT_ILI9163C* lcd, int8_t nitems);

    virtual void OnClick(Button* button) = 0;
    virtual void Run() = 0;
    virtual void TimerHook(Button* button) {}

    void Done() { _done = true; }


protected:

    void AddChild (ScreenView* child);

	bool _done;
	int8_t _idx;
	int8_t _nitems;
	TFT_ILI9163C* _lcd;
	MainButtonHandler* _bhandler;

	ScreenView*  _children[MAX_CHILDREN];



private:
	int _nchildren;

};



#endif
