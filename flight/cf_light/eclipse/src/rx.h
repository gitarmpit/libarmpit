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
 
#ifndef _RX_H
#define _RX_H

#include <stdint.h>
#include "maths.h"
#include "cf_common.h"

#define PWM_RANGE_MIN 1000
#define PWM_RANGE_MAX 2000
#define PWM_PULSE_MIN   750       // minimum PWM pulse width which is considered valid
#define PWM_PULSE_MAX   2250      // maximum PWM pulse width which is considered valid
#define CHANNEL_VALUE_TO_RXFAIL_STEP(channelValue) ((constrain(channelValue, PWM_PULSE_MIN, PWM_PULSE_MAX) - PWM_PULSE_MIN) / 25)
#define RXFAIL_STEP_TO_CHANNEL_VALUE(step) (PWM_PULSE_MIN + 25 * step)




struct rxChannelRangeConfiguration_t
{
    uint16_t min;
    uint16_t max;
};

struct rxConfig_t
{
    bool     rcSmoothing;                    // Enable/Disable RC filtering
    uint16_t midrc;                         // Some radios have not a neutral point centered on 1500. can be changed here
    uint16_t mincheck;                      // minimum rc end
    uint16_t maxcheck;                      // maximum rc end
    uint16_t rx_min_usec;
    uint16_t rx_max_usec;
} ;


struct rcControlsConfig_t
{
    uint8_t  deadband;                       // introduce a deadband around the stick center for pitch and roll axis. Must be greater than zero.
    uint8_t  yaw_deadband;                   // introduce a deadband around the stick center for yaw axis. Must be greater than zero.
    int8_t   yaw_control_direction;           // change control direction of yaw (inverted, normal)
    uint16_t tpa_breakpoint;                // Breakpoint at which TPA is activated

    uint8_t  rcRate;
    uint8_t  pitchRollExpo;
    uint8_t  yawExpo;
    uint8_t  thrExpo;
    uint8_t  thrMid;
    uint8_t  dynThrPID;
    uint8_t  rates[3];                      //rpy rate for pid and rc , 20/20/20 in cf, zeros in bf

} ;




#define REQUIRED_CHANNEL_MASK 0x0F // first 4 channels


#define PITCH_LOOKUP_LENGTH 7
#define YAW_LOOKUP_LENGTH 7
#define THROTTLE_LOOKUP_LENGTH 12


/////////////////////////////////////////////////
#define ROL_LO (1 << (2 * ROLL))
#define ROL_CE (3 << (2 * ROLL))
#define ROL_HI (2 << (2 * ROLL))
#define PIT_LO (1 << (2 * PITCH))
#define PIT_CE (3 << (2 * PITCH))
#define PIT_HI (2 << (2 * PITCH))
#define YAW_LO (1 << (2 * YAW))
#define YAW_CE (3 << (2 * YAW))
#define YAW_HI (2 << (2 * YAW))
#define THR_LO (1 << (2 * THROTTLE))
#define THR_CE (3 << (2 * THROTTLE))
#define THR_HI (2 << (2 * THROTTLE))

#define CHANNEL_RANGE_MIN 900
#define CHANNEL_RANGE_MAX 2100

//#define MODE_STEP_TO_CHANNEL_VALUE(step) (CHANNEL_RANGE_MIN + 25 * (step))
#define CHANNEL_VALUE_TO_STEP(channelValue) ((constrain((channelValue), CHANNEL_RANGE_MIN, CHANNEL_RANGE_MAX) - CHANNEL_RANGE_MIN) / 25)

#define MIN_MODE_RANGE_STEP 0
#define MAX_MODE_RANGE_STEP ((CHANNEL_RANGE_MAX - CHANNEL_RANGE_MIN) / 25)

// Roll/pitch rates are a proportion used for mixing, so it tops out at 1.0:
#define CONTROL_RATE_CONFIG_ROLL_PITCH_RATE_MAX  100

/* Meaningful yaw rates are effectively unbounded because they are treated as a rotation rate multiplier: */
#define CONTROL_RATE_CONFIG_YAW_RATE_MAX         255
#define CONTROL_RATE_CONFIG_TPA_MAX              100

class RX
{
private:
    rxConfig_t* rxConfig;
    rxChannelRangeConfiguration_t channelRanges[4];

    //int16_t rcRaw[4];     // interval [1000;2000]
    int16_t rcData[4];     // interval [1000;2000]
    //uint32_t rcInvalidPulsPeriod[4];

    bool rxSignalReceived;
    bool rxFlightChannelsValid;
    bool rxIsInFailsafeMode;
    uint32_t needRxSignalBefore;
    uint8_t rcSampleIndex;
    uint16_t rxRefreshRate;

    //armingConfig_t armingConfig;
    rcControlsConfig_t* cfg;
    int16_t rcCommand[4];

    int16_t lookupPitchRollRC[PITCH_LOOKUP_LENGTH];     // lookup table for expo & RC rate PITCH+ROLL
    int16_t lookupYawRC[YAW_LOOKUP_LENGTH];              // lookup table for expo & RC rate YAW
    int16_t lookupThrottleRC[THROTTLE_LOOKUP_LENGTH];    // lookup table for expo & mid THROTTLE

    //bool isPulseValid(uint16_t pulseDuration);
    uint16_t calculateAverage(uint8_t chan, uint16_t sample);

    int16_t rcLookupPitchRoll(int rcVal);
    int16_t rcLookupYaw(int rcYaw);
    int16_t rcLookupThrottle(int rcThrottle);

    void detectAndApplySignalLossBehaviour();
    int32_t getRcStickDeflection(int32_t axis);

    //renamed from update
    void updateRx(uint32_t currentTime);

public:
    RX (rxConfig_t* rx_cfg, rcControlsConfig_t* rc_cfg, uint16_t minthrottle,  uint16_t maxthrottle);

    //called from taskRc, calls updateRx internally first
    void updateRcCommands(uint32_t currentTime, uint8_t* PIDweight);

    //alt hold
    int16_t getRcData(uint8_t axis) { return rcData[axis]; }
    //alt hold
    void setRcCommand(uint8_t ch, int16_t val) { rcCommand[ch] = val; }
    //mixer, alt hold, mag hold, pid etc
    int16_t getRcCommand(uint8_t ch) { return rcCommand[ch]; }

    //for rc filtering in fc.cpp
    //int16_t getRxRefreshRate() { return rxRefreshRate; }

    //anti-windup in taskRc
    bool isRollPitchCentered();

    /////////////////////
    //anti-windup, mixer
    bool isThrottleLow() { return (getRcData(THROTTLE) < rxConfig->mincheck); }

    //for horizon level strength in PID
    int32_t getMostDeflectedPos();

    //void processRcStickPositions(Acceleration* acc, Gyro* gyro, Baro* baro, Compass* compass);

    //maghold in fc
    int8_t get_yaw_control_direction() { return cfg->yaw_control_direction; }

    //called from taskRc
    void headFreeMode(int16_t heading, int16_t headFreeModeHold);

    //new method factored from processRcStickPos
    int8_t getRcSticksPos();

    bool isDisarming     (uint8_t stickMask) { return (stickMask == THR_LO + YAW_LO + PIT_CE + ROL_CE); }
    bool isArming        (uint8_t stickMask) { return (stickMask == THR_LO + YAW_HI + PIT_CE + ROL_CE); }
    bool isGyroBaroCalib (uint8_t stickMask) { return (stickMask == THR_LO + YAW_LO + PIT_LO + ROL_CE); }
    bool isAccelCalib    (uint8_t stickMask) { return (stickMask == THR_HI + YAW_LO + PIT_LO + ROL_CE); }
    bool isCompassCalib  (uint8_t stickMask) { return (stickMask == THR_HI + YAW_HI + PIT_LO + ROL_CE); }
    bool isSaveCfg       (uint8_t stickMask) { return (stickMask == THR_LO + YAW_LO + PIT_LO + ROL_HI); }


};


#endif

