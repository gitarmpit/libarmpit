#ifndef _5883_H
#define _5883_H

#include "board.h"
#include "utils.h"

bool hmc5883lDetect(sensor_t *mag);
void hmc5883lInit(sensor_align_e align);
void hmc5883lRead(int16_t *magData);


#endif
