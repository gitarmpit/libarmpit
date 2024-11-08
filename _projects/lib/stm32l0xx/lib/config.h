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
void SystemClock_Config_MSI(void);
void SystemClock_Config_HSI_32(void);
void SystemClock_Config_HSI_16(void);
void System_EnableRTC(void);

#ifdef __cplusplus
}
#endif



#endif
