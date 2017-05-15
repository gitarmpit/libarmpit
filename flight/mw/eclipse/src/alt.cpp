#include <stdint.h>

uint8_t getEstimatedAltitude()
{
	return 0;
}
//{
//    int32_t  BaroAlt;
//    static float baroGroundTemperatureScale,logBaroGroundPressureSum;
//    static float vel = 0.0f;
//    static uint16_t previousT;
//    uint16_t currentT = micros();
//    uint16_t dTime;
//
//    dTime = currentT - previousT;
//    if (dTime < UPDATE_INTERVAL) return 0;
//    previousT = currentT;
//
//    if(calibratingB > 0) {
//        logBaroGroundPressureSum = log(baroPressureSum);
//        baroGroundTemperatureScale = ((int32_t)baroTemperature + 27315) * (2 * 29.271267f); // 2 *  is included here => no need for * 2  on BaroAlt in additional LPF
//        calibratingB--;
//    }
//
//    // baroGroundPressureSum is not supposed to be 0 here
//    // see: https://code.google.com/p/ardupilot-mega/source/browse/libraries/AP_Baro/AP_Baro.cpp
//    BaroAlt = ( logBaroGroundPressureSum - log(baroPressureSum) ) * baroGroundTemperatureScale;
//
//    alt.EstAlt = (alt.EstAlt * 6 + BaroAlt ) >> 3; // additional LPF to reduce baro noise (faster by 30 µs)
//    //P
//    int16_t error16 = constrain(AltHold - alt.EstAlt, -300, 300);
//    applyDeadband(error16, 10); //remove small P parametr to reduce noise near zero position
//    BaroPID = constrain((conf.pid[PIDALT].P8 * error16 >>7), -150, +150);
//
//    //I
//    errorAltitudeI += conf.pid[PIDALT].I8 * error16 >>6;
//    errorAltitudeI = constrain(errorAltitudeI,-30000,30000);
//    BaroPID += errorAltitudeI>>9; //I in range +/-60
//
//    applyDeadband(accZ, ACC_Z_DEADBAND);
//
//    static int32_t lastBaroAlt;
//    // could only overflow with a difference of 320m, which is highly improbable here
//    int16_t baroVel = mul((alt.EstAlt - lastBaroAlt) , (1000000 / UPDATE_INTERVAL));
//
//    lastBaroAlt = alt.EstAlt;
//
//    baroVel = constrain(baroVel, -300, 300); // constrain baro velocity +/- 300cm/s
//    applyDeadband(baroVel, 10); // to reduce noise near zero
//
//    // Integrator - velocity, cm/sec
//    vel += accZ * ACC_VelScale * dTime;
//
//    // apply Complimentary Filter to keep the calculated velocity based on baro velocity (i.e. near real velocity).
//    // By using CF it's possible to correct the drift of integrated accZ (velocity) without loosing the phase, i.e without delay
//    vel = vel * 0.985f + baroVel * 0.015f;
//
//    //D
//    alt.vario = vel;
//    applyDeadband(alt.vario, 5);
//    BaroPID -= constrain(conf.pid[PIDALT].D8 * alt.vario >>4, -150, 150);
//    return 1;
//}
