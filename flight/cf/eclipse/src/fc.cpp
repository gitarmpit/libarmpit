#include "fc.h"

#include "scheduler.h"
#include "filter.h"
#include <math.h>

#include "maths.h"
#include "rx.h"
#include "gyro.h"

#include "config.h"
#include "rc_curve.h"
#include "rc_controls.h"
#include "runtime_config.h"

#include "pid.h"
#include "altitudehold.h"
#include "mixer.h"
#include "imu.h"
#include "failsafe.h"

#include "feature.h"

#include "compass.h"
#include "baro.h"
#include "sensors/sonar.h"
#include "cfg.h"

#define GYRO_WATCHDOG_DELAY 100  // Watchdog for boards without interrupt for gyro

//TODO
static Acceleration* acc;
static Baro* baro;
static Gyro* gyro;
static Imu* imu;
static AltHold* altHold;
static RX* rx;
static RC_Controls* rc;
static Pid* pid;
static Mixer* mixer;
static FailSafe* failSafe;
static Compass* compass;
static Rc_Curve* rc_curve;

FC::FC()
{
    magHold = 0;
    headFreeModeHold = 0;
    isRXDataNew = false;

    _antiWindupEnabled = false;
    _airMode = Config::GetInstance()->getAirMode();

    accSumCount = 0;
    accDtSum = 0;
    accSum_Z = 0;


    _isArmed = false;

    //TODO control these with sticks
    stabilizeMode = NONE;
    _magMode = false;
    _headFreeMode = false;
    _altHoldMode = false;
}


//TODO delet
//void applyAndSaveAccelerometerTrimsDelta(rollAndPitchTrims_t *rollAndPitchTrimsDelta)
//{
//    accelerometerConfig()->accelerometerTrims.values.roll += rollAndPitchTrimsDelta->values.roll;
//    accelerometerConfig()->accelerometerTrims.values.pitch += rollAndPitchTrimsDelta->values.pitch;
//
//    saveConfigAndNotify();
//}

typedef enum {
    ARM_PREV_NONE       = 0,
    ARM_PREV_CLI        = 0x00205, //         0b1000000101  2 flashes - CLI active in the configurator
    ARM_PREV_FAILSAFE   = 0x00815, //       0b100000010101  3 flashes - Failsafe mode
    ARM_PREV_ANGLE      = 0x02055, //     0b10000001010101  4 flashes - Maximum arming angle exceeded
    ARM_PREV_CALIB      = 0x08155, //   0b1000000101010101  5 flashes - Calibration active
    ARM_PREV_OVERLOAD   = 0x20555  // 0b100000010101010101  6 flashes - System overload
} armingPreventedReason_e;

//armingPreventedReason_e getArmingPreventionBlinkMask(void)
//{
//    if (isCalibrating()) {
//        return ARM_PREV_CALIB;
//    }
//    if (rcModeIsActive(BOXFAILSAFE) || failsafePhase() == FAILSAFE_LANDED) {
//        return ARM_PREV_FAILSAFE;
//    }
//    if (!imuIsAircraftArmable(armingConfig()->max_arm_angle)) {
//        return ARM_PREV_ANGLE;
//    }
//    if (cliMode) {
//        return ARM_PREV_CLI;
//    }
//    if (isSystemOverloaded()) {
//        return ARM_PREV_OVERLOAD;
//    }
//    return ARM_PREV_NONE;
//}


//static void updateLEDs(void)
//{
//    if (ARMING_FLAG(ARMED)) {
//        LED0_ON;
//    } else {
//        if (rcModeIsActive(BOXARM) == 0) {
//            ENABLE_ARMING_FLAG(OK_TO_ARM);
//        }
//
//        if (!imuIsAircraftArmable(armingConfig()->max_arm_angle)) {
//            DISABLE_ARMING_FLAG(OK_TO_ARM);
//        }
//
//        if (isCalibrating() || isSystemOverloaded()) {
//            DISABLE_ARMING_FLAG(OK_TO_ARM);
//        }
//
//        uint32_t nextBlinkMask = getArmingPreventionBlinkMask();
//        warningLedSetBlinkMask(nextBlinkMask);
//        warningLedUpdate();
//    }
//}


void FC::mwDisarm(void)
{
    if (ARMING_FLAG(ARMED)) {
        DISABLE_ARMING_FLAG(ARMED);

#ifdef BLACKBOX
        if (Feature::GetInstance()->feature(FEATURE_BLACKBOX)) {
            finishBlackbox();
        }
#endif

        beeper(BEEPER_DISARMING);      // emit disarm tone
    }
}


void FC::mwArm(void)
{
    if (ARMING_FLAG(OK_TO_ARM))
    {
        if (ARMING_FLAG(ARMED))
        {
            return;
        }

        if (rc->rcModeIsActive(BOXFAILSAFE))
        {
            return;
        }

        if (!ARMING_FLAG(PREVENT_ARMING))
        {
            ENABLE_ARMING_FLAG(ARMED);
            headFreeModeHold = DECIDEGREES_TO_DEGREES(imu->getYaw());

#ifdef BLACKBOX
            if (Feature::GetInstance()->feature(FEATURE_BLACKBOX)) {
                serialPort_t *sharedBlackboxAndMspPort = findSharedSerialPort(FUNCTION_BLACKBOX, FUNCTION_MSP_SERVER);
                if (sharedBlackboxAndMspPort) {
                    mspSerialReleasePortIfAllocated(sharedBlackboxAndMspPort);
                }
                startBlackbox();
            }
#endif
            //disarmAt = millis() + rc->getArmingConfig()->auto_disarm_delay * 1000;   // start disarm timeout, will be extended when throttle is nonzero

            //beep to indicate arming
#ifdef GPS
            if (Feature::GetInstance()->feature(FEATURE_GPS) && STATE(GPS_FIX) && GPS_numSat >= 5)
                beeper(BEEPER_ARMING_GPS_FIX);
            else
                beeper(BEEPER_ARMING);
#else
            beeper(BEEPER_ARMING);
#endif

            return;
        }
    }

    if (!ARMING_FLAG(ARMED)) {
        beeperConfirmationBeeps(1);
    }
}


//for LEDs or arming prevention
//bool FC::isCalibrating(void)
//{
//#ifdef BARO
//    if (sensors(SENSOR_BARO) && !baro->isBaroCalibrationComplete()) {
//        return true;
//    }
//#endif
//
//    // Note: compass calibration is handled completely differently, outside of the main loop, see f.CALIBRATE_MAG
//
//    return (!acc->isAccelerationCalibrationComplete() && sensors(SENSOR_ACC)) ||
//            (!gyro->isGyroCalibrationComplete());
//}
//


//TODO move to RC,
void FC::updateMagHold(void)
{
    int16_t yaw = rc->getRcCommand(YAW);

    if (ABS(yaw) < 15 && _magMode)
    {
        int16_t dif = DECIDEGREES_TO_DEGREES(imu->getYaw()) - magHold;
        if (dif <= -180)
            dif += 360;
        if (dif >= +180)
            dif -= 360;

        dif *= - rc->getRcControlsConfig()->yaw_control_direction;

        if (imu->smallAngleEnabled())
        {
            //TODO move pidMAg to mag cfg?
            rc->setRcCommand(YAW,  yaw - dif * Config::GetInstance()->getPidProfile()->pidMAG_P / 30);    // 18 deg
        }
    }
    else
    {
        magHold = DECIDEGREES_TO_DEGREES(imu->getYaw());
    }
}


void FC::updateRcCommands(void)
{
    int32_t prop2;
    static controlRateConfig_t* rate_cfg = Config::GetInstance()->getControlRateConfig();
    int16_t throttle_raw = rx->getRcData(THROTTLE);

    // PITCH & ROLL only dynamic PID adjustment,  depending on throttle value
    if (throttle_raw < rate_cfg->tpa_breakpoint) {
        prop2 = 100;
    } else {
        if (throttle_raw < 2000) {
            prop2 = 100 - (uint16_t)rate_cfg->dynThrPID * (throttle_raw - rate_cfg->tpa_breakpoint) / (2000 - rate_cfg->tpa_breakpoint);
        } else {
            prop2 = 100 - rate_cfg->dynThrPID;
        }
    }

    rcControlsConfig_t* rcControlsConfig = rc->getRcControlsConfig();
    uint8_t* PIDweight = pid->getPIDweight();

    //ROLL, PITCH, YAW
    for (int axis = 0; axis < 3; axis++)
    {
        int32_t prop1;
        int32_t tmp = rc->getRcStickDeflection(axis);
        if (axis == ROLL || axis == PITCH)
        {
            //TODO hide this inside rc
            if (rcControlsConfig->deadband)
            {
                if (tmp > rcControlsConfig->deadband)
                {
                    tmp -= rcControlsConfig->deadband;
                } else {
                    tmp = 0;
                }
            }

            //TODO hide lookup inside rc
            rc->setRcCommand(axis, rc_curve->rcLookupPitchRoll(tmp));

            prop1 = 100 - (uint16_t)rate_cfg->rates[axis] * tmp / 500;
            prop1 = (uint16_t)prop1 * prop2 / 100;
            // non coupled PID reduction scaler used in PID controller 1 and PID controller 2. 100 means 100% of the pids
            PIDweight[axis] = prop2;
        }
        else //YAW
        {
            if (rcControlsConfig->yaw_deadband)
            {
                if (tmp > rcControlsConfig->yaw_deadband)
                {
                    tmp -= rcControlsConfig->yaw_deadband;
                } else
                {
                    tmp = 0;
                }
            }
            rc->setRcCommand(axis, rc_curve->rcLookupYaw(tmp) * -rcControlsConfig->yaw_control_direction);
            prop1 = 100 - (uint16_t)rate_cfg->rates[axis] * ABS(tmp) / 500;
            // YAW TPA disabled.
            PIDweight[axis] = 100;
        }

#ifdef USE_PID_MW23
        // FIXME axis indexes into pids.  use something like lookupPidIndex(rc_alias_e alias) to reduce coupling.
        dynP8[axis] = (uint16_t)pidProfile()->P[axis] * prop1 / 100;
        dynI8[axis] = (uint16_t)pidProfile()->I8[axis] * prop1 / 100;
        dynD8[axis] = (uint16_t)pidProfile()->D8[axis] * prop1 / 100;
#endif

        //TODO what is the range??  negative values??
        if (rx->getRcData(axis) < rx->getMidRc())
        {
            rc->setRcCommand(axis, -rc->getRcCommand(axis));
        }

    }  //EOF  axis loop


    //THROTTLE

    int32_t tmp = constrain(throttle_raw, rx->minCheck(), PWM_RANGE_MAX);
    tmp = (uint32_t)(tmp - rx->minCheck()) * PWM_RANGE_MIN / (PWM_RANGE_MAX - rx->minCheck());       // [MINCHECK;2000] -> [0;1000]
    rc->setRcCommand(THROTTLE, rc_curve->rcLookupThrottle(tmp));

    if (_headFreeMode)
    {
        float radDiff = degreesToRadians(DECIDEGREES_TO_DEGREES(imu->getYaw()) - headFreeModeHold);
        float cosDiff = cos_approx(radDiff);
        float sinDiff = sin_approx(radDiff);
        int16_t p = rc->getRcCommand(PITCH);
        int16_t r = rc->getRcCommand(ROLL);
        rc->setRcCommand(ROLL, r * cosDiff - p * sinDiff);
        rc->setRcCommand(PITCH, p * cosDiff + r * sinDiff);
    }
}


void FC::taskRx(void)
{
    //static bool armedBeeperOn = false;

    rx->update(sched->getCurrentTime_us(), rc->rcModeIsActive(BOXFAILSAFE), failSafe);

    if (Feature::GetInstance()->feature(FEATURE_FAILSAFE)) {

        if (sched->getCurrentTime_us() > FAILSAFE_POWER_ON_DELAY_US && !failSafe->isMonitoring()) {
            failSafe->startMonitoring();
        }

        failSafe->updateState();
    }

    bool isThrottleLow = rc->isThrottleLow();
    bool isRollPitchCentered =  rc->isRollPitchCentered();

    /* In airmode Iterm should be prevented to grow when Low thottle and Roll + Pitch Centered.
     This is needed to prevent Iterm winding on the ground, but keep full stabilisation on 0 throttle while in air
     Low Throttle + roll and Pitch centered is assuming the copter is on the ground. Done to prevent complex air/ground detections */
    if (isThrottleLow)
    {
        if (_airMode && !failSafe->isActive() && ARMING_FLAG(ARMED))
        {
            if (isRollPitchCentered)
            {
                _antiWindupEnabled = true;
            }
            else
            {
                _antiWindupEnabled = false;
            }
        }
        else
        {
#ifdef USE_PID_MW23
            pidResetITermAngle();
#endif
            pid->pidResetITerm();
        }
    }
    else
    {
        _antiWindupEnabled = false;
    }

    rc->processRcStickPositions(acc, gyro, baro, compass);


    bool canUseHorizonMode = true;

    if ((rc->rcModeIsActive(BOXANGLE) ||
            (Feature::GetInstance()->feature(FEATURE_FAILSAFE) && failSafe->isActive())))
    {
        // bumpless transfer to Level mode
        canUseHorizonMode = false;

        if (stabilizeMode != ANGLE_MODE_)
        {
#ifdef USE_PID_MW23
            pidResetITermAngle();
#endif
            stabilizeMode = ANGLE_MODE_;
        }
    }
    else
    {
        stabilizeMode = NONE;
    }

    if (rc->rcModeIsActive(BOXHORIZON) && canUseHorizonMode)
    {
        if (stabilizeMode != HORIZON_MODE_)
        {
#ifdef USE_PID_MW23
            pidResetITermAngle();
#endif
            stabilizeMode = HORIZON_MODE_;
        }
    } else {
        stabilizeMode = NONE;
    }

    //TODO
    if (stabilizeMode != NONE)
    {
        //LED1_ON;
    } else {
        //LED1_OFF;
    }

    if (rc->rcModeIsActive(BOXMAG))
    {
        if (!_magMode)
        {
            _magMode = true;
            magHold = DECIDEGREES_TO_DEGREES(imu->getYaw());
        }
    }
    else
    {
        _magMode = false;
    }

    if (rc->rcModeIsActive(BOXHEADFREE))
    {
    	_headFreeMode = true;
    }
    else
    {
    	_headFreeMode = false;
    }

    if (rc->rcModeIsActive(BOXHEADADJ)) {
        headFreeModeHold = DECIDEGREES_TO_DEGREES(imu->getYaw()); // acquire new heading
    }

#ifdef GPS
    if (sensors(SENSOR_GPS)) {
        updateGpsWaypointsAndMode();
    }
#endif


    isRXDataNew = true;

#if !defined(BARO) && !defined(SONAR)
    // updateRcCommands sets rcCommand, which is needed by updateAltHoldState and updateSonarAltHoldState
    updateRcCommands();
#endif

    //TODO
    //updateLEDs();

    if (sensors(SENSOR_BARO) || sensors(SENSOR_SONAR))
    {
        if (_altHoldMode)
        {
            altHold->activateAltHold(rx->getRcData(THROTTLE), rc->getRcCommand(THROTTLE));
        }
    }


}

void FC::filterRc(void)
{
    static int16_t lastCommand[4] = { 0, 0, 0, 0 };
    static int16_t deltaRC[4] = { 0, 0, 0, 0 };
    static int16_t factor, rcInterpolationFactor;
    uint16_t rxRefreshRate;

    // Set RC refresh rate for sampling and channels to filter
    rxRefreshRate = rx->getRxRefreshRate();


    if (isRXDataNew) {

        rxRefreshRate = constrain(sched->getTaskDeltaTime(TASK_RX), 1000, 20000) + 1000; // Add slight overhead to prevent ramps

        rcInterpolationFactor = rxRefreshRate / pid->getLoopTime() + 1;

        for (int channel=0; channel < 4; channel++) {
            deltaRC[channel] = rc->getRcCommand(channel) -  (lastCommand[channel] - deltaRC[channel] * factor / rcInterpolationFactor);
            lastCommand[channel] = rc->getRcCommand(channel);
        }

        isRXDataNew = false;
        factor = rcInterpolationFactor - 1;
    } else {
        factor--;
    }

    // Interpolate steps of rcCommand
    if (factor > 0) {
        for (int channel=0; channel < 4; channel++) {
            rc->setRcCommand(channel, lastCommand[channel] - deltaRC[channel] * factor/rcInterpolationFactor);
         }
    } else {
        factor = 0;
    }
}



void FC::taskGyro(void)
{
    gyro->update();

}


void FC::taskPid()
{
    static motorConfig_t* motorConfig = Config::GetInstance()->getMotorConfig();
    //const uint32_t startTime = micros();
    int horizonLevelStrength = 0;
    if (stabilizeMode == HORIZON_MODE_)
    {
        // Using Level D as a Sensitivity for Horizon. 0 more rate to 255 more level.
        // For more rate mode decrease D and slower flips and rolls will be possible
        int32_t mostDeflectedPos = rc->getMostDeflectedPos();
        int32_t roll = imu->getRoll();
        int32_t pitch = imu->getPitch();
        horizonLevelStrength = pid->calcHorizonLevelStrength(mostDeflectedPos, roll, pitch);
    }

    //TODO return result here and pass it to mixer
    int16_t* axisPID = pid->pidLuxFloat(rc->getRcCommand(), gyro->getADCf(), imu->getAttitude(), _antiWindupEnabled,
            mixer->isMotoroLimitReached(), horizonLevelStrength, stabilizeMode);

    mixer->mixTable(_isArmed, failSafe->isActive(), rc->getRcCommand(THROTTLE), rc->isThrottleLow(), rc->getRcCommand(YAW), axisPID);
    mixer->writeMotors();
    updateRcCommands();

    if (sensors(SENSOR_BARO) || sensors(SENSOR_SONAR))
    {
        if (_altHoldMode)
        {
            int16_t thr = altHold->applyAltHold(rx->getRcData(THROTTLE), rc->getRcCommand(THROTTLE));
            rc->setRcCommand(THROTTLE, constrain(thr, motorConfig->minthrottle, motorConfig->maxthrottle));
        }
    }

    updateMagHold();

    // If we're armed, at minimum throttle, and we do arming via the
    // sticks, do not process yaw input from the rx.  We do this so the
    // motors do not spin up while we are trying to arm or disarm.
    // Allow yaw control for tricopters if the user wants the servo to move even when unarmed.
    if (rc->isThrottleLow())
    {
        rc->setRcCommand(YAW, 0);
    }

    if (imu->throttle_correction_value() && stabilizeMode != NONE)
    {
        int16_t thr = rc->getRcCommand(THROTTLE);
        //TODO move out of imu
        thr += imu->calculateThrottleAngleCorrection();
        rc->setRcCommand(THROTTLE, thr);
    }

    if (rx->rcSmoothing())
    {
        filterRc();
    }
}


#ifdef GPS
void taskProcessGPS(void)
{
    // if GPS feature is enabled, gpsThread() will be called at some intervals to check for stuck
    // hardware, wrong baud rates, init GPS if needed, etc. Don't use SENSOR_GPS here as gpsThread() can and will
    // change this based on available hardware
    if (Feature::GetInstance()->feature(FEATURE_GPS)) {
        gpsThread();
    }

    if (sensors(SENSOR_GPS)) {
        updateGpsIndicator(currentTime);
    }
}
#endif

#ifdef MAG
void FC::taskMag(void)
{
    compass->updateCompass(sched->getCurrentTime_us());
}
#endif

#ifdef BARO
void FC::taskBaro(void)
{
    baro->baroUpdate();
}
#endif

#ifdef SONAR
void FC::taskSonar(void)
{
    if (sensors(SENSOR_SONAR)) {
        sonarUpdate();
    }
}
#endif

void FC::taskAttitude(void)
{
    static int32_t acc_Z;
    static uint32_t previousIMUUpdateTime;

    acc->update();

    int32_t* accADC = acc->getADC();
    int32_t* gyroADC = gyro->getADC();
    int32_t* magADC = compass->getMagADC();

    uint32_t currentTime = sched->getCurrentTime_us();
    uint32_t deltaT = currentTime - previousIMUUpdateTime;
    previousIMUUpdateTime = currentTime;

    bool fastGain = (!_isArmed && currentTime < 20000000);

    imu->calculateAttitude(accADC, gyroADC, magADC, deltaT, fastGain);

    imu->calculateAcceleration(accADC, deltaT, _isArmed, &acc_Z);
    accSum_Z += acc_Z;
    accDtSum += deltaT;
    accSumCount++;

}

void FC::taskAlt(void)
{
    if (baro->isBaroReady())
    {
        int32_t baroAlt = baro->baroCalculateAltitude();
        float imuPitch = imu->getPitch();
        float imuRoll = imu->getRoll();

        uint32_t time = sched->getCurrentTime_us();
        float tilt = imu->getCosTiltAngle();

        altHold->calculateEstimatedAltitude(time,
                baroAlt, accDtSum, accSumCount, accSum_Z, imuPitch, imuRoll, tilt);

        accSum_Z = 0;
        accSumCount = 0;
        accDtSum = 0;
    }
}


