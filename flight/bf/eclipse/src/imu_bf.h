#ifndef _IMU_H
#define _IMU_H

#include <stdint.h>


#define INV_GYR_CMPF_FACTOR   (1.0f / ((float)mcfg.gyro_cmpf_factor + 1.0f))
#define INV_GYR_CMPFM_FACTOR  (1.0f / ((float)mcfg.gyro_cmpfm_factor + 1.0f))

//typedef struct fp_vector {
//    float X;
//    float Y;
//    float Z;
//} t_fp_vector_def;
//
//typedef union {
//    float A[3];
//    t_fp_vector_def V;
//} t_fp_vector;


class Imu_bf
{
private:

    float acc_1G;
    float gyroScale;

    // pitch/roll: absolute angle inclination in multiple of 0.1 degree    180 deg = 1800
    //yaw: 1 degrees
    int16_t attitude[3] = { 0, 0, 0 };


    int32_t accSum[3];
    uint32_t accTimeSum = 0;        // keep track for integration of acc
    int accSumCount = 0;
    int16_t smallAngle = 0;
    float magneticDeclination = 0.0f;       // calculated at startup from config
    float accVelScale;
    float throttleAngleScale;
    float fc_acc;
    float anglerad[2] = { 0.0f, 0.0f };    // absolute angle inclination in radians
    float EstG[3];
    bool isSmallAngle;

    //    int32_t baroPressure = 0;
//    int32_t baroTemperature = 0;
//    uint32_t baroPressureSum = 0;
//    int32_t BaroAlt = 0;
//    float sonarTransition = 0;
//    int32_t baroAlt_offset = 0;
//    int32_t sonarAlt = -1;         // in cm , -1 indicate sonar is not in range
//    int32_t EstAlt;                // in cm
//    int32_t BaroPID = 0;
//    int32_t AltHold;
//    int32_t setVelocity = 0;
//    uint8_t velocityControl = 0;
//    int32_t errorVelocityI = 0;
//    int32_t vario = 0;                      // variometer in cm/s


    void acc_calc(int16_t* acc, uint32_t deltaT, bool armed);
    void accSum_reset(void);
    int16_t calculateHeading(float *vec);

    static void rotateV(float *v, float *delta);
    static int32_t applyDeadband(int32_t value, int32_t deadband);

    void calculateAcceleration(int32_t* accADC, uint32_t deltaT, bool armed, int32_t* acc_Z);

public:

    Imu_bf(float acc_1G, float gyroScale, float magDecl);
    void calculateAttitude(int32_t* accADC, int32_t* gyroADC, int32_t* magADC, uint32_t deltaT,
            bool armed, int32_t* acc_Z);

    int16_t calcualteThrottleCorrection();



};

#endif

        
        

        

        
