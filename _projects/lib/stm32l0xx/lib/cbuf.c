#include <stdio.h>

// generic circular buffer used as fifo

#define BUFFER_SIZE 128
typedef int BOOL;
#define TRUE 1 
#define FALSE 0

typedef struct {
    char buf[BUFFER_SIZE];
    int writePos;
    int readPos;
    int count;
} cbuf;


void init_buffer(cbuf *cb) {
    cb->writePos = 0;
    cb->readPos = 0;
    cb->count = 0;
}

BOOL cbuf_write(cbuf* cb, const char *msg, int len)
{
    // buffer is full
    if (cb->count + len > BUFFER_SIZE) {
        return FALSE;
    }

    for (int i = 0; i < len; ++i)
    {
        cb->buf[cb->writePos] = msg[i];
        cb->writePos = (cb->writePos + 1) % BUFFER_SIZE;
        cb->count++; 
    }

    return TRUE;
}


int cbuf_read(cbuf* cb, char *rbuf, int bytesRequested)
{
    int bytesAvailable = cb->count; 
    int bytesRead = 0;

    if (bytesAvailable > 0)
    {
        bytesRead = (bytesRequested > bytesAvailable) ? bytesAvailable : bytesRequested;
        for (int i = 0; i < bytesRead; i++)
        {
            rbuf[i] = cb->buf[(cb->readPos + i) % BUFFER_SIZE]; 
        }
        cb->readPos = (cb->readPos + bytesRead) % BUFFER_SIZE;
        cb->count -= bytesRead; 
    }
    return bytesRead;
}
