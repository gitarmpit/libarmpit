// circular buffer
// simulate DMA: can't count bytes, rely on Transfer Complete IRQ
// simulate TC flag

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>

#define BUFFER_SIZE 8
typedef int BOOL;

char buf[BUFFER_SIZE];

int writePos = 0;
int readPos = 0;
FILE *fd;
BOOL TC = 0;

char kbd_ch;

static void write_to_buf(const char *msg, int len)
{
    printf("write pos before: %d, TC: %d\n", writePos, TC);
    int writePos0 = writePos;    

    for (int i = 0; i < len; ++i)
    {
        buf[writePos] = msg[i];
        writePos = (writePos + 1) % BUFFER_SIZE;
    }

    if (writePos < writePos0) {
      if (TC == 1) {
        printf ("buffer overwflow after write\n");
      }
      TC = 1;
    }

    printf("write pos after: %d, TC: %d\n", writePos, TC);
}


static int read_from_buf(char *rbuf, int bytesRequested)
{
    if (writePos > readPos && TC == 1) {
      printf ("read: buffer overflow detected\n");
      return 0;
    }

    int bytesAvailable = (writePos - readPos + BUFFER_SIZE) % BUFFER_SIZE;
    int bytesRead = 0;
    printf("reading %d bytes, rpos=%d, avail: %d\n", bytesRequested, readPos, bytesAvailable);

    if (bytesAvailable > 0)
    {
        bytesRead = (bytesRequested > bytesAvailable) ? bytesAvailable : bytesRequested;
        for (int i = 0; i < bytesRead; i++)
        {
            printf("reading byte: %d\n", (readPos + i) % BUFFER_SIZE);
            rbuf[i] = buf[(readPos + i) % BUFFER_SIZE]; 
        }
        int readPos0 = readPos;
        readPos = (readPos + bytesRead) % BUFFER_SIZE;
        if (readPos < readPos0) {
	  TC = 0;
        }
        printf("new read pos: %d, wpos: %d\n", readPos, writePos);
    }
    return bytesRead;
}

static BOOL read_n_bytes (char* buf, int n, int timeout)
{
   int pos = 0;
   int nretries = 0;
   while (1) {
      int nread = read_from_buf(&buf[pos], n);
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
         if (read_n_bytes(rbuf, cnt, 10)) {
            printf ("read %d bytes\n", cnt);
            for (int i = 0; i < cnt; ++i) {
              fprintf(fd, "%d\n", rbuf[i]);
            }
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
         write_to_buf(wbuf, chunk);
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