#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include <platform.h>

#include "maths.h"

#include "sensors/sensors.h"

#include "sensors/mpu6500.h"
#include "gyro.h"
#include "cfg.h"

Gyro::Gyro(MPU* mpu)
{
    _mpu = mpu;

    gyroAlign = ALIGN_DEFAULT;
    calibratingG = 0;

    gyroConfig = Config::GetInstance()->getGyroConfig();

}


void Gyro::init(void)
{
    _mpu->Init(&gyroConfig->gyro_sample_hz, gyroConfig->gyro_lpf);

    if (gyroConfig->gyro_soft_lpf_hz) {  // Initialisation needs to happen once sampling rate is known
        const uint16_t gyroPeriodUs = 1000000 / gyroConfig->gyro_sample_hz;
        for (int axis = 0; axis < 3; axis++) {
            biquadFilterInitNotch(&gyroFilterNotch[axis], gyroPeriodUs, gyroConfig->gyro_soft_notch_hz, gyroConfig->gyro_soft_notch_cutoff_hz);
            if (gyroConfig->gyro_soft_type == FILTER_BIQUAD) {
                biquadFilterInitLPF(&gyroFilterLPF[axis], gyroConfig->gyro_soft_lpf_hz,  gyroPeriodUs);
            } else {
                const float gyroDt = (float)gyroPeriodUs * 0.000001f;
                pt1FilterInit(&gyroFilterPt1[axis], gyroConfig->gyro_soft_lpf_hz, gyroDt);
            }
        }
    }
}

void Gyro::startCalibration()
{
    calibratingG = CALIBRATING_GYRO_CYCLES;
}



void Gyro::performCalibration()
{
    static int32_t sum[3];

    for (int8_t axis = 0; axis < 3; axis++)
    {
        if (calibratingG == CALIBRATING_GYRO_CYCLES) 
        {
            sum[axis] = 0;
            gyroZero[axis] = 0;
        }

        sum[axis] += gyroADC[axis];

        if (calibratingG == 1)
        {
            gyroZero[axis] = (sum[axis] + (CALIBRATING_GYRO_CYCLES / 2)) / CALIBRATING_GYRO_CYCLES;
            beeper(BEEPER_GYRO_CALIBRATED);
        }
    }

    calibratingG--;
}

void Gyro::update()
{
    if (!_mpu->GyroRead(gyroADC))
    {
        return;
    }

    alignSensors(gyroADC, gyroADC, gyroAlign);

    if (calibratingG !=0)
	{
        performCalibration();
    	gyroADC[0] = gyroADC[1] = gyroADC[2] = 0;
    }
    else
    {
    	for (int axis = 0; axis < 3; axis++)
    	{
    		gyroADC[axis] -= gyroZero[axis];
    	}
        applyFilter();
    }

}



void Gyro::applyFilter()
{
    if (gyroConfig->gyro_soft_lpf_hz)
    {
         for (int axis = 0; axis < 3; axis++)
         {
             if (gyroConfig->gyro_soft_type == FILTER_BIQUAD)
                 gyroADCf[axis] = biquadFilterApply(&gyroFilterLPF[axis], (float) gyroADC[axis]);
             else
                 gyroADCf[axis] = pt1FilterApply(&gyroFilterPt1[axis], (float) gyroADC[axis]);

             if (gyroConfig->gyro_soft_notch_hz)
                 gyroADCf[axis] = biquadFilterApply(&gyroFilterNotch[axis], gyroADCf[axis]);

             gyroADC[axis] = lrintf(gyroADCf[axis]);
         }
     } 
     else
     {
         for (int axis = 0; axis < 3; axis++) 
         {
             gyroADCf[axis] = gyroADC[axis];
         }
     }
}



