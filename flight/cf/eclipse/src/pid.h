
#ifndef _PID_CF_H
#define _PID_CF_H

#include <stdint.h>
#include "filter.h"
#include "runtime_config.h"

#define PID_MAX_I 256
#define PID_MAX_D 512
#define PID_MAX_TOTAL_PID 1000

#define GYRO_I_MAX 256                      // Gyro I limiter
#define YAW_P_LIMIT_MIN 100                 // Maximum value for yaw P limiter
#define YAW_P_LIMIT_MAX 500                 // Maximum value for yaw P limiter

typedef enum
{
    PIDROLL,
    PIDPITCH,
    PIDYAW,
} pidIndex_e;

typedef enum
{
    PID_CONTROLLER_MW23 = 0,
    PID_CONTROLLER_MWREWRITE,
    PID_CONTROLLER_LUX_FLOAT,
    PID_COUNT
} pidControllerType_e;

typedef enum
{
    PID_DELTA_FROM_MEASUREMENT = 0,
    PID_DELTA_FROM_ERROR
} pidDeltaMethod_e;

typedef enum
{
    HORIZON_TILT_MODE_SAFE = 0,
    HORIZON_TILT_MODE_EXPERT
} horizonTiltMode_e;

enum StabilizationMode
{
	NONE = 0,
    ANGLE_MODE_ = 1,
    HORIZON_MODE_ = 2,

};


struct pidConfig_t
{
	//att ypr
    uint8_t  P[3];
    uint8_t  I[3];
    uint8_t  D[3];

    uint8_t  pidLevel_P;
    uint8_t  pidLevel_I;
    uint8_t  pidLevel_D;

    uint8_t  pidMAG_P;

    uint16_t yaw_p_limit;                   // set P term limit (fixed value was 300)
    uint16_t dterm_lpf_hz;                  // Delta Filter in hz
    uint16_t yaw_lpf_hz;                    // additional yaw filter when yaw axis too noisy
    uint8_t  deltaMethod;

    uint8_t horizon_tilt_effect;            // inclination factor for Horizon mode
    uint8_t horizon_tilt_mode;              // SAFE or EXPERT

    uint8_t dterm_filter_type;              // Filter selection for dterm
    uint16_t dterm_notch_hz;                // Biquad dterm notch hz
    uint16_t dterm_notch_cutoff;            // Biquad dterm notch low cutoff

    uint16_t max_angle_inclination;         // max inclination allowed in angle (level) mode. default 500 (50 degrees).
    float   gyroScale;
    int pid_process_denom;                  //// Processing denominator for PID controller vs gyro sampling rate

};

struct rollAndPitchTrims_t {
    int16_t roll;
    int16_t pitch;
};



class Pid
{
private:

    pidConfig_t* pcfg;
    int16_t axisPID[3];
    uint32_t targetPidLooptime;

    const float luxPTermScale = 1.0f / 128;
    const float luxITermScale = 1000000.0f / 0x1000000;
    const float luxDTermScale = (0.000001f * (float)0xFFFF) / 512;
    const float luxGyroScale = 16.4f / 4; // the 16.4 is needed because mwrewrite does not scale according to the gyro model gyro.scale

    float dT;
    uint8_t PIDweight[3];
    int32_t lastITerm[3], ITermLimit[3];
    float lastITermf[3], ITermLimitf[3];

    pt1Filter_t deltaFilter[3];
    pt1Filter_t yawFilter;
    biquadFilter_t dtermFilterNotch[3];
    biquadFilter_t dtermFilterLpf[3];

    uint8_t* rates;

    int32_t ITermAngle[2];

    //only used by mw32
    uint8_t dynP8[3], dynI8[3], dynD8[3];


    rollAndPitchTrims_t rollPitchTrims;

    void pidInitFilters();
    void pidResetITermAngle(void);

    int16_t pidLuxFloatCore(int axis, float gyroRate, float angleRate, bool antiWindupOn, bool motorSaturated);
    int16_t mwRewriteCore(int axis, int32_t gyroRate, int32_t angleRate, bool antiWindupOn, bool motorSaturated);

protected:

    bool _airMode;

public:
    Pid(pidConfig_t* pidConfig, int16_t gyro_sample_hz, bool airMode, uint8_t* controlRates);

    //int16_t* getAxisPID() { return axisPID; }

    int16_t* pidLuxFloat(int16_t* rcCommand,
            float* gyroADCf,
            int16_t* attitude,
            bool antiWindupOn,
            bool motorSaturated,
            int32_t horizonLevelStrength,
			StabilizationMode mode);

    int16_t* mwRewrite(int16_t* rcCommand,
            int32_t* gyroADC,
            int16_t* attitude,
            bool antiWindupOn,
            bool motorSaturated,
            int32_t horizonLevelStrength,
			StabilizationMode mode);

    int16_t* mw23(int16_t* rcCommand,
            int32_t* gyroADC,
            int16_t* attitude,
            bool antiWindupOn,
            bool motorSaturated,
			StabilizationMode mode);

    void pidResetITerm(void);

    uint32_t getLoopTime() { return targetPidLooptime; }
    uint8_t* getPIDweight() { return PIDweight; }

    int calcHorizonLevelStrength(int32_t mostDeflectedPos, int32_t imuRoll, int32_t imuPitch);

};

#endif
