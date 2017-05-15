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

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <platform.h>

#include "compass.h"
#include "rx.h"
#include "rc_controls.h"
#include "sensors/sensors.h"
#include "sensors/sonar.h"
#include "sensors/sonar_hcsr04.h"
#include "acc.h"
#include "gyro.h"
#include "baro.h"
//
//
#include "imu.h"
#include "mixer.h"
#include "failsafe.h"

#include "runtime_config.h"
#include "config.h"
#include "feature.h"
//
//
#include "scheduler.h"

#include "RCC_helper.h"
#include "GPIO_Helper.h"
#include "sensors/_pins.h"
#include "sensors/mpu6500.h"
#include "cfg.h"

void init(void)
{
    systick_enable(true);
    readEEPROM();
    delay(100);


    Config* cfg = Config::GetInstance();
    bool airMode = false;

    //TODO
    //pwmRxInit();
    Mixer mixer (cfg->getMotorConfig(), airMode);

    uint32_t i2cClockSpeed = 100000;
    I2C* i2c = GPIO_Helper::SetupI2C(MPU6050_I2C, i2cClockSpeed);
    //i2c->SetErrorCallback (I2C_Error, i2c);

    MPU mpu (i2c);
    mpu.Detect();

    Gyro gyro (&mpu);
    gyro.init();
    Acceleration accel (&mpu);

    Compass compass (i2c);
    compass.detect();

    //I2C* i2c_baro = GPIO_Helper::SetupI2C(BARO_I2C, i2cClockSpeed);
    Baro baro;
    if (baro.init())
    {
    }

    FailSafe* failSafe;
    RC_Controls* rc_controls;
    RX* rx;


#ifdef GPS
    if (Feature::GetInstance()->feature(FEATURE_GPS)) {
        gpsInit();
        navigationInit(&pidProfile);
    }
#endif

#ifdef SONAR
    sonarInit();
#endif


    //TODO compass calibration
    //gyro.startCalibration();
    //baro.startCalibration();

    //imu.enableSmallAngle(true);



    DISABLE_ARMING_FLAG(PREVENT_ARMING);
}


static void run(void)
{
    Scheduler* sched = Scheduler::GetInstance();
    sched->addTask(TASK_SYSTEM);
    sched->addTask(TASK_GYRO);
    sched->addTask(TASK_PID);
    sched->addTask(TASK_ATTITUDE);


    sched->addTask(TASK_ACCEL);

#ifdef BEEPER
    sched->addTask(TASK_BEEPER);
#endif

    //TODO
    //sched->addTask(TASK_BATTERY, Feature::GetInstance()->feature(FEATURE_VBAT) || Feature::GetInstance()->feature(FEATURE_CURRENT_METER));
    sched->addTask(TASK_RX);
#ifdef GPS
    if (Feature::GetInstance()->feature(FEATURE_GPS))
        sched->addTask(TASK_GPS);
#endif

    if (sensors(SENSOR_MAG))
        sched->addTask(TASK_COMPASS);

#ifdef BARO
    if (sensors(SENSOR_BARO))
        sched->addTask(TASK_BARO);
#endif

#ifdef SONAR
    if (sensors(SENSOR_SONAR))
        sched->addTask(TASK_SONAR);
#endif
#if defined(BARO) || defined(SONAR)

    if (sensors(SENSOR_BARO) || sensors(SENSOR_SONAR))
        sched->addTask(TASK_ALTITUDE);
#endif
#ifdef DISPLAY
    if (Feature::GetInstance()->feature(FEATURE_DISPLAY))
        sched->addTask(TASK_DISPLAY);
#endif

    while (true)
    {
        sched->run();
    }

}

int main(void)
{
    RCC_EnableHSE_168Mhz();
    RCC_EnableHSI(false);

    init();
    run();
}

