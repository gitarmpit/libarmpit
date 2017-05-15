#ifndef _BARO_H
#define _BARO_H
#include <stdint.h>

#define BARO_SAMPLE_COUNT_MAX   48
#define CALIBRATING_BARO_CYCLES             200 // 10 seconds init_delay + 200 * 25 ms = 15 seconds before ground pressure settles

class I2C;
class Baro_drv;

class Baro
{
private:

    I2C* _i2c;

    uint8_t baro_sample_count;              // size of baro filter array
    float baro_noise_lpf;                   // additional LPF to reduce baro noise

    uint16_t calibratingB;      // baro calibration = get new ground pressure value
    int32_t baroPressure_pa;
    int32_t baroTemperature;
    int32_t baroAlt_cm;

    int32_t baroGroundAltitude;
    int32_t baroGroundPressure;
    uint32_t baroPressureSum;

    bool baroReady;

    Baro_drv*  drv;

    uint32_t recalculateBarometerTotal(uint8_t baroSampleCount, uint32_t pressureTotal, int32_t newPressureReading);

public:
    Baro();

    bool init();


    bool isBaroCalibrationComplete(void);

    void startCalibration();

    void baroUpdate(void);
    bool isBaroReady(void);
    int32_t baroCalculateAltitude(void);
    void performBaroCalibrationCycle(void);



};

#endif
