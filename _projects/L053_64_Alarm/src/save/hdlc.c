#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "hdlc.h"
#include "crc.h"


static HDLC_FRAME* init_HDLC(uint16_t dataSize) {

  HDLC_FRAME *frame = malloc(sizeof(HDLC_FRAME) + dataSize);
  if (frame == NULL) {
    perror("Failed to allocate memory for HDLC_FRAME");
    exit(1);
  }

  memset (frame, 0xff, sizeof(HDLC_FRAME) + dataSize);
  frame->frame_start = FRAME_FLAG;
  frame->data_size = dataSize;

  return frame;
}

uint16_t calculate_frame_CRC (HDLC_FRAME* frame) {

  // exclude first 3 bytes
  uint16_t crc_buf_size = sizeof(HDLC_FRAME) + frame->data_size - 3;
  return calculateCRC ( &((uint8_t*)(frame))[3], crc_buf_size);
}


static HDLC_FRAME* init_cmd_frame(uint16_t seq, uint8_t cmd) {

  HDLC_FRAME *frame = init_HDLC(0);

  frame->frame_type = cmd;
  frame->frame_number = seq;

  // exclude first 3 bytes
  uint16_t crc_buf_size = sizeof(HDLC_FRAME) + frame->data_size - 3;
  frame->crc = calculate_frame_CRC (frame);

  return frame;
}

HDLC_FRAME* init_sync_frame(uint16_t seq) {

  return init_cmd_frame(seq, (uint8_t) FRAME_TYPE_SYN);
}

HDLC_FRAME* init_ack_frame(uint16_t seq) {

  return init_cmd_frame(seq, (uint8_t) FRAME_TYPE_ACK);
}

HDLC_FRAME* init_fin_frame(uint16_t seq) {

  return init_cmd_frame(seq, (uint8_t) FRAME_TYPE_FIN);
}


HDLC_FRAME* init_data_frame(uint8_t* data, uint16_t dataSize, uint16_t seq) {

  HDLC_FRAME *frame = init_HDLC(dataSize);

  for (int i = 0; i < frame->data_size; ++i) {
    frame->data[i] = data[i];
  }

  frame->frame_type = (uint8_t) FRAME_TYPE_DATA;
  frame->frame_number = seq;

  frame->crc = calculate_frame_CRC (frame);

  return frame;
}



uint16_t escape_frame (const uint8_t* frame, uint16_t len, uint8_t* escaped) {
  uint16_t esc_len = 0;
  escaped[esc_len++] = FRAME_FLAG;

  for (uint16_t i = 1; i < len; ++i) {
    if (frame[i] == FRAME_FLAG || frame[i] == FRAME_END || frame[i] == FRAME_ESC) {
       escaped[esc_len++] = FRAME_ESC;
       escaped[esc_len++] = frame[i] ^ 0x20;
    } 
    else {
       escaped[esc_len++] = frame[i];
    }
  }
  return esc_len;
}

uint16_t unescape_frame (const uint8_t* escaped, uint16_t len, uint8_t* frame) {
  uint16_t unesc_len = 0;
  frame[unesc_len++] = FRAME_FLAG;
  for (uint16_t i = 1; i < len; ++i) {
    if (escaped[i] == FRAME_ESC && (i+1 < len)) {
       ++i; // skip next char
       frame[unesc_len++] = escaped[i] ^ 0x20;
    } 
    else {
       frame[unesc_len++] = escaped[i];
    }
    if (unesc_len == sizeof(HDLC_FRAME)) {
      //break;
    }
  }

  return unesc_len;
}

void dump_frame (HDLC_FRAME* frame) {
  printf ("#: %d, type: %d, CRC: %x, dsize: %d\n", frame->frame_number, frame->frame_type, frame->crc, frame->data_size);
  for (int i = 0; i < frame->data_size; ++i) {
    printf ("%x ", frame->data[i]);
  }
  printf ("\n");
}


