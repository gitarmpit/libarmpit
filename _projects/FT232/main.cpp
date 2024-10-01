#include <stdio.h> 
#include "ft232.h"

#define STM32_CMD_START 0xfe

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

static void populateCurrentTime(STM32_TIME& msg) {
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


static uint8_t calculateCRC(uint8_t* data, uint8_t length) {
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


static void testSend()
{
	Ft232 ft;
	bool rc = ft.Initialize(300);
	if (!rc)
	{
		printf("FT232 init error\n");
		exit(1);
	}

	ft.SetTimeout(1000);

	uint8_t b = 0x0;

	while (1)
	{
		rc = ft.Send(&b, 1);
		++b;
		::Sleep(1000);
	}
}

static void testRecv()
{
	Ft232 ft;
	bool rc = ft.Initialize(9600);
	if (!rc)
	{
		printf("FT232 init error\n");
		exit(1);
	}

	ft.SetTimeout(1000);

	uint8_t b = 0x0;

	/*
		while (1)
		{
			rc = ft.Send(&b, 1);
			++b;
			::Sleep(1000);
		}
	*/

	int bytesReceived = 0;
	while (1)
	{
		rc = ft.Read(&b, bytesReceived, 2000);
		//rc = ft.Read(&b, 1);
		if (rc)
		{
			printf("%d %d\n", b, bytesReceived);
		}
		else
		{
			printf("error\n");
		}
	}

}

static void sendMsg(Ft232& ft, const STM32_TIME& msg, uint8_t crc)
{
	uint8_t b;

	// Send command marker 
	while (true)
	{
		b = STM32_CMD_START;
		ft.Send(&b, 1);
		b = 0;
		bool rc = ft.Read(&b, 1);
		if (rc == true && b == STM32_CMD_START)
		{
			break;
		}
		printf("Waiting...\n");
		::Sleep(1000);
	}


	// Send data length 
	b = sizeof(msg);
	ft.Send(&b, 1);

	// Send CRC
	ft.Send(&crc, 1);

	// Send msg
	ft.Send((uint8_t *) &msg, b);

	printf("Message sent\n");

}

static void sendTime()
{
	Ft232 ft;
	bool rc = ft.Initialize(300);
	if (!rc)
	{
		printf("FT232 init error\n");
		exit(1);
	}

	ft.SetTimeout(1000);

	while (1)
	{
		STM32_TIME t;
		populateCurrentTime(t);

		uint8_t crc = calculateCRC((uint8_t*)&t, sizeof t);
		printf("CRC: %x\n", crc);
		sendMsg(ft, t, crc);
		::Sleep(5000);
	}

}

int main(void)
{
	testSend();
	// sendTime();
}