#ifndef __STM32F4_DISCOVERY_ACCELEROMETER_H
#define __STM32F4_DISCOVERY_ACCELEROMETER_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4_discovery.h"
   
/* Include Accelerometer component drivers */
#include "..\Components\lis302dl\lis302dl.h"   
#include "..\Components\lis3dsh\lis3dsh.h"   

typedef enum 
{
  ACCELERO_OK = 0,
  ACCELERO_ERROR = 1,
  ACCELERO_TIMEOUT = 2
}ACCELERO_StatusTypeDef;

uint8_t BSP_ACCELERO_Init(void);
uint8_t BSP_ACCELERO_ReadID(void);
void    BSP_ACCELERO_Reset(void);
void    BSP_ACCELERO_Click_ITConfig(void);
void    BSP_ACCELERO_Click_ITClear(void);
void    BSP_ACCELERO_GetXYZ(int16_t *pDataXYZ);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_DISCOVERY_ACCELEROMETER_H */

