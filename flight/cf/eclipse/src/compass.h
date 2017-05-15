#ifndef _D_COMPASS_H
#define _D_COMPASS_H


// Get your magnetic decliniation from here : http://magnetic-declination.com/
// For example, -6deg 37min, = -637 Japan, format is [sign]dddmm (degreesminutes) default is zero.

#define MAG_DECL -444  
#include <stdint.h>
#include "sensors/sensors.h"

class I2C;
class Compass_drv;

class Compass 
{
private:
    I2C* _i2c;
    Compass_drv* _drv;

    float _magDecl;
    int32_t magADC[3];
    int32_t magZero[3] = { 0, 0, 0 };

    sensor_align_e magAlign;

    bool _startCalibrating;

    void init();

protected:
    float magGain[3] = { 1.0f, 1.0f, 1.0f };


public: 
    Compass(I2C* i2c);
    bool detect();


    //called from cleanflight_fc  tasks
    void updateCompass(uint32_t currentTime_us);

    int32_t* getMagADC() { return magADC; }

    float getMagneticDeclination() { return _magDecl; }

    void startCalibrating() { _startCalibrating = true; }

};


#endif
