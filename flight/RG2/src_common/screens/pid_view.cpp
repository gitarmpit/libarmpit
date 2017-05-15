#include "pid_view.h"
#include "cfg_model.h"
#include "debug.h"

PidView::PidView(MainButtonHandler* bhandler, TFT_ILI9163C* lcd, ScreenView* parent)
: ScreenView(bhandler, lcd, 4)
{
    _flashing = true;
    _parent = parent;
}


void PidView::OnClick(Button* button)
{
    if (button->GetId() == BUTTON1)
    {
        if (_idx == 0)
        {
            Done();
            _bhandler->SetActiveScreen(_parent);
        }
        else
        {
             if (_bhandler->GetButton(BUTTON2)->IsDown())
             {
                 _cfgModel.DecrementLow(_idx-1);
             }
             else
             {
                 _cfgModel.IncrementLow(_idx-1);
             }
        }
    }
    else
    {
        if (++_idx > _nitems - 1)
        {
            _idx = 0;
        }
    }

}

void PidView::TimerHook(Button* button)
{
    if (button->GetId() == BUTTON1 && button->GetDurDown() > 500 && _idx != 0)
    {
        if (_bhandler->getBuzzer() != 0)
        {
            _bhandler->getBuzzer()->PlayTone(500, 2);
        }
    _flashing = false;
//      _cfgModel.IncrementHigh(_idx);

         if (_bhandler->GetButton(BUTTON2)->IsDown())
         {
             _cfgModel.DecrementHigh(_idx-1);
         }
         else
         {
             _cfgModel.IncrementHigh(_idx-1);
         }

    }
    else if (button->GetId() == BUTTON1 && !_flashing)
    {
        _flashing = true;
    }
}


void PidView::Run()
{
    _cfg = FlashCfg::GetInstance()->load_cfg();
    _cfgModel.SetCfg(_cfg);

    _bhandler->SetTimerHookInterval(25);

    int8_t i = 0;
    int8_t xoff = 1;
    int8_t yoff = 0;
    _lcd->setTextSize(4);
    _lcd->setBgColor(0);
    _lcd->setTextColor(COLOR565(16,31,28));

    while (!_done)
    {
        _lcd->fillScreen(0);

        if (_idx  != 0 || i < 5)
        {
            _lcd->write ("Exit", xoff + 0, yoff + 0);
        }


        if (_idx  != 1 || i < 5 || !_flashing)
        {
            _lcd->printf (xoff + 0, yoff + 2, "P: %4.1f", _cfg->P);
        }

        if (_idx  != 2 || i < 5 || !_flashing)
        {
            _lcd->printf (xoff + 0, yoff + 3, "I: %4.1f", _cfg->I);
        }

        if (_idx  != 3 || i < 5 || !_flashing)
        {
            _lcd->printf (xoff + 0, 4, "D: %4.1f", _cfg->D);
        }


        _lcd->display();
        delay(100);
        if (++i == 10)
        {
            i = 0;
        }
    }

    _bhandler->SetTimerHookInterval(0);

    FlashCfg::GetInstance()->save_cfg();

    _done = false;

}

