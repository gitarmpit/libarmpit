#ifndef _CFG_H
#define _CFG_H

#include <stdint.h>
#include "flash.h"

typedef struct _cfg 
{
    float P;
    float I;
    float D;
    int16_t acc_offset[3];
    int16_t gyro_offset[3];
    int16_t mag_offset[3];
} CFG;


#endif
