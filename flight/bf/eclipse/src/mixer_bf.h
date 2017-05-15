#ifndef _MIXER_BF_H
#define _MIXER_BF_H

#include <stdint.h>

typedef struct motorMixer_t {
    float throttle;
    float roll;
    float pitch;
    float yaw;
} motorMixer_t;



struct motorConfig_t {

    // PWM values, in milliseconds, common range is 1000-2000 (1 to 2ms)
    uint16_t minthrottle; //min running
    uint16_t maxthrottle; //max running
    uint16_t mincommand;  //disarmed, not running
    uint16_t yaw_direction;

    // The update rate of motor outputs (50-498Hz)
    uint16_t motor_pwm_rate;
};




class Mixer_bf 
{
private:

    int16_t motor[4];
    motorMixer_t currentMixer[4];
    motorConfig_t* mcfg;


    void writeMotors(void);
    void stopMotors(void);
    void writeAllMotors(int16_t mc);

public:
    Mixer_bf (motorConfig_t* cfg);
    void mixTable(int16_t rcThrottle, int16_t rcYaw, int16_t* axisPID, bool armed, bool isThrottleLow);
    void setMinThrottle();
};



#endif
