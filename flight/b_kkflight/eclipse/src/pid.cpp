#include "pid.h"
#include "system_time.h"
#include <math.h>

struct PID_PidObject PID_Array[3];

void PID_Calculate(struct PID_PidObject *pidObject)
{
  uint32_t time = millis();
  uint32_t dt = time - pidObject->lastTime;

  float error = pidObject->measured - pidObject->setPoint;

  // Calculate product
  float output = pidObject->kP * error;

  // Calculate integral
  float integral = pidObject->integral;
  if(fabs(error) > pidObject->epsilon) {
    integral += error * dt;
  }
  output += pidObject->kI * integral;

  // Calculate differential
  output += pidObject->kD * ((pidObject->lastError - error) / dt);
  
  pidObject->integral = integral;
  pidObject->lastTime = time;
  pidObject->lastError = error;
}

void PID_CalculateAll(void)
{
  for(uint8_t i = 0; i < sizeof(PID_Array); ++i) {
    PID_Calculate(&PID_Array[i]);
  }
}

void PID_Setup(void)
{
  for(uint8_t i = 0; i < sizeof(PID_Array); ++i) {
    PID_Array[i].lastTime = millis();
  }
}
