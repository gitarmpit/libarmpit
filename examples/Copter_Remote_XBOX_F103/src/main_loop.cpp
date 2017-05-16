#include "main_loop.h"
#include "button_ctrl.h"
#include "debug.h"

void MainLoop::Click()
{
    ++_clicks;
    _sound->UpdatePWMDuty(500);
    delay(20);
    _sound->UpdatePWMDuty(0);
}

void MainLoop::Click2()
{
    _sound->UpdatePWMDuty(477);
    delay(100);
    _sound->UpdatePWMDuty(379);
    delay(100);
    _sound->UpdatePWMDuty(319);
    delay(100);
    _sound->UpdatePWMDuty(0);
    delay(200);

}

void MainLoop::Intro()
{
//      _sound->SetupPWM1(3800, 3500);
//      _sound->SetupPWM1(3000, 2700);
//      _sound->SetupPWM1(2550, 2100);
    delay(200);

//      _display->WriteLine(4, 0,  "Team");
//      _display->WriteLine(0, 1,  "Rote Gefahr");
//      _display->WriteLine(0, 3,  "Hit any key");

//_sound->SetupPWM1(7600, 5500);
    _sound->SetupPWM1(3800, 3000);
    delay(200);
    //_sound->SetupPWM1(6000, 5700);
    _sound->SetupPWM1(3000, 2000);
    delay(200);
    //_sound->SetupPWM1(5100, 4100);
    _sound->SetupPWM1(2550, 2000);
    delay(600);

    _sound->SetupPWM1(2550, 0);

}

MainLoop::MainLoop(Si4432* radio, Adafruit_Nokia5110* display, TIMER_Channel* lcdBL,
        TIMER_Channel* sound, TIMER_Channel* redLED, TIMER_Channel* greenLED) :
ButtonHandler (TIM4::GetInstance(), false)
{
    _radio = radio;
    _display = display;
    _clicks = 0;
    _lcdBL = lcdBL;
    _sound = sound;
    _redLED = redLED;
    _greenLED = greenLED;
    _blDuty = 0;

    _backLight.SetBackLightChannel(lcdBL);

    _adc_reader.SetHandler(this);
    Button_Ctrl bctr(this);
    bctr.Init();

}

void MainLoop::Run()
{
    _adc_reader.Start();
    Intro();

//      uint8_t data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
//
//      while(1)
//      {
//          while (!radio.sendPacket(sizeof data, data))
//              ;
//
//          delay(2000);
//      }

//Intro();
    _display->write("test");


    while (1)
    {
        _display->printf(0, 0, 2, "Thr: %5lu", _adc_reader.getThrottle());
        //_display->Clear();
        _display->display();
        delay(20);
    }
}

void MainLoop::OnStateChange(Button* button)
{
    UNUSED(button);
}

void MainLoop::OnClick(Button* button)
{
    UNUSED(button);
//    if (button->GetId() == START_BUTTON)
//    {
//    }
//    else if (button->GetId() == BACK_BUTTON)
//    {
//    }
//    else if (button->GetId() == DPAD_UP_BUTTON)
//    {
//    }
//    else if (button->GetId() == DPAD_DOWN_BUTTON)
//    {
//    }
//    else if (button->GetId() == DPAD_LEFT_BUTTON)
//    {
//    }
//    else if (button->GetId() == DPAD_RIGHT_BUTTON)
//    {
//    }
//    else if (button->GetId() == LSTICK_BUTTON)
//    {
//    }
//    else if (button->GetId() == RSTICK_BUTTON)
//    {
//    }

    Click();
}


void MainLoop::Analog_Clicked(ANALOG_BUTTON button)
{
    if (button == ANALOG_BUTTON::B)
    {
        _backLight.Enable();
    }
    Click();
}





//      uint32_t flash_addr = FLASH_BASE + 0x10000 - 100;
//
//      flash_addr = 0x0801FC00 + 8;
//      volatile uint8_t d = *(uint8_t*)flash_addr;
//
//      FLASH_Unlock();
//      FLASH_SetPsize_x16();
//      FLASH_WriteHalfWord(flash_addr, (uint16_t) 136);
