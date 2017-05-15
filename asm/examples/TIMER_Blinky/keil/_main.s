    PRESERVE8
    THUMB

    INCLUDE stm32.s
    INCLUDE timer_regs.s
    INCLUDE gpio_regs.s

    IF :LNOT: :DEF:STM32
        ASSERT {FALSE}
    ENDIF


    AREA    DATA2, DATA, READWRITE 
			
CNT DCD 1 
PSC DCD 1	
TTT DCD 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16


TIM2_CFG  SPACE TIM_INIT_SIZE*4


GPIOA_CFG  SPACE GPIO_CFG_SIZE
    

    AREA    CODE, CODE, READONLY

    IMPORT _udiv64

    IMPORT  TIM_Init
    IMPORT  TIM_EnableCounter
    IMPORT TIM_EnableUpdateEvent
    IMPORT TIM_EnableAutoPreload        
    IMPORT TIM_EnableUpdateInterrupt
    IMPORT TIM_EnableNVICInterrupt
    IMPORT TIM_EnableTriggerInterrupt
    IMPORT TIM_SetCounterValue
    IMPORT TIM_SetPrescalerValue
    IMPORT TIM_SetAutoReloadValue
    IMPORT TIM_GetCounterValue
    IMPORT TIM_GetPrescalerValue
    IMPORT TIM_GetAutoReloadValue
    IMPORT TIM_CalculateTimerValues
    IMPORT TIM_SetUpdatePeriod_us
    IMPORT TIM_IsUIF
    IMPORT TIM_ClearUIF

    IMPORT GPIO_init              ;GPIO_cfg: crl crh idr odr bsrr brr
    IMPORT GPIO_cfg_out_pp        ;GPIO_cfg  pin 
    IMPORT GPIO_set_pin           ;GPIO_cfg  pin
    IMPORT GPIO_reset_pin         ;GPIO_cfg  pin
    IMPORT GPIO_toggle_pin           ;GPIO_cfg  pin

    IMPORT FLASH_SetWaitState
    IMPORT RCC_EnablePLL
    IMPORT RCC_SetAPB1Prescaler

    IMPORT Debug_EnableCYCCNT
    IMPORT Debug_Delay_us

    EXPORT TIM2_IRQHandler


TIM2_IRQHandler  PROC
    push {lr}
    ldr r0, =TIM2_CFG
    bl TIM_IsUIF
    cmp r0, #TRUE
    bne tout2
    ldr r0, =TIM2_CFG
    bl TIM_ClearUIF
    ldr r0, =GPIOA_CFG
    mov r1, #GPIO_PIN0
    bl GPIO_toggle_pin
tout2
    pop {lr}
    bx lr
	ENDP


;###################################################
    EXPORT   __main
__main PROC 



    mov r0, #2
    bl FLASH_SetWaitState
    
    mov r0, #16 
    bl RCC_EnablePLL

    ;mov r0, #TRUE 
    ;bl Debug_EnableCYCCNT


    ;mov r0, #8
    ;bl RCC_SetAPB1Prescaler

    mov r0, #GPIOA
    ldr r1, =GPIOA_CFG
    bl GPIO_init
    
    ldr r0, =GPIOA_CFG
    mov r1, #GPIO_PIN0
    bl GPIO_cfg_out_pp
    mov r1, #GPIO_PIN0
    bl GPIO_toggle_pin
    bl GPIO_toggle_pin
        
    mov r0, #TIM2
    ldr r1, =TIM2_CFG

    bl TIM_Init 
    ldr r0, =TIM2_CFG


    ;=====================

    ldr r0, =TIM2_CFG
		mov r1, #TRUE
		bl TIM_EnableNVICInterrupt
		
    ldr r0, =TIM2_CFG
		mov r1, #TRUE
		bl TIM_EnableUpdateInterrupt
		
    ldr r0, =TIM2_CFG
		mov r1, #0
		bl TIM_SetCounterValue


    ldr r0, =TIM2_CFG
    ldr r1, =1000000
		;ldr r2, =CNT
		;ldr r3, =PSC
    ;bl TIM_CalculateTimerValues
	bl TIM_SetUpdatePeriod_us
    ;mov r1, 0x40
    ;bl TIM_SetAutoReloadValue	
		;ldr r2, [r2]
		;ldr r3, [r3]

    mov r0, #TRUE 
    bl Debug_EnableCYCCNT

    ldr r0, =TIM2_CFG
    mov r1, #TRUE
    bl TIM_EnableCounter
    
  ;  ldr r0, =10000000
  ;  bl Debug_Delay_us

l1  
    b l1

    ENDP

    ALIGN

    END