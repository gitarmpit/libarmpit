#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdint.h>

typedef uint8_t BOOL; 

#define TRUE 1 
#define FALSE 0

#ifdef __cplusplus
extern "C" {
#endif

void System_Config(void);
void SystemClock_Config(void);
void System_EnableRTC(void); 

#ifdef __cplusplus
}
#endif



#endif
