#ifndef _BACKLIGH_HANDLER_H

#define _BACKLIGH_HANDLER_H

#include "systick.h"
#include "timer_cpp.h"

class BacklightHandler
{
private:

    uint32_t        _systick_count;
    TIMER_Channel*  _lcdBL;


public:

    BacklightHandler();
    void Enable();
    void Disable();

    void SetBackLightChannel (TIMER_Channel* blCh);
};


#endif
