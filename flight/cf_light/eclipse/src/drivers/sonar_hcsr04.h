#pragma once

#include "sonar.h"

typedef struct sonarRange_s {
    int16_t maxRangeCm;
    // these are full detection cone angles, maximum tilt is half of this
    int16_t detectionConeDeciDegrees; // detection cone angle as in HC-SR04 device spec
    int16_t detectionConeExtendedDeciDegrees; // device spec is conservative, in practice have slightly larger detection cone
} sonarRange_t;


#define HCSR04_MAX_RANGE_CM 400 // 4m, from HC-SR04 spec sheet
#define HCSR04_DETECTION_CONE_DECIDEGREES 300 // recommended cone angle30 degrees, from HC-SR04 spec sheet
#define HCSR04_DETECTION_CONE_EXTENDED_DECIDEGREES 450 // in practice 45 degrees seems to work well

void hcsr04_init(sonarRange_t *sonarRange);
void hcsr04_start_reading(void);
int32_t hcsr04_get_distance(void);
