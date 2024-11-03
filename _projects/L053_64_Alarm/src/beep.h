#ifndef _BEEP_H
#define _BEEP_H


#include <stdint.h>
#include "buzzer.h"

extern Buzzer* g_buzzer;

void beep_init(void);
void beep_alarm(int8_t tuneNo);

void beep_handshake_ok(void);
void beep_success(void);
void beep_failure(void);

void beep (uint16_t freq, uint16_t durationMs);

#endif