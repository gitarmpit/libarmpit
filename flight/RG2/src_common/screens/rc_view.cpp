#include "rc_view.h"
#include "EXTI_PWM_InputCapture.h"
#include "ESC_PWM_Control.h"
#include "RC_PWM_Reader.h"
#include "debug.h"


RcView::RcView(MainButtonHandler* bhandler, TFT_ILI9163C* lcd, ScreenView* parent) :
        ScreenView(bhandler, lcd, 0)
{
    _parent = parent;

}

void RcView::OnClick(Button* button)
{
    Done();
    _bhandler->SetActiveScreen(_parent);
}

void RcView::Run()
{
    _lcd->setBgColor(0);
    _lcd->setTextColor(YELLOW);
    _lcd->clearScreen();
    _lcd->setTextSize(2);

    TIMER_With_Channels* pwmTimer = GPIO_Helper::SetupTimer(MOTOR_TIMER);
    ESC_PWM_Control esc (pwmTimer);

    uint16_t thr;
    RC_PWM_Reader pwmReader ((TIMER_With_Channels*)TIMER::Get_Instance(PWM_CAPTURE_TIMER));
    pwmReader.Start();
    while(!_done)
      {
          _lcd->clearScreen();
          thr = pwmReader.GetThrottle();
          _lcd->printf(0, 0, "thr: %d", thr);
          _lcd->printf (0, 1, "r: %d", pwmReader.GetRoll());
          _lcd->printf (0, 2, "p: %d", pwmReader.GetPitch());
          _lcd->display();
          esc.SetDuty(thr);
          delay(500);
      }


    while (!_done)
    {
        _lcd->clearScreen();
        delay(50);
        _lcd->display();
    }

    _done = false;
}

