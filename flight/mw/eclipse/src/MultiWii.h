#ifndef MULTIWII_H_
#define MULTIWII_H_

#define  VERSION        241
#define  NAVI_VERSION   7     //This allow sync with GUI
#include "types.h"

#define MINCHECK 1100
#define MAXCHECK 1900

extern volatile unsigned long timer0_overflow_count;

extern const char pidnames[];
extern const char boxnames[];
extern const uint8_t boxids[];

extern uint32_t currentTime;
extern uint16_t previousTime;
extern uint16_t cycleTime;
extern uint16_t calibratingA;
extern uint16_t calibratingB;
extern uint16_t calibratingG;
extern int16_t  magHold,headFreeModeHold;
extern uint8_t  vbatMin;
extern int32_t  AltHold;
extern int16_t  sonarAlt;
extern int16_t  BaroPID;
extern int16_t  errorAltitudeI;

extern int16_t  i2c_errors_count;
extern global_conf_t global_conf;

extern imu_t imu;
extern alt_t alt;
extern att_t att;

extern int16_t debug[4];

extern conf_t conf;
extern int16_t  annex650_overrun_count;
extern flags_struct_t f;
extern uint16_t intPowerTrigger1;

extern int16_t gyroZero[3];
extern int16_t angle[2];


extern int32_t baroPressure;
extern int16_t baroTemperature; // temp in 0.01 deg
extern int32_t baroPressureSum;

extern int16_t axisPID[3];
extern int16_t motor[4];

extern int16_t failsafeEvents;
extern volatile int16_t failsafeCnt;

extern int16_t rcData[RC_CHANS];
extern int16_t rcCommand[4];
extern int16_t lookupPitchRollRC[5];
extern uint16_t lookupThrottleRC[11];



#if defined(ARMEDTIMEWARNING)
  extern uint32_t  ArmedTimeWarningMicroSeconds;
#endif

#if defined(THROTTLE_ANGLE_CORRECTION)
  extern int16_t throttleAngleCorrection;
  extern int8_t  cosZ;
#endif

void annexCode();
void go_disarm();
#endif /* MULTIWII_H_ */
