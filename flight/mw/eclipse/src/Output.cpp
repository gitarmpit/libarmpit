#include "config.h"
#include "def.h"
#include "types.h"
#include "MultiWii.h"
#include "utils.h"
#include "debug.h"

void writeMotors() { // [1000;2000] => [125;250]

}

void writeAllMotors(int16_t mc)
{
  for (uint8_t i =0;i<NUMBER_MOTOR;i++)
  {
    motor[i]=mc;
  }
  writeMotors();
}

void initOutput()
{
  writeAllMotors(MINCOMMAND);
  //delay(300);
}



void mixTable()
{
    int16_t maxMotor;
    uint8_t i;
#define PIDMIX(X,Y,Z) rcCommand[THROTTLE] + axisPID[ROLL]*X + axisPID[PITCH]*Y + YAW_DIRECTION * axisPID[YAW]*Z

    motor[0] = PIDMIX(-1,+1,-1); //REAR_R
    motor[1] = PIDMIX(-1,-1,+1); //FRONT_R
    motor[2] = PIDMIX(+1,+1,+1); //REAR_L
    motor[3] = PIDMIX(+1,-1,-1); //FRONT_L

    maxMotor=motor[0];
    for(i=1; i< NUMBER_MOTOR; i++)
    {
        if (motor[i]>maxMotor)
            maxMotor = motor[i];
    }

    for(i=0; i< NUMBER_MOTOR; i++)
    {
        if (maxMotor > MAXTHROTTLE)
        {
            motor[i] -= maxMotor - MAXTHROTTLE;
        }

        motor[i] = constrain(motor[i], conf.minthrottle, MAXTHROTTLE);

        if ((rcData[THROTTLE] < MINCHECK) && !f.BARO_MODE)
        {
            motor[i] = conf.minthrottle;
        }

        if (!f.ARMED)
        {
            motor[i] = MINCOMMAND;
        }
    }

}
