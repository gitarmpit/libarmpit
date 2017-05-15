#include "FlightLoop.h"
#include <stdlib.h>
#include <stdio.h>
#include "fp2.h"
//#define CALIBRATION_MODE
#include <string.h>
#include "system_time.h"
#include "_pins.h"
#include "buzzer.h"
#include "main_button_handler.h"
#include "screens/ScreenView.h"
#include "flash_cfg.h"
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
                      RC_PWM_Reader* pwmReader) :
        _motionApp(i2c)
{
    _i2c = i2c;
    sampleIntervall = 0;
    _progress = LED_Indicators::GetInstance();

    _esc = esc;
    _pwmReader = pwmReader;

    _maxThrottle = 1830;
    _minThrottle = 1180;
    _maxPitch = 1850;
    _minPitch = 1150;
    _maxRoll = 1880;
    _minRoll = 1130;

    _proximitySPI = proximitySPI;

    _debug_hook = 0;
    _debug_interval_ms = 0;
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

    CFG* cfg = FlashCfg::GetInstance()->load_cfg();

//    _pidRoll.SetTunings(7, 0.5, 2.8);
//  _pidPitch.SetTunings(7, 0.5, 2.8);
//  _pidYaw.SetTunings(1, 0.01, 0.00);

    _pidRoll.SetTunings(cfg->P, cfg->I, cfg->D);
    _pidPitch.SetTunings(cfg->P, cfg->I, cfg->D);
    _pidYaw.SetTunings(1, 0.01, 0.00);

    _pidZ.SetSampleTime(20);
    _pidZ.SetMode(AUTOMATIC);
    _pidZ.SetControllerDirection(DIRECT);
    _pidZ.SetOutputLimits(-100, +100);
    _pidZ.SetTunings(2, 0.5, 0.8);


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

void FlightLoop::Run(TFT_ILI9163C* lcd)
{
    Buzzer*  buzzer = Buzzer::GetInstance();
    buzzer->Init(BUZZER_TIMER, BUZZER_PIN, BUZZER_TIMER_CHANNEL);
    buzzer->SetVolume(100);

    Init();

    Button b1 (GPIO_Helper::GetPin("D6"), ScreenView::BUTTON1);

    _pwmReader->Start();

    bool running = false;
    uint32_t last_time = millis();

//    _esc->SetDuty(2000);
//    _esc->SetDuty(700);
//    while (1) {};

//    _esc->SetDuty(700);
//    delay(20000);
//    _esc->SetDuty(1300);
//     delay(5000);
//     _esc->SetDuty(1700);
//     delay(5000);
//     _esc->SetDuty(2000);
//    delay(10000);
//    _esc->SetDuty(700);
//    delay(5000);
//    _esc->SetDuty(0);
//
//    while (1) {};


//    _esc->SetDuty(_maxThrottle);
//    _esc->SetDuty(_minThrottle);
//    while (1) {};

//  _esc->SetDuty(_minThrottle);
//    delay(20000);
//    _esc->SetDuty(1500);
//     delay(5000);
//     _esc->SetDuty(1650);
//     delay(5000);
//     _esc->SetDuty(_maxThrottle);
//    delay(10000);
//    _esc->SetDuty(_minThrottle);
//    delay(5000);
////
//    _esc->SetDuty(0);
//
//  while (1) {};

    while (1)
    {
        volatile uint32_t pwmThrottle = _pwmReader->GetThrottle();
        volatile uint32_t pwmPitch = _pwmReader->GetPitch();
        volatile uint32_t pwmRoll = _pwmReader->GetRoll();
        static telemetry t;

        //pwmThrottle = _minThrottle + 500;
        pwmPitch = (_maxPitch + _minPitch) / 2;
        pwmRoll = (_maxRoll + _minRoll) / 2;

        if (_debug_hook)
        {
            memset (&t, 0, sizeof(t));
            t.throttle = pwmThrottle;
            t.pitch = pwmPitch;
            t.roll = pwmRoll;
        }

#ifdef CALIBRATION_MODE
        _esc->SetDuty(pwmThrottle);
        continue;
#endif



        if (!running && pwmThrottle > _minThrottle - 20
                && pwmThrottle < _minThrottle + 20)
        {
            buzzer->PlayTune(barbie2);
            _motionApp.Init();
            _esc->SetDuty(_minThrottle);
            running = true;
        }

        if (running)
        {
            //stop
            if (pwmThrottle < _minThrottle - 50)
            {
                _progress->RGB_Off();
                _esc->SetDuty(_minThrottle);
                _motionApp.Reset();
                running = false;
            }
            else
            {
                process(pwmThrottle, pwmRoll, pwmPitch);
                if (_debug_hook)
                {
                    t.y = fix16_from_float(_y);
                    t.p = fix16_from_float(_p);
                    t.r = fix16_from_float(_r);
                    t.m[0] = _m1;
                    t.m[1] = _m2;
                    t.m[2] = _m3;
                    t.m[3] = _m4;
                    t.p_set = fix16_from_float(_setPitch);
                    t.r_set = fix16_from_float(_setRoll);
                }
            }

        }

        if (!running && !b1.IsPinSet())
        {
            while (!b1.IsPinSet())
                ;

            delay(100);
            MainButtonHandler appl (TIMER::Get_Instance(BUTTON_TIMER), lcd, _i2c, buzzer);
            appl.Run();
        }

        volatile uint32_t now = millis();
        if (_debug_hook && (now - last_time) > _debug_interval_ms)
        {
            t.timestamp_ms = millis();
            _debug_hook (&t);
            last_time = millis();
        }

    }

}


void FlightLoop::process(uint32_t pwmThrottle, uint32_t pwmRoll,
        uint32_t pwmPitch)
{

    //float maxAngle = 20.; //max degree pitch/roll

    static int i;

    if (pwmPitch < _minPitch || pwmPitch > _maxPitch)
    {
        return;
    }

    if (pwmRoll < _minRoll || pwmRoll > _maxRoll)
    {
        return;
    }

    if (_motionApp.GetYPR(&_y, &_p, &_r))
    {
#ifdef PROXIMITY_SPI
        uint8_t proximity = _proximitySPI->TransmitByte(0x0);
#endif

        _setRoll = -20.0 + ((((float) pwmRoll - 1155.0) / 720.0) * 40.0);
        _setPitch = -20.0 + ((((float) pwmPitch - 1160.0) / 650.0) * 40.0);
        float throttle = (float) pwmThrottle;

        // neutral pitch/roll stick doesn't deliver 0 exactly, assume 0 within a tolerance band
        if (_setRoll < 5.0 || _setRoll > -5.0)
            _setRoll = 0.0;
        if (_setPitch < 5.0 || _setPitch > -5.0)
            _setPitch = 0.0;

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
        _pidRoll.Setpoint = _setRoll;  //balance to this angle
        _pidYaw.Setpoint = 0.0;
        _pidPitch.Setpoint = _setPitch;

        // balance
        _pidRoll.Input = _r;
        _pidRoll.Compute(sampleIntervall);

        _pidYaw.Input = 0;
        _pidYaw.Compute(sampleIntervall);

        _pidPitch.Input = _p;
        _pidPitch.Compute(sampleIntervall);

        sampleIntervall += 20;
        _pidYaw.Output = 0;

        float r_balance = _pidRoll.Output;
        float p_balance = _pidPitch.Output;

        _m1 = (throttle + r_balance + p_balance - _pidYaw.Output) + _pidZ.Output;
        _m2 = (throttle - r_balance + p_balance + _pidYaw.Output) + _pidZ.Output;
        _m3 = (throttle - r_balance - p_balance - _pidYaw.Output) + _pidZ.Output;
        _m4 = (throttle + r_balance - p_balance + _pidYaw.Output) + _pidZ.Output;

        _m1 = LimitCorrect(_m1);
        _m2 = LimitCorrect(_m2);
        _m3 = LimitCorrect(_m3);
        _m4 = LimitCorrect(_m4);


        _esc->SetDuty(_m1, _m2, _m3, _m4);

        _lastThrottle = pwmThrottle;

        if ((++i % 10) == 0)
        {
            _progress->ToggleRGB();
        }
    }

}
