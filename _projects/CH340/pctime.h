#pragma once
#include <Windows.h>
#include <cstdint>

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

void populateCurrentTime(STM32_TIME& msg);
uint8_t calculateCRC(uint8_t* data, uint8_t length);
