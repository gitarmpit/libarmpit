#include "cfg.h"

Config::Config()
{
    set_defaults();
}

void Config::set_defaults()
{
    motorConfig.minthrottle = 1000; //1150
    motorConfig.maxthrottle = 2000; //1850
    motorConfig.mincommand  = 900;  //1000
    motorConfig.motor_pwm_rate = 400;

    rxConfig.midrc = 1500;
    rxConfig.mincheck = 1100;
    rxConfig.maxcheck = 1900;
    rxConfig.rx_min_usec = 885;          // any of first 4 channels below this value will trigger rx loss detection
    rxConfig.rx_max_usec = 2115;         // any of first 4 channels above this value will trigger rx loss detection
    rxConfig.rcSmoothing = false;

    controlRateConfig.rcRate8 = 90;
    //rcRate8 = 100;
    controlRateConfig.rcExpo8 = 65;
    controlRateConfig.thrMid8 = 5;
    controlRateConfig.tpa_breakpoint = 1500;
    controlRateConfig.dynThrPID = 0;
    controlRateConfig.thrExpo8 = 0;
    controlRateConfig.rcYawExpo8 = 0;
    controlRateConfig.rates[0] = 20;
    controlRateConfig.rates[1] = 20;
    controlRateConfig.rates[2] = 20;

    pidConfig.P[PIDROLL] = 40;
    pidConfig.I[PIDROLL] = 30;
    pidConfig.D[PIDROLL] = 23;

    pidConfig.P[PIDPITCH] = 40;
    pidConfig.I[PIDPITCH] = 30;
    pidConfig.D[PIDPITCH] = 23;

    pidConfig.P[PIDYAW] = 85;
    pidConfig.I[PIDYAW] = 45;
    pidConfig.D[PIDYAW] = 0;

    pidConfig.pidLevel_P = 20;
    pidConfig.pidLevel_I = 10;
    pidConfig.pidLevel_D = 75;

    pidConfig.pidMAG_P = 40;  //TODO move to mag config

    pidConfig.yaw_p_limit = YAW_P_LIMIT_MAX;
    pidConfig.yaw_lpf_hz = 0;
    pidConfig.dterm_filter_type = FILTER_BIQUAD;
    pidConfig.dterm_lpf_hz = 100;    // filtering ON by default
    pidConfig.dterm_notch_hz = 260;
    pidConfig.dterm_notch_cutoff = 160;
    pidConfig.deltaMethod = PID_DELTA_FROM_MEASUREMENT;
    pidConfig.horizon_tilt_effect = 75;
    pidConfig.horizon_tilt_mode = HORIZON_TILT_MODE_SAFE;
    pidConfig.max_angle_inclination = 500;    // 50 degrees
    pidConfig.pid_process_denom = 2;
    // 16.4 dps/lsb scalefactor: LSB sensitivity 32768 / 2000dps
    pidConfig.gyroScale = 1.0f / 16.4f;

    accConfig.acc_cut_hz = 15;
    accConfig.accSamplingInterval = 1000;
    accConfig.acc_1G = 4096;  //32768 / 8g full scale (mpu configuration, see mpu6500)

    imuConfig.accz_lpf_cutoff = 5.0f;
    imuConfig.accDeadband_xy = 40;
    imuConfig.accDeadband_z = 40;
    imuConfig.acc_unarmedcal = 1;
    imuConfig.dcm_kp = 2500;                // 1.0 * 10000
    imuConfig.small_angle = 25;
    imuConfig.throttle_correction_value = 0;      // could 10 with althold or 40 for fpv
    imuConfig.throttle_correction_angle = 800;    // could be 80.0 deg with atlhold or 45.0 for fpv

    gyroConfig.gyro_lpf = GYRO_LPF_256HZ;
    gyroConfig.gyro_soft_type = FILTER_PT1;
    gyroConfig.gyro_soft_lpf_hz = 90;    // software based lpf filter for gyro
    gyroConfig.gyro_soft_notch_hz = 0;
    gyroConfig.gyro_soft_notch_cutoff_hz = 130;
    gyroConfig.gyro_sync = 1;
    gyroConfig.gyro_sample_hz = 4000;  //cannot exceed sensor's sample freq
    gyroConfig.gyroMovementCalibrationThreshold = 32;  //TODO  not used, need to?

    rcControlsConfig.deadband = 0;
    rcControlsConfig.yaw_deadband = 0;
    rcControlsConfig.yaw_control_direction = 1;

    altHoldConfig.alt_hold_deadband = 40;
    altHoldConfig.alt_hold_fast_change = 1;
    altHoldConfig.pidAlt_P = 50;
    altHoldConfig.pidAlt_I = 0;
    altHoldConfig.pidAlt_D = 0;
    altHoldConfig.pidVelocity_P = 120;
    altHoldConfig.pidVelocity_I = 45;
    altHoldConfig.pidVelocity_D = 1;
    altHoldConfig.baro_cf_vel = 0.985f;
    altHoldConfig.baro_cf_alt = 0.965f;



    failsafeConfig.failsafe_delay = 10;              // 1sec
    failsafeConfig.failsafe_off_delay = 200;         // 20sec
    failsafeConfig.failsafe_throttle = 1000;         // default throttle off.
    failsafeConfig.failsafe_throttle_low_delay = 100; // default throttle low delay for "just disarm" on failsafe condition
    //    failsafeConfig()->failsafe_delay = 2;
    //    failsafeConfig()->failsafe_off_delay = 0;

    rollPitchTrims.pitch = 0;
    rollPitchTrims.roll = 0;

    airMode = false;

}

//void Config::applyAndSaveAccelerometerTrimsDelta(rollAndPitchTrims_t* trims)
//{
//    rollPitchTrims.roll += trims->roll;
//    rollPitchTrims.pitch += trims->pitch;
//
//    //TODO
//    //saveConfigAndNotify();
//}


//TODO shoulde we pass them
//void Config::resetRollAndPitchTrims()
//{
//    rollPitchTrims.roll = 0;
//    rollPitchTrims.pitch = 0;
//}


