#ifndef _PROGRESS_BLINKER_H
#define _PROGRESS_BLINKER_H

#include "gpio_cpp.h"
#include "timer_cpp.h"

class LED_Indicators
{
private:
	uint32_t  _cnt = 0;
	uint32_t  _delay = 0;

	TIMER_Channel* _pitchRollCh;


    LED_Indicators(LED_Indicators const&);
	LED_Indicators();
    void operator=(LED_Indicators const&);


public:

	static LED_Indicators* GetInstance()
	{
		static LED_Indicators  instance;
		return &instance;
	}


    void EnablePitchRollLeds(bool enable);

    void DisplayPitchRoll (int8_t p, int8_t r);

};

#endif
