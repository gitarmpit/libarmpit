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

typedef struct {
  int8_t day;  // date or weekday
  uint8_t isWeekDay;
  int8_t hour;
  int8_t minute;
  int8_t second;
  uint8_t skipWeeks; // 0 = every week, 1 = skip one week, etc
  uint8_t alarmNo; // 0 = A, 1 = B
} STM32_ALARM;



#define RTC_TR_RESERVED_MASK    ((uint32_t)(RTC_TR_HT  | RTC_TR_HU  | \
                                            RTC_TR_MNT | RTC_TR_MNU | \
                                            RTC_TR_ST  | RTC_TR_SU  | \
                                            RTC_TR_PM))
#define RTC_DR_RESERVED_MASK    ((uint32_t)(RTC_DR_YT | RTC_DR_YU | \
                                            RTC_DR_MT | RTC_DR_MU | \
                                            RTC_DR_DT | RTC_DR_DU | \
                                            RTC_DR_WDU))


void initRTC(void);

void setAlarmA (STM32_ALARM* t);
void setAlarmB (STM32_ALARM* t);

void disableAlarmA(void);
void disableAlarmB(void);
void disableAlarm(void);

void enableAlarmA(void);
void enableAlarmB(void);

void clearAlarmA(void);
void clearAlarmB(void);

void setRTCTime (const STM32_TIME* t);
void getRTCTime (STM32_TIME* t);

void getAlarmA (STM32_ALARM* t);
void getAlarmB (STM32_ALARM* t);

#ifdef __cplusplus
}
#endif


#endif