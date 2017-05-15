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


#include "rx.h"

#include "rc_controls.h"
#include "runtime_config.h"
#include "config.h"

#include "failsafe.h"
#include "fc.h"
#include "rx.h"
#include "cfg.h"

/*
 * Usage:
 *
 * failsafeInit() and useFailsafeConfig() must be called before the other methods are used.
 *
 * failsafeInit() and useFailsafeConfig() can be called in any order.
 * failsafeInit() should only be called once.
 *
 * enable() should be called after system initialisation.
 */

FailSafe::FailSafe(FC* fc, RX* rx, RC_Controls* rc_conrols )
{
    this->fc = fc;
    this->rx = rx;
    this->rc_controls = rc_conrols;


    failsafeConfig = Config::GetInstance()->getFailsafeConfig();

    events = 0;
    monitoring = false;
    rxDataFailurePeriod = PERIOD_RXDATA_FAILURE + failsafeConfig->failsafe_delay * MILLIS_PER_TENTH_SECOND;
    validRxDataReceivedAt = 0;
    validRxDataFailedAt = 0;
    throttleLowPeriod = 0;
    landingShouldBeFinishedAt = 0;
    receivingRxDataPeriod = 0;
    receivingRxDataPeriodPreset = 0;
    phase = FAILSAFE_IDLE;
    rxLinkState = FAILSAFE_RXLINK_DOWN;
    active = false;
}


bool FailSafe::isMonitoring(void)
{
    return monitoring;
}

bool FailSafe::isActive(void)
{
    return active;
}

void FailSafe::startMonitoring(void)
{
    monitoring = true;
}

 bool FailSafe::failsafeShouldHaveCausedLandingByNow(void)
{
    return (millis() > landingShouldBeFinishedAt);
}

void FailSafe::failsafeActivate(void)
{
    active = true;
    phase = FAILSAFE_LANDING;
    landingShouldBeFinishedAt = millis() + failsafeConfig->failsafe_off_delay * MILLIS_PER_TENTH_SECOND;

    events++;
}

void FailSafe::failsafeApplyControlInput(void)
{
    rx->setMidRc();
    rx->setRcData(THROTTLE, failsafeConfig->failsafe_throttle);
}


void FailSafe::onRxSuspend(uint32_t usSuspendPeriod)
{
    validRxDataReceivedAt += (usSuspendPeriod / 1000);    // / 1000 to convert micros to millis
}

void FailSafe::onRxResume(void)
{
    validRxDataReceivedAt = millis();                     // prevent RX link down trigger, restart rx link up
    rxLinkState = FAILSAFE_RXLINK_UP;                     // do so while rx link is up
}

void FailSafe::rxLinkUp(void)
{
    validRxDataReceivedAt = millis();
    if ((validRxDataReceivedAt - validRxDataFailedAt) > PERIOD_RXDATA_RECOVERY) {
        rxLinkState = FAILSAFE_RXLINK_UP;
    }
}

void FailSafe::rxLinkDown(void)
{
    validRxDataFailedAt = millis();
    if ((validRxDataFailedAt - validRxDataReceivedAt) > rxDataFailurePeriod) {
        rxLinkState = FAILSAFE_RXLINK_DOWN;
    }
}

void FailSafe::updateState(void)
{
    if (!isMonitoring()) {
        return;
    }

    bool receivingRxData = (rxLinkState == FAILSAFE_RXLINK_UP);
    bool armed = ARMING_FLAG(ARMED);
    bool failsafeSwitchIsOn = rc_controls->rcModeIsActive(BOXFAILSAFE);

    bool reprocessState;

    do {
        reprocessState = false;

        switch (phase) {
            case FAILSAFE_IDLE:
                if (armed) {
                    // Track throttle command below minimum time
                    if (!rc_controls->isThrottleLow()) {
                        throttleLowPeriod = millis() + failsafeConfig->failsafe_throttle_low_delay * MILLIS_PER_TENTH_SECOND;
                    }
                    // Kill switch logic (must be independent of receivingRxData to skip PERIOD_RXDATA_FAILURE delay before disarming)
                    if (failsafeSwitchIsOn && failsafeConfig->failsafe_kill_switch) {
                        // KillswitchEvent: failsafe switch is configured as KILL switch and is switched ON
                        failsafeActivate();
                        phase = FAILSAFE_LANDED;      // skip auto-landing procedure
                        receivingRxDataPeriodPreset = PERIOD_OF_1_SECONDS;    // require 1 seconds of valid rxData
                        reprocessState = true;
                    } else if (!receivingRxData) {
                        if (millis() > throttleLowPeriod) {
                            // JustDisarm: throttle was LOW for at least 'failsafe_throttle_low_delay' seconds
                            failsafeActivate();
                            phase = FAILSAFE_LANDED;      // skip auto-landing procedure
                            receivingRxDataPeriodPreset = PERIOD_OF_3_SECONDS; // require 3 seconds of valid rxData
                        } else {
                            phase = FAILSAFE_RX_LOSS_DETECTED;
                        }
                        reprocessState = true;
                    }
                } else {
                    throttleLowPeriod = 0;
                }
                break;

            case FAILSAFE_RX_LOSS_DETECTED:
                if (receivingRxData) {
                    phase = FAILSAFE_RX_LOSS_RECOVERED;
                } else {
                    switch (failsafeConfig->failsafe_procedure) {
                        default:
                        case FAILSAFE_PROCEDURE_AUTO_LANDING:
                            // Stabilize, and set Throttle to specified level
                            failsafeActivate();
                            break;

                        case FAILSAFE_PROCEDURE_DROP_IT:
                            // Drop the craft
                            failsafeActivate();
                            phase = FAILSAFE_LANDED;      // skip auto-landing procedure
                            receivingRxDataPeriodPreset = PERIOD_OF_3_SECONDS; // require 3 seconds of valid rxData
                            break;
                    }
                }
                reprocessState = true;
                break;

            case FAILSAFE_LANDING:
                if (receivingRxData) {
                    phase = FAILSAFE_RX_LOSS_RECOVERED;
                    reprocessState = true;
                }
                if (armed)
                {
                    failsafeApplyControlInput();
                }
                if (failsafeShouldHaveCausedLandingByNow() || !armed) {
                    receivingRxDataPeriodPreset = PERIOD_OF_30_SECONDS; // require 30 seconds of valid rxData
                    phase = FAILSAFE_LANDED;
                    reprocessState = true;
                }
                break;

            case FAILSAFE_LANDED:
                ENABLE_ARMING_FLAG(PREVENT_ARMING); // To prevent accidently rearming by an intermittent rx link
                fc->mwDisarm();
                receivingRxDataPeriod = millis() + receivingRxDataPeriodPreset; // set required period of valid rxData
                phase = FAILSAFE_RX_LOSS_MONITORING;
                reprocessState = true;
                break;

            case FAILSAFE_RX_LOSS_MONITORING:
                // Monitoring the rx link to allow rearming when it has become good for > `receivingRxDataPeriodPreset` time.
                if (receivingRxData) {
                    if (millis() > receivingRxDataPeriod)
                    {
                        // rx link is good now, when arming via ARM switch, it must be OFF first
                        DISABLE_ARMING_FLAG(PREVENT_ARMING);
                        phase = FAILSAFE_RX_LOSS_RECOVERED;
                        reprocessState = true;
                    }
                } else
                {
                    receivingRxDataPeriod = millis() + receivingRxDataPeriodPreset;
                }
                break;

            case FAILSAFE_RX_LOSS_RECOVERED:
                // Entering IDLE with the requirement that throttle first must be at min_check for failsafe_throttle_low_delay period.
                // This is to prevent that JustDisarm is activated on the next iteration.
                // Because that would have the effect of shutting down failsafe handling on intermittent connections.
                throttleLowPeriod = millis() + failsafeConfig->failsafe_throttle_low_delay * MILLIS_PER_TENTH_SECOND;
                phase = FAILSAFE_IDLE;
                active = false;
                reprocessState = true;
                break;

            default:
                break;
        }
    } while (reprocessState);

}
