#include "_pins.h"
#include "9163/TFT_ILI9163C.h"
#include "pwm_sound.h"
#include "buzzer.h"
#include "debug.h"



void buzzer()
{
    Buzzer*  buzzer = Buzzer::GetInstance();
    buzzer->Init(BUZZER_TIMER, BUZZER_PIN, BUZZER_TIMER_CHANNEL);

    //buzzer->SetVolume(2);
    //Buzzer::GetInstance()->PlayTune("t:d=8,o=6,b=300:c,e,c,e,c,4g.,e,g,e,g,e,4c.");
    Buzzer::GetInstance()->PlayTune(mars_short);
    //buzzer.PlayTune("t:d=16,o=7,b=400:c,g,c,g,c,g,c,g,8p,c,g,c,g,c,g,c,g");
    //    buzzer.PlayTune("t:d=16,o=6,b=300:c,g,c,g,c,g,c,g,8p");
//    buzzer.PlayTune("t:d=16,o=6,b=300:c,g,c,g,c,g,c,g,8p");

    //buzzer.PlayTuneAsync(nokia, TIMER::Get_Instance(TIM6));

    //const char *t = "r1:d=8,o=7,b=160:c,eb,g,eb,4c,4p,c,eb,g,eb,4g,4p";
    //buzzer.PlayTune(t);
    //delay(1000);
    //buzzer->PlayTuneAsync(t, TIMER::Get_Instance(TIM6), true);
    //buzzer.PlayTune(inter);

    while(1);
}
