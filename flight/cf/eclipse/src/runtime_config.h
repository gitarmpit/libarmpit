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

// FIXME some of these are flight modes, some of these are general status indicators
typedef enum {
    OK_TO_ARM       = (1 << 0),
    PREVENT_ARMING  = (1 << 1),
    ARMED           = (1 << 2)
} armingFlag_e;

extern uint8_t armingFlags;

#define DISABLE_ARMING_FLAG(mask) (armingFlags &= ~(mask))
#define ENABLE_ARMING_FLAG(mask) (armingFlags |= (mask))
#define ARMING_FLAG(mask) (armingFlags & (mask))


////TODO for now set modes directly, to get rid of BOXes
//typedef enum
//{
////    ANGLE_MODE      = (1 << 0),  //BOXANGLE
////    HORIZON_MODE    = (1 << 1),  //BOXHORIZON
//    //MAG_MODE        = (1 << 2),  //BOXMAG
//    //BARO_MODE       = (1 << 3),  //BOXBARO      //alt hold
//    //GPS_HOME_MODE   = (1 << 4),  //BOXGPSHOME
//    //GPS_HOLD_MODE   = (1 << 5),  //BOXGPSHOLD
//    //HEADFREE_MODE   = (1 << 6),  //BOXHEADFREE
//    //UNUSED_MODE     = (1 << 7), // old autotune
//    //PASSTHRU_MODE   = (1 << 8),  //BOXPASSTHROUGH
//    //SONAR_MODE      = (1 << 9),  //BOXSONAR        //alt hold
//    //FAILSAFE_MODE   = (1 << 10), //BOXFAILSAFE    //handled inside failsave class
//    //GTUNE_MODE      = (1 << 11), //commented out
//} flightModeFlags_e;
//
//extern uint16_t flightModeFlags;

//#define DISABLE_FLIGHT_MODE(mask) disableFlightMode(mask)
//#define ENABLE_FLIGHT_MODE(mask) enableFlightMode(mask)
//#define FLIGHT_MODE(mask) (flightModeFlags & (mask))


//uint16_t enableFlightMode(flightModeFlags_e mask);
//uint16_t disableFlightMode(flightModeFlags_e mask);

bool sensors(uint32_t mask);
void sensorsSet(uint32_t mask);
void sensorsClear(uint32_t mask);
uint32_t sensorsMask(void);

