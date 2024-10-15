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
    uint8_t isWeekDay;
    int8_t hour;
    int8_t minute;
    int8_t second;
    uint8_t skipWeeks; // 0 = every week, 1 = skip one week, etc
    uint8_t alarmNo; // 0 = A, 1 = B

} STM32_ALARM;



void populateCurrentTime(STM32_TIME& msg);
uint8_t calculateCRC(uint8_t* data, uint8_t length);


// hr:min:sec:day:isdow:skipWeeks:alarmNo
bool parseAlarmString(const char* input, STM32_ALARM& a);
