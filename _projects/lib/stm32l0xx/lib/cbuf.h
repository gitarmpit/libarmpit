#ifndef _CBUF_H
#define _CBUF_H

BOOL cbuf_write(cbuf* cb, const char *msg, int len);
int  cbuf_read(cbuf* cb, char *rbuf, int bytesRequested);

#endif