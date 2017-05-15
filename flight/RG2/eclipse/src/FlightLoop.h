#ifndef MPU_TIMER_RUNNER_H
#define MPU_TIMER_RUNNER_H

#include "motionapp_reader.h"
#include "ESC_PWM_Control.h"
#include "PidController.h"
#include "GPIO_Helper.h"
#include "LED_Indicators.h"
#include "RC_PWM_Reader.h"
#include "telemetry.h"
#include "button_handler/button.h"
#include "displays/9163/TFT_ILI9163C.h"

#define SAMPLE_INTERVAL 20
class FlightLoop
{

private:

    I2C* _i2c;
	MotionAppReader _motionApp;
	uint32_t        FLASH_MEM_ADDR = 0x08040000; //sector6

	uint32_t  _maxThrottle;
	uint32_t  _minThrottle;
	uint32_t  _maxPitch;
	uint32_t  _minPitch;
	uint32_t  _maxRoll;
	uint32_t  _minRoll;

	uint32_t _pwmProximity = 0;

	uint32_t _lastThrottle = 0;
	uint32_t _lastHeight = 0;


	unsigned long sampleIntervall = 0;


	ESC_PWM_Control* _esc;

	RC_PWM_Reader* _pwmReader;


	PidController _pidRoll, _pidYaw, _pidPitch, _pidZ;

	LED_Indicators* _progress;

	SPI* _proximitySPI;

	float _m1, _m2, _m3, _m4;
	float _y, _p, _r;
	float _setRoll, _setPitch;

	uint16_t _debug_interval_ms;

	void (*_debug_hook)(telemetry*);

	void process(uint32_t pwmThrottle, uint32_t pwmRoll, uint32_t pwmPitch);

	float LimitCorrect(float value);

	void Init();


public:

	FlightLoop(I2C* i2c, SPI* proximitySPI, ESC_PWM_Control* esc, RC_PWM_Reader* pwmReader);

	void Run(TFT_ILI9163C* lcd);

	void SetDebugHook (void (*debug_hook)(telemetry*), uint16_t ms)
	{ _debug_hook = debug_hook; _debug_interval_ms = ms; }


	virtual ~FlightLoop() {}



};

#endif
