#include <platform.h>
#include "maths.h"
#include "filter.h"
#include "pid.h"
#include "runtime_config.h"

// constants to scale pidLuxFloat so output is same as pidMultiWiiRewrite

int16_t Pid::pidLuxFloatCore(int axis, float gyroRate, float angleRate, bool antiWindupOn, bool motorSaturated)
{
    static float lastRateForDelta[3];

    const float rateError = angleRate - gyroRate;

    // -----calculate P component
    float PTerm = luxPTermScale * rateError * pcfg->P[axis] * PIDweight[axis] / 100;
    // Constrain YAW by yaw_p_limit value if not servo driven, in that case servolimits apply
    if (axis == YAW) {
        if (pcfg->yaw_lpf_hz) {
            PTerm = pt1FilterApply4(&yawFilter, PTerm, pcfg->yaw_lpf_hz, dT);
        }
        if (pcfg->yaw_p_limit) {
            PTerm = constrainf(PTerm, -pcfg->yaw_p_limit, pcfg->yaw_p_limit);
        }
    }

    // -----calculate I component
    float ITerm = lastITermf[axis] + luxITermScale * rateError * dT * pcfg->I[axis];
    // limit maximum integrator value to prevent WindUp - accumulating extreme values when system is saturated.
    // I coefficient (I8) moved before integration to make limiting independent from PID settings
    ITerm = constrainf(ITerm, -PID_MAX_I, PID_MAX_I);
    // Anti windup protection
    if (_airMode) {
        if (antiWindupOn || motorSaturated) {
            ITerm = constrainf(ITerm, -ITermLimitf[axis], ITermLimitf[axis]);
        } else {
            ITermLimitf[axis] = ABS(ITerm);
        }
    }
    lastITermf[axis] = ITerm;

    // -----calculate D component
    float DTerm;
    if (pcfg->D[axis] == 0) {
        // optimisation for when D8 is zero, often used by YAW axis
        DTerm = 0;
    } else {
        float delta;
        if (pcfg->deltaMethod == PID_DELTA_FROM_MEASUREMENT) {
            delta = -(gyroRate - lastRateForDelta[axis]);
            lastRateForDelta[axis] = gyroRate;
        } else {
            delta = rateError - lastRateForDelta[axis];
            lastRateForDelta[axis] = rateError;
        }
        // Divide delta by targetLooptime to get differential (ie dr/dt)
        delta /= dT;

        // Filter delta
        if (pcfg->dterm_notch_hz) {
            delta = biquadFilterApply(&dtermFilterNotch[axis], delta);
        }

        if (pcfg->dterm_lpf_hz) {
            if (pcfg->dterm_filter_type == FILTER_BIQUAD) {
                delta = biquadFilterApply(&dtermFilterLpf[axis], delta);
            } else {
                // DTerm delta low pass filter
                delta = pt1FilterApply4(&deltaFilter[axis], delta, pcfg->dterm_lpf_hz, dT);
            }
        }

        DTerm = luxDTermScale * delta * pcfg->D[axis] * PIDweight[axis] / 100;
        DTerm = constrainf(DTerm, -PID_MAX_D, PID_MAX_D);
    }

    // -----calculate total PID output
    return lrintf(PTerm + ITerm + DTerm);
}

int16_t* Pid::pidLuxFloat(int16_t* rcCommand,
        float* gyroADCf,
        int16_t* attitude,
        bool antiWindupOn,
        bool motorSaturated,
        int32_t horizonLevelStrength,
		StabilizationMode mode)
{

    // ----------PID controller----------
    for (int axis = 0; axis < 3; axis++)
    {
        const uint8_t rate = rates[axis];

        // -----Get the desired angle rate depending on flight mode
        float angleRate;
        if (axis == FD_YAW)
        {
            // YAW is always gyro-controlled (MAG correction is applied to rcCommand) 100dps to 1100dps max yaw rate
            angleRate = (float)((rate + 27) * rcCommand[YAW]) / 32.0f;
        }
        else
        {
            // control is GYRO based for ACRO and HORIZON - direct sticks control is applied to rate PID
            angleRate = (float)((rate + 27) * rcCommand[axis]) / 16.0f; // 200dps to 1200dps max roll/pitch rate
            if (mode == ANGLE_MODE_ || mode == HORIZON_MODE_)
            {
                // calculate error angle and limit the angle to the max inclination
                // multiplication of rcCommand corresponds to changing the sticks scaling here
#ifdef GPS
                const float errorAngle = constrain(2 * rcCommand[axis] + GPS_angle[axis], -((int)imu->max_angle_inclination()), imu->max_angle_inclination())
                        - imu->getAttitude()[axis] + accel->getAccelerometerConfig()->accelerometerTrims.raw[axis];
#else
                const float errorAngle = constrain(2 * rcCommand[axis], -((int)pcfg->max_angle_inclination), pcfg->max_angle_inclination)
                        - attitude[axis] +  (axis == FD_PITCH) ? rollPitchTrims.pitch : rollPitchTrims.roll;
#endif
                if (mode == ANGLE_MODE_)
                {
                    // ANGLE mode
                    angleRate = errorAngle * pcfg->pidLevel_P / 16.0f;
                }
                else
                {
                    // HORIZON mode
                    // mix in errorAngle to desired angleRate to add a little auto-level feel.
                    // horizonLevelStrength has been scaled to the stick input
                    angleRate += errorAngle * pcfg->pidLevel_I * ((float)horizonLevelStrength / 100.) / 16.0f;
                }
            }
        }

        // --------low-level gyro-based PID. ----------
        const float gyroRate = luxGyroScale * gyroADCf[axis] * pcfg->gyroScale;
        axisPID[axis] = pidLuxFloatCore(axis, gyroRate, angleRate, antiWindupOn, motorSaturated);
        //axisPID[axis] = constrain(axisPID[axis], -PID_LUX_FLOAT_MAX_PID, PID_LUX_FLOAT_MAX_PID);
    }

    return axisPID;
}


