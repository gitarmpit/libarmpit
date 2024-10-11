#pragma once
#include "UART.h"
#include "pctime.h"

#define STM32_CMD_START     0xdeaffead
#define STM32_CMD_END       0xbeefbeef
#define STM32_CRC_ERROR     0xdeaddead
#define STM32_ACK 0xaa
#define STM32_CMD_SETTIME    0x1
#define STM32_CMD_GETTIME    0x2


class Session {
public:
	Session(const char* port, int baudRate);
	bool Handshake();
	bool SetTime();
	bool GetTime(STM32_TIME& t);
private:
	void SendHandshake();
	bool ReceiveHandshake();
	bool ReceiveAck();
	void SendAck();

	UART _uart;
};