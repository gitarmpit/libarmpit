#ifndef MPU_TIMER_RUNNER_H
#define MPU_TIMER_RUNNER_H

#include "timer_cpp.h"
#include "i2c_cpp.h"
#include "motionapp_reader.h"

#include "ESC_PWM_Control.h"
#include "PidController.h"
#include "usart_cpp.h"
#include "GPIO_Helper.h"
#include "spi_cpp.h"
#include "5110/Adafruit_Nokia5110.h"
#include "RF22/si4432.h"
#include "LED_Indicators.h"
#include "RC_PWM_Reader.h"

#define SAMPLE_INTERVAL 20
class FlightLoop
{

private:

    MotionAppReader _motionApp;

    uint32_t  _maxThrottle;
    uint32_t  _minThrottle;
    uint32_t  _maxPitch;
    uint32_t  _minPitch;
    uint32_t  _maxRoll;
    uint32_t  _minRoll;

    uint32_t _pwmProximity = 0;

    uint32_t _lastThrottle = 0;
    uint32_t _lastHeight = 0;

    bool _autoHover;

    unsigned long sampleIntervall = 0;


    ESC_PWM_Control* _esc;

    RC_PWM_Reader* _pwmReader;

    Adafruit_Nokia5110* _lcd;
    Si4432* _radio;

    PidController _pidRoll, _pidYaw, _pidPitch, _pidZ;

    LED_Indicators* _progress;

    SPI* _proximitySPI;

    void process(uint32_t pwmThrottle, uint32_t pwmRoll, uint32_t pwmPitch);

    float LimitCorrect(float value);

    void Init();


public:

    FlightLoop(I2C* i2c, SPI* proximitySPI, ESC_PWM_Control* esc, RC_PWM_Reader* pwmReader, Adafruit_Nokia5110* lcd, Si4432* radio);

    void Run();



    virtual ~FlightLoop() {}



};

#endif
