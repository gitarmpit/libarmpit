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

#include <platform.h>

#include "maths.h"
#include "feature.h"
#include "config.h"
#include "failsafe.h"
#include "rx.h"
#include "cfg.h"


#define MAX_INVALID_PULS_TIME    300
#define PWM_SAMPLE_COUNT 3

#define DELAY_50_HZ (1000000 / 50)
#define DELAY_100_MS (1000000 / 10)
#define DELAY_5_HZ (1000000 / 5)
#define SKIP_RC_ON_SUSPEND_PERIOD 1500000           // 1.5 second period in usec (call frequency independent)
#define SKIP_RC_SAMPLES_ON_RESUME  2                // flush 2 samples to drop wrong measurements (timing independent)


RX::RX()
{
	rxConfig = Config::GetInstance()->getRxConfig();

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
        failsafeChannelConfigs[i].mode = RX_FAILSAFE_MODE_AUTO;
        failsafeChannelConfigs[i].step = (i == THROTTLE)
            ? CHANNEL_VALUE_TO_RXFAIL_STEP(rxConfig->rx_min_usec)
            : CHANNEL_VALUE_TO_RXFAIL_STEP(rxConfig->midrc);
    }

    for (int i = 0; i < 4; i++)
    {
        rcData[i] = rxConfig->midrc;
        rcInvalidPulsPeriod[i] = millis() + MAX_INVALID_PULS_TIME;
    }

    rcData[THROTTLE] = rxConfig->rx_min_usec;

    //used for filtering
    rxRefreshRate = 20000;

    channelCount = 0;
}


void RX::setMidRc()
{
    for (int i = 0; i < 3; i++)
    {
        rcData[i] = rxConfig->midrc;
    }
}

bool RX::isPulseValid(uint16_t pulseDuration)
{
    return  pulseDuration >= rxConfig->rx_min_usec &&
            pulseDuration <= rxConfig->rx_max_usec;
}



bool RX::rxIsReceivingSignal(void)
{
    return rxSignalReceived;
}





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

uint16_t RX::getRxfailValue(uint8_t channel)
{

    rxFailsafeChannelConfig_t *failsafeChannelConfig = &failsafeChannelConfigs[channel];
    uint8_t mode = failsafeChannelConfig->mode;

    // force auto mode to prevent fly away when failsafe stage 2 is disabled
    if ((!Feature::GetInstance()->feature(FEATURE_FAILSAFE)) )
    {
        mode = RX_FAILSAFE_MODE_AUTO;
    }

    if (mode == RX_FAILSAFE_MODE_AUTO)
    {
    	return (channel == THROTTLE) ? rxConfig->rx_min_usec : rxConfig->midrc;
    }
    else
    {
    	return RXFAIL_STEP_TO_CHANNEL_VALUE(failsafeChannelConfig->step);
    }
}


void RX::detectAndApplySignalLossBehaviour(bool boxFailSafe, FailSafe* fs)
{
    int channel;
    uint16_t sample;

    uint32_t currentMilliTime = millis();


    uint8_t validFlightChannelMask = REQUIRED_CHANNEL_MASK;

    for (channel = 0; channel < channelCount; channel++)
    {
        sample = rcRaw[channel];

        bool validPulse = isPulseValid(sample);

        if (!validPulse)
        {
            if (currentMilliTime < rcInvalidPulsPeriod[channel])
            {
                sample = rcData[channel];           // hold channel for MAX_INVALID_PULS_TIME
            }
            else
            {
                sample = getRxfailValue(channel);   // after that apply rxfail value
                validFlightChannelMask &= ~(1 << channel);
            }
        }
        else
        {
            rcInvalidPulsPeriod[channel] = currentMilliTime + MAX_INVALID_PULS_TIME;
        }

        rcData[channel] = calculateAverage(channel, sample);
    }

    rxFlightChannelsValid = (validFlightChannelMask == REQUIRED_CHANNEL_MASK);

    if ((rxFlightChannelsValid) && !(boxFailSafe && Feature::GetInstance()->feature(FEATURE_FAILSAFE)))
    {
        fs->rxLinkUp();
    }
    else
    {
        fs->rxLinkDown();

        for (channel = 0; channel < channelCount; channel++)
        {
            rcData[channel] = getRxfailValue(channel);
        }
    }
}

//TODO
static bool isPWMDataBeingReceived()
{
    return true;
}

void RX::update(uint32_t currentTime, bool boxFailSafe, FailSafe* fs)
{
    if (rxSignalReceived && (int32_t)(currentTime - needRxSignalBefore) >= 0)
    {
        rxSignalReceived = false;
    }

    if (isPWMDataBeingReceived())
    {
        needRxSignalBefore = currentTime + DELAY_100_MS;
    }

    for (uint8_t channel = 0; channel < channelCount; channel++) {

        // sample the channel
        uint16_t sample = 0; //TODO read PWM here

        if (sample)
        {
            sample = scaleRange(sample, channelRanges[channel].min, channelRanges[channel].max, PWM_RANGE_MIN, PWM_RANGE_MAX);
            sample = MIN(MAX(PWM_PULSE_MIN, sample), PWM_PULSE_MAX);
        }
        rcRaw[channel] = sample;
    }

    detectAndApplySignalLossBehaviour(boxFailSafe, fs);

    rcSampleIndex++;
}




