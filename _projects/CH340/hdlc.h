#ifndef HDLC_H
#define HDLC_H

#include <stdint.h>

#define FRAME_FLAG 0x7E
#define FRAME_ESC  0x7D

typedef enum {
  FRAME_TYPE_SYN,
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

#ifdef __cplusplus
 extern "C" {
#endif


HDLC_FRAME* init_HDLC(uint16_t dataSize);
void        init_data_frame (HDLC_FRAME* frame, uint8_t* data, uint16_t seq);
int         escape_frame (const uint8_t* frame, int len, uint8_t* escaped);
int         unescape_frame (const uint8_t* escaped, int len, uint8_t* frame);
void        dump_frame (HDLC_FRAME* frame);
uint16_t    calculate_frame_CRC (HDLC_FRAME* frame);


#ifdef __cplusplus
}
#endif

#endif
