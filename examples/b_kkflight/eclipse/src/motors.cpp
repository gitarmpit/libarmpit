#include "motors.h"

#include "receiver.h"

int16_t MotorOut1;
int16_t MotorOut2;
int16_t MotorOut3;
int16_t MotorOut4;



void output_motor_ppm()
{
    //pwm   MotorOut1..4

}

void motorsIdentify()
{
    MotorOut1 = 0;
    MotorOut2 = 0;
    MotorOut3 = 0;
    MotorOut4 = 0;

    output_motor_ppm();
}

void motorsThrottleCalibration()
{
  while(1) 
  {
    RxGetChannels();
    MotorOut1 = RxInCollective;
    MotorOut2 = RxInCollective;
    MotorOut3 = RxInCollective;
    MotorOut4 = RxInCollective;
    output_motor_ppm();  // this regulates rate at which we output signals
  }
}
