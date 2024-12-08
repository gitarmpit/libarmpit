#ifndef _CBUF_H
#define _CBUF_H
#include "config.h"

#define CBUFFER_SIZE 128

typedef struct {
    char buf[CBUFFER_SIZE];
    int writePos;
    int readPos;
    int count;
} cbuf;


#ifdef __cplusplus
 extern "C" {
#endif

BOOL cbuf_write(cbuf* cb, const char *msg, int len);
int  cbuf_read(cbuf* cb, char *rbuf, int bytesRequested);

#ifdef __cplusplus
}
#endif

#endif