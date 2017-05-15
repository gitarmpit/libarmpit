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

#pragma once


// Keep synchronized with featureNames[] in serial_cli.c
typedef enum {
    FEATURE_VBAT = 1 << 1,
    //FEATURE_INFLIGHT_ACC_CAL = 1 << 2,
    //FEATURE_MOTOR_STOP = 1 << 4,
    FEATURE_GPS = 1 << 7,
    FEATURE_FAILSAFE = 1 << 8,
    //FEATURE_SONAR = 1 << 9,
    //FEATURE_TELEMETRY = 1 << 10,
    //FEATURE_CURRENT_METER = 1 << 11,
    //FEATURE_RSSI_ADC = 1 << 15,
    //FEATURE_DISPLAY = 1 << 17,
} features_e;

//void handleOneshotFeatureChangeOnRestart(void);


//TODO
//void writeEEPROM();
//void resetEEPROM(void);

void initEEPROM(void);

void readEEPROM(void);
//void ensureEEPROMContainsValidData(void);
void saveConfigAndNotify(void);

//void changeProfile(uint8_t profileIndex);

//uint8_t getCurrentControlRateProfile(void);
//void changeControlRateProfile(uint8_t profileIndex);
