#include "UART_HDLC.h"
#include "crc.h"

UART_HDLC::UART_HDLC(UART* uart) : _uart(uart) {
	_framePos = 0;
	_inFrame = false;
	memset(_buf, 0, sizeof _buf);
	const int frame_size = sizeof(HDLC_FRAME) + DATA_SIZE;
	_esc = (uint8_t*) malloc (frame_size * 2);
	_frame = (HDLC_FRAME*) malloc(frame_size);
	_read_seq = 0;
	_write_seq = 0;
}

void UART_HDLC::SendFrame() {

	_frame->frame_number = ++_write_seq;
	_frame->crc = calculate_frame_CRC(_frame);

	uint16_t frameSize = sizeof(HDLC_FRAME) + _frame->data_size;
	uint16_t esc_len = escape_frame(frameSize);
	uint8_t b;
	// send two garbage bytes
	b = 0xff;
	_uart->Write(&b, 1);
	_uart->Write(&b, 1);

	// send frame
	_uart->Write(_esc, esc_len);

	b = FRAME_END;
	_uart->Write(&b, 1);

	// send two garbage bytes
	b = 0xff;
	_uart->Write(&b, 1);
	_uart->Write(&b, 1);

}

void UART_HDLC::SendSync() {
	init_sync_frame();
	SendFrame();
}

void UART_HDLC::SendAck() {
	init_ack_frame();
	SendFrame();
}

void UART_HDLC::SendFin() {
	init_fin_frame();
	SendFrame();
}

void UART_HDLC::SendData(uint8_t* data, uint16_t dataSize) {

	if (dataSize > DATA_SIZE) {
		dataSize = DATA_SIZE;
	}

	init_data_frame(data, dataSize);
	SendFrame();
}

void UART_HDLC::init_HDLC(uint16_t dataSize) {
	memset(_frame, 0xff, sizeof(HDLC_FRAME) + dataSize);
	_frame->frame_start = FRAME_FLAG;
	_frame->data_size = dataSize;

}

void UART_HDLC::init_cmd_frame(uint8_t cmd) {
	init_HDLC(0);
	_frame->frame_type = cmd;
}


uint16_t UART_HDLC::calculate_frame_CRC(HDLC_FRAME* frame) {
	// exclude first 3 bytes
	uint16_t crc_buf_size = sizeof(HDLC_FRAME) + frame->data_size - 3;
	return calculateCRC(&((uint8_t*)(frame))[3], crc_buf_size);
}

void UART_HDLC::init_sync_frame() {
	init_cmd_frame((uint8_t)FRAME_TYPE_SYN);
}

void UART_HDLC::init_ack_frame() {
	init_cmd_frame((uint8_t)FRAME_TYPE_ACK);
}

void UART_HDLC::init_fin_frame() {
	init_cmd_frame((uint8_t)FRAME_TYPE_FIN);
}

void UART_HDLC::init_data_frame(uint8_t* data, uint16_t dataSize) {
	init_HDLC(dataSize);
	memcpy(_frame->data, data, dataSize);
	_frame->frame_type = (uint8_t)FRAME_TYPE_DATA;
}

uint16_t UART_HDLC::escape_frame(uint16_t len) {
	uint16_t esc_len = 0;
	_esc[esc_len++] = FRAME_FLAG;

	uint8_t* pf = (uint8_t*)_frame;

	for (uint16_t i = 1; i < len; ++i) {
		if (pf[i] == FRAME_FLAG || pf[i] == FRAME_END || pf[i] == FRAME_ESC) {
			_esc[esc_len++] = FRAME_ESC;
			_esc[esc_len++] = pf[i] ^ 0x20;
		}
		else {
			_esc[esc_len++] = pf[i];
		}
	}
	return esc_len;
}

uint16_t UART_HDLC::unescape_frame(uint16_t len) {

	uint16_t unesc_len = 0;
	uint8_t* pf = (uint8_t*)_frame;

	pf[unesc_len++] = FRAME_FLAG;
	for (uint16_t i = 1; i < len; ++i) {
		if (_esc[i] == FRAME_ESC && (i + 1 < len)) {
			++i; // skip next char
			pf[unesc_len++] = _esc[i] ^ 0x20;
		}
		else {
			pf[unesc_len++] = _esc[i];
		}
		if (unesc_len == sizeof(HDLC_FRAME)) {
			//break;
		}
	}

	return unesc_len;
}

void UART_HDLC::ProcessReadBuffer(uint32_t nBytes) {

	for (uint32_t i = 0; i < nBytes; ++i) {
		uint8_t b = _buf[i];
		//printf("received from UART: %x\n", _buf[i]);
		if (b == FRAME_FLAG) {
			_inFrame = true;
			_framePos = 0;
			//printf("frame start\n");
		}
		else if (b == FRAME_END) {
			_inFrame = 0;
			//printf("frame end. framePos: %d\n", _framePos);
			uint16_t len = unescape_frame(_framePos);
			//printf("reader received frame, esc len: %d, len: %d\n", _framePos, len);
			dump_frame(_frame);
			uint16_t CRC = calculate_frame_CRC(_frame);
			if (CRC == _frame->crc) {
				printf("CRCs match\n");
			}
			else {
				printf("CRC error\n");
			}
			if (_frame->frame_number == ++_read_seq) {
				printf("frame numbers match\n");
			}
			else {
				printf("frame number error\n");
			}
		}

		if (_inFrame) {
			//printf("adding: %x, pos: %d\n", b, _framePos);
			_esc[_framePos++] = b;
		}

	}

}


void UART_HDLC::Run() {
	uint8_t b = 0;

	while (true)
	{
		uint32_t rxBytes = _uart->WaitRead(100);
		if (rxBytes) {
			uint32_t bread = _uart->Read(_buf, rxBytes);
			ProcessReadBuffer(bread);
		}
	}

}