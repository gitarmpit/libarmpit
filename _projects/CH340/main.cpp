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


static void testSend() {

	UART uart;
	if (!uart.Initialize("COM8", CBR_1200)) {
		exit(1);
	}
	uint8_t b = 0;

	while (true)
	{
		//printf("sending %d\n", b);
		uart.Write(&b, 1);
		++b;
		::Sleep(1000);
	}

}

static void testRecv() {

	UART uart;
	if (!uart.Initialize("COM8", CBR_1200)) {
		exit(1);
	}
	uint8_t b = 0;
	int total = 0;
	int nerrors = 0;

	while (true)
	{
		if (uart.Read(&b, 1, 10000)) {
			//printf("%02x\n", b & 0x7f);
			//b &= 0x7f;
			++total;

			if (b != 0x4f) {
				++nerrors;
				printf("total: %d, errors: %d, %%: %f\n", total, nerrors, (float)nerrors / (float)total * 100.0);
			}

		}
		if ((total % 1000) == 0)
			printf("total: %d, errors: %d, %%: %f\n", total, nerrors, (float)nerrors / (float)total * 100.0);

	}

}


static void testRecv2() {

	UART uart;
	if (!uart.Initialize("COM7", CBR_1200)) {
		exit(1);
	}
	uint8_t b = 0;
	int total = 0;
	int nerrors = 0;

	while (true)
	{
		if (uart.Read(&b, 1, 10000)) {
			printf("%d\n", b);
		}
	}

}




int main(int argc, char* argv[]) {
	
	testRecv2();

	if (argc < 3) {
		printf("Usage: <com port> <settime|gettime|setalarm|getalarm>\n");
		exit(1);
	}

	const char* comPort = argv[1];
	STM32_ALARM a;

	RTC_CMD cmd = RTC_CMD::SETTIME;

	if (!_strcmpi(argv[2], "settime")) {
		printf("setting time\n");
		cmd = RTC_CMD::SETTIME;
	}
	else if (!_strcmpi(argv[2], "gettime")) {
		printf("getting time\n");
		cmd = RTC_CMD::GETTIME;
	}
	else if (!_strcmpi(argv[2], "setalarm")) {
		printf("setting alarm\n");
		cmd = RTC_CMD::SETALARM;
		if (argc != 4) {
			printf("Usage: <com port> <settime|gettime|setalarm|getalarm>\n");
			exit(1);
		}
		const char* alarmstr = argv[3];
		if (!parseAlarmString(alarmstr, a)) {
			printf("error parsing alarm string: %s\n", alarmstr);
			exit(1);
		}
		printf("Setting alarm: h: %02d, m: %02d, s: %02d, d: %02d, is wd: %d, skip weeks: %d, alarmNo: %d\n", 
			a.hour, a.minute, a.second, a.day, a.isWeekDay, a.skipWeeks, a.alarmNo);
	}
	else if (!_strcmpi(argv[2], "getalarm")) {
		cmd = RTC_CMD::GETALARM;
		printf("getting alarm\n");
	}
	else {
		printf("Command unknown: %s\n", argv[2]);
		exit(1);
	}

	Session session(comPort, CBR_1200);
	bool rc = session.Handshake();
	if (!rc) {
		printf("Handshake error\n");
		exit(1);
	}

	printf("Handshake OK\n");

	if (cmd == RTC_CMD::SETTIME) {
		bool rc = session.SetTime();
		printf("SetTime sent: %d\n", rc);
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
	else if (cmd == RTC_CMD::SETALARM) {
		bool rc = session.SetAlarm(a);
		printf("SetAlarm sent: %d\n", rc);
	}
	else if (cmd == RTC_CMD::GETALARM) {
		STM32_ALARM a;
		if (session.GetAlarm(a)) {
			printf("Got alarm A: h: %02d, m: %02d, s: %02d, d: %02d, is wd: %d, skip weeks: %d\n",
				a.hour, a.minute, a.second, a.day, a.isWeekDay, a.skipWeeks);
		}
		else {
			printf("GetAlarm error\n");
		}
		if (session.GetAlarm(a)) {
			printf("Got alarm B: h: %02d, m: %02d, s: %02d, d: %02d, is wd: %d, skip weeks: %d\n",
				a.hour, a.minute, a.second, a.day, a.isWeekDay, a.skipWeeks);
		}
		else {
			printf("GetAlarm error\n");
		}
	}
}
