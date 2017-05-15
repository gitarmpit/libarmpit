//#ifdef BARO
//#define UPDATE_INTERVAL 25000   // 40hz update rate (20hz LPF on acc)
//
//int Imu::getEstimatedAltitude(void)
//{
//    static uint32_t previousT;
//    uint32_t currentT = micros();
//    uint32_t dTime;
//    int32_t error;
//    int32_t baroVel;
//    int32_t vel_tmp;
//    int32_t BaroAlt_tmp;
//    int32_t setVel;
//    float dt;
//    float vel_acc;
//    float accZ_tmp;
//    static float accZ_old = 0.0f;
//    static float vel = 0.0f;
//    static float accAlt = 0.0f;
//    static int32_t lastBaroAlt;
//    static int32_t baroGroundAltitude = 0;
//    static int32_t baroGroundPressure = 0;
//    int16_t tiltAngle = max(abs(angle[ROLL]), abs(angle[PITCH]));
//
//    dTime = currentT - previousT;
//    if (dTime < UPDATE_INTERVAL)
//        return 0;
//    previousT = currentT;
//
//    if (calibratingB > 0) {
//        baroGroundPressure -= baroGroundPressure / 8;
//        baroGroundPressure += baroPressureSum / (cfg.baro_tab_size - 1);
//        baroGroundAltitude = (1.0f - powf((baroGroundPressure / 8) / 101325.0f, 0.190295f)) * 4433000.0f;
//
//        vel = 0;
//        accAlt = 0;
//        calibratingB--;
//    }
//
//    // calculates height from ground via baro readings
//    // see: https://github.com/diydrones/ardupilot/blob/master/libraries/AP_Baro/AP_Baro.cpp#L140
//    BaroAlt_tmp = lrintf((1.0f - powf((float)(baroPressureSum / (cfg.baro_tab_size - 1)) / 101325.0f, 0.190295f)) * 4433000.0f); // in cm
//    BaroAlt_tmp -= baroGroundAltitude;
//    BaroAlt = lrintf((float)BaroAlt * cfg.baro_noise_lpf + (float)BaroAlt_tmp * (1.0f - cfg.baro_noise_lpf)); // additional LPF to reduce baro noise
//
//    // calculate sonar altitude only if the sonar is facing downwards(<25deg)
//    if (tiltAngle > 250)
//        sonarAlt = -1;
//    else
//        sonarAlt = sonarAlt * (900.0f - tiltAngle) / 900.0f;
//
//    // do sonarAlt and baroAlt fusion
//    if (sonarAlt > 0 && sonarAlt < 200) {
//        baroAlt_offset = BaroAlt - sonarAlt;
//        BaroAlt = sonarAlt;
//    } else {
//        BaroAlt -= baroAlt_offset;
//        if (sonarAlt > 0) {
//            sonarTransition = (300 - sonarAlt) / 100.0f;
//            BaroAlt = sonarAlt * sonarTransition + BaroAlt * (1.0f - sonarTransition);
//        }
//    }
//
//    dt = accTimeSum * 1e-6f; // delta acc reading time in seconds
//
//    // Integrator - velocity, cm/sec
//    accZ_tmp = (float)accSum[2] / (float)accSumCount;
//    vel_acc = accZ_tmp * accVelScale * (float)accTimeSum;
//
//    // Integrator - Altitude in cm
//    accAlt += (vel_acc * 0.5f) * dt + vel * dt;                                         // integrate velocity to get distance (x= a/2 * t^2)
//    accAlt = accAlt * cfg.baro_cf_alt + (float)BaroAlt * (1.0f - cfg.baro_cf_alt);      // complementary filter for altitude estimation (baro & acc)
//
//    // when the sonar is in his best range
//    if (sonarAlt > 0 && sonarAlt < 200)
//        EstAlt = BaroAlt;
//    else
//        EstAlt = accAlt;
//
//    vel += vel_acc;
//
//#if 0
//    debug[0] = accSum[2] / accSumCount; // acceleration
//    debug[1] = vel;                     // velocity
//    debug[2] = accAlt;                  // height
//#endif
//
//    accSum_reset();
//
//    baroVel = (BaroAlt - lastBaroAlt) * 1000000.0f / dTime;
//    lastBaroAlt = BaroAlt;
//
//    baroVel = constrain(baroVel, -1500, 1500);    // constrain baro velocity +/- 1500cm/s
//    baroVel = applyDeadband(baroVel, 10);         // to reduce noise near zero
//
//    // apply Complimentary Filter to keep the calculated velocity based on baro velocity (i.e. near real velocity).
//    // By using CF it's possible to correct the drift of integrated accZ (velocity) without loosing the phase, i.e without delay
//    vel = vel * cfg.baro_cf_vel + baroVel * (1 - cfg.baro_cf_vel);
//    vel_tmp = lrintf(vel);
//
//    // set vario
//    vario = applyDeadband(vel_tmp, 5);
//
//    if (tiltAngle < 800) { // only calculate pid if the copters thrust is facing downwards(<80deg)
//        // Altitude P-Controller
//        if (!velocityControl) {
//            error = constrain(AltHold - EstAlt, -500, 500);
//            error = applyDeadband(error, 10);       // remove small P parametr to reduce noise near zero position
//            setVel = constrain((cfg.P8[PIDALT] * error / 128), -300, +300); // limit velocity to +/- 3 m/s
//        } else {
//            setVel = setVelocity;
//        }
//
//        // Velocity PID-Controller
//        // P
//        error = setVel - vel_tmp;
//        BaroPID = constrain((cfg.P8[PIDVEL] * error / 32), -300, +300);
//
//        // I
//        errorVelocityI += (cfg.I8[PIDVEL] * error);
//        errorVelocityI = constrain(errorVelocityI, -(8196 * 200), (8196 * 200));
//        BaroPID += errorVelocityI / 8196;     // I in the range of +/-200
//
//        // D
//        BaroPID -= constrain(cfg.D8[PIDVEL] * (accZ_tmp + accZ_old) / 512, -150, 150);
//
//    } else {
//        BaroPID = 0;
//    }
//
//    accZ_old = accZ_tmp;
//
//    return 1;
//}
//#endif /* BARO */
