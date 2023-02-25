#include <sys/stat.h>

#ifdef __cplusplus
extern "C"
{
#endif


//In all the examples system init is done as the first step in main (starting clocks, setting flash latency etc) 
//Can also do it here. (Makes no difference?)  This location is pointed to by the assembly start-up files provided by ARM
void SystemInit(void)
{
}

// stubs 

int _close(int file) {
  return -1;
}

int _fstat(int file, struct stat *st) {
  st->st_mode = S_IFCHR;
  return 0;
}

int _isatty(int file) {
  return 1;
}

int _lseek(int file, int ptr, int dir) {
  return 0;
}

int _read(int file, char *ptr, int len) {
  return 0;
}

int _write(int file, char *ptr, int len) {
  return 0;
}

#ifdef __cplusplus
}
#endif
