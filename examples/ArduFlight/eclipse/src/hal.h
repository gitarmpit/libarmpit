#ifndef _HAL_H
#define _HAL_H
//#include <memory.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define isnan(x) _isnan(x)
#define millis() 0
#define micros() 0
#define micros64() 0
#define delay_us(x) 0
#define panic(msg) 
#define delay(x) 
//#define isinf(x) (!_finite(x))


#define AP_Float float 
#define AP_Int8  int8_t 
#define AP_Int16 int16_t 
#define AP_Int32 int32_t 


class AP_GPS
{

};
class AP_Airspeed
{

};

#endif
