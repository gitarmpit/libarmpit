#ifndef _AHRS_H
#define _AHRS_H
#include <math.h>

class AHRS 
{
private:

	float q[4] =    { 1.0f, 0.0f, 0.0f, 0.0f };    // vector to hold quaternion
	float eInt[3] =     { 0.0f, 0.0f, 0.0f };       // vector to hold integral error for Mahony method

	// global constants for 9 DoF fusion and AHRS (Attitude and Heading Reference System)
	float GyroMeasError;
	float GyroMeasDrift;

	float beta; // compute beta
	float zeta;// compute zeta, the other free parameter in the Madgwick scheme usually set to a small or zero value



public:
    AHRS();
    void madgwickQuaternionUpdate(float deltat, float* a, float* g, float* m);
    void mahonyQuaternionUpdate(float deltat, float* a, float* g, float* m);

    void getEuler (float* y, float* p, float* r);
};



#endif
