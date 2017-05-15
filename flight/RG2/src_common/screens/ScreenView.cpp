#include "ScreenView.h"

ScreenView::ScreenView(MainButtonHandler* bhandler, TFT_ILI9163C* lcd, int8_t nitems)
{
	_lcd = lcd;
	_bhandler = bhandler;
	_done = false;
	_idx = 0;
	_nitems = nitems;
	_nchildren = 0;
	for (int8_t i = 0; i < MAX_CHILDREN; ++i)
	{
		_children[i] = 0;
	}
}

void ScreenView::AddChild (ScreenView* child)
{
	if (_nchildren <= MAX_CHILDREN)
	{
		_children [ _nchildren++ ] =  child;
	}
}


