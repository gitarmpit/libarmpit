#include "HDLC.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "crc.h"

HDLC::HDLC(ISerialDev* dev, uint32_t initial_seq) : _dev(dev) {

  _esc_max_size = (sizeof(HDLC_FRAME) + DATA_SIZE) * 2;
  _esc = (uint8_t*) malloc (_esc_max_size);
  _frame = (HDLC_FRAME*) malloc (sizeof(HDLC_FRAME) + DATA_SIZE);
  _write_seq = 0;
  _read_seq = initial_seq;
  _framePos = 0;
  _rx_buf = (uint8_t*) malloc (_esc_max_size);
  _connect_timeout = 0;
  _io_timeout = 0;
}

bool HDLC::Accept () {
  bool rc = false;
  uint32_t peer_seq;
  while(true) {
    if (ReceiveSYN(peer_seq)) {
      _write_seq = peer_seq;
      rc = true;
      break;
    }
  }
  return rc;
}

bool HDLC::Connect () {
  bool rc = false;
  uint32_t peer_seq;
  SendSYN(_read_seq);
  while(true) {
    if (ReceiveSYNACK(peer_seq)) {
      _write_seq = peer_seq;
      rc = true;
      break;
    }
  }
  return false;
}

bool HDLC::ProcessReadBuffer(uint32_t nBytes, bool& complete) {

  for (uint32_t i = 0; i < nBytes; ++i) {
    uint8_t b = _rx_buf[i];
    if (b == FRAME_FLAG) {
      _inFrame  = true;
      _framePos = 0;
    } else if (b == FRAME_END) {
      _inFrame = 0;
      uint16_t len = unescape_frame(_framePos);
      complete = true;
      return true;
    }

    if (_inFrame) {
      if (_framePos >= _esc_max_size) {
        return false;
      }
      _esc[_framePos++] = b;
    }
  }
  return true;
}

bool HDLC::ReceiveSYN(uint32_t& peer_seq) {
  return ReceiveControl(FRAME_TYPE_SYN, peer_seq);
}
  
bool HDLC::ReceiveSYNACK(uint32_t& peer_seq) {
  return ReceiveControl(FRAME_TYPE_SYNACK, peer_seq);
}
  
bool HDLC::ReceiveACK(uint32_t& peer_seq) {
  return ReceiveControl(FRAME_TYPE_ACK, peer_seq);
}

bool HDLC::ReceiveControl(uint8_t cmd, uint32_t& peer_seq) {
  bool rc = false;
  if (ReceiveFrame() && _frame->frame_type == cmd) {
    memcpy (&peer_seq, _frame->data, _frame->data_size);
    rc = true;
  }
  return rc;
}

bool HDLC::ReceiveData(uint8_t* data, uint16_t& dataSize) {
  bool rc = false;
  if (ReceiveFrame() && _frame->frame_type == FRAME_TYPE_DATA) {
    memcpy(data, _frame->data, _frame->data_size);
    dataSize = _frame->data_size;
    rc       = true;
  }
  return rc;
}

bool HDLC::ReceiveFrame() {
  bool rc       = false;
  bool complete = false;
  while (1) {
    uint16_t bytesRead = _dev->Read(_rx_buf, _esc_max_size);
    if (bytesRead) {
      rc = ProcessReadBuffer(bytesRead, complete);
      if (!rc) {
        break;
      }
      if (complete) {
        uint16_t crc = calculate_frame_CRC(_frame);

        if (crc != _frame->crc) {
          printf("CRC error\n");
          rc = false;
          break;
        }
        if (_frame->frame_number != ++_read_seq) {
          printf("frame number error: %d\n", _read_seq);
          rc = false;
          break;
        }
        printf ("%d\n", _frame->frame_type);
        rc = true;
        break;
      }
    }
  }
  return rc;
}

void HDLC::SendFrame() {

  _frame->frame_number = ++_write_seq;
  _frame->crc = calculate_frame_CRC (_frame);

  uint16_t frameSize = sizeof(HDLC_FRAME) + _frame->data_size;
  uint16_t esc_len = escape_frame(frameSize);
  uint8_t b;
  // send two garbage bytes
  b = 0xff;
  _dev->Write(&b, 1);
  _dev->Write(&b, 1);

  // send frame
  _dev->Write(_esc, esc_len);

  b = FRAME_END;
  _dev->Write(&b, 1);

  // send two garbage bytes
  b = 0xff;
  _dev->Write(&b, 1);
  _dev->Write(&b, 1);

}

void HDLC::SendSYN(uint32_t seq) {
  init_HDLC(sizeof(uint32_t));
  _frame->frame_type = (uint8_t) FRAME_TYPE_SYN;
  memcpy (_frame->data, &seq, _frame->data_size);
  SendFrame();
}

void HDLC::SendSYNACK(uint32_t seq) {
  init_HDLC(sizeof(uint32_t));
  _frame->frame_type = (uint8_t) FRAME_TYPE_SYNACK;
  memcpy (_frame->data, &seq, _frame->data_size);
  SendFrame();
}

void HDLC::SendACK(uint32_t seq) {
  init_HDLC(sizeof(uint32_t));
  _frame->frame_type = (uint8_t) FRAME_TYPE_SYNACK;
  memcpy (_frame->data, &seq, _frame->data_size);
  SendFrame();
}

void HDLC::SendFIN() {
  init_HDLC(0);
  _frame->frame_type = (uint8_t) FRAME_TYPE_FIN;
  SendFrame();
}

void HDLC::SendData(uint8_t* data, uint16_t dataSize) {

  if (dataSize > DATA_SIZE) {
    dataSize = DATA_SIZE;
  }

  init_HDLC(dataSize);
  memcpy (_frame->data, data, dataSize);
  _frame->frame_type = (uint8_t) FRAME_TYPE_DATA;

  SendFrame();
}

void HDLC::init_HDLC(uint16_t dataSize) {

  memset (_frame, 0xff, sizeof(HDLC_FRAME) + dataSize);
  _frame->frame_start = FRAME_FLAG;
  _frame->data_size = dataSize;

}

uint16_t HDLC::calculate_frame_CRC (HDLC_FRAME* frame) {

  // exclude first 3 bytes
  uint16_t crc_buf_size = sizeof(HDLC_FRAME) + frame->data_size - 3;
  return calculateCRC ( &((uint8_t*)(frame))[3], crc_buf_size);
}


uint16_t HDLC::escape_frame (uint16_t len) {
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


uint16_t HDLC::unescape_frame (uint16_t len) {

  uint16_t unesc_len = 0;
  uint8_t* pf = (uint8_t*)_frame;

  pf[unesc_len++] = FRAME_FLAG;
  for (uint16_t i = 1; i < len; ++i) {
    if (_esc[i] == FRAME_ESC && (i+1 < len)) {
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
