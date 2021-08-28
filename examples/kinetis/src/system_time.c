#include "systick.h"
#include "system_time.h"

static volatile int32_t sec;
static volatile int32_t ts1, ts2, sec1, sec2;
static volatile int32_t us_per_tick = 1;
static volatile uint32_t us;

#define TIME_RES 50000

static void systick_handler(void)
{
    us += TIME_RES;
    ++sec;
}


#ifdef __cplusplus
 extern "C" {
#endif

#ifdef __GNUC__
#pragma GCC push_options
#pragma GCC optimize ("O3")
#endif
     
void systick_timer_start()
{
    ts1 = SYST_VAL;
    sec1 = sec;
}
#ifdef __GNUC__
#pragma GCC push_options
#pragma GCC optimize ("O3")
#endif
uint32_t systick_timer_stop()
{
    ts2 = SYST_VAL;
    sec2 = sec;
    return systick_timediff_us();
}

#ifdef __GNUC__
#pragma GCC push_options
#pragma GCC optimize ("O3")
#endif
uint32_t systick_timediff_us()
{
    return (sec2-sec1)*TIME_RES + ((ts1-ts2)/us_per_tick);
}



#ifdef __GNUC__
#pragma GCC push_options
#pragma GCC optimize ("O3")
#endif
uint32_t micros()
{
    return us + TIME_RES - SYST_VAL/us_per_tick;
}

#ifdef __GNUC__
#pragma GCC pop_options
#endif


void systick_enable(BOOL enable)
{
    if (enable)
    {
        us_per_tick = CORE_FREQ / 1000000;
        uint32_t count = us_per_tick * TIME_RES;
        SystickEnable(count, TRUE, FALSE, systick_handler);
    }
    else
    {
        SystickDisable();
    }
}


#ifdef __cplusplus
 }
 #endif






