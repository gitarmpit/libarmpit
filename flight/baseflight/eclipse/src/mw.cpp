/*
 * This file is part of baseflight
 * Licensed under GPL V3 or modified DCL - see https://github.com/multiwii/baseflight/blob/master/README.md
 */
#include "board.h"
#include "mw.h"
#include "utils.h"

//#include "cli.h"  TODO

flags_t flags;
int16_t debug[4];
uint32_t currentTime = 0;
uint32_t previousTime = 0;
uint16_t cycleTime = 0; // this is the number in micro second to achieve a full loop, it can differ a little and is taken into account in the PID loop
int16_t headFreeModeHold;

uint16_t vbat;                  // battery voltage in 0.1V steps
int32_t amperage;               // amperage read by current sensor in centiampere (1/100th A)
int32_t mAhdrawn;              // milliampere hours drawn from the battery since start
int16_t telemTemperature1;      // gyro sensor temperature

int16_t failsafeCnt = 0;
int16_t failsafeEvents = 0;
int16_t rcData[RC_CHANS];       // interval [1000;2000]
int16_t rcCommand[4];           // interval [1000;2000] for THROTTLE and [-500;+500] for ROLL/PITCH/YAW
int16_t lookupPitchRollRC[PITCH_LOOKUP_LENGTH];     // lookup table for expo & RC rate PITCH+ROLL
int16_t lookupThrottleRC[THROTTLE_LOOKUP_LENGTH];   // lookup table for expo & mid THROTTLE
uint16_t rssi;                  // range: [0;1023]
rcReadRawDataPtr rcReadRawFunc = NULL;  // receive data from default (pwm/ppm) or additional (spek/sbus/?? receiver drivers)

static void pidMultiWii(void);
static void pidRewrite(void);
pidControllerFuncPtr pid_controller = pidMultiWii; // which pid controller are we using, defaultMultiWii

uint8_t dynP8[3], dynI8[3], dynD8[3];
uint8_t rcOptions[CHECKBOXITEMS];

int16_t axisPID[3];

// **********************
// GPS
// **********************
int32_t GPS_coord[2];
int32_t GPS_home[3];
int32_t GPS_hold[3];
uint8_t GPS_numSat;
uint16_t GPS_distanceToHome;        // distance to home point in meters
int16_t GPS_directionToHome;        // direction to home or hol point in degrees
uint16_t GPS_altitude, GPS_speed;   // altitude in 0.1m and speed in 0.1m/s
uint8_t GPS_update = 0;             // it's a binary toogle to distinct a GPS position update
int16_t GPS_angle[3] =
{ 0, 0, 0 }; // it's the angles that must be applied for GPS correction
uint16_t GPS_ground_course = 0;     // degrees * 10
int16_t nav[2];
int16_t nav_rated[2];               // Adding a rate controller to the navigation to make it smoother
int8_t nav_mode = NAV_MODE_NONE;    // Navigation mode
uint8_t GPS_numCh;                  // Number of channels
uint8_t GPS_svinfo_chn[32];         // Channel number
uint8_t GPS_svinfo_svid[32];        // Satellite ID
uint8_t GPS_svinfo_quality[32];     // Bitfield Qualtity
uint8_t GPS_svinfo_cno[32];         // Carrier to Noise Ratio (Signal Strength)
uint32_t GPS_update_rate[2];        // GPS coordinates updating rate (column 0 = last update time, 1 = current update ms)
uint32_t GPS_svinfo_rate[2];        // GPS svinfo updating rate (column 0 = last update time, 1 = current update ms)
uint32_t GPS_HorizontalAcc;         // Horizontal accuracy estimate (mm)
uint32_t GPS_VerticalAcc;           // Vertical accuracy estimate (mm)

// Automatic ACC Offset Calibration
bool AccInflightCalibrationArmed = false;
bool AccInflightCalibrationMeasurementDone = false;
bool AccInflightCalibrationSavetoEEProm = false;
bool AccInflightCalibrationActive = false;
uint16_t InflightcalibratingA = 0;

// Battery monitoring stuff
uint8_t batteryCellCount = 3;       // cell count
uint16_t batteryWarningVoltage;     // slow buzzer after this one, recommended 80% of battery used. Time to land.
uint16_t batteryCriticalVoltage;    // annoying buzzer after this one, battery is going to be dead.

// Time of automatic disarm when "Don't spin the motors when armed" is enabled.
static uint32_t disarmTime = 0;

uint16_t pwmReadRawRC(uint8_t chan)
{
    return 0;
//TODO    return pwmRead(mcfg.rcmap[chan]);
}

void computeRC(void)
{
    uint16_t capture;
    int i, chan;

    static int16_t rcDataAverage[RC_CHANS][4];
    static int rcAverageIndex = 0;

    for (chan = 0; chan < mcfg.rc_channel_count; chan++)
    {
        //TODO func is null
        capture = rcReadRawFunc(chan);

        // validate input
        if (capture < PULSE_MIN || capture > PULSE_MAX)
            capture = mcfg.midrc;
        rcDataAverage[chan][rcAverageIndex % 4] = capture;
        // clear this since we're not accessing it elsewhere. saves a temp var
        rcData[chan] = 0;
        for (i = 0; i < 4; i++)
            rcData[chan] += rcDataAverage[chan][i];
        rcData[chan] /= 4;
    }
    rcAverageIndex++;
}

static void mwArm(void)
{
    if (calibratingG == 0 && flags.ACC_CALIBRATED)
    {
        // TODO: feature(FEATURE_FAILSAFE) && failsafeCnt < 2
        // TODO: && ( !feature || ( feature && ( failsafecnt > 2) )
        if (!flags.ARMED)
        {         // arm now!
            flags.ARMED = 1;
            headFreeModeHold = heading;
            // Beep for inform about arming
#ifdef GPS
            if (feature(FEATURE_GPS) && flags.GPS_FIX && GPS_numSat >= 5)
            buzzer(BUZZER_ARMING_GPS_FIX);
            else
            buzzer(BUZZER_ARMING);
#else
            buzzer(BUZZER_ARMING);
#endif
        }
    }
    else if (!flags.ARMED)
    {
        //    blinkLED(2, 255, 1);  //TODO
    }
}

static void mwDisarm(void)
{
    if (flags.ARMED)
    {
        flags.ARMED = 0;
        // Beep for inform about disarming
        buzzer(BUZZER_DISARMING);
        // Reset disarm time so that it works next time we arm the board.
        if (disarmTime != 0)
            disarmTime = 0;
    }
}

static void mwVario(void)
{

}

static int32_t errorGyroI[3] =
{ 0, 0, 0 };
static int32_t errorAngleI[2] =
{ 0, 0 };

static void pidMultiWii(void)
{
    int axis, prop;
    int32_t error, errorAngle;
    int32_t PTerm, ITerm, PTermACC = 0, ITermACC = 0, PTermGYRO = 0, ITermGYRO = 0, DTerm;
    static int16_t lastGyro[3] =
    { 0, 0, 0 };
    static int32_t delta1[3], delta2[3];
    int32_t deltaSum;
    int32_t delta;

    // **** PITCH & ROLL & YAW PID ****
    prop = max(abs(rcCommand[PITCH]), abs(rcCommand[ROLL])); // range [0;500]
    for (axis = 0; axis < 3; axis++)
    {
        if ((flags.ANGLE_MODE || flags.HORIZON_MODE) && axis < 2)
        { // MODE relying on ACC
            // 50 degrees max inclination
            errorAngle = constrain(2 * rcCommand[axis] + GPS_angle[axis], -((int) mcfg.max_angle_inclination), +mcfg.max_angle_inclination)
                    - angle[axis] + cfg.angleTrim[axis];
            PTermACC = errorAngle * cfg.P8[PIDLEVEL] / 100; // 32 bits is needed for calculation: errorAngle*P8[PIDLEVEL] could exceed 32768   16 bits is ok for result
            PTermACC = constrain(PTermACC, -cfg.D8[PIDLEVEL] * 5, +cfg.D8[PIDLEVEL] * 5);

            errorAngleI[axis] = constrain(errorAngleI[axis] + errorAngle, -10000, +10000); // WindUp
            ITermACC = (errorAngleI[axis] * cfg.I8[PIDLEVEL]) >> 12;
        }
        if (!flags.ANGLE_MODE || flags.HORIZON_MODE || axis == 2)
        { // MODE relying on GYRO or YAW axis
            error = (int32_t) rcCommand[axis] * 10 * 8 / cfg.P8[axis];
            error -= gyroData[axis];

            PTermGYRO = rcCommand[axis];

            errorGyroI[axis] = constrain(errorGyroI[axis] + error, -16000, +16000); // WindUp
            if ((abs(gyroData[axis]) > 640) || ((axis == YAW) && (abs(rcCommand[axis]) > 100)))
                errorGyroI[axis] = 0;
            ITermGYRO = (errorGyroI[axis] / 125 * cfg.I8[axis]) >> 6;
        }
        if (flags.HORIZON_MODE && axis < 2)
        {
            PTerm = (PTermACC * (500 - prop) + PTermGYRO * prop) / 500;
            ITerm = (ITermACC * (500 - prop) + ITermGYRO * prop) / 500;
        }
        else
        {
            if (flags.ANGLE_MODE && axis < 2)
            {
                PTerm = PTermACC;
                ITerm = ITermACC;
            }
            else
            {
                PTerm = PTermGYRO;
                ITerm = ITermGYRO;
            }
        }

        PTerm -= (int32_t) gyroData[axis] * dynP8[axis] / 10 / 8; // 32 bits is needed for calculation
        delta = gyroData[axis] - lastGyro[axis];
        lastGyro[axis] = gyroData[axis];
        deltaSum = delta1[axis] + delta2[axis] + delta;
        delta2[axis] = delta1[axis];
        delta1[axis] = delta;
        DTerm = (deltaSum * dynD8[axis]) / 32;
        axisPID[axis] = PTerm + ITerm - DTerm;
    }
}

#define GYRO_I_MAX 256

static void pidRewrite(void)
{
    int32_t errorAngle = 0;
    int axis;
    int32_t delta, deltaSum;
    static int32_t delta1[3], delta2[3];
    int32_t PTerm, ITerm, DTerm;
    static int32_t lastError[3] =
    { 0, 0, 0 };
    int32_t AngleRateTmp, RateError;

    // ----------PID controller----------
    for (axis = 0; axis < 3; axis++)
    {
        // -----Get the desired angle rate depending on flight mode
        if (axis == 2)
        { // YAW is always gyro-controlled (MAG correction is applied to rcCommand)
            AngleRateTmp = (((int32_t) (cfg.yawRate + 27) * rcCommand[YAW]) >> 5);
        }
        else
        {
            // calculate error and limit the angle to 50 degrees max inclination
            errorAngle = (constrain(rcCommand[axis] + GPS_angle[axis], -500, +500) - angle[axis] + cfg.angleTrim[axis]) / 10.0f; // 16 bits is ok here
            if (!flags.ANGLE_MODE)
            { //control is GYRO based (ACRO and HORIZON - direct sticks control is applied to rate PID
                AngleRateTmp = ((int32_t) (cfg.rollPitchRate[axis] + 27) * rcCommand[axis]) >> 4;

                if (flags.HORIZON_MODE)
                {
                    // mix up angle error to desired AngleRateTmp to add a little auto-level feel
                    AngleRateTmp += (errorAngle * cfg.I8[PIDLEVEL]) >> 8;
                }
            }
            else
            { // it's the ANGLE mode - control is angle based, so control loop is needed
                AngleRateTmp = (errorAngle * cfg.P8[PIDLEVEL]) >> 4;
            }
        }

        // --------low-level gyro-based PID. ----------
        // Used in stand-alone mode for ACRO, controlled by higher level regulators in other modes
        // -----calculate scaled error.AngleRates
        // multiplication of rcCommand corresponds to changing the sticks scaling here
        RateError = AngleRateTmp - gyroData[axis];

        // -----calculate P component
        PTerm = (RateError * cfg.P8[axis]) >> 7;
        // -----calculate I component
        // there should be no division before accumulating the error to integrator, because the precision would be reduced.
        // Precision is critical, as I prevents from long-time drift. Thus, 32 bits integrator is used.
        // Time correction (to avoid different I scaling for different builds based on average cycle time)
        // is normalized to cycle time = 2048.
        errorGyroI[axis] = errorGyroI[axis] + ((RateError * cycleTime) >> 11) * cfg.I8[axis];

        // limit maximum integrator value to prevent WindUp - accumulating extreme values when system is saturated.
        // I coefficient (I8) moved before integration to make limiting independent from PID settings
        errorGyroI[axis] = constrain(errorGyroI[axis], (int32_t) (-GYRO_I_MAX) << 13, (int32_t) (+GYRO_I_MAX) << 13);
        ITerm = errorGyroI[axis] >> 13;

        //-----calculate D-term
        delta = RateError - lastError[axis];  // 16 bits is ok here, the dif between 2 consecutive gyro reads is limited to 800
        lastError[axis] = RateError;

        // Correct difference by cycle time. Cycle time is jittery (can be different 2 times), so calculated difference
        // would be scaled by different dt each time. Division by dT fixes that.
        delta = (delta * ((uint16_t) 0xFFFF / (cycleTime >> 4))) >> 6;
        // add moving average here to reduce noise
        deltaSum = delta1[axis] + delta2[axis] + delta;
        delta2[axis] = delta1[axis];
        delta1[axis] = delta;
        DTerm = (deltaSum * cfg.D8[axis]) >> 8;

        // -----calculate total PID output
        axisPID[axis] = PTerm + ITerm + DTerm;
    }
}

void setPIDController(int type)
{
    switch (type)
    {
    case 0:
    default:
        pid_controller = pidMultiWii;
        break;
    case 1:
        pid_controller = pidRewrite;
        break;
    }
}

void loop(void)
{
    static uint8_t rcDelayCommand; // this indicates the number of time (multiple of RC measurement at 50Hz) the sticks must be maintained to run or switch off motors
    static uint8_t rcSticks;            // this hold sticks position for command combos
    uint8_t stTmp = 0;
    int i;
    static uint32_t rcTime = 0;
#ifdef BARO
    static int16_t initialThrottleHold;
#endif
    static uint32_t loopTime;
    uint32_t auxState = 0;
#ifdef GPS
    static uint8_t GPSNavReset = 1;
#endif
    bool isThrottleLow = false;
    bool rcReady = false;

    if (((int32_t) (currentTime - rcTime) >= 0) || rcReady)
    { // 50Hz or data driven
        rcReady = false;
        rcTime = currentTime + 20000;
        computeRC();

        // Read rssi value
        rssi = RSSI_getValue();

        // Failsafe routine
//TODO
#ifdef FEATURE_FAILSAFE
        if (failsafeCnt > (5 * cfg.failsafe_delay) && flags.ARMED)
        { // Stabilize, and set Throttle to specified level
            for (i = 0; i < 3; i++)
            rcData[i] = mcfg.midrc;// after specified guard time after RC signal is lost (in 0.1sec)
            rcData[THROTTLE] = cfg.failsafe_throttle;
            buzzer(BUZZER_TX_LOST_ARMED);
            if ((failsafeCnt > 5 * (cfg.failsafe_delay + cfg.failsafe_off_delay)) && !flags.FW_FAILSAFE_RTH_ENABLE)
            {  // Turn OFF motors after specified Time (in 0.1sec)
                mwDisarm();// This will prevent the copter to automatically rearm if failsafe shuts it down and prevents
                flags.OK_TO_ARM = 0;// to restart accidentely by just reconnect to the tx - you will have to switch off first to rearm
                buzzer(BUZZER_TX_LOST);
            }
            failsafeEvents++;
        }
        if (failsafeCnt > (5 * cfg.failsafe_delay) && !flags.ARMED)
        {  // Turn off "Ok To arm to prevent the motors from spinning after repowering the RX with low throttle and aux to arm
            mwDisarm();// This will prevent the copter to automatically rearm if failsafe shuts it down and prevents
            flags.OK_TO_ARM = 0;// to restart accidentely by just reconnect to the tx - you will have to switch off first to rearm
            buzzer(BUZZER_TX_LOST);
        }
        failsafeCnt++;
    }
    // end of failsafe routine - next change is made with RcOptions setting
#endif
        // ------------------ STICKS COMMAND HANDLER --------------------
        // checking sticks positions
        for (i = 0; i < 4; i++)
        {
            stTmp >>= 2;
            if (rcData[i] > mcfg.mincheck)
                stTmp |= 0x80;  // check for MIN
            if (rcData[i] < mcfg.maxcheck)
                stTmp |= 0x40;  // check for MAX
        }
        if (stTmp == rcSticks)
        {
            if (rcDelayCommand < 250)
                rcDelayCommand++;
        }
        else
            rcDelayCommand = 0;
        rcSticks = stTmp;

        // perform actions
        if ((rcData[THROTTLE] < mcfg.mincheck))
            isThrottleLow = true;
        if (isThrottleLow)
        {
            errorGyroI[ROLL] = 0;
            errorGyroI[PITCH] = 0;
            errorGyroI[YAW] = 0;
            errorAngleI[ROLL] = 0;
            errorAngleI[PITCH] = 0;
            if (cfg.activate[BOXARM] > 0)
            { // Arming via ARM BOX
                if (rcOptions[BOXARM] && flags.OK_TO_ARM)
                    mwArm();
            }
        }

        if (cfg.activate[BOXARM] > 0)
        { // Disarming via ARM BOX
            if (!rcOptions[BOXARM] && flags.ARMED)
            {
                if (mcfg.disarm_kill_switch)
                {
                    mwDisarm();
                }
                else if (isThrottleLow)
                {
                    mwDisarm();
                }
            }
        }

        if (rcDelayCommand == 20)
        {
            if (flags.ARMED)
            {      // actions during armed
                // Disarm on throttle down + yaw
                if (cfg.activate[BOXARM] == 0 && (rcSticks == THR_LO + YAW_LO + PIT_CE + ROL_CE))
                    mwDisarm();
                // Disarm on roll (only when retarded_arm is enabled)
                if (mcfg.retarded_arm && cfg.activate[BOXARM] == 0 && (rcSticks == THR_LO + YAW_CE + PIT_CE + ROL_LO))
                    mwDisarm();
            }
            else
            {            // actions during not armed
                i = 0;
                // GYRO calibration
                if (rcSticks == THR_LO + YAW_LO + PIT_LO + ROL_CE)
                {
                    calibratingG = CALIBRATING_GYRO_CYCLES;
#ifdef GPS
                    if (feature(FEATURE_GPS))
                    GPS_reset_home_position();
#endif
                    if (sensors_has_baro())
                        calibratingB = 10; // calibrate baro to new ground level (10 * 25 ms = ~250 ms non blocking)
                    if (!sensors_has_mag())
                        heading = 0; // reset heading to zero after gyro calibration
                    // Inflight ACC Calibration
                }

                //TODO
//                else if (feature(FEATURE_INFLIGHT_ACC_CAL) && (rcSticks == THR_LO + YAW_LO + PIT_HI + ROL_HI)) {
//                    if (AccInflightCalibrationMeasurementDone) {        // trigger saving into eeprom after landing
//                        AccInflightCalibrationMeasurementDone = false;
//                        AccInflightCalibrationSavetoEEProm = true;
//                    } else {
//                        AccInflightCalibrationArmed = !AccInflightCalibrationArmed;
//                        if (AccInflightCalibrationArmed) {
//                            buzzer(BUZZER_ACC_CALIBRATION);
//                        } else {
//                            buzzer(BUZZER_ACC_CALIBRATION_FAIL);
//                        }
//                    }
//                }

                // Multiple configuration profiles
                if (rcSticks == THR_LO + YAW_LO + PIT_CE + ROL_LO)          // ROLL left  -> Profile 1
                    i = 1;
                else if (rcSticks == THR_LO + YAW_LO + PIT_HI + ROL_CE)     // PITCH up   -> Profile 2
                    i = 2;
                else if (rcSticks == THR_LO + YAW_LO + PIT_CE + ROL_HI)     // ROLL right -> Profile 3
                    i = 3;
                if (i)
                {
                    mcfg.current_profile = i - 1;
                    writeEEPROM(0, false);
                    //blinkLED(2, 40, i);
                    // TODO alarmArray[0] = i;
                }

                // Arm via YAW
                if (cfg.activate[BOXARM] == 0 && (rcSticks == THR_LO + YAW_HI + PIT_CE + ROL_CE))
                    mwArm();
                // Arm via ROLL
                else if (mcfg.retarded_arm && cfg.activate[BOXARM] == 0 && (rcSticks == THR_LO + YAW_CE + PIT_CE + ROL_HI))
                    mwArm();
                // Calibrating Acc
                else if (rcSticks == THR_HI + YAW_LO + PIT_LO + ROL_CE)
                    calibratingA = CALIBRATING_ACC_CYCLES;
                // Calibrating Mag
                else if (rcSticks == THR_HI + YAW_HI + PIT_LO + ROL_CE)
                    flags.CALIBRATE_MAG = 1;
                i = 0;
                // Acc Trim
                if (rcSticks == THR_HI + YAW_CE + PIT_HI + ROL_CE)
                {
                    cfg.angleTrim[PITCH] += 2;
                    i = 1;
                }
                else if (rcSticks == THR_HI + YAW_CE + PIT_LO + ROL_CE)
                {
                    cfg.angleTrim[PITCH] -= 2;
                    i = 1;
                }
                else if (rcSticks == THR_HI + YAW_CE + PIT_CE + ROL_HI)
                {
                    cfg.angleTrim[ROLL] += 2;
                    i = 1;
                }
                else if (rcSticks == THR_HI + YAW_CE + PIT_CE + ROL_LO)
                {
                    cfg.angleTrim[ROLL] -= 2;
                    i = 1;
                }
                if (i)
                {
                    writeEEPROM(1, true);
                    rcDelayCommand = 0; // allow autorepetition
                }
            }
        }

        //TODO
//        if (feature(FEATURE_INFLIGHT_ACC_CAL)) {
//            if (AccInflightCalibrationArmed && flags.ARMED && rcData[THROTTLE] > mcfg.mincheck && !rcOptions[BOXARM]) {   // Copter is airborne and you are turning it off via boxarm : start measurement
//                InflightcalibratingA = 50;
//                AccInflightCalibrationArmed = false;
//            }
//            if (rcOptions[BOXCALIB]) {      // Use the Calib Option to activate : Calib = TRUE Meausrement started, Land and Calib = 0 measurement stored
//                if (!AccInflightCalibrationActive && !AccInflightCalibrationMeasurementDone)
//                    InflightcalibratingA = 50;
//                AccInflightCalibrationActive = true;
//            } else if (AccInflightCalibrationMeasurementDone && !flags.ARMED) {
//                AccInflightCalibrationMeasurementDone = false;
//                AccInflightCalibrationSavetoEEProm = true;
//            }
//        }

        // Check AUX switches

        //TODO
//        for (i = 0; i < core.numAuxChannels; i++)
//            auxState |= (rcData[AUX1 + i] < 1300) << (3 * i) | (1300 < rcData[AUX1 + i] && rcData[AUX1 + i] < 1700) << (3 * i + 1) | (rcData[AUX1 + i] > 1700) << (3 * i + 2);

        for (i = 0; i < CHECKBOXITEMS; i++)
            rcOptions[i] = (auxState & cfg.activate[i]) > 0;
        flags.CRUISE_MODE = rcOptions[BOXGCRUISE];
        if (flags.CRUISE_MODE)
        {
            rcOptions[BOXGPSHOLD] = true;
            rcOptions[BOXHORIZON] = true;
        }

        // note: if FAILSAFE is disable, failsafeCnt > 5 * FAILSAVE_DELAY is always false
        if ((rcOptions[BOXANGLE] || (failsafeCnt > 5 * cfg.failsafe_delay)))
        {
            // bumpless transfer to Level mode
            if (!flags.ANGLE_MODE)
            {
                errorAngleI[ROLL] = 0;
                errorAngleI[PITCH] = 0;
                flags.ANGLE_MODE = 1;
            }
            //TODO
//            if (feature(FEATURE_FW_FAILSAFE_RTH)) {
//                if ((failsafeCnt > 5 * cfg.failsafe_delay) && sensors(SENSOR_GPS)) {
//                    flags.FW_FAILSAFE_RTH_ENABLE = 1;
//                }
//            }
        }
        else
        {
            flags.ANGLE_MODE = 0;   // failsafe support
            flags.FW_FAILSAFE_RTH_ENABLE = 0;
        }

        if (rcOptions[BOXHORIZON])
        {
            flags.ANGLE_MODE = 0;
            if (!flags.HORIZON_MODE)
            {
                errorAngleI[ROLL] = 0;
                errorAngleI[PITCH] = 0;
                flags.HORIZON_MODE = 1;
            }
        }
        else
        {
            flags.HORIZON_MODE = 0;
        }

        if ((rcOptions[BOXARM]) == 0)
            flags.OK_TO_ARM = 1;
        if (flags.ANGLE_MODE || flags.HORIZON_MODE)
        {
            //LED1_ON; //TODO
        }
        else
        {
            // LED1_OFF;
        }

#ifdef BARO
        if (sensors(SENSOR_BARO))
        {
            // Baro alt hold activate
            if (rcOptions[BOXBARO])
            {
                if (!flags.BARO_MODE)
                {
                    flags.BARO_MODE = 1;
                    AltHold = EstAlt;
                    initialThrottleHold = rcCommand[THROTTLE];
                    errorVelocityI = 0;
                    BaroPID = 0;
                }
            }
            else
            {
                flags.BARO_MODE = 0;
            }
            // Vario signalling activate
            if (feature(FEATURE_VARIO))
            {
                if (rcOptions[BOXVARIO])
                {
                    if (!flags.VARIO_MODE)
                    {
                        flags.VARIO_MODE = 1;
                    }
                }
                else
                {
                    flags.VARIO_MODE = 0;
                }
            }
        }
#endif

        if (sensors_has_mag())
        {
            if (rcOptions[BOXMAG])
            {
                if (!flags.MAG_MODE)
                {
                    flags.MAG_MODE = 1;
                    magHold = heading;
                }
            }
            else
            {
                flags.MAG_MODE = 0;
            }
            if (rcOptions[BOXHEADFREE])
            {
                if (!flags.HEADFREE_MODE)
                {
                    flags.HEADFREE_MODE = 1;
                }
            }
            else
            {
                flags.HEADFREE_MODE = 0;
            }
            if (rcOptions[BOXHEADADJ])
            {
                headFreeModeHold = heading; // acquire new heading
            }
        }

#ifdef GPS
        if (sensors(SENSOR_GPS))
        {
            if (flags.GPS_FIX && GPS_numSat >= 5)
            {
                if (nav_mode != NAV_MODE_NONE && (!flags.HORIZON_MODE && !flags.ANGLE_MODE))
                flags.ANGLE_MODE = true; // Force a stable mode in GPS Mode

                // if both GPS_HOME & GPS_HOLD are checked => GPS_HOME is the priority
                if (rcOptions[BOXGPSHOME] || flags.FW_FAILSAFE_RTH_ENABLE )
                {
                    if (!flags.GPS_HOME_MODE)
                    {
                        flags.GPS_HOME_MODE = 1;
                        flags.GPS_HOLD_MODE = 0;
                        GPSNavReset = 0;
                        GPS_set_next_wp(&GPS_home[LAT], &GPS_home[LON]);
                        nav_mode = NAV_MODE_WP;
                        GPS_hold[ALT] = GPS_altitude;
                        flags.CLIMBOUT_FW = 1;
                    }
                }
                else
                {
                    flags.GPS_HOME_MODE = 0;
                    if (rcOptions[BOXGPSHOLD] && abs(rcCommand[ROLL]) < cfg.ap_mode && abs(rcCommand[PITCH]) < cfg.ap_mode)
                    {
                        if (!flags.GPS_HOLD_MODE)
                        {
                            flags.GPS_HOLD_MODE = 1;
                            GPSNavReset = 0;
                            GPS_hold[LAT] = GPS_coord[LAT];
                            GPS_hold[LON] = GPS_coord[LON];
                            GPS_set_next_wp(&GPS_hold[LAT], &GPS_hold[LON]);
                            nav_mode = NAV_MODE_POSHOLD;
                            GPS_hold[ALT] = GPS_altitude;
                            flags.CLIMBOUT_FW = 0;
                        }
                    }
                    else
                    {
                        flags.GPS_HOLD_MODE = 0;
                        // both boxes are unselected here, nav is reset if not already done
                        if (GPSNavReset == 0)
                        {
                            GPSNavReset = 1;
                            GPS_reset_nav();
                            flags.CLIMBOUT_FW = 0;
                        }
                    }
                }
                // Beep for indication that GPS has found satellites and naze32 is ready to fly
                buzzer(BUZZER_READY_BEEP);
            }
            else
            {
                flags.GPS_HOME_MODE = 0;
                flags.GPS_HOLD_MODE = 0;
                nav_mode = NAV_MODE_NONE;
            }
        }
#endif

        if (rcOptions[BOXPASSTHRU] && !flags.FW_FAILSAFE_RTH_ENABLE)
        {
            flags.PASSTHRU_MODE = 1;
        }
        else
        {
            flags.PASSTHRU_MODE = 0;
        }

        // When armed and motors aren't spinning. Make warning beeps so that accidentally won't lose fingers...
        // Also disarm board after 5 sec so users without buzzer won't lose fingers.
        //TODO
//        if (feature(FEATURE_MOTOR_STOP) && flags.ARMED) {
//            if (isThrottleLow) {
//                if (disarmTime == 0)
//                    disarmTime = millis() + 1000 * mcfg.auto_disarm_board;
//                else if (disarmTime < millis() && mcfg.auto_disarm_board != 0)
//                    mwDisarm();
//                buzzer(BUZZER_ARMED);
//            } else if (disarmTime != 0)
//                disarmTime = 0;
//        }
    }
    else
    {                        // not in rc loop
        static int taskOrder = 0;   // never call all function in the same loop, to avoid high delay spikes
        switch (taskOrder)
        {
        case 0:
            taskOrder++;
            if (sensors_has_mag() && Mag_getADC())
                break;
        case 1:
            taskOrder++;
//TODO
#ifdef BARO
            if (sensors(SENSOR_BARO) && Baro_update())
            break;
#endif
        case 2:
            taskOrder++;
#ifdef BARO
            if (sensors(SENSOR_BARO) && getEstimatedAltitude())
            break;
#endif
        case 3:
            // if GPS feature is enabled, gpsThread() will be called at some intervals to check for stuck
            // hardware, wrong baud rates, init GPS if needed, etc. Don't use SENSOR_GPS here as gpsThread() can and will
            // change this based on available hardware
            taskOrder++;

//TODO
#ifdef GPS
            if (feature(FEATURE_GPS))
            {
                gpsThread();
                break;
            }
#endif
        case 4:
            taskOrder = 0;
//TODO
#ifdef SONAR
            if (sensors(SENSOR_SONAR))
            {
                Sonar_update();
            }
#endif

            //TODO
            //mwVario();
            break;
        }
    }

    currentTime = micros();
    if (mcfg.looptime == 0 || (int32_t) (currentTime - loopTime) >= 0)
    {
        loopTime = currentTime + mcfg.looptime;

        computeIMU();
        // Measure loop rate just afer reading the sensors
        currentTime = micros();
        cycleTime = (int32_t) (currentTime - previousTime);
        previousTime = currentTime;
        if (sensors_has_mag())
        {
            if (abs(rcCommand[YAW]) < 70 && flags.MAG_MODE)
            {
                int16_t dif = heading - magHold;
                if (dif <= -180)
                    dif += 360;
                if (dif >= +180)
                    dif -= 360;
                dif *= -mcfg.yaw_control_direction;
                if (flags.SMALL_ANGLE)
                    rcCommand[YAW] -= dif * cfg.P8[PIDMAG] / 30;    // 18 deg
            }
            else
                magHold = heading;
        }

//TODO
#ifdef BARO
        if (sensors(SENSOR_BARO))
        {
            if (flags.BARO_MODE)
            {
                static uint8_t isAltHoldChanged = 0;
                if (!flags.FIXED_WING)
                {
                    // multirotor alt hold
                    if (cfg.alt_hold_fast_change)
                    {
                        // rapid alt changes
                        if (abs(rcCommand[THROTTLE] - initialThrottleHold) > cfg.alt_hold_throttle_neutral)
                        {
                            errorVelocityI = 0;
                            isAltHoldChanged = 1;
                            rcCommand[THROTTLE] += (rcCommand[THROTTLE] > initialThrottleHold) ? -cfg.alt_hold_throttle_neutral : cfg.alt_hold_throttle_neutral;
                        }
                        else
                        {
                            if (isAltHoldChanged)
                            {
                                AltHold = EstAlt;
                                isAltHoldChanged = 0;
                            }
                            rcCommand[THROTTLE] = constrain(initialThrottleHold + BaroPID, mcfg.minthrottle, mcfg.maxthrottle);
                        }
                    }
                    else
                    {
                        // slow alt changes for apfags
                        if (abs(rcCommand[THROTTLE] - initialThrottleHold) > cfg.alt_hold_throttle_neutral)
                        {
                            // set velocity proportional to stick movement +100 throttle gives ~ +50 cm/s
                            setVelocity = (rcCommand[THROTTLE] - initialThrottleHold) / 2;
                            velocityControl = 1;
                            isAltHoldChanged = 1;
                        }
                        else if (isAltHoldChanged)
                        {
                            AltHold = EstAlt;
                            velocityControl = 0;
                            isAltHoldChanged = 0;
                        }
                        rcCommand[THROTTLE] = constrain(initialThrottleHold + BaroPID, mcfg.minthrottle, mcfg.maxthrottle);
                    }
                }
                else
                {
                    // handle fixedwing-related althold. UNTESTED! and probably wrong
                    // most likely need to check changes on pitch channel and 'reset' althold similar to
                    // how throttle does it on multirotor
                    rcCommand[PITCH] += BaroPID * mcfg.fw_althold_dir;
                }
            }
        }
#endif

        if (cfg.throttle_correction_value && (flags.ANGLE_MODE || flags.HORIZON_MODE))
        {
            rcCommand[THROTTLE] += throttleAngleCorrection;
        }
//TODO
#ifdef GPS
        if (sensors(SENSOR_GPS))
        {
            if ((flags.GPS_HOME_MODE || flags.GPS_HOLD_MODE) && flags.GPS_FIX_HOME)
            {
                float sin_yaw_y = sinf(heading * 0.0174532925f);
                float cos_yaw_x = cosf(heading * 0.0174532925f);
                if (!flags.FIXED_WING)
                {
                    if (cfg.nav_slew_rate)
                    {
                        nav_rated[LON] += constrain(wrap_18000(nav[LON] - nav_rated[LON]), -cfg.nav_slew_rate, cfg.nav_slew_rate); // TODO check this on uint8
                        nav_rated[LAT] += constrain(wrap_18000(nav[LAT] - nav_rated[LAT]), -cfg.nav_slew_rate, cfg.nav_slew_rate);
                        GPS_angle[ROLL] = (nav_rated[LON] * cos_yaw_x - nav_rated[LAT] * sin_yaw_y) / 10;
                        GPS_angle[PITCH] = (nav_rated[LON] * sin_yaw_y + nav_rated[LAT] * cos_yaw_x) / 10;
                    }
                    else
                    {
                        GPS_angle[ROLL] = (nav[LON] * cos_yaw_x - nav[LAT] * sin_yaw_y) / 10;
                        GPS_angle[PITCH] = (nav[LON] * sin_yaw_y + nav[LAT] * cos_yaw_x) / 10;
                    }
                }
                else fw_nav();
            }
            else
            {
                GPS_angle[ROLL] = 0;
                GPS_angle[PITCH] = 0;
                GPS_angle[YAW] = 0;
            }
        }
#endif

        // PID - note this is function pointer set by setPIDController()
        pid_controller();

        mixTable();
        writeMotors();
    }
}
