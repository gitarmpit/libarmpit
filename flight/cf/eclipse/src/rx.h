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
#include "rc_mode_act.h"

#define PWM_RANGE_MIN 1000
#define PWM_RANGE_MAX 2000
#define PWM_PULSE_MIN   750       // minimum PWM pulse width which is considered valid
#define PWM_PULSE_MAX   2250      // maximum PWM pulse width which is considered valid
#define CHANNEL_VALUE_TO_RXFAIL_STEP(channelValue) ((constrain(channelValue, PWM_PULSE_MIN, PWM_PULSE_MAX) - PWM_PULSE_MIN) / 25)
#define RXFAIL_STEP_TO_CHANNEL_VALUE(step) (PWM_PULSE_MIN + 25 * step)


typedef enum {
    RX_FAILSAFE_MODE_AUTO = 0,
    RX_FAILSAFE_MODE_SET,
} rxFailsafeChannelMode_e;


typedef enum {
    RX_FAILSAFE_TYPE_FLIGHT = 0,
    RX_FAILSAFE_TYPE_AUX,
} rxFailsafeChannelType_e;


typedef struct rxFailsafeChannelConfiguration_s {
    uint8_t mode; // See rxFailsafeChannelMode_e
    uint8_t step;
} rxFailsafeChannelConfig_t;

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


#define REQUIRED_CHANNEL_MASK 0x0F // first 4 channels


class FailSafe;

class RX
{
private:
    rxConfig_t* rxConfig;
    rxFailsafeChannelConfig_t failsafeChannelConfigs[4];
    rxChannelRangeConfiguration_t channelRanges[4];

    int16_t rcRaw[4];     // interval [1000;2000]
    int16_t rcData[4];     // interval [1000;2000]
    uint32_t rcInvalidPulsPeriod[4];


    bool rxSignalReceived;
    bool rxFlightChannelsValid;
    bool rxIsInFailsafeMode;

    uint32_t needRxSignalBefore;

    uint8_t rcSampleIndex;

    uint16_t rxRefreshRate;



    uint8_t channelCount;


    bool isPulseValid(uint16_t pulseDuration);
    void detectAndApplySignalLossBehaviour(bool boxFailSafe, FailSafe* fs);
    uint16_t getRxfailValue(uint8_t channel);
    uint16_t calculateAverage(uint8_t chan, uint16_t sample);
    void resetAllRxChannelRangeConfigurations(rxChannelRangeConfiguration_t *rxChannelRangeConfiguration);


public:
    RX ();


    int16_t getRcData(uint8_t axis) { return rcData[axis]; }
    void setRcData(uint8_t axis, int16_t val) { rcData[axis] = val; }

    void setMidRc();
    bool rxIsReceivingSignal(void);

    int16_t minCheck() { return rxConfig->mincheck; }
    bool rcSmoothing() { return rxConfig->rcSmoothing; }
    uint16_t getMidRc() { return rxConfig->midrc; }

    void update(uint32_t currentTime, bool boxFailSafe, FailSafe* fs);
    int16_t getRxRefreshRate() { return rxRefreshRate; }



};


#endif

