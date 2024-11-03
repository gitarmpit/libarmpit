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

bool parseAlarmString(const char* input, STM32_ALARM& a) {


	std::stringstream ss(input);
	int i = 0;
	while (ss.good())
	{
		std::string s;
		getline(ss, s, ':');
		if (s.empty()) {
			return false;
		}
		int n = ::atoi(s.c_str());
		if (i == 0) {
			if (n < -1 || n > 23) {
				printf("invalid hour: %s\n", s.c_str());
				exit(1);
			}
			a.hour = n;
		}
		else if (i == 1) {
			if (n < -1 || n > 59) {
				printf("invalid min: %s\n", s.c_str());
				exit(1);
			}
			a.minute = n;
		}
		else if (i == 2) {
			if (n < -1 || n > 59) {
				printf("invalid sec: %s\n", s.c_str());
				exit(1);
			}
			a.second = n;
		}
		else if (i == 3) {
			if (n < -1 || n > 31) {
				printf("invalid day: %s\n", s.c_str());
				exit(1);
			}
			a.day = n;
		}
		else if (i == 4) {
			if (n != 0 && n != 1) {
				printf("invalid value for isWeekDay (should be 0 or 1): %s\n", s.c_str());
				exit(1);
			}
			a.isWeekDay = n;
			if (a.isWeekDay && (a.day < 1 || a.day > 7)) {
				printf("isWeekDay is selected, day has to be between 1 and 7 but is: %d\n", a.day);
				exit(1);
			}
		}
		else if (i == 5) {
			a.skipFirst = n;
		}
		else if (i == 6) {
			a.period = n;
		}
		else if (i == 7) {
			if (n != 0 && n != 1) {
				printf("invalid wkOnly value (should be 0 or 1): %s\n", s.c_str());
				exit(1);
			}
			a.wkOnly = n;
		}
		else if (i == 8) {
			if (n != 0 && n != 1) {
				printf("invalid alarm number (should be 0(A) or 1(B)): %s\n", s.c_str());
				exit(1);
			}
			a.alarmNo = n;
		}
		else if (i == 9) {
			a.alarmType = n;
		}

		++i;
	}
	return (i == 10);
}


