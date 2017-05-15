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

#include <platform.h>


#include "feature.h"
#include "config.h"

void resetConf(void)
{
    Feature::GetInstance()->featureClearAll();
    Feature::GetInstance()->featureSet(FEATURE_FAILSAFE);
}

static void activateConfig(void)
{

#ifdef GPS
    gpsUsePIDs(&pidProfile);
#endif


    //TODO
    //recalculateMagneticDeclination();

}

static void validateAndFixConfig(void)
{

    // The retarded_arm setting is incompatible with pid_at_min_throttle because full roll causes the craft to roll over on the ground.
    // The pid_at_min_throttle implementation ignores yaw on the ground, but doesn't currently ignore roll when retarded_arm is enabled.

    //TODO
//  if (armingConfig.retarded_arm && mixerConfig.pid_at_min_throttle) {
//        mixerConfig.pid_at_min_throttle = 0;
//    }




}

void readEEPROM(void)
{
    //suspendRxSignal();

    validateAndFixConfig();
    activateConfig();

   // resumeRxSignal();
}


//TODO
static void writeEEPROM(void)
{
//    suspendRxSignal();
//    resumeRxSignal();
}


//void resetEEPROM(void)
//{
//    resetConf();
//    writeEEPROM();
//}

//used to save accel trimmings etc
void saveConfigAndNotify(void)
{
    writeEEPROM();
    readEEPROM();
    beeperConfirmationBeeps(1);
}

