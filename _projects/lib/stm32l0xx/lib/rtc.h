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
    int8_t isWeekDay;
    int8_t hour;
    int8_t minute;
    int8_t second;
    int8_t skipFirst; // how many runs to skip
    int8_t period; // 0 = every time 1 = every other, etc.
    int8_t wkOnly; // weekdays only
    int8_t alarmNo; // 0 = A, 1 = B
    int8_t alarmTune;

} STM32_ALARM;

#define BKP_TUNE_POS         0
#define BKP_SKIP_POS         4
#define BKP_PERIOD_POS       8
#define BKP_PERIOD_CNT_POS  12
#define BKP_WKONLY_POS      16



#define BKP_SETTING_MASK    0xf

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

BOOL isSkipAlarm (int8_t alarmNo);
int8_t getAlarmTuneNo(int8_t alarmNo);

#ifdef __cplusplus
}
#endif


#endif