#ifndef _TELEMETRY_H
#define _TELEMETRY_H

#include "fp2.h"

typedef struct _telemetry
{
    uint32_t timestamp_ms;
    int16_t throttle, pitch, roll;
    fix16_t y, p, r;
    fix16_t y_set, p_set, r_set;
    int16_t m[4];

}telemetry;

#endif
