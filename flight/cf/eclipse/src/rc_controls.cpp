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
#include <string.h>

#include <math.h>

#include <platform.h>

#include "maths.h"

#include "feature.h"

#include "rc_controls.h"
#include "config.h"
#include "runtime_config.h"

#include "baro.h"
#include "sensors/sensors.h"
#include "gyro.h"
#include "acc.h"
#include "compass.h"

#include "pid.h"
#include "navigation.h"
#include "failsafe.h"
#include "rx.h"
#include "fc.h"
#include "cfg.h"

#define AIRMODE_DEADBAND 12


RC_Controls::RC_Controls(FC* fc, RX* rx, FailSafe* fs)
{
    this->fc = fc;
    this->rx = rx;
    this->failSafe = fs;

    rcModeActivationMask = 0;

    rcControlsConfig = Config::GetInstance()->getRcControlsConfig();

    //TODO move to Cfg
    armingConfig.disarm_kill_switch = 1;
    armingConfig.auto_disarm_delay = 5;
    armingConfig.max_arm_angle = 25;
}




bool RC_Controls::areSticksInApModePosition(uint16_t ap_mode)
{
    return ABS(rcCommand[ROLL]) < ap_mode && ABS(rcCommand[PITCH]) < ap_mode;
}

bool RC_Controls::isThrottleLow()
{
    return (rx->getRcData(THROTTLE) < rx->minCheck());
}

bool RC_Controls::isRollPitchCentered()
{
	uint16_t mid = rx->getMidRc();
	int16_t p = rx->getRcData(PITCH);
	int16_t r = rx->getRcData(ROLL);

    return (((p < (mid + AIRMODE_DEADBAND)) && (p > (mid -AIRMODE_DEADBAND)))
            && ((r < (mid + AIRMODE_DEADBAND)) && (r > (mid -AIRMODE_DEADBAND))));

}



void RC_Controls::processRcStickPositions(Acceleration* acc, Gyro* gyro, Baro* baro, Compass* compass)
{

    static uint8_t rcDelayCommand;      // this indicates the number of time (multiple of RC measurement at 50Hz) the sticks must be maintained to run or switch off motors
    static uint8_t rcSticks;            // this hold sticks position for command combos

    uint8_t stTmp = 0;
    for (int i = 0; i < 4; i++)
    {
        stTmp >>= 2;
        if (rx->getRcData(i) > rx->minCheck())
            stTmp |= 0x80;  // check for MIN

        if (rx->getRcData(i) < rx->minCheck())
            stTmp |= 0x40;  // check for MAX
    }

    if (stTmp == rcSticks)
    {
        if (rcDelayCommand < 250)
            rcDelayCommand++;
    }
    else
    {
        rcDelayCommand = 0;
    }
    rcSticks = stTmp;

    if (rcDelayCommand != 20)
    {
        return;
    }

    //DISARMING
    // Disarm on throttle down + yaw
    if (rcSticks == THR_LO + YAW_LO + PIT_CE + ROL_CE)
    {
        if (ARMING_FLAG(ARMED))
        {
            fc->mwDisarm();
        }
        else
        {
            beeper(BEEPER_DISARM_REPEAT);    // sound tone while stick held
            rcDelayCommand = 0;              // reset so disarm tone will repeat
        }
    }

    if (ARMING_FLAG(ARMED)) {
        // actions during armed
        return;
    }

    // actions during not armed

    if (rcSticks == THR_LO + YAW_LO + PIT_LO + ROL_CE)
    {
        // GYRO calibration
        gyro->startCalibration();

#ifdef GPS
        if (Feature::GetInstance()->feature(FEATURE_GPS)) {
            GPS_reset_home_position();
        }
#endif

        baro->startCalibration();
    }


    if (rcSticks == THR_LO + YAW_LO + PIT_LO + ROL_HI) {
        saveConfigAndNotify();
    }


    if (rcSticks == THR_LO + YAW_HI + PIT_CE + ROL_CE) {
        // Arm via YAW
        fc->mwArm();
        return;
    }
    if (rcSticks == THR_HI + YAW_LO + PIT_LO + ROL_CE)
    {
        acc->startCalibration();
        //TODO
        //Config::GetInstance()->resetRollAndPitchTrims();

        return;
    }
    if (rcSticks == THR_HI + YAW_HI + PIT_LO + ROL_CE)
    {
        compass->startCalibrating();
        return;
    }


    // Accelerometer Trim

    rollAndPitchTrims_t accelerometerTrimsDelta;
    memset(&accelerometerTrimsDelta, 0, sizeof(accelerometerTrimsDelta));

    bool shouldApplyRollAndPitchTrimDelta = false;
    if (rcSticks == THR_HI + YAW_CE + PIT_HI + ROL_CE) {
        accelerometerTrimsDelta.pitch = 2;
        shouldApplyRollAndPitchTrimDelta = true;
    } else if (rcSticks == THR_HI + YAW_CE + PIT_LO + ROL_CE) {
        accelerometerTrimsDelta.pitch = -2;
        shouldApplyRollAndPitchTrimDelta = true;
    } else if (rcSticks == THR_HI + YAW_CE + PIT_CE + ROL_HI) {
        accelerometerTrimsDelta.roll = 2;
        shouldApplyRollAndPitchTrimDelta = true;
    } else if (rcSticks == THR_HI + YAW_CE + PIT_CE + ROL_LO) {
        accelerometerTrimsDelta.roll = -2;
        shouldApplyRollAndPitchTrimDelta = true;
    }
    if (shouldApplyRollAndPitchTrimDelta) {

        //TODO
        //Config::GetInstance()->applyAndSaveAccelerometerTrimsDelta(&accelerometerTrimsDelta);
        rcDelayCommand = 0; // allow autorepetition
        return;
    }

#ifdef DISPLAY
    if (rcSticks == THR_LO + YAW_CE + PIT_HI + ROL_LO) {
        displayDisablePageCycling();
    }

    if (rcSticks == THR_LO + YAW_CE + PIT_HI + ROL_HI) {
        displayEnablePageCycling();
    }
#endif

}



bool RC_Controls::rcModeIsActive(boxId_e modeId)
{
    return rcModeActivationMask & (1 << modeId);
}


int32_t RC_Controls::getRcStickDeflection(int32_t axis)
{
    return MIN(ABS(rx->getRcData(axis) - rx->getMidRc()), 500);
}

int32_t RC_Controls::getMostDeflectedPos()
{
    const int32_t stickPosRoll = getRcStickDeflection(FD_ROLL);
    const int32_t stickPosPitch = getRcStickDeflection(FD_PITCH);

    // 0 at center stick, 500 at max stick deflection:
   return MAX(ABS(stickPosRoll), ABS(stickPosPitch));
}


