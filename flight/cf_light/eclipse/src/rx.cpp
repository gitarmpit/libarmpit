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


#include "rx.h"

#define MAX_INVALID_PULS_TIME    300
#define PWM_SAMPLE_COUNT 3

#define DELAY_50_HZ (1000000 / 50)
#define DELAY_100_MS (1000000 / 10)
#define DELAY_5_HZ (1000000 / 5)
//#define SKIP_RC_ON_SUSPEND_PERIOD 1500000           // 1.5 second period in usec (call frequency independent)
//#define SKIP_RC_SAMPLES_ON_RESUME  2                // flush 2 samples to drop wrong measurements (timing independent)


RX::RX(rxConfig_t* rx_cfg, rcControlsConfig_t* rc_cfg, uint16_t minthrottle,  uint16_t maxthrottle)
{
	rxConfig = rx_cfg;
	this->cfg = rc_cfg;

    rxSignalReceived = false;
    rxFlightChannelsValid = false;
    rxIsInFailsafeMode = true;
    needRxSignalBefore = 0;
    rcSampleIndex = 0;

    for (int i = 0; i < 4; i++)
    {
        channelRanges[i].min = PWM_RANGE_MIN;
        channelRanges[i].max = PWM_RANGE_MAX;
    }

    for (int i = 0; i < 4; i++)
    {
        rcData[i] = rxConfig->midrc;
    }

    rcData[THROTTLE] = rxConfig->rx_min_usec;

    //used for filtering
    rxRefreshRate = 20000;

    ///////////////////////////////////////
    //TODO move to Cfg
//    armingConfig.disarm_kill_switch = 1;  //not need, disarming logic
//    armingConfig.auto_disarm_delay = 5; //not needed, only for auto disarm for don't sping when armed feautre
//    armingConfig.max_arm_angle = 25;  //prevent arming if angle is this high

    for (int i = 0; i < PITCH_LOOKUP_LENGTH; i++)
    {
        lookupPitchRollRC[i] = (2500 + cfg->pitchRollExpo * (i * i - 25)) * i * (int32_t) cfg->rcRate / 2500;
    }

    for (int i = 0; i < YAW_LOOKUP_LENGTH; i++)
    {
       lookupYawRC[i] = (2500 + cfg->yawExpo * (i * i - 25)) * i / 25;
   }

    for (int i = 0; i < THROTTLE_LOOKUP_LENGTH; i++)
    {
        const int16_t tmp = 10 * i - cfg->thrMid;
        uint8_t y = 1;
        if (tmp > 0)
            y = 100 - cfg->thrMid;
        if (tmp < 0)
            y = cfg->thrMid;
        lookupThrottleRC[i] =
                10 * cfg->thrMid + tmp * (100 - cfg->thrExpo + (int32_t)cfg->thrExpo * (tmp * tmp) / (y * y)) / 10;
        lookupThrottleRC[i] = minthrottle + (int32_t) (maxthrottle - minthrottle) * lookupThrottleRC[i] / 1000; // [MINTHROTTLE;MAXTHROTTLE]
    }



}

//failSafe
//void RX::setMidRc()
//{
//    for (int i = 0; i < 3; i++)
//    {
//        rcData[i] = rxConfig->midrc;
//    }
//}

//bool RX::isPulseValid(uint16_t pulseDuration)
//{
//    return  pulseDuration >= rxConfig->rx_min_usec &&
//            pulseDuration <= rxConfig->rx_max_usec;
//}



//bool RX::rxIsReceivingSignal(void)
//{
//    return rxSignalReceived;
//}



uint16_t RX::calculateAverage(uint8_t chan, uint16_t sample)
{
    static uint16_t rcSamples[4][PWM_SAMPLE_COUNT];
    static bool rxSamplesCollected = false;

    uint8_t currentSampleIndex = rcSampleIndex % PWM_SAMPLE_COUNT;

    // update the recent samples and compute the average of them
    rcSamples[chan][currentSampleIndex] = sample;

    // avoid returning an incorrect average which would otherwise occur before enough samples
    if (!rxSamplesCollected)
    {
        if (rcSampleIndex < PWM_SAMPLE_COUNT)
        {
            return sample;
        }
        rxSamplesCollected = true;
    }

    uint16_t rcDataMean = 0;
    uint8_t sampleIndex;
    for (sampleIndex = 0; sampleIndex < PWM_SAMPLE_COUNT; sampleIndex++)
        rcDataMean += rcSamples[chan][sampleIndex];

    return rcDataMean / PWM_SAMPLE_COUNT;
}




//TODO
static bool isPWMDataBeingReceived()
{
    return true;
}

void RX::updateRx(uint32_t currentTime)
{
    if (rxSignalReceived && (int32_t)(currentTime - needRxSignalBefore) >= 0)
    {
        rxSignalReceived = false;
    }

    if (isPWMDataBeingReceived())
    {
        needRxSignalBefore = currentTime + DELAY_100_MS;
    }

    for (uint8_t channel = 0; channel < 4; channel++) {

        // sample the channel
        uint16_t sample = 0; //TODO read PWM here

        if (sample)
        {
            sample = scaleRange(sample, channelRanges[channel].min, channelRanges[channel].max, PWM_RANGE_MIN, PWM_RANGE_MAX);
            sample = MIN(MAX(PWM_PULSE_MIN, sample), PWM_PULSE_MAX);
        }

        //no need without failsafe
        //rcRaw[channel] = sample;

        //copied from detectAndApply below
        rcData[channel] = calculateAverage(channel, sample);

    }

    //TODO detectAndApply... from old rx
//    uint16_t sample;
//
//    for (uint8_t channel = 0; channel < 4; channel++)
//    {
//        sample = rcRaw[channel];
//
//        bool validPulse = isPulseValid(sample);
//
//        if (!validPulse)
//        {
//            if (currentMilliTime < rcInvalidPulsPeriod[channel])
//            {
//                sample = rcData[channel];           // hold channel for MAX_INVALID_PULS_TIME
//            }
//            else
//            {
//                sample = getRxfailValue(channel);   // after that apply rxfail value
//                validFlightChannelMask &= ~(1 << channel);
//            }
//        }
//        else
//        {
//            rcInvalidPulsPeriod[channel] = currentMilliTime + MAX_INVALID_PULS_TIME;
//        }
//
//        rcData[channel] = calculateAverage(channel, sample);
//    }


    rcSampleIndex++;
}

void RX::updateRcCommands(uint32_t currentTime, uint8_t* PIDweight)
{
	updateRx(currentTime);

    int32_t prop2;
    int16_t throttle_raw = getRcData(THROTTLE);

    // PITCH & ROLL only dynamic PID adjustment,  depending on throttle value
    if (throttle_raw < cfg->tpa_breakpoint)
    {
        prop2 = 100;
    }
    else
    {
        if (throttle_raw < 2000)
        {
            prop2 = 100 - (uint16_t)cfg->dynThrPID * (throttle_raw - cfg->tpa_breakpoint) / (2000 - cfg->tpa_breakpoint);
        }
        else
        {
            prop2 = 100 - cfg->dynThrPID;
        }
    }

    //ROLL, PITCH, YAW
    for (int axis = 0; axis < 3; axis++)
    {
        int32_t prop1;
        int32_t tmp = getRcStickDeflection(axis);
        if (axis == ROLL || axis == PITCH)
        {
            if (cfg->deadband)
            {
                if (tmp > cfg->deadband)
                {
                    tmp -= cfg->deadband;
                } else {
                    tmp = 0;
                }
            }

            rcCommand[axis] = rcLookupPitchRoll(tmp);

            prop1 = 100 - (uint16_t)cfg->rates[axis] * tmp / 500;
            prop1 = (uint16_t)prop1 * prop2 / 100;
            // non coupled PID reduction scaler used in PID controller 1 and PID controller 2. 100 means 100% of the pids
            PIDweight[axis] = prop2;
        }
        else //YAW
        {
            if (cfg->yaw_deadband)
            {
                if (tmp > cfg->yaw_deadband)
                {
                    tmp -= cfg->yaw_deadband;
                } else
                {
                    tmp = 0;
                }
            }

            rcCommand[axis] = rcLookupYaw(tmp) * -cfg->yaw_control_direction;
            prop1 = 100 - (uint16_t)cfg->rates[axis] * ABS(tmp) / 500;
            // YAW TPA disabled.
            PIDweight[axis] = 100;
        }

#ifdef USE_PID_MW23
        // FIXME axis indexes into pids.  use something like lookupPidIndex(rc_alias_e alias) to reduce coupling.
        dynP8[axis] = (uint16_t)pidProfile()->P[axis] * prop1 / 100;
        dynI8[axis] = (uint16_t)pidProfile()->I8[axis] * prop1 / 100;
        dynD8[axis] = (uint16_t)pidProfile()->D8[axis] * prop1 / 100;
#endif

        if (getRcData(axis) < rxConfig->midrc)
        {
            rcCommand[axis] = -rcCommand[axis];
        }

    }  //EOF  axis loop


    //THROTTLE

    int32_t tmp = constrain(throttle_raw, rxConfig->mincheck, PWM_RANGE_MAX);
    tmp = (uint32_t)(tmp - rxConfig->mincheck) * PWM_RANGE_MIN / (PWM_RANGE_MAX - rxConfig->mincheck);       // [MINCHECK;2000] -> [0;1000]
    rcCommand[THROTTLE] = rcLookupThrottle(tmp);
}


//TODO this is smth related to GPS
//bool RC::areSticksInApModePosition(uint16_t ap_mode)
//{
//    return ABS(rcCommand[ROLL]) < ap_mode && ABS(rcCommand[PITCH]) < ap_mode;
//}



#define AIRMODE_DEADBAND 12
//anti windup logic
bool RX::isRollPitchCentered()
{
	uint16_t mid = rxConfig->midrc;
	int16_t p = getRcData(PITCH);
	int16_t r = getRcData(ROLL);

    return (((p < (mid + AIRMODE_DEADBAND)) && (p > (mid -AIRMODE_DEADBAND)))
            && ((r < (mid + AIRMODE_DEADBAND)) && (r > (mid -AIRMODE_DEADBAND))));

}

int32_t RX::getRcStickDeflection(int32_t axis)
{
    return MIN(ABS(getRcData(axis) - rxConfig->midrc), 500);
}


//horizon strengh calc for pid
int32_t RX::getMostDeflectedPos()
{
    const int32_t stickPosRoll = getRcStickDeflection(FD_ROLL);
    const int32_t stickPosPitch = getRcStickDeflection(FD_PITCH);

    // 0 at center stick, 500 at max stick deflection:
   return MAX(ABS(stickPosRoll), ABS(stickPosPitch));
}


int16_t RX::rcLookupPitchRoll(int tmp)
{
    const int tmp2 = tmp / 100;
    return lookupPitchRollRC[tmp2] + (tmp - tmp2 * 100) * (lookupPitchRollRC[tmp2 + 1] - lookupPitchRollRC[tmp2]) / 100;
}

int16_t RX::rcLookupYaw(int tmp)
{
    const int tmp2 = tmp / 100;
    return lookupYawRC[tmp2] + (tmp - tmp2 * 100) * (lookupYawRC[tmp2 + 1] - lookupYawRC[tmp2]) / 100;
}

int16_t RX::rcLookupThrottle(int tmp)
{
    const int tmp2 = tmp / 100;
    return lookupThrottleRC[tmp2] + (tmp - tmp2 * 100) * (lookupThrottleRC[tmp2 + 1] - lookupThrottleRC[tmp2]) / 100;
}

void RX::headFreeMode(int16_t yaw, int16_t headFreeModeHold)
{
    float radDiff = degreesToRadians(DECIDEGREES_TO_DEGREES(yaw) - headFreeModeHold);
    float cosDiff = cos_approx(radDiff);
    float sinDiff = sin_approx(radDiff);
    int16_t p = rcCommand[PITCH];
    int16_t r = rcCommand[ROLL];
    rcCommand[ROLL] = r * cosDiff - p * sinDiff;
    rcCommand[PITCH] = p * cosDiff + r * sinDiff;
}





int8_t RX::getRcSticksPos()
{
    static uint8_t rcSticks;            // this hold sticks position for command combos

    // this indicates the number of time (multiple of RC measurement at 50Hz)
    //the sticks must be maintained to run or switch off motors
    static uint8_t rcDelayCommand;

    uint8_t stTmp = 0;
    for (int i = 0; i < 4; i++)
    {
        stTmp >>= 2;
        if (getRcData(i) > rxConfig->mincheck)
            stTmp |= 0x80;  // check for MIN

        if (getRcData(i) < rxConfig->mincheck)
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
        rcSticks = 0;
    }

    return rcSticks;

}



/*
void RC::processRcStickPositions(Acceleration* acc, Gyro* gyro, Baro* baro, Compass* compass)
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

    /////////////////////////////////////////////////////////////////////////

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


}
*/

