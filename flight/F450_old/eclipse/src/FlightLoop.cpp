#include "FlightLoop.h"
#include <stdlib.h>
#include "LED_Indicators.h"
#include "flash.h"
#include "usart_cpp.h"
#include "fp2.h"
#include "RF22/si4432.h"
#include "debug.h"

class My_Timeout_Handler: public PWM_Timeout_Handler
{
public:
    virtual void Handle_Timeout()
    {

    }
};

class My_Timeout_Handler2: public PWM_Timeout_Handler
{
public:
    virtual void Handle_Timeout()
    {

    }
};

FlightLoop::FlightLoop(I2C* i2c, SPI* proximitySPI,
                      ESC_PWM_Control* esc,
                      RC_PWM_Reader* pwmReader,
                      Adafruit_PCD8544* lcd,
                      Si4432* radio) :
        _motionApp(i2c)
{

    sampleIntervall = 0;
    _progress = LED_Indicators::GetInstance();

    _esc = esc;
    _pwmReader = pwmReader;

    _maxThrottle = 1820;
    _minThrottle = 1175;
    _maxPitch = 1850;
    _minPitch = 1150;
    _maxRoll = 1880;
    _minRoll = 1130;

    _lcd = lcd;
    _radio = radio;
    _proximitySPI = proximitySPI;

    _autoHover = false;

}

float FlightLoop::LimitCorrect(float value)
{
    if (value > _maxThrottle)
    {
        value = _maxThrottle;
    }
    else if (value < _minThrottle)
    {
        value = _minThrottle;
    }

    return value;
}


void FlightLoop::Init()
{
    _pidRoll.SetSampleTime(SAMPLE_INTERVAL);
    _pidRoll.SetMode(AUTOMATIC);
    _pidRoll.SetControllerDirection(DIRECT);
    _pidRoll.SetOutputLimits(-100, +100);

    _pidYaw.SetMode(AUTOMATIC);
    _pidYaw.SetControllerDirection(DIRECT);
    _pidYaw.SetSampleTime(SAMPLE_INTERVAL);
    _pidYaw.SetOutputLimits(-1, +1);

    _pidPitch.SetSampleTime(SAMPLE_INTERVAL);
    _pidPitch.SetMode(AUTOMATIC);
    _pidPitch.SetControllerDirection(DIRECT);
    _pidPitch.SetOutputLimits(-100, +100);

    _pidRoll.SetTunings(2, 0.2, 2.8);
    _pidPitch.SetTunings(2, 0.2, 2.8);
    _pidYaw.SetTunings(1, 0.01, 0.00);

    _pidZ.SetSampleTime(20);
    _pidZ.SetMode(AUTOMATIC);
    _pidZ.SetControllerDirection(DIRECT);
    _pidZ.SetOutputLimits(-100, +100);
    _pidZ.SetTunings(2, 0.5, 0.8);

    _progress->EnablePitchRollLeds(true);
    _progress->EnableThrottleLevelLeds(true);

#ifdef PROXIMITY_SPI
    //turn LED off
    _proximitySPI->TransmitByte(0x5);
//    delay(100);
    //Activate Ultrasonic
    _proximitySPI->TransmitByte(0x1);
    delay(50);
    //Set inches
    _proximitySPI->TransmitByte(0x4);
#endif

}

void FlightLoop::Run()
{
    Init();


    bool running = false;
    bool autoHover = false;


    uint32_t radio_counter = 0;
    while (1)
    {
        volatile uint32_t pwmThrottle = _pwmReader->GetThrottle();
        volatile uint32_t pwmPitch = _pwmReader->GetPitch();
        volatile uint32_t pwmRoll = _pwmReader->GetRoll();


#ifdef USE_LCD
        _lcd->clearDisplay();
#endif

#ifdef USE_RADIO
        static uint32_t radio_buf[128];
        if (!running)
        {
            if ((radio_counter++ % 100) == 0)
            {
                radio_buf[0] = pwmThrottle;
                radio_buf[1] = pwmPitch;
                radio_buf[2] = pwmRoll;
                _radio->trySendPacket(12, (uint8_t*) radio_buf, 150);

            }
        }
#endif

        //_autoHover = (pwmThrottle > 985 && pwmThrottle < 1015);

        if (!running && pwmThrottle > _minThrottle - 20
                && pwmThrottle < _minThrottle + 20)
        {
            _motionApp.Init();
            _esc->SetDuty(_minThrottle);
            running = true;
        }

        if (running)
        {
            if (pwmThrottle >= _minThrottle - 50 || _autoHover)
            {
                if (autoHover)
                {
                    pwmThrottle = _lastThrottle;
                }

                process(pwmThrottle, pwmRoll, pwmPitch);
            }
            else
            {
                _progress->RGB_Off();
                _progress->DisplayPitchRoll(0, 0);
                _progress->DisplayThrottleLevel(0, 0, 0, 0);
                _esc->SetDuty(_minThrottle);
                _motionApp.Reset();
                running = false;
            }

        }
    }

}

void FlightLoop::process(uint32_t pwmThrottle, uint32_t pwmRoll,
        uint32_t pwmPitch)
{

    float y = 0.0, p = 0.0, r = 0.0;

    float maxAngle = 20.; //max degree pitch/roll

    static uint32_t radio_buf[128];

    if (pwmPitch < _minPitch || pwmPitch > _maxPitch)
    {
        return;
    }

    if (pwmRoll < _minRoll || pwmRoll > _maxRoll)
    {
        return;
    }

    if (_motionApp.GetYPR(&y, &p, &r))
    {
        //float setPitch = (float) ((pwmPitch - minPitchPWM) / (maxPitchPWM - minPitchPWM) - 0.5) * 2 * maxAngle;
        //float setRoll = (float) ((pwmRoll - minRollPWM) / (maxRollPWM - minRollPWM) - 0.5) * 2 * maxAngle;

        // this needs to be read in this loop, we rely on the delay otherwise we get funky readings if called too often

#ifdef PROXIMITY_SPI
        uint8_t proximity = _proximitySPI->TransmitByte(0x0);
#endif

        float setRoll = -20.0 + ((((float) pwmRoll - 1155.0) / 720.0) * 40.0);
        float setPitch = -20.0 + ((((float) pwmPitch - 1160.0) / 650.0) * 40.0);
        float throttle = (float) pwmThrottle;

        // neutral pitch/roll stick doesn't deliver 0 exactly, assume 0 within a tolerance band
        if (setRoll < 5.0 || setRoll > -5.0)
            setRoll = 0.0;
        if (setPitch < 5.0 || setPitch > -5.0)
            setPitch = 0.0;

        /*
         if (_autoHover)
         {

         _pidZ.Input = (float) proximity;
         _pidZ.Setpoint = (float) 40;    // hover at  meter
         _pidZ.Compute(sampleIntervall);
         }
         else
         {
         _pidZ.Output = 0.0;
         }*/
        _pidZ.Output = 0.0;
        _pidRoll.Setpoint = setRoll;  //balance to this angle
        _pidYaw.Setpoint = 0.0;
        _pidPitch.Setpoint = setPitch;

        // balance
        _pidRoll.Input = r;
        _pidRoll.Compute(sampleIntervall);

        _pidYaw.Input = 0;
        _pidYaw.Compute(sampleIntervall);

        _pidPitch.Input = p;
        _pidPitch.Compute(sampleIntervall);

        sampleIntervall += 20;
        _pidYaw.Output = 0;

        float r_balance = _pidRoll.Output;
        float p_balance = _pidPitch.Output;

        float m1 = (throttle + r_balance + p_balance - _pidYaw.Output) + _pidZ.Output;
        float m2 = (throttle - r_balance + p_balance + _pidYaw.Output) + _pidZ.Output;
        float m3 = (throttle - r_balance - p_balance - _pidYaw.Output) + _pidZ.Output;
        float m4 = (throttle + r_balance - p_balance + _pidYaw.Output) + _pidZ.Output;

        m1 = LimitCorrect(m1);
        m2 = LimitCorrect(m2);
        m3 = LimitCorrect(m3);
        m4 = LimitCorrect(m4);

#ifdef USE_RADIO
        radio_buf[0] = pwmThrottle;
        radio_buf[1] = pwmPitch;
        radio_buf[2] = pwmRoll;
        uint32_t maxRetry = 150;
        _radio->trySendPacket(12, (uint8_t *) radio_buf, maxRetry);
#endif

        _esc->SetDuty(m1, m2, m3, m4);

        _lastThrottle = pwmThrottle;


        _progress->RGB_BlinkAll();

        _progress->DisplayPitchRoll((int8_t) p, (int8_t) r);
        _progress->DisplayThrottleLevel((int32_t) m1, (int32_t) m2, (int32_t) m3, (int32_t) m4);
    }

}
//#ifdef USE_LCD
////this is from process
////      sprintf (buf, "Prox: %d", proximity);
////      display->write(0, 0, buf);
////
////      fix16_to_str (fix16_from_float(y), buf2, 2);
////      //sprintf (buf, "y: %s", buf2);
////        display->write(0, 1, buf2);
////        fix16_to_str (fix16_from_float(p), buf2, 2);
////        //sprintf (buf, "p: %s", buf2);
////        display->write(0, 2, buf2);
////        fix16_to_str (fix16_from_float(r), buf2, 2);
////        //sprintf (buf, "r: %s", buf2);
////        display->write(0, 3, buf2);
//
////fix16_to_str (fix16_from_int(pwmThrottle), buf2, 2);
////sprintf (buf, "%d %d %d", pwmThrottle, pwmPitch, pwmRoll);
////fix16_to_str (fix16_from_int(pwmPitch), buf2, 2);
//
//char buf[64];
//
//sprintf (buf, "%d %d", (int) m2,(int) m3);
//display->write(0, 0, buf);
//
//display->display();
//#endif
