#include "config.h"
#include "def.h"
#include "types.h"
#include "MultiWii.h"
#include "utils.h"
#include "debug.h"

void writeMotors() { // [1000;2000] => [125;250]

}

/**************************************************************************************/
/************          Writes the mincommand to all Motors           ******************/
/**************************************************************************************/
void writeAllMotors(int16_t mc) {   // Sends commands to all motors
  for (uint8_t i =0;i<NUMBER_MOTOR;i++) {
    motor[i]=mc;
  }
  writeMotors();
}

/**************************************************************************************/
/************        Initialize the PWM Timers and Registers         ******************/
/**************************************************************************************/
void initOutput()
{
  /****************            mark all PWM pins as Output             ******************/
  writeAllMotors(MINCOMMAND);
  delay(300);
}



// int8_t servodir(uint8_t n, uint8_t b) { return ((conf.servoConf[n].rate & b) ? -1 : 1) ; }

void mixTable()
{
  int16_t maxMotor;
  uint8_t i;
  #define PIDMIX(X,Y,Z) rcCommand[THROTTLE] + axisPID[ROLL]*X + axisPID[PITCH]*Y + YAW_DIRECTION * axisPID[YAW]*Z

  /****************                   main Mix Table                ******************/
  #if defined( MY_PRIVATE_MIXING )
    #include MY_PRIVATE_MIXING
  #elif defined( QUADX )
    motor[0] = PIDMIX(-1,+1,-1); //REAR_R
    motor[1] = PIDMIX(-1,-1,+1); //FRONT_R
    motor[2] = PIDMIX(+1,+1,+1); //REAR_L
    motor[3] = PIDMIX(+1,-1,-1); //FRONT_L
  #endif // MY_PRIVATE_MIXING


  /****************                normalize the Motors values                ******************/
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
          // this is a way to still have good gyro corrections if at least one motor reaches its max.
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
