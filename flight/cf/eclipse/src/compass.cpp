#include "compass.h"
#include "runtime_config.h"
#include "i2c_cpp.h"
#include "sensors/compass_ak8963.h"

Compass::Compass(I2C* i2c)
{
    _i2c = i2c;
    magAlign = ALIGN_DEFAULT;
    _magDecl = 0;
    _drv = 0;
    _startCalibrating = false;
}

bool Compass::detect() 
{
    _drv = new Ak8963(_i2c);
    bool rc = _drv->detect();
    if (rc)
    {
        init();
    }
    return rc;
}


void Compass::init()
{
    int16_t deg, min;

    // calculate magnetic declination
    deg = MAG_DECL / 100;
    min = MAG_DECL % 100;

    _magDecl = (deg + ((float)min * (1.0f / 60.0f))) * 10; // heading is in 0.1deg units

    _drv->init();

}



void Compass::updateCompass(uint32_t currentTime_us) //TODO us vs ms
{
    static uint32_t tCal = 0;
    static int16_t magZeroTempMin[3];
    static int16_t magZeroTempMax[3];
    static int16_t magADCRaw[3];

    _drv->read(magADCRaw);

    for (int axis = 0; axis < 3; axis++)
    {
        magADC[axis] = magADCRaw[axis];  // int32_t copy to work with
    }

    alignSensors(magADC, magADC, magAlign);

    if (_startCalibrating)
    {
        tCal = currentTime_us;
        for (int axis = 0; axis < 3; axis++)
        {
            magZero[axis] = 0;
            magZeroTempMin[axis] = magADC[axis];
            magZeroTempMax[axis] = magADC[axis];
        }
        _startCalibrating = false;
    }

    magADC[0] -= magZero[0];
    magADC[1] -= magZero[1];
    magADC[2] -= magZero[2];

    if (tCal != 0)
    {
        if ((currentTime_us - tCal) < 30000000)
        {    // 30s: you have 30s to turn the multi in all directions
//            LED0_TOGGLE;
            for (int axis = 0; axis < 3; axis++)
            {
                if (magADC[axis] < magZeroTempMin[axis])
                    magZeroTempMin[axis] = magADC[axis];
                if (magADC[axis] > magZeroTempMax[axis])
                    magZeroTempMax[axis] = magADC[axis];
            }
        }
        else
        {
            tCal = 0;
            for (int axis = 0; axis < 3; axis++)
            {
                magZero[axis] = (magZeroTempMin[axis] + magZeroTempMax[axis]) / 2; // Calculate offsets
            }

            //TODO
            //saveConfigAndNotify();
        }
    }
}


