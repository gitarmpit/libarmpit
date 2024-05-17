#ifndef GYROS_H
#define GYROS_H

#include <stdint.h>

// Gyro gain shift-right (after 32-bit multiplication of GainInADC[] value).
#define GYRO_GAIN_SHIFT 5

// Skip yaw gyro calculations if using external yaw gyro
//#define EXTERNAL_YAW_GYRO

#define ADC_MAX 1023

enum GyroDirection { GYRO_NORMAL = 0, GYRO_REVERSED };
enum GyroArrayIndex { ROLL = 0, PITCH, YAW };

extern uint16_t GainInADC[3];        // ADC result
extern int16_t  gyroADC[3];          // Holds Gyro ADC's
extern int16_t  gyroZero[3];         // used for calibrating Gyros on ground

void init_adc(void);
void read_adc(uint8_t channel);
void ReadGainPots(void);
void ReadGyros(void);
void CalibrateGyros(void);
void gyrosSetup(void);
void gyrosReverse(void);

#endif
