#pragma once
#include "UART.h"
#include "pctime.h"

#define STM32_CMD_START     0xdeaffead
#define STM32_CMD_END       0xbeefbeef
#define STM32_CRC_ERROR     0xdeaddead
#define STM32_ACK 0xaa
#define STM32_CMD_SETTIME    0x1
#define STM32_CMD_GETTIME    0x2
#define STM32_CMD_SETALARM   0x3
#define STM32_CMD_GETALARM   0x4
#define STM32_CMD_PLAYTUNE   0x5


class Session {
public:
	Session(const char* port, int baudRate);
	bool Handshake();
	bool SetTime();
	bool GetTime(STM32_TIME& t);
	bool GetAlarms(STM32_ALARM& a, STM32_ALARM& b);
	bool SetAlarm(const STM32_ALARM& a);
	bool PlayTune(uint8_t tuneNo);
private:
	void SendHandshake();
	bool ReceiveHandshake();
	bool ReceiveAck();
	void SendAck();
	bool GetAlarm(STM32_ALARM&);

	UART _uart;
};