#include "common.h"
#include "system_time.h"
#include "RCC.h"

#define SONAR
#define BARO
#define MAG

#define USE_MPU9250_MAG

//TODO
#define beeper(x)
#define beeperConfirmationBeeps(x)

#define failureMode(x)

#define delay(x) RCC_Delay_ms(x)

#define ROLL  0
#define PITCH 1
#define YAW   2
#define THROTTLE 3
