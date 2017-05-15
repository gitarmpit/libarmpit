    PRESERVE8
    THUMB


    INCLUDE timer_regs.s
    INCLUDE irq.s
    INCLUDE gpio_regs.s
 
    IF :LNOT: :DEF:STM32
        ASSERT {FALSE}
    ENDIF


    AREA    DATA2, DATA, READWRITE 

GPIOA_CFG  SPACE 28

    AREA    CODE, CODE, READONLY

    ;IMPORT RCC_GetSystemClock 
    ;IMPORT RCC_EnableHSI 
    ;IMPORT RCC_EnableHSE 
    ;IMPORT RCC_EnablePLL
    ;IMPORT RCC_DisablePLL 
    ;IMPORT RCC_EnablePeripheral
    ;IMPORT FLASH_SetWaitState    
    ;IMPORT FLASH_GetWaitState    
    ;IMPORT FLASH_Unlock
    ;IMPORT FLASH_Write   
    ;IMPORT FLASH_ErasePage
    ;IMPORT FLASH_Lock
    ;IMPORT __Vectors
    ;IMPORT |Image$$XO$$Base|
    ;IMPORT |Image$$XO$$Limit|
    ;IMPORT SysTick_Handler    
    ;IMPORT NVIC_SetEnable
    ;IMPORT NVIC_ClearEnable
    ;IMPORT NVIC_IsActive
    ;IMPORT NVIC_SetPriority
    ;IMPORT NVIC_GetPriority
    ;IMPORT _d
    
    IMPORT FPU_Enable
    IMPORT SCB_SetSleepDeep    
    IMPORT SCB_SetSleepOnExit
    IMPORT SCB_SystemReset

    IMPORT Debug_EnableCYCCNT
    IMPORT Debug_Delay_us    
    IMPORT SCB_EnableDWT

    IMPORT SystickEnable
    IMPORT SystickSetReloadValue
    IMPORT SystickDisable    

    IMPORT RCC_GetAHB_CLK

    IMPORT GPIO_init              ;GPIO_cfg: crl crh idr odr bsrr brr
    IMPORT GPIO_cfg               ;GPIO_cfg  pin cnf mode 
    IMPORT GPIO_set_mode          ;GPIO_cfg  pin mode 
    IMPORT GPIO_cfg_in_float      ;GPIO_cfg  pin
    IMPORT GPIO_cfg_in_anal       ;GPIO_cfg  pin
    IMPORT GPIO_cfg_in_pulldown   ;GPIO_cfg  pin
    IMPORT GPIO_cfg_in_pullup     ;GPIO_cfg  pin
    IMPORT GPIO_cfg_out_pp        ;GPIO_cfg  pin 
    IMPORT GPIO_cfg_out_od        ;GPIO_cfg  pin
    IMPORT GPIO_cfg_out_alt_pp    ;GPIO_cfg  pin
    IMPORT GPIO_cfg_out_alt_od    ;GPIO_cfg  pin
    IMPORT GPIO_set_speed_low     ;GPIO_cfg  pin
    IMPORT GPIO_set_speed_medium  ;GPIO_cfg  pin
    IMPORT GPIO_set_speed_high    ;GPIO_cfg  pin
    IMPORT GPIO_set_pin           ;GPIO_cfg  pin
    IMPORT GPIO_reset_pin         ;GPIO_cfg  pin
    IMPORT GPIO_toggle_pin           ;GPIO_cfg  pin
    IMPORT GPIO_set_output        ;GPIO_cfg  pin 
    IMPORT GPIO_is_set            ;GPIO_cfg  pin



    EXPORT SysTick_Handler
SysTick_Handler PROC
    push {lr}
    add r9, #1
    ldr r0, =GPIOA_CFG
    mov r1, #1
    lsl r1, #0
    bl GPIO_toggle_pin
    
    pop {lr}
    bx lr
    ENDP

    EXPORT   __main
__main PROC 
 

    mov r0, #1
    bl Debug_EnableCYCCNT


    ;RCC_EnableGPIOA or smth

    mov r0, #GPIOA
    ldr r1, =GPIOA_CFG
    bl GPIO_init
    
    ldr r0, =GPIOA_CFG
    mov r1, #1
    lsl r1, #0
    bl GPIO_cfg_out_pp

    ldr r0, =GPIOA_CFG
    mov r1, #1
    lsl r1, #0
    bl GPIO_set_pin
    bl GPIO_reset_pin
    bl GPIO_set_pin

    bl GPIO_toggle_pin
    bl GPIO_toggle_pin
    bl GPIO_toggle_pin


    ;mov r9, #0
    ;bl RCC_GetAHB_CLK
    ;mov r1, #1 
    ;mov r2, #0
    ;bl SystickEnable



  ;  ldr r0, =_d
  ;  ldr r0, [r0]
  ;  ldr r0, =data1
  ;  ldr r0, [r0]
  ;  ldr r0, =ddd
  ;  ldr r0, [r0, #32]
    
    
 ;   bl FLASH_Unlock
 ;   bl FLASH_Lock
 ;   bl FLASH_Unlock

 
    ;mov r0, #2
    ;bl FLASH_SetWaitState
    ;bl FLASH_GetWaitState
 
 ;   mov r0, #63
 ;   bl FLASH_ErasePage
 
;    ldr r0, =0x800FC00
;    ldr r1, =0x1234
;    bl FLASH_Write

;    mov r0, #63
;    bl FLASH_ErasePage
;    mov r0, #63
;    bl FLASH_ErasePage
;    ldr r0, =0x800FC00
;    ldr r1, =0x1234
;    bl FLASH_Write
;    ldr r0, =0x800FC00
;    ldr r1, =0x1234
;    bl FLASH_Write
 
    ;mov r0, #1 
    ;ldr r1, =RCC_APB1ENR
    ;ldr r2, =RCC_APB1ENR_PWREN
    ;ldr r3, =RCC_APB1RSTR
    ;ldr r4, =RCC_APB1RSTR_PWRRST
    ;push {r4}
    ;bl RCC_EnablePeripheral 
    ;pop {r4}


forever 
    ldr r0, =1000000
    bl Debug_Delay_us
    ;mov r9, #0
    ldr r0, =GPIOA_CFG
    mov r1, #1
    lsl r1, #0
    bl GPIO_toggle_pin


    b forever
    
    ENDP

    ALIGN

    END