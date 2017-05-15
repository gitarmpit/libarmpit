    PRESERVE8
    THUMB

    INCLUDE stm32.s
    INCLUDE timer_regs.s
    INCLUDE gpio_regs.s

    IF :LNOT: :DEF:STM32
        ASSERT {FALSE}
    ENDIF


    AREA    DATA2, DATA, READWRITE 

GPIO_CFG  SPACE GPIO_CFG_SIZE
    

    AREA    CODE, CODE, READONLY


    IMPORT GPIO_init              ;GPIO_cfg: crl crh idr odr bsrr brr
    IMPORT GPIO_cfg_out_alt_pp        ;GPIO_cfg  pin 
    IMPORT GPIO_set_pin           ;GPIO_cfg  pin
    IMPORT GPIO_reset_pin         ;GPIO_cfg  pin
    IMPORT GPIO_toggle_pin           ;GPIO_cfg  pin

    IMPORT RCC_SetAPB1Prescaler
    IMPORT FLASH_SetWaitState
    IMPORT RCC_EnablePLL
    IMPORT EXTI_EnableEventFallingEdge
    IMPORT EXTI_EnableEventRisingEdge
    IMPORT EXTI_EnableInterruptFallingEdge
    IMPORT EXTI_EnableInterruptRisingEdge
    IMPORT EXTI_IsPending
    
    IMPORT GPIO_cfg_in_pulldown

    IMPORT AFIO_Enable


    IMPORT AFIO_Remap_SPI1
    IMPORT AFIO_Remap_I2C1
    IMPORT AFIO_Remap_USART1    
    IMPORT AFIO_Remap_USART2    
    IMPORT AFIO_Remap_USART3    
    IMPORT AFIO_Remap_PD01
    IMPORT AFIO_Remap_SWJ_CFG
    IMPORT AFIO_Remap_TIM1
    IMPORT AFIO_Remap_TIM2_partial1
    IMPORT AFIO_Remap_TIM2_partial2
    IMPORT AFIO_Remap_TIM2_full
    IMPORT AFIO_Remap_TIM3
    IMPORT AFIO_Remap_TIM4
    IMPORT AFIO_Remap_TIM5_Ch4
    IMPORT AFIO_Remap_EXTI; r0: line r1: port no
    IMPORT AFIO_Remap_TIM9
    IMPORT AFIO_Remap_TIM10
    IMPORT AFIO_Remap_TIM13
    IMPORT AFIO_Remap_TIM14


    EXPORT EXTI3_IRQHandler 
EXTI3_IRQHandler  PROC
    push {lr}
    mov r0, #3
    mov r1, #TRUE
    bl EXTI_IsPending
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

    
    ;mov r0, #8
    ;bl RCC_SetAPB1Prescaler

    mov r0, #GPIOB
    ldr r1, =GPIO_CFG
    bl GPIO_init
    
    ldr r0, =GPIO_CFG
    mov r1, #GPIO_PIN3
    bl GPIO_cfg_in_pulldown

    ;=====================
    mov r0, #TRUE
    bl AFIO_Enable


    ;====================
    ;r0: isJTAG r1: isSW  r2: isNJRST
    mov r0, #FALSE
    mov r1, #TRUE
    bl AFIO_Remap_SWJ_CFG  
   
   
    ;====================

    mov r0, #3
    mov r1, #GPIOB
    bl AFIO_Remap_EXTI


    mov r0, #3
    mov r1, #TRUE
    bl EXTI_EnableInterruptRisingEdge


l1  b l1

    ENDP

    ALIGN

    END