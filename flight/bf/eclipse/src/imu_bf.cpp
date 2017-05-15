#include "imu_bf.h"
#include <math.h>
#include "bf_common.h"


//TODO make a cfg
uint8_t small_angle = 15;
uint8_t throttle_correction_value = 0;
// the angle when the throttle correction is maximal. in 0.1 degres, ex 225 = 22.5 ,30.0, 450 = 45.0 deg
uint16_t throttle_correction_angle = 800;

// cutoff frequency for the low pass filter used on the acc z-axis for althold in Hz
uint8_t acc_lpf_factor = 4;
uint8_t accz_deadband = 40;
uint8_t accxy_deadband = 40;
float accz_lpf_cutoff = 5.0f;
bool acc_unarmedcal = true;

// Set the Gyro Weight for Gyro/Acc complementary filter.
//Increasing this value would reduce and delay Acc influence on the output of the filter.
uint16_t gyro_cmpf_factor = 600;

//Set the Gyro Weight for Gyro/Magnetometer complementary filter.
//Increasing this value would reduce and delay Magnetometer influence on the output of the filter
uint16_t gyro_cmpfm_factor = 250;


Imu_bf::Imu_bf(float acc_1G, float gyroScale, float magDeclination)
{
    this->acc_1G = acc_1G;
    this->gyroScale = gyroScale;
    magneticDeclination = magDeclination;

    smallAngle = lrintf(acc_1G * cosf(M_PI / 180. * small_angle));
    accVelScale = 9.80665f / acc_1G / 10000.0f;
    throttleAngleScale = (1800.0f / M_PI) * (900.0f / throttle_correction_angle);
    fc_acc = 0.5f / (M_PI * accz_lpf_cutoff); // calculate RC time constant used in the accZ lpf

    isSmallAngle = true;

}


// **************************************************
// Simplified IMU based on "Complementary Filter"
// Inspired by http://starlino.com/imu_guide.html
//
// adapted by ziss_dm : http://www.multiwii.com/forum/viewtopic.php?f=8&t=198
//
// The following ideas was used in this project:
// 1) Rotation matrix: http://en.wikipedia.org/wiki/Rotation_matrix
//
// Currently Magnetometer uses separate CF which is used only
// for heading approximation.
//
// **************************************************


//// Normalize a vector
//static void normalizeV(float* src, float *dest)
//{
//    float length;
//
//    length = sqrtf(src[X] * src[X] + src[Y] * src[Y] + src[Z] * src[Z]);
//    if (length != 0) {
//        dest[X] = src[X] / length;
//        dest[Y] = src[Y] / length;
//        dest[Z] = src[Z] / length;
//    }
//}

// Rotate Estimated vector(s) with small angle approximation, according to the gyro data
void Imu_bf::rotateV(float *v, float *delta)
{
    float v_tmp[3] = {v[0], v[1], v[2]};

    // This does a  "proper" matrix rotation using gyro deltas without small-angle approximation
    float mat[3][3];
    float cosx, sinx, cosy, siny, cosz, sinz;
    float coszcosx, sinzcosx, coszsinx, sinzsinx;

    cosx = cosf(delta[X]);
    sinx = sinf(delta[X]);
    cosy = cosf(delta[Y]);
    siny = sinf(delta[Y]);
    cosz = cosf(delta[Z]);
    sinz = sinf(delta[Z]);

    coszcosx = cosz * cosx;
    sinzcosx = sinz * cosx;
    coszsinx = sinx * cosz;
    sinzsinx = sinx * sinz;

    mat[0][0] = cosz * cosy;
    mat[0][1] = -cosy * sinz;
    mat[0][2] = siny;
    mat[1][0] = sinzcosx + (coszsinx * siny);
    mat[1][1] = coszcosx - (sinzsinx * siny);
    mat[1][2] = -sinx * cosy;
    mat[2][0] = (sinzsinx) - (coszcosx * siny);
    mat[2][1] = (coszsinx) + (sinzcosx * siny);
    mat[2][2] = cosy * cosx;

    v[X] = v_tmp[X] * mat[0][0] + v_tmp[Y] * mat[1][0] + v_tmp[Z] * mat[2][0];
    v[Y] = v_tmp[X] * mat[0][1] + v_tmp[Y] * mat[1][1] + v_tmp[Z] * mat[2][1];
    v[Z] = v_tmp[X] * mat[0][2] + v_tmp[Y] * mat[1][2] + v_tmp[Z] * mat[2][2];
}

int32_t Imu_bf::applyDeadband(int32_t value, int32_t deadband)
{
    if (abs(value) < deadband) {
        value = 0;
    } else if (value > 0) {
        value -= deadband;
    } else if (value < 0) {
        value += deadband;
    }
    return value;
}

// rotate acc into Earth frame and calculate acceleration in it
//void Imu::acc_calc(int16_t* acc, uint32_t deltaT, bool armed)
void Imu_bf::calculateAcceleration(int32_t* accADC, uint32_t deltaT, bool armed, int32_t* acc_Z)
{
    static int32_t accZoffset = 0;
    static float accz_smooth = 0;
    float dT = 0;
    float rpy[3];
    float accel_ned[3];

    // deltaT is measured in us ticks
    dT = (float)deltaT * 1e-6f;

    // the accel values have to be rotated into the earth frame
    rpy[0] = -(float)anglerad[ROLL];
    rpy[1] = -(float)anglerad[PITCH];
    rpy[2] = -(float)attitude[YAW] * M_PI / 180.;


    accel_ned[X] = accADC[0];
    accel_ned[Y] = accADC[1];
    accel_ned[Z] = accADC[2];

    rotateV(accel_ned, rpy);

    if (acc_unarmedcal == 1)
    {
        if (!armed) {
            accZoffset -= accZoffset / 64;
            accZoffset += accel_ned[Z];
        }
        accel_ned[Z] -= accZoffset / 64;  // compensate for gravitation on z-axis
    }
    else
    {
        accel_ned[Z] -= acc_1G;
    }

    accz_smooth = accz_smooth + (dT / (fc_acc + dT)) * (accel_ned[Z] - accz_smooth); // low pass filter


    //TODO move this outside
    // apply Deadband to reduce integration drift and vibration influence and
    // sum up Values for later integration to get velocity and distance
    accSum[X] += applyDeadband(lrintf(accel_ned[X]), accxy_deadband);
    accSum[Y] += applyDeadband(lrintf(accel_ned[Y]), accxy_deadband);
    accSum[Z] += applyDeadband(lrintf(accz_smooth), accz_deadband);

    accTimeSum += deltaT;
    accSumCount++;

    *acc_Z = applyDeadband(lrintf(accz_smooth), accz_deadband);
}


//TODO move this
void Imu_bf::accSum_reset(void)
{
    accSum[0] = 0;
    accSum[1] = 0;
    accSum[2] = 0;
    accSumCount = 0;
    accTimeSum = 0;
}

// baseflight calculation by Luggi09 originates from arducopter
int16_t Imu_bf::calculateHeading(float *vec)
{
    float cosineRoll = cosf(anglerad[ROLL]);
    float sineRoll = sinf(anglerad[ROLL]);
    float cosinePitch = cosf(anglerad[PITCH]);
    float sinePitch = sinf(anglerad[PITCH]);
    float Xh = vec[X] * cosinePitch + vec[Y] * sineRoll * sinePitch + vec[Z] * sinePitch * cosineRoll;
    float Yh = vec[Y] * cosineRoll - vec[Z] * sineRoll;
    float hd = (atan2f(Yh, Xh) * 1800.0f / M_PI + magneticDeclination) / 10.0f;
    int16_t head = lrintf(hd);
    if (head < 0)
        head += 360;

    return head;
}


void Imu_bf::calculateAttitude(int32_t* accADC, int32_t* gyroADC, int32_t* magADC,
        uint32_t deltaT, bool armed, int32_t* acc_Z)
{
    static float inv_cmpf_factor = 1.0f / ((float)gyro_cmpf_factor + 1.0f);
    static float inv_cmpfm_factor = 1.0f / ((float)gyro_cmpfm_factor + 1.0f);
    static int32_t accSmooth[3]= {0,0,0};
    static float accLPF[3];

    int32_t axis;
    int32_t accMag = 0;
    static float EstM[3];

    float scale, deltaGyroAngle[3];
    scale = deltaT * gyroScale;

    // Initialization
    for (axis = 0; axis < 3; axis++)
    {
        if (acc_lpf_factor > 0)
        {
            accLPF[axis] = accLPF[axis] * (1.0f - (1.0f / acc_lpf_factor)) + accADC[axis] * (1.0f / acc_lpf_factor);
            accSmooth[axis] = accLPF[axis];
        }
        else
        {
            accSmooth[axis] = accADC[axis];
        }

        deltaGyroAngle[axis] = gyroADC[axis] * scale;
        accMag += (int32_t)accSmooth[axis] * accSmooth[axis];
    }
    accMag = accMag * 100 / ((int32_t)acc_1G * acc_1G);

    rotateV(EstG, deltaGyroAngle);

    // Apply complimentary filter (Gyro drift correction)
    // If accel magnitude >1.15G or <0.85G and ACC vector outside of the limit range => we neutralize the effect of accelerometers in the angle estimation.
    // To do that, we just skip filter, as EstV already rotated by Gyro
    if (72 < (uint16_t)accMag && (uint16_t)accMag < 133)
    {
        for (axis = 0; axis < 3; axis++)
            EstG[axis] = (EstG[axis] * (float)gyro_cmpf_factor + accSmooth[axis]) * inv_cmpf_factor;
    }

    isSmallAngle = (EstG[Z] > smallAngle);

    // Attitude of the estimated vector
    anglerad[ROLL] = atan2f(EstG[Y], EstG[Z]);
    anglerad[PITCH] = atan2f(-EstG[X], sqrtf(EstG[Y] * EstG[Y] + EstG[Z] * EstG[Z]));

    attitude[ROLL] = lrintf(anglerad[ROLL] * (1800.0f / M_PI));
    attitude[PITCH] = lrintf(anglerad[PITCH] * (1800.0f / M_PI));

    rotateV(EstM, deltaGyroAngle);
    for (axis = 0; axis < 3; axis++)
    {
        EstM[axis] = (EstM[axis] * (float)gyro_cmpfm_factor + magADC[axis]) * inv_cmpfm_factor;
    }

    attitude[YAW] = calculateHeading(EstM);

    calculateAcceleration(accSmooth, deltaT, armed, acc_Z); // rotate acc vector into earth frame
    calcualteThrottleCorrection();
}

//TODO remove from here
int16_t Imu_bf::calcualteThrottleCorrection()
{
	int16_t throttleAngleCorrection = 0;
    if (throttle_correction_value)
    {

        float cosZ = EstG[Z] / sqrtf(EstG[X] * EstG[X] + EstG[Y] * EstG[Y] + EstG[Z] * EstG[Z]);

        if (cosZ <= 0.015f) { // we are inverted, vertical or with a small angle < 0.86 deg
            throttleAngleCorrection = 0;
        }
        else
        {
            int deg = lrintf(acosf(cosZ) * throttleAngleScale);
            if (deg > 900)
                deg = 900;
            throttleAngleCorrection = lrintf(throttle_correction_value * sinf(deg / (900.0f * M_PI / 2.0f)));
        }

    }
    return throttleAngleCorrection;
}


