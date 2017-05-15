#ifndef _IMU_CF_H
#define _IMU_CF_H
#include <stdint.h>
#include "maths.h"


#define DEGREES_TO_DECIDEGREES(angle) (angle * 10)
#define DECIDEGREES_TO_DEGREES(angle) (angle / 10)
#define DECIDEGREES_TO_RADIANS(angle) ((angle / 10.0f) * 0.0174532925f)
#define DEGREES_TO_RADIANS(angle) ((angle) * 0.0174532925f)

typedef union {
    int16_t raw[3];
    struct {
        // absolute angle inclination in multiple of 0.1 degree    180 deg = 1800
        int16_t roll;
        int16_t pitch;
        int16_t yaw;
    } values;
} attitudeEulerAngles_t;






struct  imuConfig_t
{
    // IMU configuration
    uint8_t small_angle;                    // Angle used for mag hold threshold.
    uint16_t dcm_kp;                        // DCM filter proportional gain ( x 10000)
    uint16_t dcm_ki;                        // DCM filter integral gain ( x 10000)

    float accz_lpf_cutoff;                  // cutoff frequency for the low pass filter used on the acc z-axis for althold in Hz
    uint8_t accDeadband_xy;
    uint8_t accDeadband_z;
    uint8_t acc_unarmedcal;                 // turn automatic acc compensation on/off
    uint16_t throttle_correction_angle;     // the angle when the throttle correction is maximal. in 0.1 degres, ex 225 = 22.5 ,30.0, 450 = 45.0 deg
    uint8_t throttle_correction_value;      // the correction that will be applied at throttle_correction_angle.

};




class Imu
{
private:

    float acc_1G;
    float magDecl;


    int16_t throttleAngleCorrection;
    float accVelScale;

    imuConfig_t* imuConfig;

    float dcm_ki;
    float dcm_kp;
    float throttleAngleScale;
    float fc_acc;
    float smallAngleCosZ;
    float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;    // quaternion of sensor frame relative to earth frame
    float rMat[3][3];
    attitudeEulerAngles_t attitude = { { 0, 0, 0 } };     // absolute angle inclination in multiple of 0.1 degree    180 deg = 1800
    float gyroScale;
    bool smallAngle;

    void imuComputeRotationMatrix(void);

    float calculateThrottleAngleScale(uint16_t throttle_correction_angle);
    float calculateAccZLowPassFilterRCTimeConstant(float accz_lpf_cutoff);

    int16_t imuCalculateHeading(t_fp_vector *vec);



    bool imuIsAircraftArmable(uint8_t arming_angle);
    void imuTransformVectorBodyToEarth(t_fp_vector * v);
    void imuMahonyAHRSupdate(float dt, float gx, float gy, float gz,
            bool useAcc, float ax, float ay, float az,
            bool useMag, float mx, float my, float mz,
            bool useYaw, float yawError, float gainScaleFactor);

    void imuUpdateEulerAngles(void);
    bool imuIsAccelerometerHealthy(int32_t* accADC);



public:

    Imu(imuConfig_t* cfg, float acc_1G, float gyroScale, float magDecl);


    void calculateAttitude(int32_t* accADC, int32_t* gyroADC, int32_t* magADC, uint32_t deltaT, bool fastGain);
    //needed for alt hold
    void calculateAcceleration(int32_t* accADC, uint32_t deltaT, bool armed, int32_t* acc_Z);

    int16_t calculateThrottleAngleCorrection();

    //needed by altitude hold
    float getCosTiltAngle(void);

    int16_t* getAttitude() { return attitude.raw; }

    int16_t getYaw() { return attitude.values.yaw; }
    int16_t getPitch() { return attitude.values.pitch; }
    int16_t getRoll() { return attitude.values.roll; }


    uint8_t throttle_correction_value() { return imuConfig->throttle_correction_value; }

    void enableSmallAngle (bool enable);
    bool smallAngleEnabled() { return smallAngle; }



};

#endif

        
        

        

        
