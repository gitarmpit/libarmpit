#ifdef __GNUC__

#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <malloc.h>

//C++ stubs. Required to be able to build C++ code using gcc instead of g++ and without linking with libstdc++
//C++ limitations: no rtti, no exceptions: -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics

void __cxa_pure_virtual() { while (1); } 


//Uncomment if using -no-startfiles
//void _init(){};  // dummy _init function for support of GNU toolchain from https://launchpad.net/gcc-arm-embedded


/*This function is used for handle heap option*/
extern int  _end;

//#ifndef SEMIHOSTING

__attribute__ ((used))
 caddr_t _sbrk ( int incr )
{
    static unsigned char *heap = NULL;
    unsigned char *prev_heap;

    if (heap == NULL) {
        heap = (unsigned char *)&_end;
    }
    prev_heap = heap;

    heap += incr;

    return (caddr_t) prev_heap;
}

//#endif


// stubs 

#ifndef SEMIHOSTING

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

#endif


#endif
