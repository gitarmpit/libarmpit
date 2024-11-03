#pragma once
#include <Windows.h>
#include <cstdint>
#include <string>
#include <sstream>


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
    int8_t alarmType;

} STM32_ALARM;



void populateCurrentTime(STM32_TIME& msg);
uint8_t calculateCRC(uint8_t* data, uint8_t length);


// hr:min:sec:day:isdow:skipWeeks:alarmNo:alarmType
bool parseAlarmString(const char* input, STM32_ALARM& a);
