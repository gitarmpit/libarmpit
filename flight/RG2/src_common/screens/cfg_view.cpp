#include "cfg_view.h"
#include "debug.h"

CfgView::CfgView(CfgModel* cfgModel, TIMER* t, TFT_ILI9163C* lcd, Buzzer* buzzer)
 : ButtonHandler(t)
{
    _t = t;
    _buzzer = buzzer;
    _cfgModel = cfgModel;
    _idx = 0;
    _lcd = lcd;
    _done = false;
    _lcd->setBgColor(0);
    _lcd->setTextColor(WHITE);
    _lcd->setTextSize(4);
    _idx = 0;
    _cfg = cfgModel->GetCfg();
    _flashing = true;


    SetTimerHookInterval(25);
}


void CfgView::StopTimer()
{
    _t->EnableCounter(false);
}

void CfgView::OnClick(Button* button)
{

    if (_buzzer)
    {
        _buzzer->PlayTone(500, 2);
    }

    if (button->GetId() == UP)
    {
        if (_idx < _cfgModel->Count())
        {
             if (GetButton(DOWN)->IsDown())
             {
                 _cfgModel->DecrementLow(_idx);
             }
             else
             {
                 _cfgModel->IncrementLow(_idx);
             }
        }
        else
        {
            Done();
        }
    }
    else
    {
        ++_idx;
        if (_idx ==  _cfgModel->Count() + 1)
        {
            _idx = 0;
        }
    }
}

void CfgView::OnDoubleClick(Button* button)
{
    if (_buzzer)
    {
        _buzzer->PlayTone(500, 2);
    }
}

void CfgView::TimerHook(Button* button)
{
    if (button->GetId() == UP && button->GetDurDown() > 500 && _idx < _cfgModel->Count())
    {
        _flashing = false;
//      _cfgModel->IncrementHigh(_idx);

         if (GetButton(DOWN)->IsDown())
         {
             _cfgModel->DecrementHigh(_idx);
         }
         else
         {
             _cfgModel->IncrementHigh(_idx);
         }

    }
    else if (button->GetId() == UP && !_flashing)
    {
        _flashing = true;
    }
}


void CfgView::Run()
{
    int8_t i = 0;
    int8_t xoff = 1;
    int8_t yoff = 0;
    while (!_done)
    {
        _lcd->clearScreen();
        if (_idx  != 0 || i < 5 || !_flashing)
        {
            _lcd->printf (xoff + 0, yoff + 0, "P: %4.1f", _cfg->P);
        }

        if (_idx  != 1 || i < 5 || !_flashing)
        {
            _lcd->printf (xoff + 0, yoff + 1, "I: %4.1f", _cfg->I);
        }

        if (_idx  != 2 || i < 5 || !_flashing)
        {
            _lcd->printf (xoff + 0, 2, "D: %4.1f", _cfg->D);
        }

        if (_idx  != 3 || i < 5)
        {
            _lcd->printf (xoff + 0, yoff + 4, "Exit");
        }

        _lcd->display();
        delay(100);
        if (++i == 10)
        {
            i = 0;
        }
    }

    StopTimer();
}

//void CfgView::Run()
//{
//    int8_t i = 0;
//    int8_t xoff = 1;
//    int8_t yoff = 0;
//    while (!_done)
//    {
//        _lcd->clearScreen();
//        if (_idx  != 0 || i < 5)
//        {
//            _lcd->write ("P:", xoff + 0, yoff + 0);
//        }
//        _lcd->printf (xoff + 2, yoff + 0, "%4.1f", _cfg->P);
//
//        if (_idx  != 1 || i < 5)
//        {
//            _lcd->write ("I:", xoff + 0, yoff + 1);
//        }
//        _lcd->printf (xoff + 2, yoff + 1, "%4.1f", _cfg->I);
//
//        if (_idx  != 2 || i < 5)
//        {
//            _lcd->write ("D:", xoff + 0, yoff + 2);
//        }
//        _lcd->printf (xoff + 2, 2, "%4.1f", _cfg->D);
//
//        if (_idx  != 3 || i < 5)
//        {
//            _lcd->write ("Exit", xoff + 0, yoff + 3);
//        }
//        _lcd->display();
//        delay(100);
//        if (++i == 10)
//        {
//            i = 0;
//        }
//    }
//
//    StopTimer();
//}



//void CfgView::Publish(CFG* cfg)
//{
//    memcpy (&_cfg, cfg, sizeof(CFG));
//}
