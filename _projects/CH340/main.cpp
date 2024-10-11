#include "UART.h"
#include "pctime.h"
#include "Session.h"

static uint8_t calculateCRC(uint8_t* data, int length) {
	uint8_t crc = 0x00;
	const uint8_t polynomial = 0x31;

	for (int i = 0; i < length; i++) {
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



static void setTime(Session& session) {

}

typedef enum {
	SETTIME,
	GETTIME,
	SETALARM,
	GETALARM
} RTC_CMD;


int main(int argc, char* argv[]) {

	if (argc != 2) {
		printf("Usage: <settime|gettime>\n");
		exit(1);
	}

	RTC_CMD cmd = RTC_CMD::SETTIME;

	if (!_strcmpi(argv[1], "settime")) {
		printf("setting time\n");
		cmd = RTC_CMD::SETTIME;
	}
	else if (!_strcmpi(argv[1], "gettime")) {
		printf("getting time\n");
		cmd = RTC_CMD::GETTIME;
	}
	else {
		printf("Command unknown: %s\n", argv[1]);
		exit(1);
	}

	Session session("COM8", CBR_1200);
	bool rc = session.Handshake();
	if (!rc) {
		printf("Handshake error\n");
		exit(1);
	}

	printf("Handshake OK\n");

	if (cmd == RTC_CMD::SETTIME) {
		session.SetTime();
		printf("SetTime sent\n");
	}
	else if (cmd == RTC_CMD::GETTIME) {
		STM32_TIME time;
		if (session.GetTime(time)) {
			printf("%4d-%02d-%02d %02d:%02d:%02d\n", time.year, time.month, time.day, time.hour, time.minute, time.second);
		}
		else {
			printf("GetTime error\n");
		}
	}
}
