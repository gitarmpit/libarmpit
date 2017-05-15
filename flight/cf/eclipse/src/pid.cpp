#include "pid.h"
#include "maths.h"

#define US_FROM_HZ(hz) (1000000 / (hz))

Pid::Pid(pidConfig_t* pidConfig, int16_t gyro_sample_hz, bool airMode, uint8_t* controlRates)
{
	pcfg = pidConfig;
    const uint16_t pidPeriodUs = US_FROM_HZ(gyro_sample_hz);
    targetPidLooptime = pidPeriodUs * pcfg->pid_process_denom;
    dT = (float)targetPidLooptime * 0.000001f;
    rates = controlRates;

    //TODO
    //rollPitchTrims = Config::GetInstance()->getRollPitchTrims();
    _airMode = airMode;
    pidInitFilters();

}




void Pid::pidInitFilters()
{
    int axis;

    if (pcfg->dterm_notch_hz) {
        for (axis = 0; axis < 3; axis++) {
            biquadFilterInitNotch(&dtermFilterNotch[axis], targetPidLooptime, pcfg->dterm_notch_hz, pcfg->dterm_notch_cutoff);
        }
    }

    if (pcfg->dterm_filter_type == FILTER_BIQUAD && pcfg->dterm_lpf_hz) {
        for (axis = 0; axis < 3; axis++) {
            biquadFilterInitLPF(&dtermFilterLpf[axis], pcfg->dterm_lpf_hz, targetPidLooptime);
        }
    }
}

void Pid::pidResetITerm(void)
{
    for (int axis = 0; axis < 3; axis++) {
        lastITerm[axis] = 0;
        lastITermf[axis] = 0.0f;
    }
}


// calculates strength of horizon leveling; 0 = none, 100 = most leveling
int Pid::calcHorizonLevelStrength(int32_t mostDeflectedPos, int32_t imuRoll, int32_t imuPitch)
{

    // start with 100 at center stick, 0 at max stick deflection:
    int horizonLevelStrength = (500 - mostDeflectedPos) / 5;

    // 0 at level, 900 at vertical, 1800 at inverted (degrees * 10)
    const int currentInclination = MAX(ABS(imuRoll), ABS(imuPitch));

    // horizonTiltMode:  SAFE = leveling always active when sticks centered,
    // EXPERT = leveling can be totally off when inverted
    if (pcfg->horizon_tilt_mode == HORIZON_TILT_MODE_EXPERT)
    {
        if (pcfg->horizon_tilt_effect < 175) {
            // pidProfile->horizon_tilt_effect 0 to 125 => 2700 to 900
            //  (represents where leveling goes to zero):
            const int cutoffDeciDegrees = (175-pcfg->horizon_tilt_effect) * 18;
            // inclinationLevelRatio (0 to 100) is smaller (less leveling)
            //  for larger inclinations; 0 at cutoffDeciDegrees value:
            const int inclinationLevelRatio = constrain(
                              (((cutoffDeciDegrees-currentInclination)*10) /
                                           (cutoffDeciDegrees/10)), 0, 100);
            // apply configured horizon sensitivity:
            if (pcfg->pidLevel_D <= 0) {       // zero means no leveling
                horizonLevelStrength = 0;
            } else {
                // when stick is near center (horizonLevelStrength ~= 100)
                //  H_sensitivity value has little effect,
                // when stick is deflected (horizonLevelStrength near 0)
                //  H_sensitivity value has more effect:
                horizonLevelStrength = (horizonLevelStrength - 100) *
                        100 / pcfg->pidLevel_D + 100;
            }
            // apply inclination ratio, which may lower leveling
            //  to zero regardless of stick position:
            horizonLevelStrength = horizonLevelStrength * inclinationLevelRatio / 100;
        }
        else
          horizonLevelStrength = 0;
    } else {  // horizon_tilt_mode = SAFE (leveling always active when sticks centered)
        int sensitFact;
        if (pcfg->horizon_tilt_effect > 0) {
            // ratio of 100 to 0 (larger means more leveling):
            const int factorRatio = 100 - pcfg->horizon_tilt_effect;
            // inclinationLevelRatio (0 to 100) is smaller (less leveling)
            //  for larger inclinations, goes to 100 at inclination level:
            int inclinationLevelRatio =
                  ((1800-currentInclination)/18 * (100-factorRatio)) / 100 + factorRatio;
            // apply ratio to configured horizon sensitivity:
            sensitFact = pcfg->pidLevel_D * inclinationLevelRatio / 100;
        }
        else   // pidProfile->horizon_tilt_effect=0 for "old" functionality
            sensitFact = pcfg->pidLevel_D;

        if (sensitFact <= 0) {           // zero means no leveling
            horizonLevelStrength = 0;
        } else {
            // when stick is near center (horizonLevelStrength ~= 100)
            //  sensitFact value has little effect,
            // when stick is deflected (horizonLevelStrength near 0)
            //  sensitFact value has more effect:
            horizonLevelStrength =  (horizonLevelStrength - 100) * 100 / sensitFact + 100;
        }
    }
    return constrain(horizonLevelStrength, 0, 100);
}
