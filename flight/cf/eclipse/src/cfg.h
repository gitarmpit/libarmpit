#ifndef _CFG_H
#define _CFG_H
#include <stdint.h>
#include "pid.h"
#include "acc.h"
#include "gyro.h"
#include "rc_controls.h"
#include "failsafe.h"
#include "rc_curve.h"
#include "altitudehold.h"
#include "rx.h"
#include "imu.h"
#include "mixer.h"

class Config 
{
private:

	motorConfig_t motorConfig;
	controlRateConfig_t controlRateConfig;
	//TODO hide it inside Pid class
	pidConfig_t pidConfig;

	accConfig_t accConfig;
	gyroConfig_t gyroConfig;
	rcControlsConfig_t rcControlsConfig;
	failsafeConfig_t failsafeConfig;
	altHoldConfig_t altHoldConfig;
	rxConfig_t rxConfig;
	imuConfig_t imuConfig;

	rollAndPitchTrims_t rollPitchTrims;


	bool airMode;

    Config();
    Config(const Config&);

    void set_defaults();

public:
    static Config* GetInstance()
    {
        static Config instance;
        return &instance;
    }
    
    controlRateConfig_t* getControlRateConfig() { return &controlRateConfig; }
    pidConfig_t* getPidProfile() { return &pidConfig; }

    motorConfig_t* getMotorConfig() { return &motorConfig; }

    accConfig_t* getAccConfig() { return &accConfig; }
    gyroConfig_t* getGyroConfig() { return &gyroConfig; }
    rcControlsConfig_t* getRcControlsConfig() { return &rcControlsConfig; }
    failsafeConfig_t* getFailsafeConfig() { return &failsafeConfig; }
    altHoldConfig_t* getAltHoldConfig() { return &altHoldConfig; }
    rxConfig_t*  getRxConfig() { return &rxConfig; }
    imuConfig_t* getImpuConfig() { return &imuConfig; }

//    void applyAndSaveAccelerometerTrimsDelta(rollAndPitchTrims_t* trims);
//    void resetRollAndPitchTrims();
//    rollAndPitchTrims_t* getRollPitchTrims() { return &rollPitchTrims; }

    bool getAirMode() { return airMode; }
};



#endif
