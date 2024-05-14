#ifndef MOTORS_H
#define MOTORS_H

#include <stdint.h>

extern int16_t MotorOut1;
extern int16_t MotorOut2;
extern int16_t MotorOut3;
extern int16_t MotorOut4;

void motorsSetup(void);
void motorLoop(void);
void motorsIdentify(void);
void motorsThrottleCalibration(void);
void output_motor_ppm(void);

#endif
