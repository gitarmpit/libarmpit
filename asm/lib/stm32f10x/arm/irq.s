IRQn_NONE                   EQU 9999
IRQn_IRQnNonMaskableInt     EQU -14    
IRQn_MemoryManagement       EQU -12    
IRQn_BusFault               EQU -11    
IRQn_UsageFault             EQU -10    
IRQn_SVCall                 EQU -5     
IRQn_DebugMonitor           EQU -4     
IRQn_PendSV                 EQU -2     
IRQn_SysTick                EQU -1     

IRQn_WWDG                   EQU 0      
IRQn_PVD                    EQU 1      
IRQn_TAMPER                 EQU 2      
IRQn_RTC                    EQU 3      
IRQn_FLASH                  EQU 4      
IRQn_RCC                    EQU 5      
IRQn_EXTI0                  EQU 6      
IRQn_EXTI1                  EQU 7      
IRQn_EXTI2                  EQU 8      
IRQn_EXTI3                  EQU 9      
IRQn_EXTI4                  EQU 10     
IRQn_DMA1_Channel1          EQU 11     
IRQn_DMA1_Channel2          EQU 12     
IRQn_DMA1_Channel3          EQU 13     
IRQn_DMA1_Channel4          EQU 14     
IRQn_DMA1_Channel5          EQU 15     
IRQn_DMA1_Channel6          EQU 16     
IRQn_DMA1_Channel7          EQU 17     

    IF :DEF:STM32F10X_LD
IRQn_ADC1_2                 EQU 18     
IRQn_USB_HP_CAN1_TX         EQU 19     
IRQn_USB_LP_CAN1_RX0        EQU 20     
IRQn_CAN1_RX1               EQU 21     
IRQn_CAN1_SCE               EQU 22     
IRQn_EXTI9_5                EQU 23     
IRQn_TIM1_BRK               EQU 24     
IRQn_TIM1_UP                EQU 25     
IRQn_TIM1_TRG_COM           EQU 26     
IRQn_TIM1_CC                EQU 27     
IRQn_TIM2                   EQU 28     
IRQn_TIM3                   EQU 29     
IRQn_I2C1_EV                EQU 31     
IRQn_I2C1_ER                EQU 32     
IRQn_SPI1                   EQU 35     
IRQn_USART1                 EQU 37     
IRQn_USART2                 EQU 38     
IRQn_EXTI15_10              EQU 40     
IRQn_RTCAlarm               EQU 41     
IRQn_USBWakeUp              EQU 42      

    ELIF :DEF:STM32F10X_LD_VL
IRQn_ADC1                   EQU 18     
IRQn_EXTI9_5                EQU 23     
IRQn_TIM1_BRK_TIM15         EQU 24     
IRQn_TIM1_UP_TIM16          EQU 25     
IRQn_TIM1_TRG_COM_TIM17     EQU 26     
IRQn_TIM1_CC                EQU 27     
IRQn_TIM2                   EQU 28     
IRQn_TIM3                   EQU 29     
IRQn_I2C1_EV                EQU 31     
IRQn_I2C1_ER                EQU 32     
IRQn_SPI1                   EQU 35     
IRQn_USART1                 EQU 37     
IRQn_USART2                 EQU 38     
IRQn_EXTI15_10              EQU 40     
IRQn_RTCAlarm               EQU 41     
IRQn_CEC                    EQU 42     
IRQn_TIM6_DAC               EQU 54     
IRQn_TIM7                   EQU 55      

    ELIF :DEF:STM32F10X_MD
IRQn_ADC1_2                 EQU 18     
IRQn_USB_HP_CAN1_TX         EQU 19     
IRQn_USB_LP_CAN1_RX0        EQU 20     
IRQn_CAN1_RX1               EQU 21     
IRQn_CAN1_SCE               EQU 22     
IRQn_EXTI9_5                EQU 23     
IRQn_TIM1_BRK               EQU 24     
IRQn_TIM1_UP                EQU 25     
IRQn_TIM1_TRG_COM           EQU 26     
IRQn_TIM1_CC                EQU 27     
IRQn_TIM2                   EQU 28     
IRQn_TIM3                   EQU 29     
IRQn_TIM4                   EQU 30     
IRQn_I2C1_EV                EQU 31     
IRQn_I2C1_ER                EQU 32     
IRQn_I2C2_EV                EQU 33     
IRQn_I2C2_ER                EQU 34     
IRQn_SPI1                   EQU 35     
IRQn_SPI2                   EQU 36     
IRQn_USART1                 EQU 37     
IRQn_USART2                 EQU 38     
IRQn_USART3                 EQU 39     
IRQn_EXTI15_10              EQU 40     
IRQn_RTCAlarm               EQU 41     
IRQn_USBWakeUp              EQU 42      

    ELIF :DEF:STM32F10X_MD_VL
IRQn_ADC1                   EQU 18     
IRQn_EXTI9_5                EQU 23     
IRQn_TIM1_BRK_TIM15         EQU 24     
IRQn_TIM1_UP_TIM16          EQU 25     
IRQn_TIM1_TRG_COM_TIM17     EQU 26     
IRQn_TIM1_CC                EQU 27     
IRQn_TIM2                   EQU 28     
IRQn_TIM3                   EQU 29     
IRQn_TIM4                   EQU 30     
IRQn_I2C1_EV                EQU 31     
IRQn_I2C1_ER                EQU 32     
IRQn_I2C2_EV                EQU 33     
IRQn_I2C2_ER                EQU 34     
IRQn_SPI1                   EQU 35     
IRQn_SPI2                   EQU 36     
IRQn_USART1                 EQU 37     
IRQn_USART2                 EQU 38     
IRQn_USART3                 EQU 39     
IRQn_EXTI15_10              EQU 40     
IRQn_RTCAlarm               EQU 41     
IRQn_CEC                    EQU 42     
IRQn_TIM6_DAC               EQU 54     
IRQn_TIM7                   EQU 55      

    ELIF :DEF:STM32F10X_HD
IRQn_ADC1_2                 EQU 18     
IRQn_USB_HP_CAN1_TX         EQU 19     
IRQn_USB_LP_CAN1_RX0        EQU 20     
IRQn_CAN1_RX1               EQU 21     
IRQn_CAN1_SCE               EQU 22     
IRQn_EXTI9_5                EQU 23     
IRQn_TIM1_BRK               EQU 24     
IRQn_TIM1_UP                EQU 25     
IRQn_TIM1_TRG_COM           EQU 26     
IRQn_TIM1_CC                EQU 27     
IRQn_TIM2                   EQU 28     
IRQn_TIM3                   EQU 29     
IRQn_TIM4                   EQU 30     
IRQn_I2C1_EV                EQU 31     
IRQn_I2C1_ER                EQU 32     
IRQn_I2C2_EV                EQU 33     
IRQn_I2C2_ER                EQU 34     
IRQn_SPI1                   EQU 35     
IRQn_SPI2                   EQU 36     
IRQn_USART1                 EQU 37     
IRQn_USART2                 EQU 38     
IRQn_USART3                 EQU 39     
IRQn_EXTI15_10              EQU 40     
IRQn_RTCAlarm               EQU 41     
IRQn_USBWakeUp              EQU 42     
IRQn_TIM8_BRK               EQU 43     
IRQn_TIM8_UP                EQU 44     
IRQn_TIM8_TRG_COM           EQU 45     
IRQn_TIM8_CC                EQU 46     
IRQn_ADC3                   EQU 47     
IRQn_FSMC                   EQU 48     
IRQn_SDIO                   EQU 49     
IRQn_TIM5                   EQU 50     
IRQn_SPI3                   EQU 51     
IRQn_UART4                  EQU 52     
IRQn_UART5                  EQU 53     
IRQn_TIM6                   EQU 54     
IRQn_TIM7                   EQU 55     
IRQn_DMA2_Channel1          EQU 56     
IRQn_DMA2_Channel2          EQU 57     
IRQn_DMA2_Channel3          EQU 58     
IRQn_DMA2_Channel4_5        EQU 59      

    ELIF :DEF:STM32F10X_HD_VL
IRQn_ADC1                   EQU 18     
IRQn_EXTI9_5                EQU 23     
IRQn_TIM1_BRK_TIM15         EQU 24     
IRQn_TIM1_UP_TIM16          EQU 25     
IRQn_TIM1_TRG_COM_TIM17     EQU 26     
IRQn_TIM1_CC                EQU 27     
IRQn_TIM2                   EQU 28     
IRQn_TIM3                   EQU 29     
IRQn_TIM4                   EQU 30     
IRQn_I2C1_EV                EQU 31     
IRQn_I2C1_ER                EQU 32     
IRQn_I2C2_EV                EQU 33     
IRQn_I2C2_ER                EQU 34     
IRQn_SPI1                   EQU 35     
IRQn_SPI2                   EQU 36     
IRQn_USART1                 EQU 37     
IRQn_USART2                 EQU 38     
IRQn_USART3                 EQU 39     
IRQn_EXTI15_10              EQU 40     
IRQn_RTCAlarm               EQU 41     
IRQn_CEC                    EQU 42     
IRQn_TIM12                  EQU 43     
IRQn_TIM13                  EQU 44     
IRQn_TIM14                  EQU 45     
IRQn_TIM5                   EQU 50     
IRQn_SPI3                   EQU 51     
IRQn_UART4                  EQU 52     
IRQn_UART5                  EQU 53     
IRQn_TIM6_DAC               EQU 54     
IRQn_TIM7                   EQU 55     
IRQn_DMA2_Channel1          EQU 56     
IRQn_DMA2_Channel2          EQU 57     
IRQn_DMA2_Channel3          EQU 58     
IRQn_DMA2_Channel4_5        EQU 59     
IRQn_DMA2_Channel5          EQU 60      

    ELIF :DEF:STM32F10X_XL
IRQn_ADC1_2                 EQU 18     
IRQn_USB_HP_CAN1_TX         EQU 19     
IRQn_USB_LP_CAN1_RX0        EQU 20     
IRQn_CAN1_RX1               EQU 21     
IRQn_CAN1_SCE               EQU 22     
IRQn_EXTI9_5                EQU 23     
IRQn_TIM1_BRK_TIM9          EQU 24     
IRQn_TIM1_UP_TIM10          EQU 25     
IRQn_TIM1_TRG_COM_TIM11     EQU 26     
IRQn_TIM1_CC                EQU 27     
IRQn_TIM2                   EQU 28     
IRQn_TIM3                   EQU 29     
IRQn_TIM4                   EQU 30     
IRQn_I2C1_EV                EQU 31     
IRQn_I2C1_ER                EQU 32     
IRQn_I2C2_EV                EQU 33     
IRQn_I2C2_ER                EQU 34     
IRQn_SPI1                   EQU 35     
IRQn_SPI2                   EQU 36     
IRQn_USART1                 EQU 37     
IRQn_USART2                 EQU 38     
IRQn_USART3                 EQU 39     
IRQn_EXTI15_10              EQU 40     
IRQn_RTCAlarm               EQU 41     
IRQn_USBWakeUp              EQU 42     
IRQn_TIM8_BRK_TIM12         EQU 43     
IRQn_TIM8_UP_TIM13          EQU 44     
IRQn_TIM8_TRG_COM_TIM14     EQU 45     
IRQn_TIM8_CC                EQU 46     
IRQn_ADC3                   EQU 47     
IRQn_FSMC                   EQU 48     
IRQn_SDIO                   EQU 49     
IRQn_TIM5                   EQU 50     
IRQn_SPI3                   EQU 51     
IRQn_UART4                  EQU 52     
IRQn_UART5                  EQU 53     
IRQn_TIM6                   EQU 54     
IRQn_TIM7                   EQU 55     
IRQn_DMA2_Channel1          EQU 56     
IRQn_DMA2_Channel2          EQU 57     
IRQn_DMA2_Channel3          EQU 58     
IRQn_DMA2_Channel4_5        EQU 59      
    ELIF :DEF:STM32F10X_CL
IRQn_ADC1_2                 EQU 18     
IRQn_CAN1_TX                EQU 19     
IRQn_CAN1_RX0               EQU 20     
IRQn_CAN1_RX1               EQU 21     
IRQn_CAN1_SCE               EQU 22     
IRQn_EXTI9_5                EQU 23     
IRQn_TIM1_BRK               EQU 24     
IRQn_TIM1_UP                EQU 25     
IRQn_TIM1_TRG_COM           EQU 26     
IRQn_TIM1_CC                EQU 27     
IRQn_TIM2                   EQU 28     
IRQn_TIM3                   EQU 29     
IRQn_TIM4                   EQU 30     
IRQn_I2C1_EV                EQU 31     
IRQn_I2C1_ER                EQU 32     
IRQn_I2C2_EV                EQU 33     
IRQn_I2C2_ER                EQU 34     
IRQn_SPI1                   EQU 35     
IRQn_SPI2                   EQU 36     
IRQn_USART1                 EQU 37     
IRQn_USART2                 EQU 38     
IRQn_USART3                 EQU 39     
IRQn_EXTI15_10              EQU 40     
IRQn_RTCAlarm               EQU 41     
IRQn_OTG_FS_WKUP            EQU 42     
IRQn_TIM5                   EQU 50     
IRQn_SPI3                   EQU 51     
IRQn_UART4                  EQU 52     
IRQn_UART5                  EQU 53     
IRQn_TIM6                   EQU 54     
IRQn_TIM7                   EQU 55     
IRQn_DMA2_Channel1          EQU 56     
IRQn_DMA2_Channel2          EQU 57     
IRQn_DMA2_Channel3          EQU 58     
IRQn_DMA2_Channel4          EQU 59     
IRQn_DMA2_Channel5          EQU 60     
IRQn_ETH                    EQU 61     
IRQn_ETH_WKUP               EQU 62     
IRQn_CAN2_TX                EQU 63     
IRQn_CAN2_RX0               EQU 64     
IRQn_CAN2_RX1               EQU 65     
IRQn_CAN2_SCE               EQU 66     
IRQn_OTG_FS                 EQU 67      
    ENDIF
    END