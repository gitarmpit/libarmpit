/*
 * This file is part of baseflight
 * Licensed under GPL V3 or modified DCL - see https://github.com/multiwii/baseflight/blob/master/README.md
 */
#include "board.h"
#include "mw.h"


core_t core;

extern rcReadRawDataPtr rcReadRawFunc;

// receiver read function
extern uint16_t pwmReadRawRC(uint8_t chan);



int main(void)
{
    uint8_t i;
    bool sensorsOK = false;
#ifdef SOFTSERIAL_LOOPBACK
    serialPort_t *loopbackPort1 = NULL;
    serialPort_t *loopbackPort2 = NULL;
#endif

    initEEPROM();
    checkFirstTime(false);
    readEEPROM();
    delay(100);

    activateConfig();

    if (feature(FEATURE_VBAT))
        batteryInit();

    initBoardAlignment();

    // We have these sensors; SENSORS_SET defined in board.h depending on hardware platform
    sensorsSet(SENSORS_SET);
    // drop out any sensors that don't seem to work, init all the others. halt if gyro is dead.
    sensorsOK = sensorsAutodetect();


    // if gyro was not detected due to whatever reason, we give up now.
    if (!sensorsOK)
    {
        failureMode(3);
    }


    imuInit(); // Mag is initialized inside imuInit
    mixerInit(); // this will set core.useServo var depending on mixer type



    // configure PWM/CPPM read function and max number of channels. spektrum or sbus below will override both of these, if enabled
    for (i = 0; i < RC_CHANS; i++)
        rcData[i] = 1502;
    rcReadRawFunc = pwmReadRawRC;

    core.numRCChannels = MAX_PWM_INPUTS;

#ifndef CJMCU
    // Optional GPS - available in both PPM, PWM and serialRX input mode, in PWM input, reduces number of available channels by 2.
    // gpsInit will return if FEATURE_GPS is not enabled.
    gpsInit(mcfg.gps_baudrate);
#endif




    previousTime = micros();

    calibratingG = CALIBRATING_GYRO_CYCLES;
    calibratingB = CALIBRATING_BARO_CYCLES;             // 10 seconds init_delay + 200 * 25 ms = 15 seconds before ground pressure settles
    f.SMALL_ANGLE = 1;

    // loopy
    while (1) {
        loop();
    }
}

void HardFault_Handler(void)
{
    // fall out of the sky
    writeAllMotors(mcfg.mincommand);
    while (1);
}
