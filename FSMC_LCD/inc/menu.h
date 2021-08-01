#ifndef __MENU_H
#define __MENU_H

#include "stm32f10x.h"

#define  MAX_MENU_LEVELS 4
#define  NOKEY  0
#define  SEL    1
#define  RIGHT  2
#define  LEFT   3
#define  UP     4
#define  DOWN   5
#define  KEY    6

#define SLIDE_SIZE    76866 
#define SLIDE1        0x64000000
#define SLIDE2        0x64025842

#define countof(a) (sizeof(a) / sizeof(*(a)))

void Menu_Init(void);
void DisplayMenu(void);
void SelFunc(void);
void UpFunc(void);
void DownFunc(void);
void ReturnFunc(void);
uint8_t ReadKey(void);
void IdleFunc(void);
void DisplayIcons(void);
void LCD_NORDisplay(uint32_t address);
void ShowMenuIcons(void);
void STM32_LCD_DemoIntro(void);
uint32_t CheckBitmapFiles(void);
void InternalFlashToLCD(void);
void InternalFlashToLCD_DMA(void);
void InternalFlashToLCD_Speed(void);
void NORFlashToLCD(void);
void NORFlashToLCD_DMA(void);
void NORFlashToLCD_Speed(void);
void NANDFlashToLCD(void);
void NANDFlashToLCD_Speed(void);
void ExternalSRAMToLCD(void);
void ExternalSRAMToLCD_DMA(void);
void ExternalSRAMToLCD_Speed(void);
void SDCardToLCD(void);
void SDCardToLCD_Speed(void);
void TimingMeasurement_Config(void);
void DisplayTimingCompute(void);
void CopyToInternalFlash(void);
void CopyToExternalSRAM(void);
void CopyToNANDFlash(void);
void CopyToSDCard(void);
void NAND_PhysicalErase(void);

#endif /* __MENU_H */

