#ifndef _RC_H 
#include <stdint.h>

#define PULSE_MIN   (750)       // minimum PWM pulse width which is considered valid
#define PULSE_MAX   (2250)      // maximum PWM pulse width which is considered valid

#include "bf_common.h"

#define ROL_LO (1 << (2 * ROLL))
#define ROL_CE (3 << (2 * ROLL))
#define ROL_HI (2 << (2 * ROLL))
#define PIT_LO (1 << (2 * PITCH))
#define PIT_CE (3 << (2 * PITCH))
#define PIT_HI (2 << (2 * PITCH))
#define YAW_LO (1 << (2 * YAW))
#define YAW_CE (3 << (2 * YAW))
#define YAW_HI (2 << (2 * YAW))
#define THR_LO (1 << (2 * THROTTLE))
#define THR_CE (3 << (2 * THROTTLE))
#define THR_HI (2 << (2 * THROTTLE))


struct rcConfig_t
{
    uint16_t midrc;                         // Some radios have not a neutral point centered on 1500. can be changed here
    uint16_t mincheck;                      // minimum rc end
    uint16_t maxcheck;                      // maximum rc end
    uint16_t tpa_breakpoint;                // Breakpoint where TPA is activated

    uint8_t deadband_rp;                       // introduce a deadband around the stick center for pitch and roll axis. Must be greater than zero.
    uint8_t deadband_yaw;                    // introduce a deadband around the stick center for yaw axis. Must be greater than zero.
    int8_t yaw_control_direction;           // change control direction of yaw (inverted, normal)

    uint8_t rcRate;
    uint8_t rcExpo;
    uint8_t thrMid;
    uint8_t thrExpo;
    uint8_t dynThrPID;

	uint8_t rates[3];                      //rpy rate for pid and rc , 20/20/20 in cf, zeros in bf

};

enum RC_ACTION
{
    RC_ACTION_NONE,
    RC_ACTION_ACC_CALIB,
    RC_ACTION_GYRO_CALIB,
    RC_ACTION_MAG_CALIB,
    RC_ACTION_ARM,
    RC_ACTION_DISARM,
    RC_ACTION_ANGLE_TRIM
};

#define PITCH_LOOKUP_LENGTH 7
#define THROTTLE_LOOKUP_LENGTH 12

class RC 
{
private:
	rcConfig_t* cfg;
	int16_t rcData[4];       // interval [1000;2000]
	int16_t rcCommand[4];           // interval [1000;2000] for THROTTLE and [-500;+500] for ROLL/PITCH/YAW
	int16_t lookupPitchRollRC[PITCH_LOOKUP_LENGTH];     // lookup table for expo & RC rate PITCH+ROLL
	int16_t lookupThrottleRC[THROTTLE_LOOKUP_LENGTH];   // lookup table for expo & mid THROTTLE


	void processRC(int32_t* dynAdj);

public:
	RC(rcConfig_t* cfg,  uint16_t minthrottle,  uint16_t maxthrottle);
	RC_ACTION computeRC(bool isArmed, uint8_t* trims);
	void headFreeMode(int16_t heading, int16_t headFreeModeHold);
};



#endif
