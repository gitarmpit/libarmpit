#include "Session.h"
#include "pctime.h"

Session::Session(const char* port, int baudRate) {
	if (!_uart.Initialize(port, baudRate)) {
		exit(1);
	}

}

bool Session::Handshake() {
	while (true) {
		if (ReceiveHandshake()) {
			printf("receive handshake ok\n");
			SendHandshake();
			break;
		}
	}
	return ReceiveAck();
}

bool Session::SetTime() {
	uint8_t cmd = STM32_CMD_SETTIME;
	_uart.Write(&cmd, 1);

	STM32_TIME time;
	populateCurrentTime(time);
	_uart.Write((uint8_t*) & time, sizeof time);
	uint8_t crc = calculateCRC((uint8_t*)&time, sizeof time);
	_uart.Write(&crc, 1);
	return ReceiveAck();
}

bool Session::SetAlarm(const STM32_ALARM& a) {

	uint8_t cmd = STM32_CMD_SETALARM;
	_uart.Write(&cmd, 1);

	_uart.Write((uint8_t*)&a, sizeof a);
	uint8_t crc = calculateCRC((uint8_t*)&a, sizeof a);
	_uart.Write(&crc, 1);
	return ReceiveAck();
}


bool Session::GetTime(STM32_TIME& time) {
	uint8_t cmd = STM32_CMD_GETTIME;
	_uart.Write(&cmd, 1);

	for (int i = 0; i < 10; ++i) {
		if (!_uart.Read((uint8_t*)&time, sizeof time, 10000)) {
			return false;
		}
		uint8_t crc = calculateCRC((uint8_t*)&time, sizeof time);

		uint8_t expected_crc;
		if (!_uart.Read(&expected_crc, 1, 1000)) {
			return false;
		}

		if (crc == expected_crc) {
			SendAck();
			return true;
		}
		else {
			printf("crc error\n");
			continue;
		}
	}

	return false;
}

bool Session::GetAlarm(STM32_ALARM& a) {

	if (!_uart.Read((uint8_t*)&a, sizeof a, 10000)) {
		return false;
	}

	uint8_t expected_crc;
	if (!_uart.Read(&expected_crc, 1, 1000)) {
		return false;
	}

	uint8_t crc = calculateCRC((uint8_t*)&a, sizeof a);

	if (crc != expected_crc) {
		printf("crc error\n");
		return false;
	}

	SendAck();
	return true;
}
bool Session::GetAlarms(STM32_ALARM& a, STM32_ALARM& b) {

	uint8_t cmd = STM32_CMD_GETALARM;
	_uart.Write(&cmd, 1);
	return (GetAlarm(a) && GetAlarm(b));
}

bool Session::PlayTune(uint8_t tuneNo) {
	uint32_t cmd = STM32_CMD_PLAYTUNE;
	_uart.Write((uint8_t*)&cmd, 1);
	_uart.Write(&tuneNo, 1);
	return ReceiveAck();

}


void Session::SendHandshake() {
	uint32_t cmd = STM32_CMD_START;
	_uart.Write((uint8_t*)&cmd, sizeof cmd);
}

bool Session::ReceiveHandshake() {
	uint32_t cmd = 0;
	bool rc = _uart.Read((uint8_t*)&cmd, sizeof cmd, 10000);
	printf("receive handshake, rc: %d\n", rc);
	return (rc && (cmd == STM32_CMD_START));
}

bool Session::ReceiveAck() {
	uint8_t ack = 0;
	bool rc = _uart.Read(&ack, sizeof ack, 1000);
	printf("receive ack, rc: %d\n", rc);
	return (rc && (ack == STM32_ACK));
}

void Session::SendAck() {
	uint8_t ack = STM32_ACK;
	_uart.Write(&ack, 1);
}