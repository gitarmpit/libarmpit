#include "_pins.h"
#include "9163/TFT_ILI9163C.h"
#include "buzzer.h"
#include "systick.h"
#include "bitmaps/symb_16.cpp"
#include "bitmaps/ardu_jboss_16.cpp"
#include "bitmaps/menu5_16.cpp"
#include "_func.h"
#include "rtc.h"
#include "debug.h"


static void next_boot(TFT_ILI9163C* lcd, const char* msg, int ms)
{
    static int y = 0;
    lcd->write (msg, 0, y++);
    lcd->display();
    delay(ms);
}



void boot()
{
    TFT_ILI9163C* lcd = get_lcd();

    Buzzer* buzzer = Buzzer::GetInstance();
    buzzer->Init(BUZZER_TIMER, BUZZER_PIN, BUZZER_TIMER_CHANNEL);
    buzzer->SetVolume(2);
    buzzer->PlayTone(1750, 400);
    lcd->setBgColor(0);
    lcd->setTextSize(1);
    //lcd->set
    lcd->setTextColor(WHITE);
    next_boot (lcd, "Booting Symbian OS v6.0", 1000);// Series 80 v1.0   ARM920T");
    next_boot (lcd, "ELKS Kernel 457K", 700);
    next_boot (lcd,  "ELKS Sockets", 200);
    next_boot (lcd,  "ELKS Serial driver v0.01", 600);
    //ELKS version 0.1.0-pre4
    next_boot (lcd,  "VFS: Mounting root", 500);
    next_boot (lcd,  "Loading init", 500);
    next_boot (lcd,  "Initializing Java ME", 2000);
    next_boot (lcd,  "Mika VM 1.4.7-RC2", 500);
    next_boot (lcd,  "Starting X server...", 3000);

    delay(2000);

    lcd->setTextSize(2);
    lcd->setBgColor(WHITE);
    lcd->setTextColor(BLACK);
    lcd->fillScreen(WHITE);
    lcd->drawBmp(0, 0, symb_16);
    lcd->display();

    //const char* nokia = "nokia:d=8,o=5,b=220:e6,d6,4f#,4g#,c#6,b,4d,4e,b,a,4c#,4e,1a";
    buzzer->PlayTuneAsync(nokia, TIM6::GetInstance());
    delay(2000);
    lcd->clearScreen();
    lcd->display();

    delay(2000);

    lcd->fillScreen(WHITE);
    lcd->drawBmp(0, 0, ardu_jboss_16);
    lcd->display();
    delay(2000);

    //const char* mars_short = "mars:d=8,o=5,b=200:16d,d.,16d,4g,4g,4a,4a,4d6.,b,g.";
    buzzer->PlayTune(mars_short);

    //lcd->drawBmp(0, 0, menu5_16);


}
