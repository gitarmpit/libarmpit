/*
 * This file is part of baseflight
 * Licensed under GPL V3 or modified DCL - see https://github.com/multiwii/baseflight/blob/master/README.md
 */
#include "board.h"
#include "mw.h"
#include "utils.h"

static uint8_t MOTORS = 4;
int16_t motor_pwm_val[MAX_MOTORS];
int16_t motor_disarmed[MAX_MOTORS];

static motorMixer_t currentMixer[MAX_MOTORS];


static const motorMixer_t mixerQuadX[] = {
    { 1.0f, -1.0f,  1.0f, -1.0f },          // REAR_R
    { 1.0f, -1.0f, -1.0f,  1.0f },          // FRONT_R
    { 1.0f,  1.0f,  1.0f,  1.0f },          // REAR_L
    { 1.0f,  1.0f, -1.0f, -1.0f },          // FRONT_L
};


void mixerInit(void)
{
    int i;

    for (i = 0; i < MOTORS; i++)
        currentMixer[i] = mixerQuadX[i];


    mixerResetMotors();
}

void mixerResetMotors(void)
{
    int i;
    // set disarmed motor values
    for (i = 0; i < MAX_MOTORS; i++)
        motor_disarmed[i] = mcfg.mincommand;
}




//TODO
static void pwmWriteMotor(uint8_t index, uint16_t value)
{
}

void writeMotors(void)
{
    uint8_t i;

    for (i = 0; i < MOTORS; i++)
        pwmWriteMotor(i, motor_pwm_val[i]);
}


void writeAllMotors(int16_t mc)
{
    uint8_t i;

    // Sends commands to all motors
    for (i = 0; i < MOTORS; i++)
        motor_pwm_val[i] = mc;
    writeMotors();
}


void mixTable(void)
{
    int16_t maxMotor;
    uint32_t i;

    // prevent "yaw jump" during yaw correction
    axisPID[YAW] = constrain(axisPID[YAW], -100 - abs(rcCommand[YAW]), +100 + abs(rcCommand[YAW]));

    for (i = 0; i < MOTORS; i++) {
        motor_pwm_val[i] = rcCommand[THROTTLE] * currentMixer[i].throttle +
                axisPID[PITCH] * currentMixer[i].pitch +
                axisPID[ROLL] * currentMixer[i].roll +
                -cfg.yaw_direction * axisPID[YAW] * currentMixer[i].yaw;
    }


    maxMotor = motor_pwm_val[0];
    for (i = 1; i < MOTORS; i++)
    {
        if (motor_pwm_val[i] > maxMotor)
        {
            maxMotor = motor_pwm_val[i];
        }
    }

    for (i = 0; i < MOTORS; i++)
    {
        if (maxMotor > mcfg.maxthrottle)
            // this is a way to still have good gyro corrections
            //if at least one motor reaches its max.
            motor_pwm_val[i] -= maxMotor - mcfg.maxthrottle;

        motor_pwm_val[i] = constrain(motor_pwm_val[i], mcfg.minthrottle, mcfg.maxthrottle);
        if ((rcData[THROTTLE]) < mcfg.mincheck) {
#ifdef FEATURE_MOTOR_STOP
                motor_pwm_val[i] = mcfg.minthrottle;
#else
                motor_pwm_val[i] = mcfg.mincommand;
#endif
                flags.MOTORS_STOPPED = 1;
        }
        else
        {
            flags.MOTORS_STOPPED = 0;
        }
        if (!flags.ARMED) {
            motor_pwm_val[i] = motor_disarmed[i];
            flags.MOTORS_STOPPED = 1;
        }
    }
}
