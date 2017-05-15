#ifndef _BARO_DRV_H
#define _BARO_DRV_H

#include "i2c_cpp.h"

class Baro_drv 
{
protected:
    I2C* _i2c;

public:
    Baro_drv(I2C* i2c) { _i2c = i2c; }

    virtual bool detect() = 0;
    virtual void read(int32_t *pressure, int32_t *temperature) = 0;

};


#endif
