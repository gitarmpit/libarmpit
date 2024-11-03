#include "beep.h"

Buzzer* g_buzzer;

void beep_init(void) {
  g_buzzer = Buzzer::GetInstance();
  g_buzzer->Init(LPBuzzerTimer::GetInstance());
  g_buzzer->SetVolume(10);
}

void beep_alarm(int8_t tuneNo) {
  const char* tune = barbie_short;
  if (tuneNo == 0) {
    tune = barbie_short;
  }
  else if (tuneNo == 1) {
    tune = cuca_fast;
  }
  else if (tuneNo == 2) {
    tune = chiz;
  }
  else if (tuneNo == 3) {
    tune = nokia;
  }
  else if (tuneNo == 4) {
    tune =  polonez2;
  }
  else if (tuneNo == 5) {
    tune =  pv_short2;
  }
  else if (tuneNo == 6) {
    tune =  inter;
  }
  else if (tuneNo == 7) {
    tune =  mars_short;
  }
  else if (tuneNo == 8) {
    tune =  rt1;
  }
  else if (tuneNo == 9) {
    tune =  rt3;
  }
  else if (tuneNo == 10) {
    tune =  rt4;  // phone
  }
  else if (tuneNo == 11) {
    tune =  rt_triple;
  }
  else if (tuneNo == 12) {
    tune =  "beep:d=4,o=5,t=500:8c,8c,8c,1g";
  }
  else if (tuneNo == 13) {
    tune =  "beep:d=4,o=6,t=500:8c,8c,8c,1g";
  }

  if (g_buzzer) {
    g_buzzer->PlayTune(tune);
  }
}

void beep_success(void) {
  if (g_buzzer) {
    g_buzzer->PlayTune("success1:d=16,o=5,b=300:gl,cl,gl,cl,2g");
  }
}


void beep_handshake_ok(void) {
  if (g_buzzer) {
    g_buzzer->PlayTune("t:d=16,o=6,b=400:cl,gl,4c7");
  }
}

void beep_failure(void) {
  if (g_buzzer) {
    g_buzzer->PlayTune("failure:d=16,o=5,b=200:g4l,c4l,g3l,c3l,2g2l");
  }
}

void beep(uint16_t freq, uint16_t durationMs) {
  if (g_buzzer) {
    g_buzzer->Beep(freq, durationMs);
  }
}