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

#ifndef _RATE_PROFILE_H
#define _RATE_PROFILE_H
#include <stdint.h>
#include "mixer.h"

#define PITCH_LOOKUP_LENGTH 7
#define YAW_LOOKUP_LENGTH 7
#define THROTTLE_LOOKUP_LENGTH 12

//TODO move where it is used
struct controlRateConfig_t
{
    uint8_t rcRate8;
    uint8_t rcExpo8;
    uint8_t thrMid8;
    uint8_t thrExpo8;
    uint8_t rates[3];
    uint8_t dynThrPID;
    uint8_t rcYawExpo8;
    uint16_t tpa_breakpoint;                // Breakpoint at which TPA is activated
};


class Config;

class Rc_Curve
{
private:

    controlRateConfig_t* rate_cfg;
    motorConfig_t*  motor_cfg;

    int16_t lookupPitchRollRC[PITCH_LOOKUP_LENGTH];      // lookup table for expo & RC rate PITCH+ROLL
    int16_t lookupYawRC[YAW_LOOKUP_LENGTH];              // lookup table for expo & RC rate YAW
    int16_t lookupThrottleRC[THROTTLE_LOOKUP_LENGTH];    // lookup table for expo & mid THROTTLE


public:
    Rc_Curve();


    int16_t rcLookupPitchRoll(int rcVal);
    int16_t rcLookupYaw(int rcYaw);
    int16_t rcLookupThrottle(int rcThrottle);

    void generatePitchRollCurve(void);
    void generateYawCurve(void);
    void generateThrottleCurve(void);


};

#endif

