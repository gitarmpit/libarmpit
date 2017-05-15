uint16_t pwmReadRawRC(uint8_t chan)
{
    return 0; //TODO read pwm here
}

void computeRC(void)
{
}

static void mwArm(void)
{
}

static void mwDisarm(void)
{
}

static void mwVario(void)
{

}
static void pidMultiWii(void)
{
}

static void pidRewrite(void)
{
}

void annexCode(void)
{
   rcData ->rcCommand
   headfree
   vbat
   buzzerUpdate();
   calib leds
   gps led toggle
}

static void rc_loop() 
{
        readRC();
        failsafe
        process sticks  (rc)

        if (f.ARMED) 
            {      // actions during armed
                check stick pos to disarm
            } 
        else 
            {            // actions during not armed
                check stick pos and trigger multi profile, arm/disarm/calib/trim
            }

        inflight cal

        // note: if FAILSAFE is disabled, failsafeCnt > 5 * FAILSAVE_DELAY is always false
        if (rcOptions[BOXANGLE] || (failsafeCnt > 5 * cfg.failsafe_delay) ) {
            // bumpless transfer to Level mode
            if (!f.ANGLE_MODE) {
                errorAngleI[ROLL] = 0;
                errorAngleI[PITCH] = 0;
                f.ANGLE_MODE = 1;
            }
            if (feature(FEATURE_FW_FAILSAFE_RTH)) {
                if ((failsafeCnt > 5 * cfg.failsafe_delay) && sensors(SENSOR_GPS)) {
                    f.FW_FAILSAFE_RTH_ENABLE = 1;
                }
            }
        } else {
            f.ANGLE_MODE = 0;   // failsafe support
            f.FW_FAILSAFE_RTH_ENABLE = 0;
        }

        chec horizon box
        ok to arm flag and led
        activate baro/magHold/horizon if box set

}

void loop(void)
{

    if (((int32_t)(currentTime - rcTime) >= 0) || rcReady) 
    { // 50Hz or data driven
        rc_loop:
                readRC();
                failsafe
                process sticks  (rc)

    } 
    else 
    {                        // not in rc loop
        read sensors, calc alt etc
    }

    computeIMU();
    processRC(); //rcdata -> rcCommand, set dynPID   ("annex" code) 
    headFree
    magHold
    baro : alt hold

    if (cfg.throttle_correction_value && (f.ANGLE_MODE || f.HORIZON_MODE)) {
        rcCommand[THROTTLE] += throttleAngleCorrection;
    }

    gps 
    pid_controller();
    mixTable();
    writeMotors();
}
