#include <stdio.h>
#include <math.h>
#include <stdint.h>

uint32_t _pwmPeriod_us;
uint32_t _totalSteps;
float _stepSize;
uint32_t _currentStep;


static void HandleInterrupt()  {
    
    float ds = 1.0 + sin (_stepSize*_currentStep*M_PI/180. - M_PI/2.0);
    ds = _pwmPeriod_us*ds/2.0;
    printf ("%d %f\n", _currentStep, ds);

    if (++_currentStep >= _totalSteps) {
      _currentStep = 0;
    }

}


int main(void) {

    _pwmPeriod_us = 1000; // us
    
    float freqSineHz = 50;
    float freqPWM_Hz = 1.0 / _pwmPeriod_us * 1000000.0;
    // printf ("freqPWM: %f\n", freqPWM_Hz);
    _totalSteps = freqPWM_Hz / freqSineHz; // number of PWM periods, each with a varying duty cycle according to sine
    _stepSize = 360.0 / _totalSteps;
    _currentStep = 0;

    // printf ("_totalSteps: %d, _stepSize: %f\n", _totalSteps, _stepSize);
    for (int i = 0; i <= _totalSteps; ++i) {
      float s = (1.0 + sin (_stepSize*i*M_PI/180. - M_PI/2.0)) / 2.0;
      float ds = _pwmPeriod_us * s;
      printf ("%d,%f,%d\n", i, ds, (int)(ds + 0.5));
    }
}
