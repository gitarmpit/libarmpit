/*
 * This file is part of baseflight
 * Licensed under GPL V3 or modified DCL - see https://github.com/multiwii/baseflight/blob/master/README.md
 */
#include "board.h"
#include "mw.h"


#include "buzzer.h"

flags_t f;
int16_t debug[4];
uint32_t currentTime = 0;
uint32_t previousTime = 0;
uint16_t cycleTime = 0;         // this is the number in micro second to achieve a full loop, it can differ a little and is taken into account in the PID loop
int16_t headFreeModeHold;

uint16_t vbat;                  // battery voltage in 0.1V steps
int32_t amperage;               // amperage read by current sensor in centiampere (1/100th A)
int32_t mAhdrawn;              // milliampere hours drawn from the battery since start
int16_t telemTemperature1;      // gyro sensor temperature

int16_t failsafeCnt = 0;
int16_t failsafeEvents = 0;
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
int16_t GPS_angle[3] = { 0, 0, 0 }; // it's the angles that must be applied for GPS correction
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


static void mwArm(void)
{
    if (calibratingG == 0 && f.ACC_CALIBRATED) {
        // TODO: feature(FEATURE_FAILSAFE) && failsafeCnt < 2
        // TODO: && ( !feature || ( feature && ( failsafecnt > 2) )
        if (!f.ARMED) {         // arm now!
            f.ARMED = 1;
            headFreeModeHold = heading;
            // Beep for inform about arming
#ifdef GPS
            if (feature(FEATURE_GPS) && f.GPS_FIX && GPS_numSat >= 5)
                buzzer(BUZZER_ARMING_GPS_FIX);
            else
                buzzer(BUZZER_ARMING);
#else
            buzzer(BUZZER_ARMING);
#endif
        }
    } else if (!f.ARMED) {
        blinkLED(2, 255, 1);
    }
}

static void mwDisarm(void)
{
    if (f.ARMED) {
        f.ARMED = 0;
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

static int32_t errorGyroI[3] = { 0, 0, 0 };
static int32_t errorAngleI[2] = { 0, 0 };


static void rc_loop() 
{
        rcReady = false;
        rcTime = currentTime + 20000;
        computeRC();


        // Failsafe routine
        if (feature(FEATURE_FAILSAFE) || feature(FEATURE_FW_FAILSAFE_RTH))
        {
            if (failsafeCnt > (5 * cfg.failsafe_delay) && f.ARMED) { // Stabilize, and set Throttle to specified level
                for (i = 0; i < 3; i++)
                    rcData[i] = mcfg.midrc;      // after specified guard time after RC signal is lost (in 0.1sec)
                rcData[THROTTLE] = cfg.failsafe_throttle;
                buzzer(BUZZER_TX_LOST_ARMED);
                if ((failsafeCnt > 5 * (cfg.failsafe_delay + cfg.failsafe_off_delay)) && !f.FW_FAILSAFE_RTH_ENABLE) {  // Turn OFF motors after specified Time (in 0.1sec)
                    mwDisarm();             // This will prevent the copter to automatically rearm if failsafe shuts it down and prevents
                    f.OK_TO_ARM = 0;        // to restart accidentely by just reconnect to the tx - you will have to switch off first to rearm
                    buzzer(BUZZER_TX_LOST);
                }
                failsafeEvents++;
            }
            if (failsafeCnt > (5 * cfg.failsafe_delay) && !f.ARMED) {  // Turn off "Ok To arm to prevent the motors from spinning after repowering the RX with low throttle and aux to arm
                mwDisarm();         // This will prevent the copter to automatically rearm if failsafe shuts it down and prevents
                f.OK_TO_ARM = 0;    // to restart accidentely by just reconnect to the tx - you will have to switch off first to rearm
                buzzer(BUZZER_TX_LOST);
            }
            failsafeCnt++;
        }
        // end of failsafe routine - next change is made with RcOptions setting

        //removed: process RC 


        f.CRUISE_MODE = rcOptions[BOXGCRUISE];
        if (f.CRUISE_MODE) {
            rcOptions[BOXGPSHOLD] = true;
            rcOptions[BOXHORIZON] = true;
        }

        // note: if FAILSAFE is disabled, failsafeCnt > 5 * FAILSAVE_DELAY is always false
        if ((rcOptions[BOXANGLE] || (failsafeCnt > 5 * cfg.failsafe_delay)) && (sensors(SENSOR_ACC))) {
            // bumpless transfer to Level mode
            if (!f.ANGLE_MODE) {
                errorAngleI[ROLL] = 0;
                errorAngleI[PITCH] = 0;
                f.ANGLE_MODE = 1;
            }
            if (feature(FEATURE_FW_FAILSAFE_RTH)) {
                if ((failsafeCnt > 5 * cfg.failsafe_delay) && sensors(SENSOR_GPS)) {
                    f.FW_FAILSAFE_RTH_ENABLE = 1;
                }
            }
        } else {
            f.ANGLE_MODE = 0;   // failsafe support
            f.FW_FAILSAFE_RTH_ENABLE = 0;
        }

        if (rcOptions[BOXHORIZON]) {
            f.ANGLE_MODE = 0;
            if (!f.HORIZON_MODE) {
                errorAngleI[ROLL] = 0;
                errorAngleI[PITCH] = 0;
                f.HORIZON_MODE = 1;
            }
        } else {
            f.HORIZON_MODE = 0;
        }

            
       f.OK_TO_ARM = 1;

       //led on if angle or horizon

        //removed baro alt hold activate
        //removed mag
        //removed GPS

}

void loop(void)
{
    int i;
    static uint32_t rcTime = 0;
    static uint32_t loopTime;
    bool rcReady = false;

    if (((int32_t)(currentTime - rcTime) >= 0) || rcReady) 
    { // 50Hz or data driven
        
        rc_loop();
    } 
    else 
    {
        update magnetometer
        update baro 
        getEstimatedAltitude()
        gps()
        Sonar_update();
    }
    computeIMU();
    annexCode();
    //part 2 mag
    //BARO part 2  (alt hold)
    //REMOVED: add throttle correction value to RC command if angle or horizon mode
    //REMOVED:  GPS part 2
    pid_controller();
    mixTable();
    writeMotors();
}
