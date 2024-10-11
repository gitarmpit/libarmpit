#ifndef _RTC_H
#define _RTC_H

#include "config.h"

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct {
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t dow;

  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  uint16_t ms;

} STM32_TIME;


#define RTC_TR_RESERVED_MASK    ((uint32_t)(RTC_TR_HT  | RTC_TR_HU  | \
                                            RTC_TR_MNT | RTC_TR_MNU | \
                                            RTC_TR_ST  | RTC_TR_SU  | \
                                            RTC_TR_PM))
#define RTC_DR_RESERVED_MASK    ((uint32_t)(RTC_DR_YT | RTC_DR_YU | \
                                            RTC_DR_MT | RTC_DR_MU | \
                                            RTC_DR_DT | RTC_DR_DU | \
                                            RTC_DR_WDU))


void initRTC(void);
void setAlarm(void);
void disableAlarm(void);
void enableAlarm(void);
void setRTCTime (const STM32_TIME* t);
void getRTCTime (STM32_TIME* t);

#ifdef __cplusplus
}
#endif


#endif