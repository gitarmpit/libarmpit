#include "gyros.h"

#include "receiver.h"
#include "settings.h"



uint16_t GainInADC[3];        // ADC result
int16_t  gyroADC[3];          // Holds Gyro ADC's
int16_t  gyroZero[3];         // used for calibrating Gyros on ground

void init_adc()
{
}

void gyrosSetup()
{
  init_adc();
}


void read_adc(uint8_t channel)
{
}

/*
 * ADC reads 10-bit results (0-1023), so we cannot just multiply Gyro ADC
 * by Gain ADC, or we can wrap results. Full ADC range in a 16 bit value
 * is ADC shifted left by 6, so we scale the gain to 6-bit by shifting
 * right by 10 - 6 = 4 bits.
 */
void ReadGainPots()
{
  read_adc(3);      // read roll gain ADC3
  GainInADC[ROLL] = 0;

  read_adc(4);      // read pitch gain ADC4
  GainInADC[PITCH] = 0;

  read_adc(5);      // read yaw gain ADC5
  GainInADC[YAW] = 0;
}

void ReadGyros()
{
  read_adc(2);      // read roll gyro ADC2
  gyroADC[ROLL] = 0;

  read_adc(1);      // read pitch gyro ADC1
  gyroADC[PITCH] = 0;

  read_adc(0);      // read yaw gyro ADC0
  gyroADC[YAW] = 0;
}

void CalibrateGyros()
{
  uint8_t i;

  ReadGainPots();  // about time we did this !

  // get/set gyro zero value (average of 16 readings)
  gyroZero[ROLL] = 0;
  gyroZero[PITCH] = 0;
  gyroZero[YAW] = 0;

  for(i = 0;i < 16;i++) {
    ReadGyros();

    gyroZero[ROLL]+= gyroADC[ROLL];
    gyroZero[PITCH]+= gyroADC[PITCH];
    gyroZero[YAW]+= gyroADC[YAW];
  }

  gyroZero[ROLL] = (gyroZero[ROLL] + 8) >> 4;
  gyroZero[PITCH] = (gyroZero[PITCH] + 8) >> 4;
  gyroZero[YAW] = (gyroZero[YAW] + 8) >> 4;
}

