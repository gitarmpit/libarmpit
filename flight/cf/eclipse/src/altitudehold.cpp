

#include <platform.h>
#include "altitudehold.h"

#include "maths.h"

#include "sensors/sonar_hcsr04.h"
#include "sensors/sensors.h"
#include "sensors/sonar.h"
#include "runtime_config.h"
#include "cfg.h"



// 40hz update rate (20hz LPF on acc)
#define BARO_UPDATE_FREQUENCY_40HZ (1000 * 25)

#define DEGREES_80_IN_DECIDEGREES 800

AltHold::AltHold(float acc_1G)
{
    accVelScale = 9.80665f / acc_1G / 10000.0f;

    altHold = 0;
    vario = 0;
    initialRawThrottleHold = 0;
    initialThrottleHold = 0;
    _estimatedAltitude = 0;
    setVelocity = 0;
    velocityControl = false;
    errorVelocityI = 0;
    altHoldThrottleAdjustment = 0;
    altHoldCfg = Config::GetInstance()->getAltHoldConfig();

}

int16_t AltHold::applyAltHold(int16_t thr_raw, int16_t thr)
{
    static bool isAltHoldChanged = false;

    if (altHoldCfg->alt_hold_fast_change)
    {
        // rapid alt changes
        if (ABS(thr_raw - initialRawThrottleHold) > altHoldCfg->alt_hold_deadband)
        {
            errorVelocityI = 0;
            isAltHoldChanged = true;
            thr += (thr_raw > initialRawThrottleHold) ? -altHoldCfg->alt_hold_deadband : altHoldCfg->alt_hold_deadband;
        }
        else
        {
            if (isAltHoldChanged)
            {
                altHold = _estimatedAltitude;
                isAltHoldChanged = false;
            }
            thr = initialThrottleHold + altHoldThrottleAdjustment;
        }
    }
    else
    {
        // slow alt changes, mostly used for aerial photography
        if (ABS(thr_raw - initialRawThrottleHold) > altHoldCfg->alt_hold_deadband)
        {
            // set velocity proportional to stick movement +100 throttle gives ~ +50 cm/s
            setVelocity = (thr_raw - initialRawThrottleHold) / 2;
            velocityControl = true;
            isAltHoldChanged = true;
        }
        else if (isAltHoldChanged)
        {
            altHold = _estimatedAltitude;
            velocityControl = false;
            isAltHoldChanged = false;
        }

        thr = initialThrottleHold + altHoldThrottleAdjustment;
    }

    return thr;
}



//TODO pass a flag or smth to replace BOXBARO
void AltHold::activateAltHold(int16_t raw_thr, int16_t thr)
{
    altHold = _estimatedAltitude;
    initialRawThrottleHold = raw_thr;
    initialThrottleHold = thr;
    errorVelocityI = 0;
    altHoldThrottleAdjustment = 0;
}


bool AltHold::isThrustFacingDownwards(float imuPitch, float imuRoll)
{
    return ABS(imuRoll) < DEGREES_80_IN_DECIDEGREES && ABS(imuPitch) < DEGREES_80_IN_DECIDEGREES;
}

int32_t AltHold::calculateAltHoldThrottleAdjustment(int32_t vel_tmp, float accZ_tmp, float accZ_old, float imuPitch, float imuRoll)
{
    int32_t result = 0;
    int32_t error;
    int32_t setVel;

    if (!isThrustFacingDownwards(imuPitch, imuRoll)) {
        return result;
    }

    // Altitude P-Controller

    if (!velocityControl) {
        error = constrain(altHold - _estimatedAltitude, -500, 500);
        error = applyDeadband(error, 10); // remove small P parameter to reduce noise near zero position
        setVel = constrain((altHoldCfg->pidAlt_P * error / 128), -300, +300); // limit velocity to +/- 3 m/s
    } else {
        setVel = setVelocity;
    }


    // Velocity PID-Controller
    // P
    error = setVel - vel_tmp;
    result = constrain((altHoldCfg->pidVelocity_P * error / 32), -300, +300);

    // I
    errorVelocityI += (altHoldCfg->pidVelocity_I * error);
    errorVelocityI = constrain(errorVelocityI, -(8192 * 200), (8192 * 200));
    result += errorVelocityI / 8192;     // I in range +/-200

    // D
    result -= constrain(altHoldCfg->pidVelocity_D * (accZ_tmp + accZ_old) / 512, -150, 150);

    return result;
}

void AltHold::calculateEstimatedAltitude(uint32_t currentTime,
										 int32_t baroAlt,
										 uint32_t accDtSum,
										 int32_t  accSumCount,
										 int32_t accSum,
										 float imuPitch,
										 float imuRoll,
										 float cosTiltAngle)
{
    static uint32_t previousTime;
    uint32_t dTime;
    int32_t baroVel;
    float dt;
    float vel_acc;
    int32_t vel_tmp;
    float accZ_tmp;
    static float accZ_old = 0.0f;
    static float vel = 0.0f;
    static float accAlt = 0.0f;
    static int32_t lastBaroAlt;

#ifdef SONAR
    int32_t sonarAlt = SONAR_OUT_OF_RANGE;
    static int32_t baroAlt_offset = 0;
    float sonarTransition;
#endif

    dTime = currentTime - previousTime;
    previousTime = currentTime;


#ifdef SONAR
    sonarAlt = sonarRead();
    sonarAlt = sonarCalculateAltitude(sonarAlt, cosTiltAngle);

    if (sonarAlt > 0 && sonarAlt < sonarCfAltCm) {
        // just use the SONAR
        baroAlt_offset = baroAlt - sonarAlt;
        baroAlt = sonarAlt;
    } else {
        baroAlt -= baroAlt_offset;
        if (sonarAlt > 0  && sonarAlt <= sonarMaxAltWithTiltCm) {
            // SONAR in range, so use complementary filter
            sonarTransition = (float)(sonarMaxAltWithTiltCm - sonarAlt) / (sonarMaxAltWithTiltCm - sonarCfAltCm);
            baroAlt = sonarAlt * sonarTransition + baroAlt * (1.0f - sonarTransition);
        }
    }
#endif

    dt = accDtSum * 1e-6f; // delta acc reading time in seconds

    // Integrator - velocity, cm/sec
    if (accSumCount)
    {
        accZ_tmp = (float)(accSum) / (float)accSumCount;
    }
    else
    {
        accZ_tmp = 0;
    }

    vel_acc = accZ_tmp * accVelScale * (float)accDtSum;

    // Integrator - Altitude in cm
    accAlt += (vel_acc * 0.5f) * dt + vel * dt;  // integrate velocity to get distance (x= a/2 * t^2)
#ifdef BARO
    // complementary filter for altitude estimation (baro & acc)
    accAlt = accAlt * altHoldCfg->baro_cf_alt + (float)baroAlt * (1.0f - altHoldCfg->baro_cf_alt);
#endif
    vel += vel_acc;


#ifdef SONAR
    if (sonarAlt > 0 && sonarAlt < sonarCfAltCm) {
        // the sonar has the best range
        _estimatedAltitude = baroAlt;
    } else {
        _estimatedAltitude = accAlt;
    }
#else
    _estimatedAltitude = accAlt;
#endif

    baroVel = (baroAlt - lastBaroAlt) * 1000000.0f / dTime;
    lastBaroAlt = baroAlt;

    baroVel = constrain(baroVel, -1500, 1500);  // constrain baro velocity +/- 1500cm/s
    baroVel = applyDeadband(baroVel, 10);       // to reduce noise near zero

    // apply Complimentary Filter to keep the calculated velocity based on baro velocity (i.e. near real velocity).
    // By using CF it's possible to correct the drift of integrated accZ (velocity) without loosing the phase, i.e without delay
#ifdef BARO
    vel = vel * altHoldCfg->baro_cf_vel + baroVel * (1.0f - altHoldCfg->baro_cf_vel);
#endif
    vel_tmp = lrintf(vel);

    // set vario
    vario = applyDeadband(vel_tmp, 5);

    altHoldThrottleAdjustment = calculateAltHoldThrottleAdjustment(vel_tmp, accZ_tmp, accZ_old, imuPitch, imuRoll);

    accZ_old = accZ_tmp;
}



