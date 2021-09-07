
#ifndef _TEMPLM74_H
#define _TEMPLM74_H

#include "spi.h"
#include "gpio.h"

/*
 *
 *    Vdd  /CS
 *     8    7     6    5
 *
 *     1    2     3    4
 *    SI/O  CLK        GND
 *
 *
 *   SPI1:  NSS A4, CLK A5, MISO A6, MOSI A7
 *
 */


void LM74_Init(SPI* spi, GPIO_PIN* ssPin);
void LM74_Shutdown();
uint16_t LM74_GetProductCode();
float LM74_GetTemp();


#endif
