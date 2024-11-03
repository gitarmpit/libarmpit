#include "MSLP.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint16_t cksum(void* buf, uint16_t size) {
  printf("cksum len: %d\n", size);
  uint32_t sum = 0;
  uint16_t* p  = (uint16_t*)buf;

  while (size > 1) {
    printf("p: %x\n", *p);
    sum += *p++;
    size -= 2;
  }

  if (size > 0) {
    printf("extra: %x\n", *(uint8_t*)p);
    sum += *(uint8_t*)p;
  }

  while (sum >> 16)
    sum = (sum & 0xffff) + (sum >> 16);

  return ~sum;
}

MSLP::MSLP(ISerialDev* dev, uint16_t isn) : _dev(dev) {
  _esc_max_size    = MAX_FRAME_SIZE * 2;
  _esc_buf         = (uint8_t*)malloc(_esc_max_size);
  _frame           = (MSLP_FRAME*)malloc(MAX_FRAME_SIZE);
  _rx_buf          = (uint8_t*)malloc(_esc_max_size);
  _rcv_nxt         = 0;
  _snd_nxt         = isn;
  _snd_una         = isn;
  _framePos        = 0;
  _inFrame         = false;
  _connect_timeout = 0;
  _io_timeout      = 0;
  _state           = MSLP_CLOSED;
}

bool MSLP::Connect() {
  sendSYN();
  return true;
}

void MSLP::sendSYN() {
  init_HDLC(0);
  SendFrame(MSLP_FLAG_SYN, 0, 0);
  _snd_nxt++;
  _state = MSLP_SYN_SENT;
}

void MSLP::SendData(const void* data, uint16_t dataSize) {
  uint8_t* pdata = (uint8_t*)data;
  while (dataSize > 0) {
    uint16_t chunk = (dataSize > MAX_DATA_SIZE) ? MAX_DATA_SIZE : dataSize;
    SendFrame(MSLP_FLAG_ACK, data, chunk);
    dataSize -= chunk;
    pdata += chunk;
  }
}

void MSLP::SendAck(uint8_t ack) {
  init_HDLC(0);
  _frame->hdr.ack = ack;
  SendFrame(MSLP_FLAG_ACK, 0, 0);
}

void MSLP::init_HDLC(uint16_t dataSize) {
  memset(_frame, 0, MAX_FRAME_SIZE);
  _frame->hdr.frame_start = FRAME_START;
  _frame->hdr.data_size   = dataSize;
}

void MSLP::MakeFrame(uint8_t flags, const void* data, uint16_t dataSize) {
  init_HDLC(0);
  _frame->hdr.seq       = _snd_nxt;
  _frame->hdr.ack       = _rcv_nxt;
  _frame->hdr.data_size = dataSize;
  _frame->hdr.flags     = flags;
  _frame->hdr.data_off  = sizeof(MSLP_HEADER);
  _frame->hdr.window    = 0xeeee;
  SetTimeStamp();
  memcpy(((uint8_t*)_frame) + _frame->hdr.data_off, data, dataSize);
  _frame->hdr.cs = cksum(_frame, _frame->hdr.data_off + dataSize);
  printf("cs: %x\n", _frame->hdr.cs);
}

void MSLP::SetTimeStamp() {
  uint16_t ts     = 0xffff;
  uint8_t opt_len = sizeof(MSLP_OPT) + sizeof ts;
  MSLP_OPT* opt   = (MSLP_OPT*)calloc(1, opt_len);
  opt->kind       = MSLP_OPT_TIMESTAMP;
  memcpy(opt->data, &ts, sizeof ts);
  opt->len             = opt_len;
  _frame->hdr.data_off = sizeof(MSLP_HEADER) + opt_len;
  memcpy(((uint8_t*)_frame) + sizeof(MSLP_HEADER), opt, opt->len);
  free(opt);
}

void MSLP::SendFrame(uint8_t flags, const void* data, uint16_t dataSize) {
  MakeFrame(flags, data, dataSize);

  uint16_t frameSize = _frame->hdr.data_off + dataSize;
  // for (int i = 0; i < frameSize; ++i) {
  //   printf ("%d: %x(%d)\n", i, ((uint8_t*)_frame)[i], ((uint8_t*)_frame)[i]);
  // }
  uint16_t esc_len = escape_frame(frameSize);
  uint8_t b;
  // send two garbage bytes
  b = 0xff;
  _dev->Write(&b, 1);
  _dev->Write(&b, 1);

  // send frame
  _dev->Write(_esc_buf, esc_len);

  b = FRAME_END;
  _dev->Write(&b, 1);

  // send two garbage bytes
  b = 0xff;
  _dev->Write(&b, 1);
  _dev->Write(&b, 1);
}

uint16_t MSLP::escape_frame(uint16_t len) {
  uint16_t esc_len    = 0;
  _esc_buf[esc_len++] = FRAME_START;

  uint8_t* pf = (uint8_t*)_frame;

  for (uint16_t i = 1; i < len; ++i) {
    if (pf[i] == FRAME_START || pf[i] == FRAME_END || pf[i] == FRAME_ESC) {
      _esc_buf[esc_len++] = FRAME_ESC;
      _esc_buf[esc_len++] = pf[i] ^ 0x20;
    } else {
      _esc_buf[esc_len++] = pf[i];
    }
  }
  return esc_len;
}

//////////////////////////////////////////////////////////////////
bool MSLP::ProcessReadBuffer(uint32_t nBytes, bool& complete) {
  for (uint32_t i = 0; i < nBytes; ++i) {
    uint8_t b = _rx_buf[i];
    if (b == FRAME_START) {
      _inFrame  = true;
      _framePos = 0;
    } else if (b == FRAME_END) {
      _inFrame     = 0;
      uint16_t len = unescape_frame(_framePos);
      complete     = true;
      return true;
    }

    if (_inFrame) {
      if (_framePos >= _esc_max_size) {
        return false;
      }
      _esc_buf[_framePos++] = b;
    }
  }
  return true;
}

bool MSLP::ReceiveData(void* data, uint16_t dataSize) {
  uint16_t dataReceived = 0;
  while (dataReceived < dataSize) {
    if (ReceiveFrame()) {
      memcpy((uint8_t*)data + dataReceived, (uint8_t*)_frame + _frame->hdr.data_off, _frame->hdr.data_size);
      dataReceived += _frame->hdr.data_size;
    }
  }
  return true;
}

bool MSLP::ReceiveFrame() {
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
        // uint16_t crc = calculate_frame_CRC(_frame);
        printf("seq: %x, ack: %x, data_off: %d, flags: %x, window: %x, cs: %x, data_size: %d\n",
               _frame->hdr.seq,
               _frame->hdr.ack,
               _frame->hdr.data_off,
               _frame->hdr.flags,
               _frame->hdr.window,
               _frame->hdr.cs,
               _frame->hdr.data_size);
        _frame->hdr.cs = 0;
        uint16_t cs    = cksum(_frame, _frame->hdr.data_off + _frame->hdr.data_size);
        printf("cs: %x\n", cs);
        uint8_t opt_size = _frame->hdr.data_off - sizeof(MSLP_FRAME);
        printf("opt size: %d\n", opt_size);
        if (opt_size) {
          MSLP_OPT* opt = (MSLP_OPT*)((uint8_t*)_frame + sizeof(MSLP_FRAME));
          printf("opt: kind: %d, len: %d\n", opt->kind, opt->len);
          if (opt->kind == 8) {
            uint16_t ts = *(uint16_t*)opt->data;
            printf("ts: %x\n", ts);
          }
        }
        int size = _frame->hdr.data_off + _frame->hdr.data_size;
        for (int i = 0; i < size; ++i) {
          printf("%d: %x\n", i, ((uint8_t*)_frame)[i]);
        }
        rc = true;
        break;
      }
    }
  }
  return rc;
}

uint16_t MSLP::unescape_frame(uint16_t len) {
  uint16_t unesc_len = 0;
  uint8_t* pf        = (uint8_t*)_frame;

  pf[unesc_len++] = FRAME_START;
  for (uint16_t i = 1; i < len; ++i) {
    if (_esc_buf[i] == FRAME_ESC && (i + 1 < len)) {
      ++i;    // skip next char
      pf[unesc_len++] = _esc_buf[i] ^ 0x20;
    } else {
      pf[unesc_len++] = _esc_buf[i];
    }
    if (unesc_len == sizeof(MSLP_FRAME)) {
      // break;
    }
  }

  return unesc_len;
}
