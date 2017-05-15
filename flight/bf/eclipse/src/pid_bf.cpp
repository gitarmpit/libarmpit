#include "pid_bf.h"
#include "bf_common.h"

Pid_bf::Pid_bf(pidProfile_t* cfg)
{
    //TODO
    cfg->P[ROLL] = 40;
    cfg->I[ROLL] = 30;
    cfg->D[ROLL] = 23;
    cfg->P[PITCH] = 40;
    cfg->I[PITCH] = 30;
    cfg->D[PITCH] = 23;
    cfg->P[YAW] = 85;
    cfg->I[YAW] = 45;
    cfg->D[YAW] = 0;

    cfg->pidLevel_P = 90;
    cfg->pidLevel_I = 10;
    cfg->pidLevel_D = 100;

    cfg->yawRate = 0;

    //used by pid rewrite, seems to be zero in bf
    cfg->rollPitchRate[0] = 0;
    cfg->rollPitchRate[1] = 0;

    this->cfg = cfg;
    max_angle_inclination = 500;    // 50 degrees
}

void Pid_bf::setDynPid (int32_t* dynAdj)
{
	for (int axis = 0; axis < 3; axis++)
	{
		dynP[axis] = (uint16_t)cfg->P[axis] * dynAdj[axis] / 100;
		dynI[axis] = (uint16_t)cfg->I[axis] * dynAdj[axis] / 100;
		dynD[axis] = (uint16_t)cfg->D[axis] * dynAdj[axis] / 100;
	}

}

void Pid_bf::pidMultiWii(int16_t* rcCommand, int16_t* gyroADC, int16_t* attitude, FLIGHT_MODE flightMode, int16_t* axisPID)
{
    static int32_t errorAngleI[2] = { 0, 0 };
    static int32_t errorGyroI[3] = { 0, 0, 0 };

    int axis, maxIncline;
    int32_t error, errorAngle;
    int32_t PTerm, ITerm, PTermACC = 0, ITermACC = 0, PTermGYRO = 0, ITermGYRO = 0, DTerm;
    static int16_t lastGyro[3] = { 0, 0, 0 };
    static int32_t delta1[3], delta2[3];
    int32_t deltaSum;
    int32_t delta;

    // **** PITCH & ROLL & YAW PID ****
    maxIncline = max(abs(rcCommand[PITCH]), abs(rcCommand[ROLL])); // range [0;500]

    for (axis = 0; axis < 3; axis++)
    {
        if ((flightMode == ANGLE_MODE || flightMode == HORIZON_MODE) && axis < 2)
        { // MODE relying on ACC
            // 50 degrees max inclination
#ifdef GPS
            errorAngle = constrain(2 * rcCommand[axis] + GPS_angle[axis], -((int)mcfg.max_angle_inclination), +mcfg.max_angle_inclination) - angle[axis] + cfg.angleTrim[axis];
#else
            errorAngle = constrain(2 * rcCommand[axis], -((int)max_angle_inclination), +max_angle_inclination) - attitude[axis] + rollPitchTrim[axis];
#endif
            // 32 bits is needed for calculation: errorAngle*P8[PIDLEVEL] could exceed 32768   16 bits is ok for result
            PTermACC = errorAngle * cfg->pidLevel_P / 100;
            PTermACC = constrain(PTermACC, -cfg->pidLevel_D * 5, +cfg->pidLevel_D * 5);

            errorAngleI[axis] = constrain(errorAngleI[axis] + errorAngle, -10000, +10000); // WindUp
            ITermACC = (errorAngleI[axis] * cfg->pidLevel_I) >> 12;
        }

        if (flightMode != ANGLE_MODE || flightMode == HORIZON_MODE || axis == 2)
        { // MODE relying on GYRO or YAW axis
            error = (int32_t)rcCommand[axis] * 10 * 8 / cfg->P[axis];
            error -= gyroADC[axis];

            PTermGYRO = rcCommand[axis];

            errorGyroI[axis] = constrain(errorGyroI[axis] + error, -16000, +16000); // WindUp
            if ((abs(gyroADC[axis]) > 640) || ((axis == YAW) && (abs(rcCommand[axis]) > 100)))
            {
                errorGyroI[axis] = 0;
            }

            ITermGYRO = (errorGyroI[axis] / 125 * cfg->I[axis]) >> 6;
        }

        if (flightMode == HORIZON_MODE && axis < 2)
        {
            PTerm = (PTermACC * (500 - maxIncline) + PTermGYRO * maxIncline) / 500;
            ITerm = (ITermACC * (500 - maxIncline) + ITermGYRO * maxIncline) / 500;
        }
        else
        {
            if (flightMode == ANGLE_MODE && axis < 2)
            {
                PTerm = PTermACC;
                ITerm = ITermACC;
            }
            else
            {
                PTerm = PTermGYRO;
                ITerm = ITermGYRO;
            }
        }

        PTerm -= (int32_t)gyroADC[axis] * dynP[axis] / 10 / 8; // 32 bits is needed for calculation
        delta = gyroADC[axis] - lastGyro[axis];
        lastGyro[axis] = gyroADC[axis];
        deltaSum = delta1[axis] + delta2[axis] + delta;
        delta2[axis] = delta1[axis];
        delta1[axis] = delta;
        DTerm = (deltaSum * dynD[axis]) / 32;

        axisPID[axis] = PTerm + ITerm - DTerm;
    }
}

#define GYRO_I_MAX 256

//same as multiwii's "alexK"
void Pid_bf::pidRewrite(int16_t* rcCommand, int16_t* gyroADC, int16_t* attitude, uint32_t dT, FLIGHT_MODE flightMode, int16_t* axisPID)
{
    static int32_t errorGyroI[3] = { 0, 0, 0 };
    int32_t errorAngle = 0;
    int axis;
    int32_t delta, deltaSum;
    static int32_t delta1[3], delta2[3];
    int32_t PTerm, ITerm, DTerm;
    static int32_t lastError[3] = { 0, 0, 0 };
    int32_t AngleRateTmp, RateError;

    for (axis = 0; axis < 3; axis++)
    {
        // -----Get the desired angle rate depending on flight mode
        if (axis == 2) { // YAW is always gyro-controlled (MAG correction is applied to rcCommand)
            AngleRateTmp = (((int32_t)(cfg->yawRate + 27) * rcCommand[YAW]) >> 5);
        }
        else
        {
            // calculate error and limit the angle to 50 degrees max inclination
#ifdef GPS
            errorAngle = (constrain(rcCommand[axis] + GPS_angle[axis], -500, +500) - attitude[axis] + rollPitchTrim[axis]) / 10.0f; // 16 bits is ok here
#else
            errorAngle = (constrain(rcCommand[axis], -500, +500) - attitude[axis] + rollPitchTrim[axis]) / 10.0f; // 16 bits is ok here
#endif
            if (flightMode != ANGLE_MODE)
            { //control is GYRO based (ACRO and HORIZON - direct sticks control is applied to rate PID
                AngleRateTmp = ((int32_t)(cfg->rollPitchRate[axis] + 27) * rcCommand[axis]) >> 4;

                if (flightMode == HORIZON_MODE)
                {
                    // mix up angle error to desired AngleRateTmp to add a little auto-level feel
                    AngleRateTmp += (errorAngle * cfg->pidLevel_I) >> 8;
                }
            }
            else
            { // it's the ANGLE mode - control is angle based, so control loop is needed
                AngleRateTmp = (errorAngle * cfg->pidLevel_P) >> 4;
            }
        }

        // --------low-level gyro-based PID. ----------
        // Used in stand-alone mode for ACRO, controlled by higher level regulators in other modes
        // -----calculate scaled error.AngleRates
        // multiplication of rcCommand corresponds to changing the sticks scaling here
        RateError = AngleRateTmp - gyroADC[axis];

        // -----calculate P component
        PTerm = (RateError * cfg->P[axis]) >> 7;

        // -----calculate I component
        // there should be no division before accumulating the error to integrator, because the precision would be reduced.
        // Precision is critical, as I prevents from long-time drift. Thus, 32 bits integrator is used.
        // Time correction (to avoid different I scaling for different builds based on average dT us)
        // is normalized to cycle time = 2048.
        errorGyroI[axis] += ((RateError * dT) >> 11) * cfg->I[axis];

        // limit maximum integrator value to prevent WindUp - accumulating extreme values when system is saturated.
        // I coefficient (I8) moved before integration to make limiting independent from PID settings
        errorGyroI[axis] = constrain(errorGyroI[axis], (int32_t)(-GYRO_I_MAX) << 13, (int32_t)(+GYRO_I_MAX) << 13);

        ITerm = errorGyroI[axis] >> 13;

        //-----calculate D-term
        delta = RateError - lastError[axis];  // 16 bits is ok here, the dif between 2 consecutive gyro reads is limited to 800
        lastError[axis] = RateError;

        // Correct difference by cycle time. dT is jittery (can be different 2 times), so calculated difference
        // would be scaled by different dt each time. Division by dT fixes that.
        delta = (delta * ((uint16_t)0xFFFF / (dT >> 4))) >> 6;
        // add moving average here to reduce noise
        deltaSum = delta1[axis] + delta2[axis] + delta;
        delta2[axis] = delta1[axis];
        delta1[axis] = delta;

        DTerm = (deltaSum * cfg->D[axis]) >> 8;

        axisPID[axis] = PTerm + ITerm + DTerm;
    }
}
