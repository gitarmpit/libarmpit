#include "mixer.h"
#include "maths.h"
#include "filter.h"

/* QuadX
4CW   2CCW
   \ /
    X
   / \
3CCW  1CW
*/
static const motorMixer_t mixerQuadX[] = {
//throttle,  roll, pitch,   yaw
    { 1.0f, -1.0f,  1.0f, -1.0f },          // REAR_R  (M1)
    { 1.0f, -1.0f, -1.0f,  1.0f },          // FRONT_R (M2)
    { 1.0f,  1.0f,  1.0f,  1.0f },          // REAR_L  (M3)
    { 1.0f,  1.0f, -1.0f, -1.0f },          // FRONT_L (M4)
};

Mixer::Mixer(motorConfig_t* motorConfig, bool isAirMode)
{
    this->motorConfig = motorConfig;

    motorLimitReached = false;
    //TODO move to Cfg
    mixerConfig.pid_at_min_throttle = true;
    mixerConfig.yaw_motor_direction = 1;
    mixerConfig.yaw_jump_prevention_limit = 200;

    airModeEnabled = isAirMode;

    for (uint8_t i = 0; i < 4; i++) {
        currentMixer[i] = mixerQuadX[i];
    }

    for (int i = 0; i < 4; i++)
    {
        motor_disarmed[i] = motorConfig->mincommand;
    }

}







//TODO
static void pwmWriteMotor(int, int)
{

}
void Mixer::writeMotors(void)
{
    uint8_t i;

    //TODO shouldn't write here, get the motor values and write them outside this class
    for (i = 0; i < 4; i++)
        pwmWriteMotor(i, motor[i]);

}

void Mixer::writeAllMotors(int16_t mc)
{
    uint8_t i;

    // Sends commands to all motors
    for (i = 0; i < 4; i++)
        motor[i] = mc;
    writeMotors();
}

//void Mixer::stopMotors(void)
//{
//    writeAllMotors(motorConfig->mincommand);
//
//    delay(50); // give the timers and ESCs a chance to react.
//}



void Mixer::mixTable(bool isArmed,
                     bool isFailSafeActive,
                     int16_t rcThrottle,
                     bool isThrottleLow,
                     int16_t rcYaw,
                     int16_t* axisPID)
{

    //TODO maybe make this check in fc and call setLowThottle directly
    /* Disarmed for all mixers */
    if (!isArmed)
    {
        for (uint8_t i = 0; i < 4; i++)
        {
            motor[i] = motor_disarmed[i];
        }
        return;
    }

    //TODO should this be part of PID?
    if (mixerConfig.yaw_jump_prevention_limit < YAW_JUMP_PREVENTION_LIMIT_HIGH) {
        // prevent "yaw jump" during yaw correction
        axisPID[FD_YAW] = constrain(axisPID[FD_YAW],
                -mixerConfig.yaw_jump_prevention_limit - ABS(rcYaw),
                mixerConfig.yaw_jump_prevention_limit + ABS(rcYaw));
    }

    if (airModeEnabled)
    {
        // Initial mixer concept by bdoiron74 reused and optimized for Air Mode
        int16_t rollPitchYawMix[4];
        int16_t rollPitchYawMixMax = 0; // assumption: symetrical about zero.
        int16_t rollPitchYawMixMin = 0;

        // Find roll/pitch/yaw desired output
        for (uint32_t i = 0; i < 4; i++)
        {
            rollPitchYawMix[i] =
                axisPID[FD_PITCH] * currentMixer[i].pitch +
                axisPID[FD_ROLL] * currentMixer[i].roll +
                -mixerConfig.yaw_motor_direction * axisPID[FD_YAW] * currentMixer[i].yaw;

            if (rollPitchYawMix[i] > rollPitchYawMixMax)
                rollPitchYawMixMax = rollPitchYawMix[i];
            if (rollPitchYawMix[i] < rollPitchYawMixMin)
                rollPitchYawMixMin = rollPitchYawMix[i];
        }

        // Scale roll/pitch/yaw uniformly to fit within throttle range
        int16_t rollPitchYawMixRange = rollPitchYawMixMax - rollPitchYawMixMin;
        int16_t throttleRange;
        int16_t throttleMin, throttleMax;

        throttleMin = motorConfig->minthrottle;
        throttleMax = motorConfig->maxthrottle;

        throttleRange = throttleMax - throttleMin;

        if (rollPitchYawMixRange > throttleRange) {
            motorLimitReached = true;
            float mixReduction = (float) throttleRange / rollPitchYawMixRange;
            for (uint8_t i = 0; i < 4; i++) {
                rollPitchYawMix[i] =  lrintf((float) rollPitchYawMix[i] * mixReduction);
            }
            // Get the maximum correction by setting throttle offset to center.
            throttleMin = throttleMax = throttleMin + (throttleRange / 2);
        } else {
            motorLimitReached = false;
            throttleMin = throttleMin + (rollPitchYawMixRange / 2);
            throttleMax = throttleMax - (rollPitchYawMixRange / 2);
        }

        // Now add in the desired throttle, but keep in a range that doesn't clip adjusted
        // roll/pitch/yaw. This could move throttle down, but also up for those low throttle flips.
        for (uint8_t i = 0; i < 4; i++) {
            motor[i] = rollPitchYawMix[i] + constrain(rcThrottle * currentMixer[i].throttle, throttleMin, throttleMax);

            motor[i] = constrain(motor[i],
                                 isFailSafeActive ? motorConfig->mincommand : motorConfig->minthrottle,
                                         motorConfig->maxthrottle);

        }
    } else {
        // motors for non-servo mixes
        for (uint8_t i = 0; i < 4; i++) {
            motor[i] =
                rcThrottle * currentMixer[i].throttle +
                axisPID[FD_PITCH] * currentMixer[i].pitch +
                axisPID[FD_ROLL] * currentMixer[i].roll +
                -mixerConfig.yaw_motor_direction * axisPID[FD_YAW] * currentMixer[i].yaw;
        }

        int16_t maxMotor = motor[0];
        for (uint8_t i = 1; i < 4; i++)
        {
            if (motor[i] > maxMotor) {
                maxMotor = motor[i];
            }
        }

        int16_t maxThrottleDifference = 0;
        if (maxMotor > motorConfig->maxthrottle) {
            maxThrottleDifference = maxMotor - motorConfig->maxthrottle;
        }


        for (uint8_t i = 0; i < 4; i++)
        {
            // this is a way to still have good gyro corrections if at least one motor reaches its max.
            motor[i] -= maxThrottleDifference;

            if (isFailSafeActive)
            {
                motor[i] = constrain(motor[i], motorConfig->mincommand, motorConfig->maxthrottle);
            }
            else
            {
                motor[i] = constrain(motor[i], motorConfig->minthrottle, motorConfig->maxthrottle);
                if (isThrottleLow && !mixerConfig.pid_at_min_throttle)
                {
                    motor[i] = motorConfig->minthrottle;
                }
            }
        }
    }



}

