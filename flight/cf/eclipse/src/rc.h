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

#ifndef _RX_CONTROLS_H
#define _RX_CONTROLS_H

#include "rx.h"
#include "rc_mode_act.h"

//https://github.com/cleanflight/cleanflight/blob/master/docs/Modes.md



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


typedef struct armingConfig_s {
    // Arming configuration
    uint8_t retarded_arm;                   // allow disarm/arm on throttle down + roll left/right
    uint8_t disarm_kill_switch;             // allow disarm via AUX switch regardless of throttle value
    uint8_t auto_disarm_delay;              // allow automatically disarming multicopters after auto_disarm_delay seconds of zero throttle. Disabled when 0
    uint8_t max_arm_angle;                  // specifies the maximum angle allow arming at.
} armingConfig_t;

#define PITCH_LOOKUP_LENGTH 7
#define YAW_LOOKUP_LENGTH 7
#define THROTTLE_LOOKUP_LENGTH 12

class FC;
class RX;
class FailSafe;
class Acceleration;
class Gyro;
class Baro;
class Compass;


class RC
{
private:
    armingConfig_t armingConfig;
    rcControlsConfig_t* cfg;
    //modeActivationProfile_t modeActivationProfile;
    int16_t rcCommand[4];

    int16_t lookupPitchRollRC[PITCH_LOOKUP_LENGTH];     // lookup table for expo & RC rate PITCH+ROLL
    int16_t lookupYawRC[YAW_LOOKUP_LENGTH];              // lookup table for expo & RC rate YAW
    int16_t lookupThrottleRC[THROTTLE_LOOKUP_LENGTH];    // lookup table for expo & mid THROTTLE

    FC* fc;
    RX* rx;
    FailSafe* failSafe;

    //TODO enable/disable via stick??
    uint32_t rcModeActivationMask; // one bit per mode defined in boxId_e

    bool areSticksInApModePosition(uint16_t ap_mode);

    int16_t rcLookupPitchRoll(int rcVal);
    int16_t rcLookupYaw(int rcYaw);
    int16_t rcLookupThrottle(int rcThrottle);

public:
    RC(FC* fc, RX* rx, FailSafe* fs, uint16_t minthrottle,  uint16_t maxthrottle);
    bool rcModeIsActive(boxId_e modeId);
    bool isThrottleLow();

    int16_t* getRcCommand() { return rcCommand; }
    int16_t getRcCommand(uint8_t ch) { return rcCommand[ch]; }
    void setRcCommand(uint8_t ch, int16_t val) { rcCommand[ch] = val; }

    void updateRcCommands(uint8_t* PIDweight);

    //rcControlsConfig_t* getRcControlsConfig() { return cfg; }
    int32_t getRcStickDeflection(int32_t axis);
    int32_t getMostDeflectedPos();
    //TODO set some useful defaults there
    void useRcControlsConfig();
    bool isRollPitchCentered();
    armingConfig_t* getArmingConfig() { return &armingConfig; }
    void processRcStickPositions(Acceleration* acc, Gyro* gyro, Baro* baro, Compass* compass);
    int8_t get_yaw_control_direction() { return cfg->yaw_control_direction; }


    void headFreeMode(int16_t heading, int16_t headFreeModeHold);

};

#endif

