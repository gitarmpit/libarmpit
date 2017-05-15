#include "maths.h"
#include "filter.h"
#include "runtime_config.h"
#include "pid.h"

#include <platform.h>

int16_t Pid::mwRewriteCore(int axis,
                                    int32_t gyroRate,
                                    int32_t angleRate,
                                    bool antiWindupOn,
                                    bool motorSaturated)
{
    static int32_t lastRateForDelta[3];

    const int32_t rateError = angleRate - gyroRate;

    // -----calculate P component
    int32_t PTerm = (rateError * pcfg->P[axis] * PIDweight[axis] / 100) >> 7;
    // Constrain YAW by yaw_p_limit value if not servo driven, in that case servolimits apply
    if (axis == YAW) {
        if (pcfg->yaw_lpf_hz) {
            PTerm = pt1FilterApply4(&yawFilter, PTerm, pcfg->yaw_lpf_hz, dT);
        }
        if (pcfg->yaw_p_limit) {
            PTerm = constrain(PTerm, -pcfg->yaw_p_limit, pcfg->yaw_p_limit);
        }
    }

    // -----calculate I component
    // There should be no division before accumulating the error to integrator, because the precision would be reduced.
    // Precision is critical, as I prevents from long-time drift. Thus, 32 bits integrator (Q19.13 format) is used.
    // Time correction (to avoid different I scaling for different builds based on average cycle time)
    // is normalized to cycle time = 2048 (2^11).
    int32_t ITerm = lastITerm[axis] + ((rateError * (uint16_t)targetPidLooptime) >> 11) * pcfg->I[axis];
    // limit maximum integrator value to prevent WindUp - accumulating extreme values when system is saturated.
    // I coefficient (I8) moved before integration to make limiting independent from PID settings
    ITerm = constrain(ITerm, (int32_t)(-PID_MAX_I << 13), (int32_t)(PID_MAX_I << 13));
    // Anti windup protection
    if (_airMode)
    {
        if (antiWindupOn || motorSaturated)
        {
            ITerm = constrain(ITerm, -ITermLimit[axis], ITermLimit[axis]);
        } else {
            ITermLimit[axis] = ABS(ITerm);
        }
    }
    lastITerm[axis] = ITerm;
    ITerm = ITerm >> 13; // take integer part of Q19.13 value

    // -----calculate D component
    int32_t DTerm;
    if (pcfg->D[axis] == 0) {
        // optimisation for when D8 is zero, often used by YAW axis
        DTerm = 0;
    } else {
        int32_t delta;
        if (pcfg->deltaMethod == PID_DELTA_FROM_MEASUREMENT) {
            delta = -(gyroRate - lastRateForDelta[axis]);
            lastRateForDelta[axis] = gyroRate;
        } else {
            delta = rateError - lastRateForDelta[axis];
            lastRateForDelta[axis] = rateError;
        }
        // Divide delta by targetLooptime to get differential (ie dr/dt)
        delta = (delta * ((uint16_t)0xFFFF / ((uint16_t)targetPidLooptime >> 4))) >> 5;
        if (pcfg->dterm_lpf_hz) {
            // DTerm delta low pass filter
            delta = lrintf(pt1FilterApply4(&deltaFilter[axis], (float)delta, pcfg->dterm_lpf_hz, dT));
        }
        DTerm = (delta * pcfg->D[axis] * PIDweight[axis] / 100) >> 8;
        DTerm = constrain(DTerm, -PID_MAX_D, PID_MAX_D);
    }

    // -----calculate total PID output
    return PTerm + ITerm + DTerm;
}

int16_t* Pid::mwRewrite(int16_t* rcCommand,
        int32_t* gyroADC,
        int16_t* attitude,
        bool antiWindupOn,
        bool motorSaturated,
        int32_t horizonLevelStrength,
		StabilizationMode mode)
{


    // ----------PID controller----------
    for (int axis = 0; axis < 3; axis++) {
        const uint8_t rate = rates[axis];

        // -----Get the desired angle rate depending on flight mode
        int32_t angleRate;
        if (axis == FD_YAW) {
            // YAW is always gyro-controlled (MAG correction is applied to rcCommand)
            angleRate = (((int32_t)(rate + 27) * rcCommand[YAW]) >> 5);
        } else {
            // control is GYRO based for ACRO and HORIZON - direct sticks control is applied to rate PID
            angleRate = ((int32_t)(rate + 27) * rcCommand[axis]) >> 4;
            if (mode == ANGLE_MODE_ || mode == HORIZON_MODE_)
            {
                // calculate error angle and limit the angle to the max inclination
                // multiplication of rcCommand corresponds to changing the sticks scaling here
#ifdef GPS
                const int32_t errorAngle = constrain(2 * rcCommand[axis] + GPS_angle[axis], -((int)max_angle_inclination), max_angle_inclination)
                        - att[axis] + angleTrim->raw[axis];
#else
                const int32_t errorAngle = constrain(2 * rcCommand[axis], -((int)pcfg->max_angle_inclination), pcfg->max_angle_inclination)
                        - attitude[axis] + (axis == FD_PITCH) ? rollPitchTrims.pitch : rollPitchTrims.roll;
#endif
                if (mode == ANGLE_MODE_)
                {
                    // ANGLE mode
                    angleRate = (errorAngle * pcfg->pidLevel_P) >> 4;
                }
                else
                {
                    // HORIZON mode
                    // mix in errorAngle to desired angleRate to add a little auto-level feel.
                    // horizonLevelStrength has been scaled to the stick input
                    angleRate += (errorAngle * pcfg->pidLevel_I * horizonLevelStrength / 100) >> 4;
                }
            }
        }


        // --------low-level gyro-based PID. ----------
        const int32_t gyroRate = gyroADC[axis] / 4;
        axisPID[axis] = mwRewriteCore(axis, gyroRate, angleRate, antiWindupOn, motorSaturated);

    }

    return axisPID;
}

