#include "beep.h"

Buzzer* g_buzzer;

void beep_init(void) {
  g_buzzer = Buzzer::GetInstance();
  g_buzzer->Init(LPBuzzerTimer::GetInstance());
  g_buzzer->SetVolume(10);
}

void beep_alarmA(void) {
  if (g_buzzer) {
    g_buzzer->PlayTune(cuca_fast);
    g_buzzer->Stop();
  }
  // buzzer->PlayTone(2000, 100);
}

void beep_alarmB(void) {
  if (g_buzzer) {
    g_buzzer->PlayTune(barbie2);
    g_buzzer->Stop();
  }
}


void beep_success(void) {
  if (g_buzzer) {
    g_buzzer->PlayTune("success1:d=16,o=5,b=300:gl,cl,gl,cl,2g");
    g_buzzer->Stop();
  }
}


void beep_handshake_ok(void) {
  if (g_buzzer) {
    g_buzzer->PlayTune("t:d=16,o=6,b=400:cl,gl,4c7");
    g_buzzer->Stop();
  }
}

void beep_failure(void) {
  if (g_buzzer) {
    // g_buzzer->PlayTune("failure:d=8,o=5,b=200:g4l,c4l,g3l,c3l,2g2l");
    g_buzzer->PlayTune("failure:d=16,o=5,b=200:g4l,c4l,g3l,c3l,2g2l");
    g_buzzer->Stop();
  }
}

void beep(uint16_t freq, uint16_t durationMs) {
  if (g_buzzer) {
    g_buzzer->Beep(freq, durationMs);
  }
}