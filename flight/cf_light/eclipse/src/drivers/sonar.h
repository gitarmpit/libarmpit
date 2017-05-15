#pragma once

#define SONAR_OUT_OF_RANGE (-1)


extern int16_t sonarMaxRangeCm;
extern int16_t sonarCfAltCm;
extern int16_t sonarMaxAltWithTiltCm;

void sonarUpdate(void);
int32_t sonarRead(void);
int32_t sonarCalculateAltitude(int32_t sonarDistance, float cosTiltAngle);
int32_t sonarGetLatestAltitude(void);
void sonarInit();

