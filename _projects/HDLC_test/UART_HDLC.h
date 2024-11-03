#pragma once
#include "UART.h"
#include "hdlc.h"

#define DATA_SIZE 8

class UART_HDLC {
public:
	UART_HDLC(UART* uart);
	void SendSync();
	void SendAck();
	void SendFin();
	void SendData(uint8_t* data, uint16_t dataSize);

	static uint16_t calculate_frame_CRC(HDLC_FRAME* frame);



	void Run();
private:
	void SendFrame();
	void init_HDLC(uint16_t dataSize);
	void init_cmd_frame(uint8_t cmd);
	void init_data_frame(uint8_t* data, uint16_t dataSize);
	void init_sync_frame();
	void init_fin_frame();
	void init_ack_frame();
	uint16_t escape_frame(uint16_t len);
	uint16_t unescape_frame(uint16_t len);

	void ProcessReadBuffer(uint32_t nBytes);

	UART*    _uart;
	uint16_t _framePos;
	bool     _inFrame; 

	uint8_t _buf[256];

	HDLC_FRAME* _frame; 
	uint8_t*    _esc;  // buffer for escaped frame
	uint32_t    _read_seq;
	uint32_t    _write_seq;


};
