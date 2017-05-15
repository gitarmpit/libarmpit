ADC1_BASE EQU 0x40012400  ;APB2  100x, 10x
ADC2_BASE EQU 0x40012800  ;APB2, 10x
ADC3_BASE EQU 0x40013C00  ;APB2, 10x

ADC1_SR     EQU ADC1_BASE
ADC2_SR     EQU ADC2_BASE
ADC3_SR     EQU ADC3_BASE
ADC1_CR1    EQU ADC1_BASE + 0x4
ADC2_CR1    EQU ADC2_BASE + 0x4
ADC3_CR1    EQU ADC3_BASE + 0x4
ADC1_CR2    EQU ADC1_BASE + 0x8
ADC2_CR2    EQU ADC2_BASE + 0x8
ADC3_CR2    EQU ADC3_BASE + 0x8
ADC1_SMPR1  EQU ADC1_BASE + 0xc
ADC2_SMPR1  EQU ADC2_BASE + 0xc
ADC3_SMPR1  EQU ADC3_BASE + 0xc
ADC1_SMPR2  EQU ADC1_BASE + 0x10
ADC2_SMPR2  EQU ADC2_BASE + 0x10
ADC3_SMPR2  EQU ADC3_BASE + 0x10

ADC1_HTR   EQU ADC1_BASE + 0x24
ADC2_HTR   EQU ADC2_BASE + 0x24
ADC3_HTR   EQU ADC3_BASE + 0x24

ADC1_LTR   EQU ADC1_BASE + 0x28
ADC2_LTR   EQU ADC2_BASE + 0x28
ADC3_LTR   EQU ADC3_BASE + 0x28

ADC1_SQR1   EQU ADC1_BASE + 0x2c
ADC2_SQR1   EQU ADC2_BASE + 0x2c
ADC3_SQR1   EQU ADC3_BASE + 0x2c    
ADC1_SQR2   EQU ADC1_BASE + 0x30
ADC2_SQR2   EQU ADC2_BASE + 0x30
ADC3_SQR2   EQU ADC3_BASE + 0x30
ADC1_SQR3   EQU ADC1_BASE + 0x34
ADC2_SQR3   EQU ADC2_BASE + 0x34
ADC3_SQR3   EQU ADC3_BASE + 0x34
ADC1_JSQR   EQU ADC1_BASE + 0x38
ADC2_JSQR   EQU ADC2_BASE + 0x38
ADC3_JSQR   EQU ADC3_BASE + 0x38

ADC1_JDR1   EQU ADC1_BASE + 0x3c
ADC1_JDR2   EQU ADC1_BASE + 0x40
ADC1_JDR3   EQU ADC1_BASE + 0x44
ADC1_JDR4   EQU ADC1_BASE + 0x48

ADC2_JDR1   EQU ADC2_BASE + 0x3c
ADC2_JDR2   EQU ADC2_BASE + 0x40
ADC2_JDR3   EQU ADC2_BASE + 0x44
ADC2_JDR4   EQU ADC2_BASE + 0x48

ADC3_JDR1   EQU ADC3_BASE + 0x3c
ADC3_JDR2   EQU ADC3_BASE + 0x40
ADC3_JDR3   EQU ADC3_BASE + 0x44
ADC3_JDR4   EQU ADC3_BASE + 0x48

ADC1_DR   EQU ADC1_BASE + 0x4c
ADC2_DR   EQU ADC2_BASE + 0x4c
ADC3_DR   EQU ADC3_BASE + 0x4c


ADC_SR_AWD   (1<<0)  ;Analog watchdog flag
ADC_SR_EOC   (1<<1)  ;End of conversion
ADC_SR_JEOC  (1<<2)  ;End of conversion (injected channel)
ADC_SR_JSTRT (1<<3)  ;Injected channel Start flag
ADC_SR_STRT  (1<<4)  ;Regular channel Start flag

;ADC_CR1    Control register 1

;ADC1 analog Channel16 and Channel17 are internally connected to the temperature sensor and to VREFINT, respectively.

ADC_CR1_AWDCH  (1<<0)
ADC_CR1_EOCIE  (1<<5)  ;Interrupt enable for EOC (end of conversion) 
ADC_CR1_AWDIE  (1<<6)  ;Analog watchdog interrupt enable
ADC_CR1_JEOCIE (1<<7)  ;Interrupt enable for injected channels
ADC_CR1_SCAN   (1<<8)  ;
ADC_CR1_AWDSGL (1<<9)  ;Enable the watchdog on a single channel in scan mode
ADC_CR1_JAUTO  (1<<10) 
ADC_CR1_DISCEN (1<<11) ;Discontinuous mode on regular channels
ADC_CR1_JDISCEN (1<<12) ;Discontinuous mode on injected channels
ADC_CR1_DISCNUM (1<<13)

;Bits 19:16DUALMOD[3:0]: Dual mode selection

ADC_CR1_JAWDEN (1<<22) ;Analog watchdog enable on injected channels
ADC_CR1_AWDEN (1<<23)  ;Analog watchdog enable on regular channels

;ADC_CR2  Control register 2

ADC_CR2_ADON   (1<<0)  ; A/D converter ON / OFF
ADC_CR2_CONT   (1<<1)  ; Continuous conversion 
ADC_CR2_CAL    (1<<2)  ; A/D calibration  1: enable cal  0: cal completed
ADC_CR2_RSTCAL (1<<3) ; Reset calibration  1: initialize cal reg, 0: cal reg initialized
ADC_CR2_DMA    (1<<8)
ADC_CR2_ALIGN    (1<<11)  ;0: left 1:right

;Bits 19:17EXTSEL[2:0]: External event select for regular group

ADC_CR2_EXTSEL_ADC12_T1CC1   (0<<17)
ADC_CR2_EXTSEL_ADC12_T1CC2   (1<<17)
ADC_CR2_EXTSEL_ADC12_T1CC3   (2<<17)
ADC_CR2_EXTSEL_ADC12_T2CC2   (3<<17)
ADC_CR2_EXTSEL_ADC12_T3TRGO  (4<<17)
ADC_CR2_EXTSEL_ADC12_T4CC4   (5<<17)
ADC_CR2_EXTSEL_ADC12_EXTI11  (6<<17)
ADC_CR2_EXTSEL_ADC12_SWSTART (7<<17)

ADC_CR2_EXTTRIG  (1<<20) 
ADC_CR2_JSWSTART (1<<21)
ADC_CR2_SWSTART  (1<<22)
ADC_CR2_TSVREFE  (1<<23) ; Temperature sensor and  VREFINT enable 

; In devices with dual ADCs this bit is present only in ADC1

ADC_CR2_ADC1_TSVREFE  (1<<23)  

    END
