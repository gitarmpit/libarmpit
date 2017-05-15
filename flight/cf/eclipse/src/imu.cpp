#include "imu.h"
#include "filter.h"


// the limit (in degrees/second) beyond which we stop integrating
// omega_I. At larger spin rates the DCM PI controller can get 'dizzy'
// which results in false gyro drift. See
// http://gentlenav.googlecode.com/files/fastRotations.pdf
#define SPIN_RATE_LIMIT 20


Imu::Imu(imuConfig_t* cfg, float acc_1G, float gyroScale, float magDecl)
{
    this->acc_1G = acc_1G;
    this->magDecl = magDecl;

    throttleAngleCorrection = 0;
    q0 = 1.0f;
    q1 = 0.0f;
    q2 = 0.0f;
    q3 = 0.0f;    // quaternion of sensor frame relative to earth frame

    smallAngle = false;

    imuConfig = cfg;

    dcm_kp = imuConfig->dcm_kp / 10000.0f;
    dcm_ki = imuConfig->dcm_ki / 10000.0f;

    //init
    fc_acc = calculateAccZLowPassFilterRCTimeConstant(imuConfig->accz_lpf_cutoff);
    throttleAngleScale = calculateThrottleAngleScale(imuConfig->throttle_correction_angle);

    smallAngleCosZ = cos_approx(degreesToRadians(imuConfig->small_angle));
    this->gyroScale = gyroScale * (M_PIf / 180.0f);  // gyro output scaled to rad per second
    accVelScale = 9.80665f / acc_1G / 10000.0f;

    imuComputeRotationMatrix();
}

void Imu::imuComputeRotationMatrix(void)
{
    float q1q1 = sq(q1);
    float q2q2 = sq(q2);
    float q3q3 = sq(q3);

    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q0q3 = q0 * q3;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q3 = q2 * q3;

    rMat[0][0] = 1.0f - 2.0f * q2q2 - 2.0f * q3q3;
    rMat[0][1] = 2.0f * (q1q2 + -q0q3);
    rMat[0][2] = 2.0f * (q1q3 - -q0q2);

    rMat[1][0] = 2.0f * (q1q2 - -q0q3);
    rMat[1][1] = 1.0f - 2.0f * q1q1 - 2.0f * q3q3;
    rMat[1][2] = 2.0f * (q2q3 + -q0q1);

    rMat[2][0] = 2.0f * (q1q3 + -q0q2);
    rMat[2][1] = 2.0f * (q2q3 - -q0q1);
    rMat[2][2] = 1.0f - 2.0f * q1q1 - 2.0f * q2q2;
}

float Imu::calculateThrottleAngleScale(uint16_t throttle_correction_angle)
{
    return (1800.0f / M_PIf) * (900.0f / throttle_correction_angle);
}

/*
* Calculate RC time constant used in the accZ lpf.
*/
float Imu::calculateAccZLowPassFilterRCTimeConstant(float accz_lpf_cutoff)
{
    return 0.5f / (M_PIf * accz_lpf_cutoff);
}



void Imu::imuTransformVectorBodyToEarth(t_fp_vector * v)
{
    float x,y,z;

    /* From body frame to earth frame */
    x = rMat[0][0] * v->V.X + rMat[0][1] * v->V.Y + rMat[0][2] * v->V.Z;
    y = rMat[1][0] * v->V.X + rMat[1][1] * v->V.Y + rMat[1][2] * v->V.Z;
    z = rMat[2][0] * v->V.X + rMat[2][1] * v->V.Y + rMat[2][2] * v->V.Z;

    v->V.X = x;
    v->V.Y = -y;
    v->V.Z = z;
}

// rotate acc into Earth frame and calculate acceleration in it
void Imu::calculateAcceleration(int32_t* accADC, uint32_t deltaT, bool armed, int32_t* acc_Z)
{
    static int32_t accZoffset = 0;
    static float accz_smooth = 0;
    float dT;
    t_fp_vector accel_ned;

    // deltaT is measured in us ticks
    dT = (float)deltaT * 1e-6f;

    accel_ned.V.X = accADC[0];
    accel_ned.V.Y = accADC[1];
    accel_ned.V.Z = accADC[2];

    imuTransformVectorBodyToEarth(&accel_ned);

    if (imuConfig->acc_unarmedcal == 1)
    {
        if (!armed)
        {
            accZoffset -= accZoffset / 64;
            accZoffset += accel_ned.V.Z;
        }
        accel_ned.V.Z -= accZoffset / 64;  // compensate for gravitation on z-axis
    }
    else
    {
        accel_ned.V.Z -= acc_1G;
    }

    accz_smooth = accz_smooth + (dT / (fc_acc + dT)) * (accel_ned.V.Z - accz_smooth); // low pass filter

    // apply Deadband to reduce integration drift and vibration influence
    //acc[0] = applyDeadband(lrintf(accel_ned.V.X), imuConfig->accDeadband_xy);
    //acc[1] = applyDeadband(lrintf(accel_ned.V.Y), imuConfig->accDeadband_xy);

    *acc_Z = applyDeadband(lrintf(accz_smooth), imuConfig->accDeadband_z);

}

static float invSqrt(float x)
{
    return 1.0f / sqrtf(x);
}



void Imu::imuMahonyAHRSupdate(float dt, float gx, float gy, float gz,
                                bool useAcc, float ax, float ay, float az,
                                bool useMag, float mx, float my, float mz,
                                bool useYaw, float yawError, float gainScaleFactor)
{
    static float integralFBx = 0.0f,  integralFBy = 0.0f, integralFBz = 0.0f;    // integral error terms scaled by Ki
    float recipNorm;
    float hx, hy, bx;
    float ex = 0, ey = 0, ez = 0;
    float qa, qb, qc;

    // Calculate general spin rate (rad/s)
    float spin_rate = sqrtf(sq(gx) + sq(gy) + sq(gz));

    // Use raw heading error (from GPS or whatever else)
    if (useYaw) {
        while (yawError >  M_PIf) yawError -= (2.0f * M_PIf);
        while (yawError < -M_PIf) yawError += (2.0f * M_PIf);

        ez += sin_approx(yawError / 2.0f);
    }

    // Use measured magnetic field vector
    recipNorm = sq(mx) + sq(my) + sq(mz);
    if (useMag && recipNorm > 0.01f) {
        // Normalise magnetometer measurement
        recipNorm = invSqrt(recipNorm);
        mx *= recipNorm;
        my *= recipNorm;
        mz *= recipNorm;

        // For magnetometer correction we make an assumption that magnetic field is perpendicular to gravity (ignore Z-component in EF).
        // This way magnetic field will only affect heading and wont mess roll/pitch angles

        // (hx; hy; 0) - measured mag field vector in EF (assuming Z-component is zero)
        // (bx; 0; 0) - reference mag field vector heading due North in EF (assuming Z-component is zero)
        hx = rMat[0][0] * mx + rMat[0][1] * my + rMat[0][2] * mz;
        hy = rMat[1][0] * mx + rMat[1][1] * my + rMat[1][2] * mz;
        bx = sqrtf(hx * hx + hy * hy);

        // magnetometer error is cross product between estimated magnetic north and measured magnetic north (calculated in EF)
        float ez_ef = -(hy * bx);

        // Rotate mag error vector back to BF and accumulate
        ex += rMat[2][0] * ez_ef;
        ey += rMat[2][1] * ez_ef;
        ez += rMat[2][2] * ez_ef;
    }

    // Use measured acceleration vector
    recipNorm = sq(ax) + sq(ay) + sq(az);
    if (useAcc && recipNorm > 0.01f) {
        // Normalise accelerometer measurement
        recipNorm = invSqrt(recipNorm);
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        // Error is sum of cross product between estimated direction and measured direction of gravity
        ex += (ay * rMat[2][2] - az * rMat[2][1]);
        ey += (az * rMat[2][0] - ax * rMat[2][2]);
        ez += (ax * rMat[2][1] - ay * rMat[2][0]);
    }

    // Compute and apply integral feedback if enabled
    if(dcm_ki > 0.0f) {
        // Stop integrating if spinning beyond the certain limit
        if (spin_rate < DEGREES_TO_RADIANS(SPIN_RATE_LIMIT)) {
            float dcmKiGain = dcm_ki;
            integralFBx += dcmKiGain * ex * dt;    // integral error scaled by Ki
            integralFBy += dcmKiGain * ey * dt;
            integralFBz += dcmKiGain * ez * dt;
        }
    }
    else {
        integralFBx = 0.0f;    // prevent integral windup
        integralFBy = 0.0f;
        integralFBz = 0.0f;
    }

    // Calculate kP gain. If we are acquiring initial attitude (not armed and within 20 sec from powerup) scale the kP to converge faster
    float dcmKpGain = dcm_kp * gainScaleFactor;

    // Apply proportional and integral feedback
    gx += dcmKpGain * ex + integralFBx;
    gy += dcmKpGain * ey + integralFBy;
    gz += dcmKpGain * ez + integralFBz;

    // Integrate rate of change of quaternion
    gx *= (0.5f * dt);
    gy *= (0.5f * dt);
    gz *= (0.5f * dt);

    qa = q0;
    qb = q1;
    qc = q2;
    q0 += (-qb * gx - qc * gy - q3 * gz);
    q1 += (qa * gx + qc * gz - q3 * gy);
    q2 += (qa * gy - qb * gz + q3 * gx);
    q3 += (qa * gz + qb * gy - qc * gx);

    // Normalise quaternion
    recipNorm = invSqrt(sq(q0) + sq(q1) + sq(q2) + sq(q3));
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;

}

void Imu::imuUpdateEulerAngles(void)
{
    /* Compute pitch/roll angles */
    attitude.values.roll = lrintf(atan2_approx(rMat[2][1], rMat[2][2]) * (1800.0f / M_PIf));
    attitude.values.pitch = lrintf(((0.5f * M_PIf) - acos_approx(-rMat[2][0])) * (1800.0f / M_PIf));
    attitude.values.yaw = lrintf((-atan2_approx(rMat[1][0], rMat[0][0]) * (1800.0f / M_PIf) + magDecl));

    if (attitude.values.yaw < 0)
        attitude.values.yaw += 3600;

    /* Update small angle state */
    if (rMat[2][2] > smallAngleCosZ) {
        smallAngle = true;
    } else {
        smallAngle = false;
    }
}

bool Imu::imuIsAircraftArmable(uint8_t arming_angle)
{
    /* Update small angle state */
    
    float armingAngleCosZ = cos_approx(degreesToRadians(arming_angle));
    
    return (rMat[2][2] > armingAngleCosZ);
}

bool Imu::imuIsAccelerometerHealthy(int32_t* accADC)
{
    int32_t axis;
    int32_t accMagnitude = 0;

    for (axis = 0; axis < 3; axis++) {
        accMagnitude += (int32_t)accADC[axis] * accADC[axis];
    }

    accMagnitude = accMagnitude * 100 / (sq((int32_t)acc_1G));

    // Accept accel readings only in range 0.90g - 1.10g
    return (81 < accMagnitude) && (accMagnitude < 121);
}



void Imu::calculateAttitude(int32_t* accADC, int32_t* gyroADC, int32_t* magADC, uint32_t deltaT, bool fastGain)
{
    float rawYawError = 0;
    bool useAcc = false;
    bool useMag = false;
    bool useYaw = false;

    if (imuIsAccelerometerHealthy(accADC)) {
        useAcc = true;
    }

    if (magADC[0] != 0 && magADC[1] != 0 && magADC[2] != 0)
    {
        useMag = true;
    }

#if defined(GPS)
    else if (STATE(FIXED_WING) && sensors(SENSOR_GPS) && STATE(GPS_FIX) && GPS_numSat >= 5 && GPS_speed >= 300) {
        // In case of a fixed-wing aircraft we can use GPS course over ground to correct heading
        rawYawError = DECIDEGREES_TO_RADIANS(attitude.values.yaw - GPS_ground_course);
        useYaw = true;
    }
#endif


    imuMahonyAHRSupdate(deltaT * 1e-6f,
                        gyroADC[0] * gyroScale, gyroADC[1] * gyroScale, gyroADC[2] * gyroScale,
                        useAcc, accADC[0], accADC[1], accADC[2],
                        useMag, magADC[0], magADC[1], magADC[2],
                        useYaw, rawYawError, fastGain ? 10.0f : 1.0f);

    imuComputeRotationMatrix();
    imuUpdateEulerAngles();
}



float Imu::getCosTiltAngle(void)
{
    return rMat[2][2];
}

//TODO why is it here???
int16_t Imu::calculateThrottleAngleCorrection()
{
    /*
    * Use 0 as the throttle angle correction if we are inverted, vertical or with a
    * small angle < 0.86 deg
    * TODO: Define this small angle in config.
    */
    if (rMat[2][2] <= 0.015f) {
        return 0;
    }
    int angle = lrintf(acos_approx(rMat[2][2]) * throttleAngleScale);
    if (angle > 900)
        angle = 900;
    return lrintf(imuConfig->throttle_correction_value * sin_approx(angle / (900.0f * M_PIf / 2.0f)));
}

void Imu::enableSmallAngle (bool enable)
{
    smallAngle = enable;
}
