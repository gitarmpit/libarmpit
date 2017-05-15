#ifndef _ALTHOLD_H
#define _ALTHOLD_H


struct altHoldConfig_t
{
    uint8_t pidAlt_P;
    uint8_t pidAlt_I;
    uint8_t pidAlt_D;

    uint8_t pidVelocity_P;
    uint8_t pidVelocity_I;
    uint8_t pidVelocity_D;
    uint8_t alt_hold_deadband;              // defines the neutral zone of throttle stick during altitude hold, default setting is +/-40
    uint8_t alt_hold_fast_change;           // when disabled, turn off the althold when throttle stick is out of deadband defined with alt_hold_deadband; when enabled, altitude changes slowly proportional to stick movement
    float baro_cf_vel;                      // apply Complimentary Filter to keep the calculated velocity based on baro velocity (i.e. near real velocity)
    float baro_cf_alt;                      // apply CF to use ACC for height estimation

};

class AltHold
{
private:
    float   accVelScale;
    int32_t altHold;
    int32_t vario;
    int16_t initialRawThrottleHold;
    int16_t initialThrottleHold;
    int32_t _estimatedAltitude;                // in cm
    int32_t setVelocity;
    bool    velocityControl;
    int32_t errorVelocityI;
    int32_t altHoldThrottleAdjustment;

    altHoldConfig_t* altHoldCfg;

    bool    isThrustFacingDownwards(float imuPitch, float imuRoll);
    int32_t calculateAltHoldThrottleAdjustment(int32_t vel_tmp, float accZ_tmp, float accZ_old, float imuPitch, float imuRoll);


public:
    AltHold (float acc_1G);

    //fc.cpp
    void calculateEstimatedAltitude(uint32_t currentTime,
    		int32_t baroAlt,
			uint32_t accTimeSum,
			int32_t accSumCount,
			int32_t accSum,
			float imuPitch,
			float imuRoll,
			float cosTiltAngle);

    //returns thr adusted for alt hold
    int16_t applyAltHold(int16_t thr_raw, int16_t thr);

    void activateAltHold(int16_t raw_thr, int16_t thr);

};

#endif
