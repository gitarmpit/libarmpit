/*
 * This file is part of baseflight
 * Licensed under GPL V3 or modified DCL - see https://github.com/multiwii/baseflight/blob/master/README.md
 */
#include "board.h"
#include "mw.h"

static uint8_t numberMotor = 0;
static uint8_t numberRules = 0;
int16_t motor[MAX_MOTORS];
int16_t motor_disarmed[MAX_MOTORS];

static motorMixer_t currentMixer[MAX_MOTORS];



static const motorMixer_t mixerQuadX[] = {
    { 1.0f, -1.0f,  1.0f, -1.0f },          // REAR_R
    { 1.0f, -1.0f, -1.0f,  1.0f },          // FRONT_R
    { 1.0f,  1.0f,  1.0f,  1.0f },          // REAR_L
    { 1.0f,  1.0f, -1.0f, -1.0f },          // FRONT_L
};




// Keep this synced with MultiType struct in mw.h!
const mixer_t mixers[] = {
//    Mo Se Mixtable
    { 0, NULL },                // entry 0
    { 4, mixerQuadX }          // MULTITYPE_QUADX
};




void mixerInit(void)
{
    int i;

    if (mcfg.mixerConfiguration == MULTITYPE_CUSTOM) {
        // load custom mixer into currentMixer
        for (i = 0; i < MAX_MOTORS; i++) {
            // check if done
            if (mcfg.customMixer[i].throttle == 0.0f)
                break;
            currentMixer[i] = mcfg.customMixer[i];
            numberMotor++;
        }
    } else {
        numberMotor = mixers[mcfg.mixerConfiguration].numberMotor;
        // copy motor-based mixers
        if (mixers[mcfg.mixerConfiguration].motor) {
            for (i = 0; i < numberMotor; i++)
                currentMixer[i] = mixers[mcfg.mixerConfiguration].motor[i];
        }
    }


    mixerResetMotors();
}

void mixerResetMotors(void)
{
    int i;
    // set disarmed motor values
    for (i = 0; i < MAX_MOTORS; i++)
        motor_disarmed[i] = mcfg.mincommand;
}


void mixerLoadMix(int index)
{
    int i;

    // we're 1-based
    index++;
    // clear existing
    for (i = 0; i < MAX_MOTORS; i++)
        mcfg.customMixer[i].throttle = 0.0f;

    // do we have anything here to begin with?
    if (mixers[index].motor != NULL) {
        for (i = 0; i < mixers[index].numberMotor; i++)
            mcfg.customMixer[i] = mixers[index].motor[i];
    }
}


void writeMotors(void)
{
    uint8_t i;

    for (i = 0; i < numberMotor; i++)
        pwmWriteMotor(i, motor[i]);
}

void writeAllMotors(int16_t mc)
{
    uint8_t i;

    // Sends commands to all motors
    for (i = 0; i < numberMotor; i++)
        motor[i] = mc;
    writeMotors();
}


void mixTable(void)
{
    int16_t maxMotor;
    uint32_t i;

    if (numberMotor > 3) {
        // prevent "yaw jump" during yaw correction
        axisPID[YAW] = constrain(axisPID[YAW], -100 - abs(rcCommand[YAW]), +100 + abs(rcCommand[YAW]));
    }

    // motors for non-servo mixes
    if (numberMotor > 1) {
        for (i = 0; i < numberMotor; i++)
        {
            motor[i] = rcCommand[THROTTLE] * currentMixer[i].throttle + axisPID[PITCH] * currentMixer[i].pitch + axisPID[ROLL] * currentMixer[i].roll + -cfg.yaw_direction * axisPID[YAW] * currentMixer[i].yaw;
        }
    }


    maxMotor = motor[0];
    for (i = 1; i < numberMotor; i++)
    {
        if (motor[i] > maxMotor)
        {
            maxMotor = motor[i];
        }
    }

    for (i = 0; i < numberMotor; i++)
    {
    	// this is a way to still have good gyro corrections if at least one motor reaches its max.
        if (maxMotor > mcfg.maxthrottle)
        {
            motor[i] -= maxMotor - mcfg.maxthrottle;
        }


        motor[i] = constrain(motor[i], mcfg.minthrottle, mcfg.maxthrottle);
        if ((rcData[THROTTLE]) < mcfg.mincheck)
        {
        	motor[i] = mcfg.minthrottle;
        	f.MOTORS_STOPPED = 1;
        }
        else
        {
        	f.MOTORS_STOPPED = 0;
        }

        if (!f.ARMED) {
        	motor[i] = motor_disarmed[i];
        	f.MOTORS_STOPPED = 1;
        }
    }
}
