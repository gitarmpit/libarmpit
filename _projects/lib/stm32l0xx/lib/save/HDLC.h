#ifndef _HDLC_H
#define _HDLC_H

#include <stdint.h>
#include "ISerialDev.h"

#define FRAME_FLAG 0x7E
#define FRAME_ESC  0x7D
#define FRAME_END  0x7F

typedef enum {
  FRAME_TYPE_SYN,
  FRAME_TYPE_SYNACK,
  FRAME_TYPE_ACK,
  FRAME_TYPE_FIN,
  FRAME_TYPE_DATA
} FRAME_TYPE;

#pragma pack(push, 1) 

typedef struct {
  uint8_t frame_start;
  uint16_t crc;
  uint8_t frame_type;
  uint32_t frame_number; 
  uint16_t data_size;
  uint8_t data[];
} HDLC_FRAME;

#pragma pack(pop)

#define DATA_SIZE 8

class HDLC {
public:
  HDLC(ISerialDev* dev, uint32_t initial_seq);
  void SetConnectiTimeout (uint16_t ms) { _connect_timeout = ms; }
  void SetIoTimeout (uint16_t ms) { _io_timeout = ms; }
  bool Accept  ();
  bool Connect ();
  void SendSYN(uint32_t seq);
  void SendSYNACK(uint32_t seq);
  void SendACK(uint32_t seq);
  void SendFIN();
  void SendData(uint8_t* data, uint16_t dataSize);

  bool ReceiveData(uint8_t* data, uint16_t& dataSize);

  static uint16_t calculate_frame_CRC (HDLC_FRAME* frame);

private:

  bool ReceiveControl(uint8_t cmd, uint32_t& peer_seq);
  bool ReceiveSYN(uint32_t& peer_seq);
  bool ReceiveSYNACK(uint32_t& peer_seq);
  bool ReceiveACK(uint32_t& peer_seq);

  bool ReceiveFrame();
  void SendFrame();
  void init_HDLC(uint16_t dataSize);
  uint16_t escape_frame (uint16_t len);
  uint16_t unescape_frame (uint16_t len);
  
  bool ProcessReadBuffer(uint32_t nBytes, bool& complete);

  ISerialDev*      _dev;
  uint32_t    _write_seq;
  uint8_t*    _esc;  // buffer for escaped frame
  HDLC_FRAME* _frame;

  // reader
  uint16_t   _framePos; // read frame pos
  bool       _inFrame; 
  uint32_t   _read_seq;
  uint16_t   _esc_max_size;
  uint8_t*   _rx_buf;
  uint16_t   _io_timeout;
  uint16_t   _connect_timeout;


};

#endif