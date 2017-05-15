/*
 * This file is part of Cleanflight.
 *
 * Cleanflight is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cleanflight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cleanflight.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _ACC_H
#define _ACC_H
#include <stdint.h>
#include "sensors/sensors.h"
#include "filter.h"

#define CALIBRATING_ACC_CYCLES              400

// Type of accelerometer used/detected
typedef enum {
    ACC_DEFAULT = 0,
    ACC_NONE = 1,
    ACC_ADXL345 = 2,
    ACC_MPU6050 = 3,
    ACC_MMA8452 = 4,
    ACC_BMA280 = 5,
    ACC_LSM303DLHC = 6,
    ACC_MPU6000 = 7,
    ACC_MPU6500 = 8,
    ACC_FAKE = 9,
} accelerationSensor_e;

#define ACC_MAX  ACC_FAKE

struct accConfig_t
{
	// Set the Low Pass Filter factor for ACC.
	// Decreasing this value would reduce ACC noise but would increase ACC lag time.
    uint8_t acc_cut_hz;
    uint32_t accSamplingInterval;
    int16_t acc_1G;
};

class MPU;

class Acceleration
{
private:

	accConfig_t* acfg;
    sensor_align_e accAlign;
    biquadFilter_t accFilter[3];
    bool isFiltering;

    MPU* _mpu;

    int32_t accZero[3] = { 0, 0, 0 };
    int32_t accADC[3] = {0, 0, 0};
    uint16_t calibratingA;

    void applyAccelerationTrims();
    void performAcclerationCalibration();

public:
    Acceleration(MPU* mpu);

    int32_t* getADC() { return accADC; }
    void update();

    void startCalibration();

    void setAlignment (sensor_align_e accAlign) { this->accAlign = accAlign; }

    uint16_t GetAcc_1G() { return acfg->acc_1G; }
};
#endif


