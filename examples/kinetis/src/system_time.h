#ifndef SYSTEM_TIME_H
#define SYSTEM_TIME_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "common.h"
void systick_enable(BOOL enable);
void systick_timer_start(void);
uint32_t systick_timer_stop(void);
uint32_t systick_timediff_us(void);

uint32_t micros(void);

#define millis() micros()/1000


#ifdef __cplusplus
 }
 #endif


#endif
