#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>

enum rc {
  ROLL,
  PITCH,
  YAW,
  THROTTLE,
};

enum pid {
  PIDROLL,
  PIDPITCH,
  PIDYAW,
  PIDALT,
  PIDLEVEL,
  PIDMAG,
  PIDVEL,     // not used currently
  PIDITEMS
};

enum box {
    BOXANGLE,
    BOXHORIZON,
    BOXBARO,
    BOXMAG,
    BOXHEADFREE,
    BOXHEADADJ, // acquire heading for HEADFREE mode
    CHECKBOXITEMS
};

typedef struct {
  int16_t  accSmooth[3];
  int16_t  gyroData[3];
  int16_t  magADC[3];
  int16_t  gyroADC[3];
  int16_t  accADC[3];
} imu_t;


typedef struct {
  int32_t  EstAlt;             // in cm
  int16_t  vario;              // variometer in cm/s
} alt_t;

typedef struct {
  int16_t angle[2];            // absolute angle inclination in multiple of 0.1 degree    180 deg = 1800
  int16_t heading;             // variometer in cm/s
} att_t;

typedef struct {
  uint8_t OK_TO_ARM :1 ;
  uint8_t ARMED :1 ;
  uint8_t ACC_CALIBRATED :1 ;
  uint8_t ANGLE_MODE :1 ;
  uint8_t HORIZON_MODE :1 ;
  uint8_t MAG_MODE :1 ;
  uint8_t BARO_MODE :1 ;
  uint8_t HEADFREE_MODE :1 ;
  uint8_t SMALL_ANGLES_25 :1 ;
  uint8_t CALIBRATE_MAG :1 ;
  uint8_t GPS_mode: 2;               // 0-3 NONE,HOLD, HOME, NAV (see GPS_MODE_* defines
  uint8_t THROTTLE_IGNORED : 1;      // If it is 1 then ignore throttle stick movements in baro mode;
} flags_struct_t;

typedef struct
{
  uint8_t currentSet;
  int16_t accZero[3];
  int16_t magZero[3];
} global_conf_t;

struct pid_
{
  uint8_t P8;
  uint8_t I8;
  uint8_t D8;
};


typedef struct {
    pid_    pid[PIDITEMS];
    uint8_t rcRate8;
    uint8_t rcExpo8;
    uint8_t rollPitchRate;
    uint8_t yawRate;
    uint8_t dynThrPID;
    uint8_t thrMid8;
    uint8_t thrExpo8;
    int16_t angleTrim[2];
    uint16_t activate[CHECKBOXITEMS];
    uint8_t powerTrigger1;
    int16_t mag_declination;
    uint8_t Smoothing[3];
    int16_t failsafe_throttle;
    int16_t minthrottle;
} conf_t;



#endif /* TYPES_H_ */
