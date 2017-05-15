#include "GPIO_Helper.h"
#include "_pins.h"
#include "9163/TFT_ILI9163C.h"
#include "EXTI_PWM_InputCapture.h"
#include "ESC_PWM_Control.h"
#include "RC_PWM_Reader.h"
#include <stdio.h>
#include "_func.h"
#include "debug.h"

void pwm_capture_test2()
{
    TIMER_With_Channels* pwmTimer = GPIO_Helper::SetupTimer(MOTOR_TIMER);
    ESC_PWM_Control esc (pwmTimer);

    TFT_ILI9163C* lcd = get_lcd();

    lcd->setBgColor(0);
    lcd->setTextColor(YELLOW);
    lcd->clearScreen();
    lcd->setTextSize(2);

    uint16_t thr;
    RC_PWM_Reader pwmReader ((TIMER_With_Channels*)TIMER::Get_Instance(PWM_CAPTURE_TIMER));
    pwmReader.Start();
    while(1)
      {
          lcd->clearScreen();
          thr = pwmReader.GetThrottle();
          lcd->printf (0, 0, "t: %u", thr);
          lcd->printf (0, 1, "r: %u", pwmReader.GetRoll());
          lcd->printf (0, 2, "p: %u", pwmReader.GetPitch());
          lcd->display();
          esc.SetDuty(thr);
          //delay(500);
      }
}


void pwm_capture_test()
{
    TFT_ILI9163C* lcd = get_lcd();

    lcd->setBgColor(0);
    lcd->setTextColor(YELLOW);
    lcd->clearScreen();

    TIMER* timer_in = TIMER::Get_Instance(PWM_CAPTURE_TIMER);
    uint32_t update_period_us = 40000;
    timer_in->EnablePeripheralClock(true);
    timer_in->EnableNVICInterrupt(true);
    timer_in->EnableUpdateInterrupt(true);
    timer_in->SetCounterValue(0);
    timer_in->SetUpdatePeriod_us(update_period_us);
    timer_in->EnableAutoPreload(true);
    timer_in->EnableCounter(true);

    PWM_Capture_EXTI handler (timer_in, PWM_INPUT_THROTTLE, 0);

    handler.Start();
    volatile uint32_t period, duty;

    lcd->setTextSize(2);

    while(1)
    {
        lcd->clearScreen();
        duty = handler.GetDuty_us();
        period = handler.GetPeriod_us();
        lcd->printf (0, 1, "period: %d", period);
        lcd->printf (0, 2, "duty: %d", duty);
        lcd->display();
        //delay(500);
    }

}

