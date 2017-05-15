    PRESERVE8
    THUMB

    INCLUDE stm32.s
    INCLUDE gpio_regs.s

    IF :LNOT: :DEF:STM32
        ASSERT {FALSE}
    ENDIF


    AREA    DATA2, DATA, READWRITE 

GPIOA_CFG  SPACE GPIO_CFG_SIZE

    AREA    CODE, CODE, READONLY


    IMPORT Debug_EnableCYCCNT
    IMPORT Debug_Delay_us    

    IMPORT SystickEnable
    IMPORT SystickSetReloadValue
    IMPORT SystickDisable    


    IMPORT GPIO_init              ;GPIO_cfg: crl crh idr odr bsrr brr
    IMPORT GPIO_cfg_out_pp        ;GPIO_cfg  pin 
    IMPORT GPIO_set_pin           ;GPIO_cfg  pin
    IMPORT GPIO_reset_pin         ;GPIO_cfg  pin
    IMPORT GPIO_toggle_pin           ;GPIO_cfg  pin
    IMPORT GPIO_set_output        ;GPIO_cfg  pin 
    IMPORT GPIO_is_set            ;GPIO_cfg  pin

    IMPORT RCC_GetAHB_CLK

    EXPORT SysTick_Handler


SysTick_Handler PROC
    push {lr}
    ;ldr r0, =GPIOA_CFG
    ;mov r1, #GPIO_PIN0
    bl GPIO_toggle_pin
    
    pop {lr}
    bx lr
    ENDP


;###################################################
    EXPORT   __main
__main PROC 
 

    mov r0, #TRUE
    bl Debug_EnableCYCCNT


    mov r0, #GPIOA
    ldr r1, =GPIOA_CFG
    bl GPIO_init
    
    ldr r0, =GPIOA_CFG
    mov r1, #GPIO_PIN0
    bl GPIO_cfg_out_pp

    mov r1, #GPIO_PIN0
    bl GPIO_set_pin
    bl GPIO_reset_pin
    bl GPIO_set_pin

    bl GPIO_toggle_pin
    bl GPIO_toggle_pin


    ;SYSTICK SETUP
    bl RCC_GetAHB_CLK
    mov r1, #TRUE    ;enable
    mov r2, #FALSE   ;div8
    bl SystickEnable
    ldr r0, =GPIOA_CFG
    mov r1, #GPIO_PIN0
    

l1  b l1
    
    ENDP

    ALIGN

    END