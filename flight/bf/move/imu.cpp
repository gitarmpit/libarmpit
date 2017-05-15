#include "board.h"
#include "imu.h"


// the correction that will be applied at throttle_correction_angle
uint8_t throttle_correction_value = 0;

Imu::Imu(float acc_1G, float gyroScale, float magDecl)
{
    this->gyroScale = gyroScale;

    //TODO make a cfg
    uint8_t small_angle = 15;

    // the angle when the throttle correction is maximal. in 0.1 degres, ex 225 = 22.5 ,30.0, 450 = 45.0 deg
    uint16_t throttle_correction_angle = 800;


    smallAngle = lrintf(acc_1G * cosf(RAD * small_angle));
    accVelScale = 9.80665f / acc_1G / 10000.0f;
    throttleAngleScale = (1800.0f / M_PI) * (900.0f / throttle_correction_angle);
    fc_acc = 0.5f / (M_PI * cfg.accz_lpf_cutoff); // calculate RC time constant used in the accZ lpf

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


// Normalize a vector
void normalizeV(struct fp_vector *src, struct fp_vector *dest)
{
    float length;

    length = sqrtf(src->X * src->X + src->Y * src->Y + src->Z * src->Z);
    if (length != 0) {
        dest->X = src->X / length;
        dest->Y = src->Y / length;
        dest->Z = src->Z / length;
    }
}

// Rotate Estimated vector(s) with small angle approximation, according to the gyro data
void rotateV(struct fp_vector *v, float *delta)
{
    struct fp_vector v_tmp = *v;

    // This does a  "proper" matrix rotation using gyro deltas without small-angle approximation
    float mat[3][3];
    float cosx, sinx, cosy, siny, cosz, sinz;
    float coszcosx, sinzcosx, coszsinx, sinzsinx;

    cosx = cosf(delta[ROLL]);
    sinx = sinf(delta[ROLL]);
    cosy = cosf(delta[PITCH]);
    siny = sinf(delta[PITCH]);
    cosz = cosf(delta[YAW]);
    sinz = sinf(delta[YAW]);

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

    v->X = v_tmp.X * mat[0][0] + v_tmp.Y * mat[1][0] + v_tmp.Z * mat[2][0];
    v->Y = v_tmp.X * mat[0][1] + v_tmp.Y * mat[1][1] + v_tmp.Z * mat[2][1];
    v->Z = v_tmp.X * mat[0][2] + v_tmp.Y * mat[1][2] + v_tmp.Z * mat[2][2];
}

int32_t applyDeadband(int32_t value, int32_t deadband)
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
void Imu::acc_calc(uint32_t deltaT)
{
    static int32_t accZoffset = 0;
    static float accz_smooth = 0;
    float dT = 0;
    float rpy[3];
    t_fp_vector accel_ned;

    // deltaT is measured in us ticks
    dT = (float)deltaT * 1e-6f;

    // the accel values have to be rotated into the earth frame
    rpy[0] = -(float)anglerad[ROLL];
    rpy[1] = -(float)anglerad[PITCH];
    rpy[2] = -(float)heading * RAD;

    accel_ned.V.X = accSmooth[0];
    accel_ned.V.Y = accSmooth[1];
    accel_ned.V.Z = accSmooth[2];

    rotateV(&accel_ned.V, rpy);

    if (cfg.acc_unarmedcal == 1)
    {
        if (!f.ARMED) {
            accZoffset -= accZoffset / 64;
            accZoffset += accel_ned.V.Z;
        }
        accel_ned.V.Z -= accZoffset / 64;  // compensate for gravitation on z-axis
    } else
        accel_ned.V.Z -= acc_1G;

    accz_smooth = accz_smooth + (dT / (fc_acc + dT)) * (accel_ned.V.Z - accz_smooth); // low pass filter

    // apply Deadband to reduce integration drift and vibration influence and
    // sum up Values for later integration to get velocity and distance
    accSum[X] += applyDeadband(lrintf(accel_ned.V.X), cfg.accxy_deadband);
    accSum[Y] += applyDeadband(lrintf(accel_ned.V.Y), cfg.accxy_deadband);
    accSum[Z] += applyDeadband(lrintf(accz_smooth), cfg.accz_deadband);

    accTimeSum += deltaT;
    accSumCount++;
}

void Imu::accSum_reset(void)
{
    accSum[0] = 0;
    accSum[1] = 0;
    accSum[2] = 0;
    accSumCount = 0;
    accTimeSum = 0;
}

// baseflight calculation by Luggi09 originates from arducopter
int16_t Imu::calculateHeading(t_fp_vector *vec)
{
    int16_t head;

    float cosineRoll = cosf(anglerad[ROLL]);
    float sineRoll = sinf(anglerad[ROLL]);
    float cosinePitch = cosf(anglerad[PITCH]);
    float sinePitch = sinf(anglerad[PITCH]);
    float Xh = vec->A[X] * cosinePitch + vec->A[Y] * sineRoll * sinePitch + vec->A[Z] * sinePitch * cosineRoll;
    float Yh = vec->A[Y] * cosineRoll - vec->A[Z] * sineRoll;
    float hd = (atan2f(Yh, Xh) * 1800.0f / M_PI + magneticDeclination) / 10.0f;
    head = lrintf(hd);
    if (head < 0)
        head += 360;

    return head;
}

void Imu::calculateAttitude(int32_t* accADC, int32_t* gyroADC, int32_t* magADC, uint32_t deltaT)
{
    int32_t axis;
    int32_t accMag = 0;
    static t_fp_vector EstM;
    static t_fp_vector EstN; // = { .A = { 1.0f, 0.0f, 0.0f } };
    EstN.A[0] = 1.0f;
    EstN.A[1] = 0.0f;
    EstN.A[2] = 0.0f;

    static float accLPF[3];
    float scale, deltaGyroAngle[3];
    scale = deltaT * gyroScale;

    // Initialization
    for (axis = 0; axis < 3; axis++)
    {
        deltaGyroAngle[axis] = gyroADC[axis] * scale;
        if (cfg.acc_lpf_factor > 0)
        {
            accLPF[axis] = accLPF[axis] * (1.0f - (1.0f / cfg.acc_lpf_factor)) + accADC[axis] * (1.0f / cfg.acc_lpf_factor);
            accSmooth[axis] = accLPF[axis];
        }
        else
        {
            accSmooth[axis] = accADC[axis];
        }
        accMag += (int32_t)accSmooth[axis] * accSmooth[axis];
    }
    accMag = accMag * 100 / ((int32_t)acc_1G * acc_1G);

    rotateV(&EstG.V, deltaGyroAngle);

    // Apply complimentary filter (Gyro drift correction)
    // If accel magnitude >1.15G or <0.85G and ACC vector outside of the limit range => we neutralize the effect of accelerometers in the angle estimation.
    // To do that, we just skip filter, as EstV already rotated by Gyro
    if (72 < (uint16_t)accMag && (uint16_t)accMag < 133)
    {
        for (axis = 0; axis < 3; axis++)
            EstG.A[axis] = (EstG.A[axis] * (float)mcfg.gyro_cmpf_factor + accSmooth[axis]) * INV_GYR_CMPF_FACTOR;
    }

    f.SMALL_ANGLE = (EstG.A[Z] > smallAngle);

    // Attitude of the estimated vector
    anglerad[ROLL] = atan2f(EstG.V.Y, EstG.V.Z);
    anglerad[PITCH] = atan2f(-EstG.V.X, sqrtf(EstG.V.Y * EstG.V.Y + EstG.V.Z * EstG.V.Z));
    angle[ROLL] = lrintf(anglerad[ROLL] * (1800.0f / M_PI));
    angle[PITCH] = lrintf(anglerad[PITCH] * (1800.0f / M_PI));

    rotateV(&EstM.V, deltaGyroAngle);
    for (axis = 0; axis < 3; axis++)
    {
        EstM.A[axis] = (EstM.A[axis] * (float)mcfg.gyro_cmpfm_factor + magADC[axis]) * INV_GYR_CMPFM_FACTOR;
    }

    heading = calculateHeading(&EstM);

    acc_calc(deltaT); // rotate acc vector into earth frame

    if (throttle_correction_value)
    {

        float cosZ = EstG.V.Z / sqrtf(EstG.V.X * EstG.V.X + EstG.V.Y * EstG.V.Y + EstG.V.Z * EstG.V.Z);

        if (cosZ <= 0.015f) { // we are inverted, vertical or with a small angle < 0.86 deg
            throttleAngleCorrection = 0;
        } else {
            int deg = lrintf(acosf(cosZ) * throttleAngleScale);
            if (deg > 900)
                deg = 900;
            throttleAngleCorrection = lrintf(throttle_correction_value * sinf(deg / (900.0f * M_PI / 2.0f)));
        }

    }
}

