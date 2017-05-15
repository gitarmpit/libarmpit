/*
MultiWiiCopter by Alexandre Dubus
www.multiwii.com
March  2015     V2.4
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version. see <http://www.gnu.org/licenses/>
 */


#include "config.h"
#include "def.h"
#include "types.h"
#include "MultiWii.h"
#include "IMU.h"
#include "Output.h"
#include "RX.h"
#include "Sensors.h"
#include "utils.h"
#include "EEPROM.h"
#include "IMU.h"

/*********** RC alias *****************/

const char pidnames[]  =
        "ROLL;"
        "PITCH;"
        "YAW;"
        "ALT;"
        "Pos;"
        "PosR;"
        "NavR;"
        "LEVEL;"
        "MAG;"
        "VEL;"
        ;


uint32_t currentTime = 0;
uint16_t previousTime = 0;
uint16_t cycleTime = 0;     // this is the number in micro second to achieve a full loop, it can differ a little and is taken into account in the PID loop
uint16_t calibratingA = 0;  // the calibration is done in the main loop. Calibrating decreases at each cycle down to 0, then we enter in a normal mode.
uint16_t calibratingB = 0;  // baro calibration = get new ground pressure value
uint16_t calibratingG;
int16_t  magHold,headFreeModeHold; // [-180;+180]
int32_t  AltHold; // in cm
int16_t  sonarAlt;
int16_t  BaroPID = 0;
int16_t  errorAltitudeI = 0;

// **************
// gyro+acc IMU
// **************
int16_t gyroZero[3] = {0,0,0};

imu_t imu;


alt_t alt;

att_t att;

#if defined(ARMEDTIMEWARNING)
uint32_t  ArmedTimeWarningMicroSeconds = 0;
#endif

int16_t  debug[4];

flags_struct_t f;


int16_t  i2c_errors_count = 0;


#if defined(THROTTLE_ANGLE_CORRECTION)
int16_t throttleAngleCorrection = 0; // correction of throttle in lateral wind,
int8_t  cosZ = 100;                  // cos(angleZ)*100
#endif



uint16_t intPowerTrigger1;


// ******************
// rc functions
// ******************
#define ROL_LO  (1<<(2*ROLL))
#define ROL_CE  (3<<(2*ROLL))
#define ROL_HI  (2<<(2*ROLL))
#define PIT_LO  (1<<(2*PITCH))
#define PIT_CE  (3<<(2*PITCH))
#define PIT_HI  (2<<(2*PITCH))
#define YAW_LO  (1<<(2*YAW))
#define YAW_CE  (3<<(2*YAW))
#define YAW_HI  (2<<(2*YAW))
#define THR_LO  (1<<(2*THROTTLE))
#define THR_CE  (3<<(2*THROTTLE))
#define THR_HI  (2<<(2*THROTTLE))

int16_t failsafeEvents = 0;
volatile int16_t failsafeCnt = 0;

int16_t rcData[RC_CHANS];    // interval [1000;2000]
int16_t rcSerial[8];         // interval [1000;2000] - is rcData coming from MSP
int16_t rcCommand[4];        // interval [1000;2000] for THROTTLE and [-500;+500] for ROLL/PITCH/YAW
uint8_t rcSerialCount = 0;   // a counter to select legacy RX when there is no more MSP rc serial data
int16_t lookupPitchRollRC[5];// lookup table for expo & RC rate PITCH+ROLL
uint16_t lookupThrottleRC[11];// lookup table for expo & mid THROTTLE



// *************************
// motor and servo functions
// *************************
int16_t axisPID[3];
int16_t motor[4];

// ************************
// EEPROM Layout definition
// ************************
static uint8_t dynP8[2], dynD8[2];

global_conf_t global_conf;

conf_t conf;


// The orginal altitude used as base our new altitude during nav
int32_t original_altitude;
//This is the target what we want to reach
int32_t target_altitude;
//This is the interim value which is feeded into the althold controller
int32_t alt_to_hold;

uint32_t alt_change_timer;
int8_t alt_change_flag;
uint32_t alt_change;


int32_t baroPressure;
int16_t baroTemperature;
int32_t baroPressureSum;

// this code is excetuted at each loop and won't interfere with control loop if it lasts less than 650 microseconds
void annexCode()
{
    static uint32_t calibratedAccTime;
    uint16_t tmp,tmp2;
    uint8_t axis,prop1,prop2;

    // PITCH & ROLL only dynamic PID adjustemnt,  depending on throttle value)
    prop2 = 128; // prop2 was 100, is 128 now
    if (rcData[THROTTLE]>1500) { // breakpoint is fix: 1500
        if (rcData[THROTTLE]<2000) {
            prop2 -=  ((uint16_t)conf.dynThrPID*(rcData[THROTTLE]-1500)>>9); //  /512 instead of /500
        } else {
            prop2 -=  conf.dynThrPID;
        }
    }

    for(axis=0;axis<3;axis++)
    {
        tmp = min(abs(rcData[axis]-MIDRC),500);
        if (tmp>DEADBAND)
        {
            tmp -= DEADBAND;
        }
        else
        {
            tmp=0;
        }
        if(axis!=2)
        { //ROLL & PITCH
            tmp2 = tmp>>7; // 500/128 = 3.9  => range [0;3]
            rcCommand[axis] = lookupPitchRollRC[tmp2] + ((tmp-(tmp2<<7)) * (lookupPitchRollRC[tmp2+1]-lookupPitchRollRC[tmp2])>>7);
            prop1 = 128-((uint16_t)conf.rollPitchRate*tmp>>9); // prop1 was 100, is 128 now -- and /512 instead of /500
            prop1 = (uint16_t)prop1*prop2>>7; // prop1: max is 128   prop2: max is 128   result prop1: max is 128
            dynP8[axis] = (uint16_t)conf.pid[axis].P8*prop1>>7; // was /100, is /128 now
            dynD8[axis] = (uint16_t)conf.pid[axis].D8*prop1>>7; // was /100, is /128 now
        } else {      // YAW
            rcCommand[axis] = tmp;
        }
        if (rcData[axis]<MIDRC)
        {
            rcCommand[axis] = -rcCommand[axis];
        }
    }

    tmp = constrain(rcData[THROTTLE],MINCHECK,2000);
    tmp = (uint32_t)(tmp-MINCHECK)*2559/(2000-MINCHECK); // [MINCHECK;2000] -> [0;2559]
    tmp2 = tmp/256; // range [0;9]
    rcCommand[THROTTLE] = lookupThrottleRC[tmp2] + (tmp-tmp2*256) * (lookupThrottleRC[tmp2+1]-lookupThrottleRC[tmp2]) / 256; // [0;2559] -> expo -> [conf.minthrottle;MAXTHROTTLE]

    if(f.HEADFREE_MODE)
    { //to optimize
        float radDiff = (att.heading - headFreeModeHold) * 0.0174533f; // where PI/180 ~= 0.0174533
        float cosDiff = cos(radDiff);
        float sinDiff = sin(radDiff);
        int16_t rcCommand_PITCH = rcCommand[PITCH]*cosDiff + rcCommand[ROLL]*sinDiff;
        rcCommand[ROLL] =  rcCommand[ROLL]*cosDiff - rcCommand[PITCH]*sinDiff;
        rcCommand[PITCH] = rcCommand_PITCH;
    }

    if ( (calibratingA>0) || (calibratingG>0) )
    { // Calibration phasis
        //LEDPIN_TOGGLE;
    }
    else  if (f.ACC_CALIBRATED)
    {
        //        LEDPIN_OFF;
    }
    else if (f.ARMED)
    {
        //        LEDPIN_ON;
    }

    if ( currentTime > calibratedAccTime )
    {
        if (! f.SMALL_ANGLES_25)
        {
            // the multi uses ACC and is not calibrated or is too much inclinated
            f.ACC_CALIBRATED = 0;
            //LEDPIN_TOGGLE;
            calibratedAccTime = currentTime + 100000;
        }
        else
        {
            f.ACC_CALIBRATED = 1;
        }
    }

}

void setup() {
    initOutput();
    //EEPROM
    readGlobalSet();
    global_conf.currentSet=0;
    readEEPROM();
    configureReceiver();
    initSensors();
    previousTime = micros();
    calibratingG = 512;
    calibratingB = 200;  // 10 seconds init_delay + 200 * 25 ms = 15 seconds before ground pressure settles
    /************************************/
    f.SMALL_ANGLES_25=1; // important for gyro only conf
}

void go_arm() {
    if(calibratingG == 0
#if defined(ONLYARMWHENFLAT)
            && f.ACC_CALIBRATED
#endif
#if defined(FAILSAFE)
            && failsafeCnt < 2
#endif
    )
    {
        if(!f.ARMED && !f.BARO_MODE)
        { // arm now!
            f.ARMED = 1;
#if defined(HEADFREE)
            headFreeModeHold = att.heading;
#endif
            magHold = att.heading;
#ifdef ALTITUDE_RESET_ON_ARM
            calibratingB = 10; // calibrate baro to new ground level (10 * 25 ms = ~250 ms non blocking)
#endif

        }
    } else if(!f.ARMED)
    {
    }
}
void go_disarm()
{
    if (f.ARMED) {
        f.ARMED = 0;
    }
}

// ******** Main Loop *********
void loop () {
    static uint8_t rcDelayCommand; // this indicates the number of time (multiple of RC measurement at 50Hz) the sticks must be maintained to run or switch off motors
    static uint8_t rcSticks;       // this hold sticks position for command combos
    uint8_t axis,i;
    int16_t error,errorAngle;
    int16_t delta;
    int16_t PTerm = 0,ITerm = 0,DTerm, PTermACC, ITermACC;
    static int16_t lastGyro[2] = {0,0};
    static int16_t errorAngleI[2] = {0,0};
#if PID_CONTROLLER == 1
    static int32_t errorGyroI_YAW;
    static int16_t delta1[2],delta2[2];
    static int16_t errorGyroI[2] = {0,0};
#elif PID_CONTROLLER == 2
    static int16_t delta1[3],delta2[3];
    static int32_t errorGyroI[3] = {0,0,0};
    static int16_t lastError[3] = {0,0,0};
    int16_t deltaSum;
    int16_t AngleRateTmp, RateError;
#endif
    static uint16_t rcTime  = 0;
    static int16_t initialThrottleHold;
    int16_t rc;
    int32_t prop = 0;


        if ((int16_t)(currentTime-rcTime) >0 )
        { // 50Hz
            rcTime = currentTime + 20000;
            computeRC();
            // Failsafe routine - added by MIS
#if defined(FAILSAFE)
            if ( failsafeCnt > (5*FAILSAFE_DELAY) && f.ARMED) {                  // Stabilize, and set Throttle to specified level
                for(i=0; i<3; i++) rcData[i] = MIDRC;                               // after specified guard time after RC signal is lost (in 0.1sec)
                rcData[THROTTLE] = conf.failsafe_throttle;
                if (failsafeCnt > 5*(FAILSAFE_DELAY+FAILSAFE_OFF_DELAY)) {          // Turn OFF motors after specified Time (in 0.1sec)
                    go_disarm();     // This will prevent the copter to automatically rearm if failsafe shuts it down and prevents
                    f.OK_TO_ARM = 0; // to restart accidentely by just reconnect to the tx - you will have to switch off first to rearm
                }
                failsafeEvents++;
            }
            if ( failsafeCnt > (5*FAILSAFE_DELAY) && !f.ARMED) {  //Turn of "Ok To arm to prevent the motors from spinning after repowering the RX with low throttle and aux to arm
                go_disarm();     // This will prevent the copter to automatically rearm if failsafe shuts it down and prevents
                f.OK_TO_ARM = 0; // to restart accidentely by just reconnect to the tx - you will have to switch off first to rearm
            }
            failsafeCnt++;
#endif
            // end of failsafe routine - next change is made with RcOptions setting

            // ------------------ STICKS COMMAND HANDLER --------------------
            // checking sticks positions
            uint8_t stTmp = 0;
            for(i=0;i<4;i++) {
                stTmp >>= 2;
                if(rcData[i] > MINCHECK) stTmp |= 0x80;      // check for MIN
                if(rcData[i] < MAXCHECK) stTmp |= 0x40;      // check for MAX
            }
            if(stTmp == rcSticks) {
                if(rcDelayCommand<250) rcDelayCommand++;
            } else rcDelayCommand = 0;
            rcSticks = stTmp;

            // perform actions
            if (rcData[THROTTLE] <= MINCHECK)
            {            // THROTTLE at minimum
                errorGyroI[ROLL] = 0; errorGyroI[PITCH] = 0;
#if PID_CONTROLLER == 1
                errorGyroI_YAW = 0;
#elif PID_CONTROLLER == 2
                errorGyroI[YAW] = 0;
#endif
                errorAngleI[ROLL] = 0; errorAngleI[PITCH] = 0;
            }

            if(rcDelayCommand == 20)
            {
                if(f.ARMED)
                {                   // actions during armed
#ifdef ALLOW_ARM_DISARM_VIA_TX_YAW
                    if (conf.activate[BOXARM] == 0 && rcSticks == THR_LO + YAW_LO + PIT_CE + ROL_CE) go_disarm();    // Disarm via YAW
#endif
#ifdef ALLOW_ARM_DISARM_VIA_TX_ROLL
                    if (conf.activate[BOXARM] == 0 && rcSticks == THR_LO + YAW_CE + PIT_CE + ROL_LO) go_disarm();    // Disarm via ROLL
#endif
                }
                else
                {                        // actions during not armed
                    i=0;
                    if (rcSticks == THR_LO + YAW_LO + PIT_LO + ROL_CE)
                    {    // GYRO calibration
                        calibratingG=512;
                        calibratingB=10;  // calibrate baro to new ground level (10 * 25 ms = ~250 ms non blocking)
                    }
#ifdef ALLOW_ARM_DISARM_VIA_TX_YAW
                    else if (conf.activate[BOXARM] == 0 && rcSticks == THR_LO + YAW_HI + PIT_CE + ROL_CE) go_arm();      // Arm via YAW
#endif
#ifdef ALLOW_ARM_DISARM_VIA_TX_ROLL
                    else if (conf.activate[BOXARM] == 0 && rcSticks == THR_LO + YAW_CE + PIT_CE + ROL_HI) go_arm();      // Arm via ROLL
#endif
                    else if (rcSticks == THR_HI + YAW_LO + PIT_LO + ROL_CE) calibratingA=512;     // throttle=max, yaw=left, pitch=min
                    else if (rcSticks == THR_HI + YAW_HI + PIT_LO + ROL_CE) f.CALIBRATE_MAG = 1;  // throttle=max, yaw=right, pitch=min
                    i=0;
                    if      (rcSticks == THR_HI + YAW_CE + PIT_HI + ROL_CE) {conf.angleTrim[PITCH]+=2; i=1;}
                    else if (rcSticks == THR_HI + YAW_CE + PIT_LO + ROL_CE) {conf.angleTrim[PITCH]-=2; i=1;}
                    else if (rcSticks == THR_HI + YAW_CE + PIT_CE + ROL_HI) {conf.angleTrim[ROLL] +=2; i=1;}
                    else if (rcSticks == THR_HI + YAW_CE + PIT_CE + ROL_LO) {conf.angleTrim[ROLL] -=2; i=1;}
                    if (i)
                    {
                        writeParams(1);
                        rcDelayCommand = 0;    // allow autorepetition
                    }
                }
            }


            // note: if FAILSAFE is disable, failsafeCnt > 5*FAILSAFE_DELAY is always false
            if ((failsafeCnt > 5*FAILSAFE_DELAY) && !f.ANGLE_MODE)
            {
                errorAngleI[ROLL] = 0; errorAngleI[PITCH] = 0;
                f.ANGLE_MODE = 1;
            }


            f.OK_TO_ARM = 1;

            if (f.BARO_MODE)
            {
                    AltHold = alt.EstAlt;
#if defined(ALT_HOLD_THROTTLE_MIDPOINT)
                    initialThrottleHold = ALT_HOLD_THROTTLE_MIDPOINT;
#else
                    initialThrottleHold = rcCommand[THROTTLE];
#endif
                    errorAltitudeI = 0;
                    BaroPID=0;
            }

            if (f.MAG_MODE)
            {
                magHold = att.heading;
            }


            if (conf.headAdj)
            {
                headFreeModeHold = att.heading; // acquire new heading
            }


        } else { // not in rc loop
            static uint8_t taskOrder=0; // never call all functions in the same loop, to avoid high delay spikes
            switch (taskOrder) {
            case 0:
                taskOrder++;
                if (Mag_getADC() != 0) break; // 320 µs
            case 1:
                taskOrder++;
                if (Baro_update() != 0) break; // for MS baro: I2C set and get: 220 us  -  presure and temperature computation 160 us
            case 2:
                taskOrder++;
                if (getEstimatedAltitude() != 0) break; // 280 us
            case 3:
                taskOrder++;
            case 4:
                taskOrder=0;
#if SONAR
                Sonar_update(); //debug[2] = sonarAlt;
#endif
                break;
            }
        }

        while(1) {
            currentTime = micros();
            cycleTime = currentTime - previousTime;
#if defined(LOOP_TIME)
            if (cycleTime >= LOOP_TIME) break;
#else
            break;
#endif
        }
        previousTime = currentTime;

        computeIMU();


        if (abs(rcCommand[YAW]) <70 && f.MAG_MODE)
        {
            int16_t dif = att.heading - magHold;
            if (dif <= - 180) dif += 360;
            if (dif >= + 180) dif -= 360;
            if (f.SMALL_ANGLES_25 )
            {
                rcCommand[YAW] -= dif*conf.pid[PIDMAG].P8 >> 5;  //Always correct maghold in GPS mode
            }
        }
        else
        {
            magHold = att.heading;
        }

        if (f.BARO_MODE)
        {
            static uint8_t isAltHoldChanged = 0;
            static int16_t AltHoldCorr = 0;

            //IF Throttle not ignored then allow change altitude with the stick....
            if ( (abs(rcCommand[THROTTLE]-initialThrottleHold)>ALT_HOLD_THROTTLE_NEUTRAL_ZONE) && !f.THROTTLE_IGNORED)
            {
                // Slowly increase/decrease AltHold proportional to stick movement ( +100 throttle gives ~ +50 cm in 1 second with cycle time about 3-4ms)
                AltHoldCorr+= rcCommand[THROTTLE] - initialThrottleHold;
                if(abs(AltHoldCorr) > 512)
                {
                    AltHold += AltHoldCorr/512;
                    AltHoldCorr %= 512;
                }
                isAltHoldChanged = 1;
            } else if (isAltHoldChanged)
            {
                AltHold = alt.EstAlt;
                isAltHoldChanged = 0;
            }
            rcCommand[THROTTLE] = initialThrottleHold + BaroPID;
        }



#if defined(THROTTLE_ANGLE_CORRECTION)
        if(f.ANGLE_MODE || f.HORIZON_MODE) {
            rcCommand[THROTTLE]+= throttleAngleCorrection;
        }
#endif

        //**** PITCH & ROLL & YAW PID ****
#if PID_CONTROLLER == 1 // evolved oldschool
        if ( f.HORIZON_MODE )
            prop = min(max(abs(rcCommand[PITCH]),abs(rcCommand[ROLL])),512);

        // PITCH & ROLL
        for(axis=0;axis<2;axis++)
        {
            rc = rcCommand[axis]<<1;
            error = rc - imu.gyroData[axis];
            errorGyroI[axis]  = constrain(errorGyroI[axis]+error,-16000,+16000);       // WindUp   16 bits is ok here
            if (abs(imu.gyroData[axis])>640)
            {
                errorGyroI[axis] = 0;
            }

            ITerm = (errorGyroI[axis]>>7)*conf.pid[axis].I8>>6;                        // 16 bits is ok here 16000/125 = 128 ; 128*250 = 32000

            PTerm = mul(rc,conf.pid[axis].P8)>>6;

            if (f.ANGLE_MODE || f.HORIZON_MODE) { // axis relying on ACC
                // 50 degrees max inclination
                errorAngle         = constrain(rc,-500,+500) - att.angle[axis] + conf.angleTrim[axis]; //16 bits is ok here
                errorAngleI[axis]  = constrain(errorAngleI[axis]+errorAngle,-10000,+10000);                                                // WindUp     //16 bits is ok here

                PTermACC           = mul(errorAngle,conf.pid[PIDLEVEL].P8)>>7; // 32 bits is needed for calculation: errorAngle*P8 could exceed 32768   16 bits is ok for result

                int16_t limit      = conf.pid[PIDLEVEL].D8*5;
                PTermACC           = constrain(PTermACC,-limit,+limit);

                ITermACC           = mul(errorAngleI[axis],conf.pid[PIDLEVEL].I8)>>12;   // 32 bits is needed for calculation:10000*I8 could exceed 32768   16 bits is ok for result

                ITerm              = ITermACC + ((ITerm-ITermACC)*prop>>9);
                PTerm              = PTermACC + ((PTerm-PTermACC)*prop>>9);
            }

            PTerm -= mul(imu.gyroData[axis],dynP8[axis])>>6; // 32 bits is needed for calculation

            delta          = imu.gyroData[axis] - lastGyro[axis];  // 16 bits is ok here, the dif between 2 consecutive gyro reads is limited to 800
            lastGyro[axis] = imu.gyroData[axis];
            DTerm          = delta1[axis]+delta2[axis]+delta;
            delta2[axis]   = delta1[axis];
            delta1[axis]   = delta;

            DTerm = mul(DTerm,dynD8[axis])>>5;        // 32 bits is needed for calculation

            axisPID[axis] =  PTerm + ITerm - DTerm;
        }

        //YAW
#define GYRO_P_MAX 300
#define GYRO_I_MAX 250

        rc = mul(rcCommand[YAW] , (2*conf.yawRate + 30))  >> 5;

        error = rc - imu.gyroData[YAW];
        errorGyroI_YAW  += mul(error,conf.pid[YAW].I8);
        errorGyroI_YAW  = constrain(errorGyroI_YAW, 2-((int32_t)1<<28), -2+((int32_t)1<<28));
        if (abs(rc) > 50) errorGyroI_YAW = 0;

        PTerm = mul(error,conf.pid[YAW].P8)>>6;
        int16_t limit = GYRO_P_MAX-conf.pid[YAW].D8;
        PTerm = constrain(PTerm,-limit,+limit);

        ITerm = constrain((int16_t)(errorGyroI_YAW>>13),-GYRO_I_MAX,+GYRO_I_MAX);

        axisPID[YAW] =  PTerm + ITerm;

#elif PID_CONTROLLER == 2 // alexK
#define GYRO_I_MAX 256
#define ACC_I_MAX 256
        prop = min(max(abs(rcCommand[PITCH]),abs(rcCommand[ROLL])),500); // range [0;500]

        //----------PID controller----------
        for(axis=0;axis<3;axis++) {
            //-----Get the desired angle rate depending on flight mode
            if ((f.ANGLE_MODE || f.HORIZON_MODE) && axis<2 ) { // MODE relying on ACC
                // calculate error and limit the angle to 50 degrees max inclination
                errorAngle = constrain((rcCommand[axis]<<1) + GPS_angle[axis],-500,+500) - att.angle[axis] + conf.angleTrim[axis]; //16 bits is ok here
            }
            if (axis == 2) {//YAW is always gyro-controlled (MAG correction is applied to rcCommand)
                AngleRateTmp = (((int32_t) (conf.yawRate + 27) * rcCommand[2]) >> 5);
            } else {
                if (!f.ANGLE_MODE) {//control is GYRO based (ACRO and HORIZON - direct sticks control is applied to rate PID
                    AngleRateTmp = ((int32_t) (conf.rollPitchRate + 27) * rcCommand[axis]) >> 4;
                    if (f.HORIZON_MODE) {
                        //mix up angle error to desired AngleRateTmp to add a little auto-level feel
                        AngleRateTmp += ((int32_t) errorAngle * conf.pid[PIDLEVEL].I8)>>8;
                    }
                } else {//it's the ANGLE mode - control is angle based, so control loop is needed
                    AngleRateTmp = ((int32_t) errorAngle * conf.pid[PIDLEVEL].P8)>>4;
                }
            }

            //--------low-level gyro-based PID. ----------
            //Used in stand-alone mode for ACRO, controlled by higher level regulators in other modes
            //-----calculate scaled error.AngleRates
            //multiplication of rcCommand corresponds to changing the sticks scaling here
            RateError = AngleRateTmp  - imu.gyroData[axis];

            //-----calculate P component
            PTerm = ((int32_t) RateError * conf.pid[axis].P8)>>7;

            //-----calculate I component
            //there should be no division before accumulating the error to integrator, because the precision would be reduced.
            //Precision is critical, as I prevents from long-time drift. Thus, 32 bits integrator is used.
            //Time correction (to avoid different I scaling for different builds based on average cycle time)
            //is normalized to cycle time = 2048.
            errorGyroI[axis]  += (((int32_t) RateError * cycleTime)>>11) * conf.pid[axis].I8;
            //limit maximum integrator value to prevent WindUp - accumulating extreme values when system is saturated.
            //I coefficient (I8) moved before integration to make limiting independent from PID settings
            errorGyroI[axis]  = constrain(errorGyroI[axis], (int32_t) -GYRO_I_MAX<<13, (int32_t) +GYRO_I_MAX<<13);
            ITerm = errorGyroI[axis]>>13;

            //-----calculate D-term
            delta          = RateError - lastError[axis];  // 16 bits is ok here, the dif between 2 consecutive gyro reads is limited to 800
            lastError[axis] = RateError;

            //Correct difference by cycle time. Cycle time is jittery (can be different 2 times), so calculated difference
            // would be scaled by different dt each time. Division by dT fixes that.
            delta = ((int32_t) delta * ((uint16_t)0xFFFF / (cycleTime>>4)))>>6;
            //add moving average here to reduce noise
            deltaSum       = delta1[axis]+delta2[axis]+delta;
            delta2[axis]   = delta1[axis];
            delta1[axis]   = delta;

            //DTerm = (deltaSum*conf.pid[axis].D8)>>8;
            //Solve overflow in calculation above...
            DTerm = ((int32_t)deltaSum*conf.pid[axis].D8)>>8;
            //-----calculate total PID output
            axisPID[axis] =  PTerm + ITerm + DTerm;
        }
#else
#error "*** you must set PID_CONTROLLER to one existing implementation"
#endif
        mixTable();
        writeMotors();
    }


int main(void)
{
    setup();
    loop();

}
