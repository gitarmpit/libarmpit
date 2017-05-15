#ifndef _COMPASS_DRV_H
#define _COMPASS_DRV_H

#include "i2c_cpp.h"

class Compass_drv 
{
protected:
    I2C* _i2c;
    float magGain[3] = { 1.0f, 1.0f, 1.0f };

public:
    Compass_drv(I2C* i2c) { _i2c = i2c; }

   virtual bool detect() = 0;
   virtual void init() = 0;
   virtual bool read(int16_t *magData) = 0;

};


#endif
