    PRESERVE8
    THUMB

    INCLUDE stm32.s
    INCLUDE timer_regs.s
    INCLUDE gpio_regs.s

    IF :LNOT: :DEF:STM32
        ASSERT {FALSE}
    ENDIF


    AREA    DATA2, DATA, READWRITE 
			


TIM2_CFG  SPACE TIM_INIT_SIZE*4


GPIOA_CFG  SPACE GPIO_CFG_SIZE
    

    AREA    CODE, CODE, READONLY

    IMPORT  TIM_Init
    IMPORT  TIM_EnableCounter
    IMPORT  TIM_EnableChannel
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
    IMPORT GPIO_cfg_out_alt_pp        ;GPIO_cfg  pin 
    IMPORT GPIO_set_pin           ;GPIO_cfg  pin
    IMPORT GPIO_reset_pin         ;GPIO_cfg  pin
    IMPORT GPIO_toggle_pin           ;GPIO_cfg  pin

    IMPORT RCC_SetAPB1Prescaler
    IMPORT FLASH_SetWaitState
    IMPORT RCC_EnablePLL
    IMPORT TIM_SetupPWM1



;###################################################
    EXPORT   __main
__main PROC 


    mov r0, #2
    bl FLASH_SetWaitState

    mov r0, #16
    bl RCC_EnablePLL

    
    ;mov r0, #8
    ;bl RCC_SetAPB1Prescaler

    mov r0, #GPIOA
    ldr r1, =GPIOA_CFG
    bl GPIO_init
    
    ldr r0, =GPIOA_CFG
    mov r1, #GPIO_PIN0
    bl GPIO_cfg_out_alt_pp

    mov r0, #TIM2
    ldr r1, =TIM2_CFG
    bl TIM_Init 


    ldr r0, =TIM2_CFG
    mov r1, #1; channel
    ldr r2, =1000000; period
    ldr r3, =500000; duty 
    bl TIM_SetupPWM1

    ldr r0, =TIM2_CFG
    mov r1, #1; channel
    mov r2, #TRUE
    bl TIM_EnableChannel


    ldr r0, =TIM2_CFG
    mov r1, #TRUE
    bl TIM_EnableCounter


    ;=====================



l1  b l1

    ENDP

    ALIGN

    END