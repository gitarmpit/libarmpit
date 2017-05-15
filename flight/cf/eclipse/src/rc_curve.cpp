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
#include <stddef.h>

#include <platform.h>

#include "rc_curve.h"
#include "cfg.h"


Rc_Curve::Rc_Curve()
{
    rate_cfg = Config::GetInstance()->getControlRateConfig();
    motor_cfg = Config::GetInstance()->getMotorConfig();
    generatePitchRollCurve();
    generateYawCurve();
    generateThrottleCurve();
}





void Rc_Curve::generatePitchRollCurve(void)
{
    for (int i = 0; i < PITCH_LOOKUP_LENGTH; i++)
    {
        lookupPitchRollRC[i] = (2500 + rate_cfg->rcExpo8 * (i * i - 25)) * i * (int32_t) rate_cfg->rcRate8 / 2500;
    }
}

void Rc_Curve::generateYawCurve(void)
{
     for (int i = 0; i < YAW_LOOKUP_LENGTH; i++)
     {
        lookupYawRC[i] = (2500 + rate_cfg->rcYawExpo8 * (i * i - 25)) * i / 25;
    }
}

void Rc_Curve::generateThrottleCurve(void)
{
    for (int i = 0; i < THROTTLE_LOOKUP_LENGTH; i++)
    {
        const int16_t tmp = 10 * i - rate_cfg->thrMid8;
        uint8_t y = 1;
        if (tmp > 0)
            y = 100 - rate_cfg->thrMid8;
        if (tmp < 0)
            y = rate_cfg->thrMid8;
        lookupThrottleRC[i] =
                10 * rate_cfg->thrMid8 + tmp * (100 - rate_cfg->thrExpo8 + (int32_t) rate_cfg->thrExpo8 * (tmp * tmp) / (y * y)) / 10;
        lookupThrottleRC[i] = motor_cfg->minthrottle + (int32_t) (motor_cfg->maxthrottle - motor_cfg->minthrottle) * lookupThrottleRC[i] / 1000; // [MINTHROTTLE;MAXTHROTTLE]
    }
}

int16_t Rc_Curve::rcLookupPitchRoll(int tmp)
{
    const int tmp2 = tmp / 100;
    return lookupPitchRollRC[tmp2] + (tmp - tmp2 * 100) * (lookupPitchRollRC[tmp2 + 1] - lookupPitchRollRC[tmp2]) / 100;
}

int16_t Rc_Curve::rcLookupYaw(int tmp)
{
    const int tmp2 = tmp / 100;
    return lookupYawRC[tmp2] + (tmp - tmp2 * 100) * (lookupYawRC[tmp2 + 1] - lookupYawRC[tmp2]) / 100;
}

int16_t Rc_Curve::rcLookupThrottle(int tmp)
{
    const int tmp2 = tmp / 100;
    return lookupThrottleRC[tmp2] + (tmp - tmp2 * 100) * (lookupThrottleRC[tmp2 + 1] - lookupThrottleRC[tmp2]) / 100;
}

