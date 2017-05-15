#include "maths.h"
#include "baro.h"
#include "sensors/baro_drv.h"
#include "sensors/barometer_bmp085.h"
#include "sensors/barometer_bmp280.h"
#include "sensors/_pins.h"
#include "GPIO_Helper.h"

Baro::Baro()
{
    //TODO move to cfg
    baro_sample_count = 21;
    baro_noise_lpf = 0.6f;

    calibratingB = 0;      // baro calibration = get new ground pressure value
    baroPressure_pa = 0;
    baroTemperature = 0;
    baroAlt_cm = 0;

    baroGroundAltitude = 0;
    baroGroundPressure = 0;
    baroPressureSum = 0;

    baroReady = false;
    drv = 0;
    uint32_t i2cClockSpeed = 100000;
    _i2c = GPIO_Helper::SetupI2C(BARO_I2C, i2cClockSpeed);


}

bool Baro::init()
{
    bool rc;
    drv = new Bmp085(_i2c);
    rc = drv->detect();
    if (!rc)
    {
        delete drv;
        drv = new Bmp280(_i2c);
    }
    return drv->detect();
}


bool Baro::isBaroCalibrationComplete(void)
{
    return calibratingB == 0;
}

void Baro::startCalibration()
{
    calibratingB = CALIBRATING_BARO_CYCLES;
}


#define PRESSURE_SAMPLES_MEDIAN 3

static int32_t applyBarometerMedianFilter(int32_t newPressureReading)
{
    static int32_t barometerFilterSamples[PRESSURE_SAMPLES_MEDIAN];
    static int currentFilterSampleIndex = 0;
    static bool medianFilterReady = false;
    int nextSampleIndex;
    
    nextSampleIndex = (currentFilterSampleIndex + 1);
    if (nextSampleIndex == PRESSURE_SAMPLES_MEDIAN) {
        nextSampleIndex = 0;
        medianFilterReady = true;
    }

    barometerFilterSamples[currentFilterSampleIndex] = newPressureReading;
    currentFilterSampleIndex = nextSampleIndex;
    
    if (medianFilterReady)
        return quickMedianFilter3(barometerFilterSamples);
    else
        return newPressureReading;
}


uint32_t Baro::recalculateBarometerTotal(uint8_t baroSampleCount, uint32_t pressureTotal, int32_t newPressureReading)
{
    static int32_t barometerSamples[BARO_SAMPLE_COUNT_MAX];
    static int currentSampleIndex = 0;
    int nextSampleIndex;

    // store current pressure in barometerSamples
    nextSampleIndex = (currentSampleIndex + 1);
    if (nextSampleIndex == baroSampleCount) {
        nextSampleIndex = 0;
        baroReady = true;
    }
    barometerSamples[currentSampleIndex] = applyBarometerMedianFilter(newPressureReading);

    // recalculate pressure total
    // Note, the pressure total is made up of baroSampleCount - 1 samples
    pressureTotal += barometerSamples[currentSampleIndex];
    pressureTotal -= barometerSamples[nextSampleIndex];

    currentSampleIndex = nextSampleIndex;

    return pressureTotal;
}



bool Baro::isBaroReady(void) {
    return baroReady;
}

void Baro::baroUpdate(void)
{
    drv->read(&baroPressure_pa, &baroTemperature);
    baroPressureSum = recalculateBarometerTotal(baro_sample_count, baroPressureSum, baroPressure_pa);
}

int32_t Baro::baroCalculateAltitude(void)
{
    int32_t baroAlt_tmp;

    // calculates height from ground via baro readings
    // see: https://github.com/diydrones/ardupilot/blob/master/libraries/AP_Baro/AP_Baro.cpp#L140
    baroAlt_tmp = lrintf((1.0f - powf((float)(baroPressureSum / (baro_sample_count-1)) / 101325.0f, 0.190295f)) * 4433000.0f); // in cm
    baroAlt_tmp -= baroGroundAltitude;
    baroAlt_cm = lrintf((float)baroAlt_cm * baro_noise_lpf + (float)baroAlt_tmp * (1.0f - baro_noise_lpf)); // additional LPF to reduce baro noise

    return baroAlt_cm;
}

void Baro::performBaroCalibrationCycle(void)
{
    baroGroundPressure -= baroGroundPressure / 8;
    baroGroundPressure += baroPressureSum / (baro_sample_count-1);
    baroGroundAltitude = (1.0f - powf((baroGroundPressure / 8) / 101325.0f, 0.190295f)) * 4433000.0f;

    calibratingB--;
}

