#include "mixer_bf.h"
#include "bf_common.h"

static const motorMixer_t mixerQuadX[] = {
    { 1.0f, -1.0f,  1.0f, -1.0f },          // REAR_R
    { 1.0f, -1.0f, -1.0f,  1.0f },          // FRONT_R
    { 1.0f,  1.0f,  1.0f,  1.0f },          // REAR_L
    { 1.0f,  1.0f, -1.0f, -1.0f },          // FRONT_L
};



Mixer_bf::Mixer_bf (motorConfig_t* cfg)
{
    //TODO
    cfg->yaw_direction = 1;
    cfg->minthrottle = 1150;
    cfg->maxthrottle = 1850;
    cfg->mincommand = 1000;


    mcfg = cfg;




    for (uint8_t i = 0; i < 4; i++)
    {
        currentMixer[i] = mixerQuadX[i];
    }

    stopMotors();
}

void  Mixer_bf::stopMotors(void)
{
    for (uint8_t i = 0; i < 4; i++)
        motor[i] = mcfg->mincommand;
}

void Mixer_bf::setMinThrottle()
{
    for (uint8_t i = 0; i < 4; i++)
        motor[i] = mcfg->mincommand;
}



//TODO
static void pwmWriteMotor (int, int)
{

}


void Mixer_bf::writeMotors(void)
{
    uint8_t i;

    for (i = 0; i < 4; i++)
    {
        pwmWriteMotor(i, motor[i]);
    }
}

void Mixer_bf::writeAllMotors(int16_t mc)
{
    uint8_t i;

    // Sends commands to all motors
    for (i = 0; i < 4; i++)
        motor[i] = mc;
    writeMotors();
}


//TODO maybe dont call this if not armed or throttle low, just call them directly?
void Mixer_bf::mixTable(int16_t rcThrottle, int16_t rcYaw, int16_t* axisPID, bool armed, bool isThrottleLow)
{
    if (!armed)
    {
        stopMotors();
        return;
    }
    else if (isThrottleLow)
    {
        setMinThrottle();
        return;
    }

    uint32_t i;

    // prevent "yaw jump" during yaw correction
    axisPID[YAW] = constrain(axisPID[YAW], -100 - abs(rcYaw), +100 + abs(rcYaw));

    for (i = 0; i < 4; i++)
    {
        motor[i] = rcThrottle * currentMixer[i].throttle +
                axisPID[PITCH] * currentMixer[i].pitch + axisPID[ROLL] * currentMixer[i].roll +
                -mcfg->yaw_direction * axisPID[YAW] * currentMixer[i].yaw;
    }

    int16_t maxMotor = motor[0];
    for (i = 1; i < 4; i++)
    {
        if (motor[i] > maxMotor)
        {
            maxMotor = motor[i];
        }
    }

    int16_t maxThrottleDifference = 0;
    if (maxMotor > mcfg->maxthrottle)
    {
        maxThrottleDifference = maxMotor - mcfg->maxthrottle;
    }

    for (i = 0; i < 4; i++)
    {
        motor[i] = constrain(motor[i] - maxThrottleDifference, mcfg->minthrottle, mcfg->maxthrottle);
    }
}

//TODO do this outside the mixer
//    if ((rcData[THROTTLE]) < mcfg->mincheck)
//    {
//        motor[i] = mcfg->minthrottle;
//        //f.MOTORS_STOPPED = 1;  //TODO only needed for GPS
//    }
//    else
//    {
//        //f.MOTORS_STOPPED = 0;
//    }

