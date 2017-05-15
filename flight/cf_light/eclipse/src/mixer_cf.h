#ifndef _MIXER_CF_H
#define _MIXER_CF_H

#include <stdint.h>


#define YAW_JUMP_PREVENTION_LIMIT_LOW 80
#define YAW_JUMP_PREVENTION_LIMIT_HIGH 500

struct motorConfig_t {

    // PWM values, in milliseconds, common range is 1000-2000 (1 to 2ms)
    uint16_t minthrottle;
    uint16_t maxthrottle;
    uint16_t mincommand;  //not running

    // The update rate of motor outputs (50-498Hz)
    uint16_t motor_pwm_rate;
};



// Custom mixer data per motor
struct motorMixer_t
{
    float throttle;
    float roll;
    float pitch;
    float yaw;
};


// Custom mixer configuration
struct mixer_t
{
    uint8_t motorCount;
    uint8_t useServo;
    const motorMixer_t *motor;
};

struct mixerConfig_t
{
    uint8_t mixerMode;
    bool pid_at_min_throttle;            // when enabled pids are used at minimum throttle
    int8_t yaw_motor_direction;
    uint16_t yaw_jump_prevention_limit;      // make limit configurable (original fixed value was 100)
};

#define CHANNEL_FORWARDING_DISABLED (uint8_t)0xFF


class Mixer
{
private:
    motorConfig_t* motorConfig;
	motorMixer_t customMotorMixer[4];
	motorMixer_t currentMixer[4];
	mixerConfig_t mixerConfig;
	int16_t motor[4];
	int16_t motor_disarmed[4];

	//AIR mode
	bool motorLimitReached;

	bool airModeEnabled;

	void writeAllMotors(int16_t mc);
	void mixerLoadMix(int index, motorMixer_t *customMixers);
	void stopMotors(void);

public:
	Mixer(motorConfig_t* motorConfig, bool isAirMode);
	bool isMotoroLimitReached() { return motorLimitReached; }
	void mixTable(bool isArmed, bool isFailSafeActive, int16_t rcThrottle, bool isThrottleLow, int16_t rcYaw, int16_t* axisPID);
    void writeMotors(void);


};

#endif

