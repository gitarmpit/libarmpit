#include <stdbool.h>
#include <stdint.h>



#include "sonar_hcsr04.h"


#if defined(SONAR)

volatile int32_t measurement = -1;
static uint32_t lastMeasurementAt;


void hcsr04_init(sonarRange_t *sonarRange)
{
}

// measurement reading is done asynchronously, using interrupt
void hcsr04_start_reading(void)
{
}

/**
 * Get the distance that was measured by the last pulse, in centimeters.
 */
int32_t hcsr04_get_distance(void)
{
    return 0;
}
#endif
