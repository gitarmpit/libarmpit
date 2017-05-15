#include "button_handler/button_handler.h"
#include "GPIO_Helper.h"
#include "9163/TFT_ILI9163C.h"
#include "flash_cfg.h"
#include "_pins.h"
#include <string.h>
#include "buzzer.h"
#include "debug.h"

#include "screens/cfg_view.h"
#include "screens/cfg_model.h"

#include "_func.h"


#define UP 1
#define DOWN 2


static uint32_t time_on;
static uint32_t dclicks;

CFG* cfg;

class BHandler : public ButtonHandler
{
private:
    TFT_ILI9163C* lcd;
    int _idx;

public:
    BHandler(TIMER* t, TFT_ILI9163C* lcd) : ButtonHandler(t)
    {
        this->lcd = lcd;
        _idx = 0;
    }

    virtual void OnStateChange(Button* button)
    {
        static int i;
        ++i;
    }
    virtual void OnButtonUp(Button* button)
    {
        static int i;
        ++i;

    }
    virtual void OnButtonDown(Button* button)
    {
        static int i;
        ++i;

    }
    virtual void OnClick(Button* button)
    {
        if (button->GetId() == DOWN)
        {
            ++_idx;
            if (_idx > 8)
            {
                _idx = 0;
            }
        }
        else
        {
            --_idx;
            if (_idx < 0)
            {
                _idx = 8;
            }
        }
    }
    virtual void OnDoubleClick(Button* button)
    {
        static int i;
        ++i;

    }

    void Run()
    {
        lcd->setTextSize(2);
        while(1)
        {
            for (int i = 0; i < 9; ++i)
            {
                int row = i % 3;
                int col = i / 3;
                if (i == _idx)
                {
                    lcd->setBgColor(WHITE);
                    lcd->setTextColor(0);
                }
                else
                {
                    lcd->setBgColor(0);
                    lcd->setTextColor(WHITE);
                }
                lcd->printf (col*6, row, "item %d", i+1);
            }
            lcd->display();
            delay(100);
        }

        while(1);
    }

};

class PidConfigHandler : public ButtonHandler
{
private:
    Buzzer*        _buzzer;
    TIMER*       _t;
    bool         _saving;

public:
    PidConfigHandler(TIMER* t, Buzzer* buzzer) : ButtonHandler(t)
    {
        _t = t;
        _saving = false;
        _buzzer = buzzer;
        SetTimerHookInterval(25);
    }

    void StopTimer()
    {
       _t->EnableCounter(false);
    }

    virtual void OnClick(Button* button)
    {
        if (_saving)
            return;

        _buzzer->PlayTone(500, 2);

        float delta = 0.1;
        if (button->GetId() == DOWN)
        {
            delta = -0.1;
        }
        cfg->P += delta;
        if (cfg->P > 20.)
        {
            cfg->P -= 20.;
        }
        else if (cfg->P < 0)
        {
            cfg->P += 20.;
        }
    }
    virtual void OnDoubleClick(Button* button)
    {
        _buzzer->PlayTone(500, 2);
    }
    virtual void TimerHook(Button* button)
    {
        if (_saving)
            return;

        if (GetButton(DOWN)->GetDurDown() > 1000 &&
            GetButton(UP)->GetDurDown() > 1000)
        {
            _saving = true;
            return;
        }

        if (GetButton(DOWN)->IsDown() &&
            GetButton(UP)->IsDown())
        {
            return;
        }

        time_on = button->GetDurDown();
        if (button->GetId() == DOWN)
        {
            time_on++;
            time_on--;
        }
        if (time_on > 500)
        {
            _buzzer->PlayTone(500, 2);
            if (button->GetId() == UP)
            {
                cfg->P += 0.1;
            }
            else
            {
                cfg->P -= 0.1;
            }

            if (cfg->P > 20.)
            {
                cfg->P -= 20.;
            }
            else if (cfg->P < 0)
            {
                cfg->P += 20.;
            }
        }
    }

    bool saving() { return _saving; }

};

void button_test()
{
    TFT_ILI9163C* lcd = get_lcd();

    FlashCfg* flashCfg = FlashCfg::GetInstance();
    cfg = flashCfg->load_cfg();


    lcd->setBgColor(0);
    lcd->setTextColor(YELLOW);
    lcd->setTextSize(2);

    Buzzer*  buzzer = Buzzer::GetInstance();
    buzzer->Init(BUZZER_TIMER, BUZZER_PIN, BUZZER_TIMER_CHANNEL);
    buzzer->SetVolume(100);

    PidConfigHandler h (TIMER::Get_Instance(BUTTON_TIMER), buzzer);
    Button b1 (GPIO_Helper::GetPin("D6"), UP);
    h.AddButton(&b1);
    Button b2 (GPIO_Helper::GetPin("D7"), DOWN);
    h.AddButton(&b2);

    while(1)
    {
        lcd->clearScreen();
        if (h.saving())
        {
            h.StopTimer();
            lcd->write ("Saving config...", 0, 0, 2);
            lcd->display();
            flashCfg->save_cfg();
            lcd->write ("Done", 0, 1, 2);
            lcd->display();
            buzzer->PlayTune(":b=400:c6,g6,4c7");
            break;
        }

        lcd->printf (0, 0, "%4.1f", cfg->P);
        lcd->printf (0, 1, "%d", dclicks);
        lcd->display();
        delay(30);
    }
    h.StopTimer();
    while(1);

}

void button_test2()
{

    TFT_ILI9163C* lcd = get_lcd();

    CFG* cfg2 = FlashCfg::GetInstance()->load_cfg();

    CfgModel cfgModel(cfg2);
    CfgView cfgView (&cfgModel, TIMER::Get_Instance(BUTTON_TIMER), lcd);
    Button b1 (GPIO_Helper::GetPin(BUTTON_UP), UP);
    cfgView.AddButton(&b1);
    Button b2 (GPIO_Helper::GetPin(BUTTON_DOWN), DOWN);
    cfgView.AddButton(&b2);
    cfgView.Run();

    lcd->clearScreen();
    lcd->write("Done with cfg", 0, 0);
    lcd->display();

    FlashCfg::GetInstance()->save_cfg();

    while(1)
        ;


}

void clicks_test()
{
    TFT_ILI9163C* lcd = get_lcd();

    BHandler h (TIMER::Get_Instance(BUTTON_TIMER), lcd);
    Button b1 (GPIO_Helper::GetPin("D6"), UP);
    h.AddButton(&b1);
    Button b2 (GPIO_Helper::GetPin("D7"), DOWN);
    h.AddButton(&b2);
    h.Run();

}
