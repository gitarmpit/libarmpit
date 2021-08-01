This AN describes a firmware TFT LCD interfacing with the High Density STM32F10xxx 
FSMC peripheral. The main purpose of this firmware package is to provide resources 
facilitating the development of an application using the LCD on FSMC peripheral.

The firmware interface is composed of library source files developed in order to
support LCD features, an application demo is also provided.

- inc 
   - FSMC_LCD/inc/fonts.h           LCD fonts size definition
   - FSMC_LCD/inc/stm32f10x_conf.h  Library Configuration file
   - FSMC_LCD/inc/stm32f10x_it.h    Interrupt handlers header file
   - FSMC_LCD/inc/stm3210e_lcd.h    LCD Header file
   - FSMC_LCD/inc/fsmc_nand.h       FSMC NAND driver Header file
   - FSMC_LCD/inc/fsmc_nor.h        FSMC NOR driver Header file
   - FSMC_LCD/inc/fsmc_sram.h       FSMC SRAM driver Header file
   - FSMC_LCD/inc/main.h            Main Header file
   - FSMC_LCD/inc/menu.h            Menu Navigation Header file
   - FSMC_LCD/inc/sdcard.h          SD Card driver Header file      

- src 
   - FSMC_LCD/src/stm32f10x_it.c    Interrupt handlers
   - FSMC_LCD/src/main.c            Main program 
   - FSMC_LCD/src/fsmc_nand.c       FSMC NAND driver firmware functions
   - FSMC_LCD/src/fsmc_nor.c        FSMC NOR driver firmware functions
   - FSMC_LCD/src/fsmc_sram.c       FSMC SRAM driver firmware functions
   - FSMC_LCD/src/stm3210e_lcd.c    LCD driver firmware functions
   - FSMC_LCD/src/sdcard.c          SD Card driver firmware functions
   - FSMC_LCD/src/menu.c            Menu navigation firmware functions         

  - This example runs on STM32F10x High-Density Devices.
  
  - This example has been tested with STMicroelectronics STM3210E-EVAL (STM32F10x 
    High-Density) evaluation board and can be easily tailored to any other 
    supported device and development board.
     
- High-density devices are STM32F101xx and STM32F103xx microcontrollers where
  the Flash memory density ranges between 256 and 512 Kbytes.               
