#ifndef _LOW_POWER_H

#define _LOW_POWER_H

#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

void standby(void);


// LL_RCC_MSIRANGE_0
void lowPowerRun(uint32_t range);

#ifdef NDEBUG
#define delay_us(us) \
    do { \
        volatile int i = ((us) - 140) / 121; \
        while (i-- > 0) { \
            /* Do nothing */ \
        } \
    } while (0) 


#define delay_ms(ms) \
    do { \
        volatile int i = ((ms*1000) - 140) / 121; \
        while (i-- > 0) { \
        } \
    } while (0)

#else

#define delay_us(us) \
    do { \
        volatile int i = ((us) - 140) / 121; \
        while (i-- > 0) { \
            /* Do nothing */ \
        } \
    } while (0) 


#define delay_ms(ms) \
    do { \
        volatile int i = ((ms*1000) - 140) / 138; \
        while (i-- > 0) { \
        } \
    } while (0)

#endif

#ifdef __cplusplus
}
#endif


#endif