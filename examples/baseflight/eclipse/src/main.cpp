/*
 * This file is part of baseflight
 * Licensed under GPL V3 or modified DCL - see https://github.com/multiwii/baseflight/blob/master/README.md
 */
#include "board.h"
#include "mw.h"
#include "utils.h"

extern rcReadRawDataPtr rcReadRawFunc;

// receiver read function
extern uint16_t pwmReadRawRC(uint8_t chan);

int main(void)
{
    uint8_t i;
    //TODO drv_adc_config_t adc_params;
    bool sensorsOK = false;

    //read eeprom
    checkFirstTime(false);
    readEEPROM();

    //TODO SetSysClock(mcfg.emf_avoidance);


    // sleep for 100ms
    delay(100);

    activateConfig();

    //TODO init I2C

    // configure power ADC
    //TODO
    //    if (mcfg.power_adc_channel > 0 && (mcfg.power_adc_channel == 1 || mcfg.power_adc_channel == 9 || mcfg.power_adc_channel == 5))
//        adc_params.powerAdcChannel = mcfg.power_adc_channel;
//    else {
//        adc_params.powerAdcChannel = 0;
//        mcfg.power_adc_channel = 0;
//    }

    // configure rssi ADC
    //TODO
    //    if (mcfg.rssi_adc_channel > 0 && (mcfg.rssi_adc_channel == 1 || mcfg.rssi_adc_channel == 9 || mcfg.rssi_adc_channel == 5) && mcfg.rssi_adc_channel != mcfg.power_adc_channel)
//        adc_params.rssiAdcChannel = mcfg.rssi_adc_channel;
//    else {
//        adc_params.rssiAdcChannel = 0;
//        mcfg.rssi_adc_channel = 0;
//    }

    //TODO Init ADC

    // Check battery type/voltage
    //TODO
    //batteryInit();
    initBoardAlignment();

    // drop out any sensors that don't seem to work, init all the others. halt if gyro is dead.
    sensorsOK = init_sensors();

    // production debug output
#ifdef PROD_DEBUG
    productionDebug();
#endif

    // if gyro was not detected due to whatever reason, we give up now.
    if (!sensorsOK)
        while(3);  //TODO

    imuInit(); // Mag is initialized inside imuInit
    mixerInit(); // this will set core.useServo var depending on mixer type


    // when using airplane/wing mixer, servo/motor outputs are remapped


    // configure PWM/CPPM read function and max number of channels. spektrum or sbus below will override both of these, if enabled
    for (i = 0; i < RC_CHANS; i++)
        rcData[i] = 1502;
    rcReadRawFunc = pwmReadRawRC;

    //TODO
//    gpsInit(mcfg.gps_baudrate);

//TODO
#ifdef SONAR
        // sonar stuff only works with PPM
        if (feature(FEATURE_SONAR))
            Sonar_init();
#endif


    previousTime = micros();

    calibratingA = CALIBRATING_ACC_CYCLES;
    calibratingG = CALIBRATING_GYRO_CYCLES;
    calibratingB = CALIBRATING_BARO_CYCLES;             // 10 seconds init_delay + 200 * 25 ms = 15 seconds before ground pressure settles
    flags.SMALL_ANGLE = 1;

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
