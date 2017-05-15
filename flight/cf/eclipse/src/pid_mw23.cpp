
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include <platform.h>

#include "pid.h"
#include "maths.h"
#include "filter.h"


void Pid::pidResetITermAngle(void)
{
    ITermAngle[FD_ROLL] = 0;
    ITermAngle[FD_PITCH] = 0;
}

int16_t* Pid::mw23(int16_t* rcCommand,
                        int32_t* gyroADC,
                        int16_t* attitude,
                        bool antiWindupOn,
                        bool motorSaturated,
						StabilizationMode mode)
{


    int axis, prop = 0;
    int32_t rc, error, errorAngle, delta, gyroError;
    int32_t PTerm, ITerm, PTermACC, ITermACC, DTerm;
    static int16_t lastErrorForDelta[2];
    static int32_t delta1[2], delta2[2];

    if (mode == HORIZON_MODE_)
    {
        prop = MIN(MAX(ABS(rcCommand[PITCH]), ABS(rcCommand[ROLL])), 512);
    }

    // PITCH & ROLL
    for (axis = 0; axis < 2; axis++) {

        rc = rcCommand[axis] << 1;

        gyroError = gyroADC[axis] / 4;

        error = rc - gyroError;
        lastITerm[axis]  = constrain(lastITerm[axis] + error, -16000, +16000);   // WindUp   16 bits is ok here

        if (ABS(gyroADC[axis]) > (640 * 4)) {
            lastITerm[axis] = 0;
        }

        // Anti windup protection
        if (_airMode) {
            if (antiWindupOn || motorSaturated) {
                lastITerm[axis] = constrain(lastITerm[axis], -ITermLimit[axis], ITermLimit[axis]);
            } else {
                ITermLimit[axis] = ABS(lastITerm[axis]);
            }
        }

        ITerm = (lastITerm[axis] >> 7) * pcfg->I[axis] >> 6;   // 16 bits is ok here 16000/125 = 128 ; 128*250 = 32000

        PTerm = (int32_t)rc * pcfg->P[axis] >> 6;

        if (mode == ANGLE_MODE_ || mode == HORIZON_MODE_)
        {   // axis relying on ACC
            // 50 degrees max inclination
#ifdef GPS
            errorAngle = constrain(2 * rcCommand[axis] + GPS_angle[axis], -((int) max_angle_inclination),
                +max_angle_inclination) - attitude[axis] + angleTrim->raw[axis];
#else
            errorAngle = constrain(2 * rcCommand[axis], -((int) pcfg->max_angle_inclination),
                +pcfg->max_angle_inclination) - attitude[axis] + (axis == FD_PITCH) ? rollPitchTrims.pitch : rollPitchTrims.roll;
#endif

            ITermAngle[axis]  = constrain(ITermAngle[axis] + errorAngle, -10000, +10000);                                                // WindUp     //16 bits is ok here

            PTermACC = ((int32_t)errorAngle * pcfg->pidLevel_P) >> 7;   // 32 bits is needed for calculation: errorAngle*P8 could exceed 32768   16 bits is ok for result

            int16_t limit = pcfg->pidLevel_D * 5;
            PTermACC = constrain(PTermACC, -limit, +limit);

            ITermACC = ((int32_t)ITermAngle[axis] * pcfg->pidLevel_I) >> 12;  // 32 bits is needed for calculation:10000*I8 could exceed 32768   16 bits is ok for result

            ITerm = ITermACC + ((ITerm - ITermACC) * prop >> 9);
            PTerm = PTermACC + ((PTerm - PTermACC) * prop >> 9);
        }

        PTerm -= ((int32_t)gyroError * dynP8[axis]) >> 6;   // 32 bits is needed for calculation

        //-----calculate D-term based on the configured approach (delta from measurement or deltafromError)
        // Delta from measurement
        delta = -(gyroError - lastErrorForDelta[axis]);
        lastErrorForDelta[axis] = gyroError;
        if (pcfg->dterm_lpf_hz) {
            // Dterm delta low pass
            DTerm = delta;
            DTerm = lrintf(pt1FilterApply4(&deltaFilter[axis], (float)DTerm, pcfg->dterm_lpf_hz, dT)) * 3;  // Keep same scaling as unfiltered DTerm
        } else {
            // When dterm filter disabled apply moving average to reduce noise
            DTerm  = delta1[axis] + delta2[axis] + delta;
            delta2[axis] = delta1[axis];
            delta1[axis] = delta;
        }
        DTerm = ((int32_t)DTerm * dynD8[axis]) >> 5;   // 32 bits is needed for calculation

        axisPID[axis] = PTerm + ITerm + DTerm;


    }

    //YAW
    rc = (int32_t)rcCommand[YAW] * (2 * rates[YAW] + 30)  >> 5;
    error = rc - (gyroADC[FD_YAW] / 4);
    lastITerm[FD_YAW]  += (int32_t)error * pcfg->I[FD_YAW];
    lastITerm[FD_YAW]  = constrain(lastITerm[FD_YAW], 2 - ((int32_t)1 << 28), -2 + ((int32_t)1 << 28));
    if (ABS(rc) > 50) lastITerm[FD_YAW] = 0;

    PTerm = (int32_t)error * pcfg->P[FD_YAW] >> 6; // TODO: Bitwise shift on a signed integer is not recommended

    // Constrain YAW by D value if not servo driven in that case servolimits apply
    if(pcfg->yaw_p_limit < YAW_P_LIMIT_MAX) {
        PTerm = constrain(PTerm, -pcfg->yaw_p_limit, pcfg->yaw_p_limit);
    }

    ITerm = constrain((int16_t)(lastITerm[FD_YAW] >> 13), -GYRO_I_MAX, +GYRO_I_MAX);

    axisPID[FD_YAW] =  PTerm + ITerm;

    return axisPID;
}

