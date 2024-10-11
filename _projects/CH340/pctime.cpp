#include "pctime.h"

void populateCurrentTime(STM32_TIME& msg) {
    SYSTEMTIME st;
    GetLocalTime(&st); // Get the current local time

    msg.year = st.wYear;
    msg.month = (uint8_t)st.wMonth;       // Month (1-12)
    msg.day = (uint8_t)st.wDay;           // Day (1-31)
    msg.dow = (uint8_t)st.wDayOfWeek;     // Day of the week (0-6, 0=Sunday)
    msg.hour = (uint8_t)st.wHour;         // Hour (0-23)
    msg.minute = (uint8_t)st.wMinute;     // Minute (0-59)
    msg.second = (uint8_t)st.wSecond;     // Second (0-59)
    msg.ms = st.wMilliseconds;   // Milliseconds (0-999)
}


uint8_t calculateCRC(uint8_t* data, uint8_t length) {
    uint8_t crc = 0x00;
    const uint8_t polynomial = 0x31;

    for (uint8_t i = 0; i < length; i++) {
        crc ^= data[i];

        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ polynomial;
            }
            else {
                crc <<= 1;
            }
        }
    }

    return crc;
}

