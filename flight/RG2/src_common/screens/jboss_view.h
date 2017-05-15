#ifndef _JBOSS_VIEW_H
#define _JBOSS_VIEW_H
#include "ScreenView.h"

class JBOSSView : public ScreenView
{
private:
	ScreenView* _parent;
public:
	JBOSSView (MainButtonHandler* bhandler, TFT_ILI9163C* lcd, ScreenView* parent);

    virtual void OnClick(Button* button);
    void Run();


};



#endif
