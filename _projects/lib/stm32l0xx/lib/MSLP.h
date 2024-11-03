#ifndef _MSLP_H
#define _MSLP_H

#include <stdint.h>
#include "ISerialDev.h"

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#define FRAME_START 0x7E
#define FRAME_ESC   0x7D
#define FRAME_END   0x7F

#  define MSLP_FLAG_FIN        0x01
#  define MSLP_FLAG_SYN        0x02
#  define MSLP_FLAG_RST        0x04
#  define MSLP_FLAG_PUSH       0x08
#  define MSLP_FLAG_ACK        0x10
#  define MSLP_FLAG_URG        0x20

#pragma pack(push, 1) 

#define PAD4(size) ((4 - ((size) % 4)) % 4)

typedef struct {
  uint8_t frame_start;
  uint16_t seq;
  uint16_t ack;
  uint8_t data_off;   
  uint8_t flags; 
  uint16_t window;
  uint16_t cs;
  uint16_t data_size;
} MSLP_HEADER;

typedef struct {
  MSLP_HEADER hdr;
  uint8_t data[];
} MSLP_FRAME;

#pragma pack(pop)

typedef enum
{
  MSLP_ESTABLISHED = 1, // Both CLI and SRV
  MSLP_SYN_SENT,   //  CLI SYN sent
  MSLP_SYN_RECV,   //  SRV SIN recv
  MSLP_FIN_WAIT1,  // CLI Sent FIN
  MSLP_FIN_WAIT2,  // CLI recv ACK
  MSLP_TIME_WAIT,  // CLI recv FIN, sent ACK
  MSLP_CLOSED,     // CLI, SRV, initial State
  MSLP_CLOSE_WAIT, // SRV recv FIN sent ACK
  MSLP_LAST_ACK,   // SRV sent FIN
  MSLP_LISTEN,     // SRV first state
} MSLP_STATE;


#define MSLP_OPT_EOL        0 /* End of options */
#define MSLP_OPT_NOP        1 /* Padding */
#define MSLP_OPT_MSS        2 /* Segment size negotiating */
#define MSLP_OPT_TIMESTAMP	8 /* Better RTT estimations/PAWS */

#define MAX_DATA_SIZE 8
#define MAX_OPTIONS_SIZE 20
#define MAX_FRAME_SIZE sizeof(MSLP_FRAME) + MAX_DATA_SIZE + MAX_OPTIONS_SIZE

typedef struct {
  uint8_t kind;
  uint8_t len;
  uint8_t data[];
} MSLP_OPT;

// Microcontroller Serial Link Protocol 
class MSLP {
public:
  MSLP(ISerialDev* dev, uint16_t isn);
  void SetConnectiTimeout (uint16_t ms) { _connect_timeout = ms; }
  void SetIoTimeout (uint16_t ms) { _io_timeout = ms; }
  bool Accept  ();
  bool Connect ();
  void SendData(const void* data, uint16_t dataSize); // TODO make bool, store last error
  bool ReceiveData(void* data, uint16_t dataSize);

private:

  void MakeFrame (uint8_t flags, const void* data, uint16_t dataSize);
  void SetTimeStamp();
  void sendSYN();
  void SendAck(uint8_t ack);
  bool ReceiveFrame();
  void SendFrame(uint8_t flags, const void* data, uint16_t dataSize);
  void init_HDLC(uint16_t dataSize);
  uint16_t escape_frame (uint16_t len);
  uint16_t unescape_frame (uint16_t len);
  
  bool ProcessReadBuffer(uint32_t nBytes, bool& complete);

  ISerialDev*      _dev;
  
  uint16_t    _snd_nxt;
  uint16_t    _snd_una;
  uint16_t    _rcv_nxt;

  // writer
  uint8_t*    _esc_buf;  // buffer for escaped frame
  MSLP_FRAME* _frame;

  // reader
  uint16_t   _framePos; // read frame pos
  bool       _inFrame; 
  uint16_t   _esc_max_size;
  uint8_t*   _rx_buf;
  uint16_t   _io_timeout;
  uint16_t   _connect_timeout;

  MSLP_STATE _state;


};

#endif