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
