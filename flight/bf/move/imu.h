#ifndef _IMU_H
#define _IMU_H

#include <stdint.h>

enum {
    ROLL = 0,
    PITCH,
    YAW,
    THROTTLE,
};

#define INV_GYR_CMPF_FACTOR   (1.0f / ((float)mcfg.gyro_cmpf_factor + 1.0f))
#define INV_GYR_CMPFM_FACTOR  (1.0f / ((float)mcfg.gyro_cmpfm_factor + 1.0f))

typedef struct fp_vector {
    float X;
    float Y;
    float Z;
} t_fp_vector_def;

typedef union {
    float A[3];
    t_fp_vector_def V;
} t_fp_vector;


class Imu
{
private:

    float acc_1G;
    float magDecl;
    int16_t accSmooth[3];

    float gyroScale;
    int16_t angle[2] = { 0, 0 };     // absolute angle inclination in multiple of 0.1 degree    180 deg = 1800

    int16_t heading = 0;


    int32_t accSum[3];
    uint32_t accTimeSum = 0;        // keep track for integration of acc
    int accSumCount = 0;
    int16_t smallAngle = 0;
    int32_t baroPressure = 0;
    int32_t baroTemperature = 0;
    uint32_t baroPressureSum = 0;
    int32_t BaroAlt = 0;
    float sonarTransition = 0;
    int32_t baroAlt_offset = 0;
    int32_t sonarAlt = -1;         // in cm , -1 indicate sonar is not in range
    int32_t EstAlt;                // in cm
    int32_t BaroPID = 0;
    int32_t AltHold;
    int32_t setVelocity = 0;
    uint8_t velocityControl = 0;
    int32_t errorVelocityI = 0;
    int32_t vario = 0;                      // variometer in cm/s
    int16_t throttleAngleCorrection = 0;    // correction of throttle in lateral wind,
    float magneticDeclination = 0.0f;       // calculated at startup from config
    float accVelScale;
    float throttleAngleScale;
    float fc_acc;

    float anglerad[2] = { 0.0f, 0.0f };    // absolute angle inclination in radians
    t_fp_vector EstG;

    void acc_calc(uint32_t deltaT);
    void accSum_reset(void);
    int16_t calculateHeading(t_fp_vector *vec);


public:

    Imu(float acc_1G, float gyroScale, float magDecl);
    void calculateAttitude(int32_t* accADC, int32_t* gyroADC, int32_t* magADC, uint32_t deltaT);


};

#endif

        
        

        

        
