// circular buffer

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>

#define BUFFER_SIZE 8
typedef int BOOL;
#define TRUE 1 
#define FALSE 0

FILE *fd;

char kbd_ch;

typedef struct {
    char buf[BUFFER_SIZE];
    int writePos;
    int readPos;
    int count;
} cbuf;


cbuf cb;


void init_buffer(cbuf *cb) {
    cb->writePos = 0;
    cb->readPos = 0;
    cb->count = 0;
}

static BOOL write_to_buf(cbuf* cb, const char *msg, int len)
{
    if (cb->count + len > BUFFER_SIZE) {
        printf("buffer is full\n");
        return FALSE;
    }

    for (int i = 0; i < len; ++i)
    {
        cb->buf[cb->writePos] = msg[i];
        cb->writePos = (cb->writePos + 1) % BUFFER_SIZE;
        cb->count++; 
    }

    printf("write pos after: %d, buffer size: %d\n", cb->writePos, cb->count);
    return TRUE;
}


static int read_from_buf(cbuf* cb, char *rbuf, int bytesRequested)
{
    int bytesAvailable = cb->count; 
    int bytesRead = 0;
    printf("reading %d bytes, rpos=%d, avail: %d\n", bytesRequested, cb->readPos, bytesAvailable);

    if (bytesAvailable > 0)
    {
        bytesRead = (bytesRequested > bytesAvailable) ? bytesAvailable : bytesRequested;
        for (int i = 0; i < bytesRead; i++)
        {
            rbuf[i] = cb->buf[(cb->readPos + i) % BUFFER_SIZE]; 
        }
        cb->readPos = (cb->readPos + bytesRead) % BUFFER_SIZE;
        cb->count -= bytesRead; // Decrement the count of bytes in the buffer
    }
    printf("read pos after: %d, buffer size: %d\n", cb->readPos, cb->count);
    return bytesRead;
}

static BOOL read_n_bytes (cbuf* cb, char* buf, int n, int timeout)
{
   int pos = 0;
   int nretries = 0;
   while (1) {
      int nread = read_from_buf(cb, &buf[pos], n);
      if (nread == 0) {
	return FALSE;
      }
      else if (nread == n) {
         return TRUE;
      }
      if (++nretries >= timeout) {
         return FALSE;
      }
      pos += nread;
      n -= nread;
      sleep(1);
   }
}

void *threadReader(void *arg)
{
   int threadId = *((int *)arg);
   char rbuf[256];
   while (1)
   {
      usleep(1000);
      char tmp_c = kbd_ch;
      if (tmp_c > '0' && tmp_c < '5')
      {
         kbd_ch = 0;
         int cnt = tmp_c - '0';
         if (read_n_bytes(&cb, rbuf, cnt, 10)) {
            printf ("read %d bytes\n", cnt);
            for (int i = 0; i < cnt; ++i) {
              fprintf(fd, "%d\n", rbuf[i]);
            }
         }
         else {
           printf ("read failed\n");    
         }
      }
   }
}

void *threadWriter(void *arg)
{
   int threadId = *((int *)arg);
   int cnt = 0;
   const int chunk = 2;
   char wbuf[chunk];
   while (1)
   {
      if (kbd_ch == 'w')
      {
         kbd_ch = 0;
         for (int i = 0; i < chunk; ++i)
         {
            wbuf[i] = cnt++;
         }
         if (!write_to_buf(&cb, wbuf, chunk)) {
	   cnt -= chunk;	
         }
      }
      usleep(1000);
   }
}

void *kbdreader(void *arg)
{
   while (1)
   {
      if (_kbhit())
      {
         kbd_ch = _getch();
      }
   }
}

int main(void)
{

   fd = fopen("./out", "w");
   if (fd == NULL)
   {
      perror("Error opening file");
      exit(1);
   }

   pthread_t reader;
   pthread_t writer;
   pthread_t tkbd;

   if (pthread_create(&writer, NULL, threadWriter, &writer) != 0)
   {
      perror("Failed to create thread");
      return 1;
   }

   if (pthread_create(&reader, NULL, threadReader, &reader) != 0)
   {
      perror("Failed to create thread");
      return 1;
   }

   if (pthread_create(&tkbd, NULL, kbdreader, &tkbd) != 0)
   {
      perror("Failed to create thread");
      return 1;
   }

   pthread_join(tkbd, NULL);
}