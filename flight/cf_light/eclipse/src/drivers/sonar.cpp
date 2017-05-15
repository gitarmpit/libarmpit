#include <stdbool.h>
#include <stdint.h>
#include <math.h>


#include "maths.h"

#include "sonar_hcsr04.h"


#include "sensors.h"
#include "sonar.h"

// Sonar measurements are in cm, a value of SONAR_OUT_OF_RANGE indicates sonar is not in range.
// Inclination is adjusted by imu
float baro_cf_vel;                      // apply Complimentary Filter to keep the calculated velocity based on baro velocity (i.e. near real velocity)
float baro_cf_alt;                      // apply CF to use ACC for height estimation

#ifdef SONAR
int16_t sonarMaxRangeCm;
int16_t sonarMaxAltWithTiltCm;
int16_t sonarCfAltCm; // Complimentary Filter altitude
int16_t sonarMaxTiltDeciDegrees;
float sonarMaxTiltCos;

static int32_t calculatedAltitude;


void sonarInit()
{
    sonarRange_t sonarRange;
    hcsr04_init(&sonarRange);
    sensorsSet(SENSOR_SONAR);
    sonarMaxRangeCm = sonarRange.maxRangeCm;
    sonarCfAltCm = sonarMaxRangeCm / 2;
    sonarMaxTiltDeciDegrees =  sonarRange.detectionConeExtendedDeciDegrees / 2;
    sonarMaxTiltCos = cos_approx(sonarMaxTiltDeciDegrees / 10.0f * RAD);
    sonarMaxAltWithTiltCm = sonarMaxRangeCm * sonarMaxTiltCos;
    calculatedAltitude = SONAR_OUT_OF_RANGE;
}

#define DISTANCE_SAMPLES_MEDIAN 5

static int32_t applySonarMedianFilter(int32_t newSonarReading)
{
    static int32_t sonarFilterSamples[DISTANCE_SAMPLES_MEDIAN];
    static int currentFilterSampleIndex = 0;
    static bool medianFilterReady = false;
    int nextSampleIndex;

    if (newSonarReading > SONAR_OUT_OF_RANGE) // only accept samples that are in range
    {
        nextSampleIndex = (currentFilterSampleIndex + 1);
        if (nextSampleIndex == DISTANCE_SAMPLES_MEDIAN) {
            nextSampleIndex = 0;
            medianFilterReady = true;
        }

        sonarFilterSamples[currentFilterSampleIndex] = newSonarReading;
        currentFilterSampleIndex = nextSampleIndex;
    }
    if (medianFilterReady)
        return quickMedianFilter5(sonarFilterSamples);
    else
        return newSonarReading;
}

void sonarUpdate(void)
{
    hcsr04_start_reading();
}

/**
 * Get the last distance measured by the sonar in centimeters. When the ground is too far away, SONAR_OUT_OF_RANGE is returned.
 */
int32_t sonarRead(void)
{
    int32_t distance = hcsr04_get_distance();
    if (distance > HCSR04_MAX_RANGE_CM)
        distance = SONAR_OUT_OF_RANGE;

    return applySonarMedianFilter(distance);
}

/**
 * Apply tilt correction to the given raw sonar reading in order to compensate for the tilt of the craft when estimating
 * the altitude. Returns the computed altitude in centimeters.
 *
 * When the ground is too far away or the tilt is too large, SONAR_OUT_OF_RANGE is returned.
 */
int32_t sonarCalculateAltitude(int32_t sonarDistance, float cosTiltAngle)
{
    // calculate sonar altitude only if the ground is in the sonar cone
    if (cosTiltAngle <= sonarMaxTiltCos)
        calculatedAltitude = SONAR_OUT_OF_RANGE;
    else
        // altitude = distance * cos(tiltAngle), use approximation
        calculatedAltitude = sonarDistance * cosTiltAngle;
    return calculatedAltitude;
}

/**
 * Get the latest altitude that was computed by a call to sonarCalculateAltitude(), or SONAR_OUT_OF_RANGE if sonarCalculateAltitude
 * has never been called.
 */
int32_t sonarGetLatestAltitude(void)
{
    return calculatedAltitude;
}

#endif
