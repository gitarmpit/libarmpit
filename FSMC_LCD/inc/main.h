#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f10x_conf.h"
#include "stm3210e_lcd.h"
#include "stm32f10x_it.h"
#include "menu.h"
#include "sdcard.h"
#include "fsmc_sram.h"
#include "fsmc_nand.h"
#include "fsmc_nor.h"
#include <stdio.h>
#include <string.h>

void STM32_LCD_Demo(void);
void InterruptConfig(void);
void RCC_Config(void);
void NVIC_Config(void);
void GPIO_Config(void);
void SysTick_Configuration(void);
void Delay(uint32_t nCount);
uint32_t DelayJoyStick(uint32_t nTime);
void Decrement_TimingDelay(void);
void Set_SELStatus(void);
void CheckBitmapFilesStatus(void);
void IntExtOnOffConfig(FunctionalState NewState);
void ADC_Config(void);

#endif /* __MAIN_H */

