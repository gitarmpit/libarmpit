    PRESERVE8
    THUMB

    INCLUDE stm32.s
    INCLUDE timer_regs.s
    INCLUDE gpio_regs.s

    IF :LNOT: :DEF:STM32
        ASSERT {FALSE}
    ENDIF


    AREA  RO, DATA, READONLY 
MAX_MISSED_COUNT  DCD 20        

    AREA DATA, DATA, READWRITE

period_array SPACE  128*4 
duty_array  SPACE  128*4


gpio_cfg  SPACE GPIO_CFG_SIZE


    AREA    CODE, CODE, READONLY

    INCLUDE imp.s

    IMPORT TIM3_CFG
    IMPORT Capture_Cfg    
    IMPORT PWM_Out
        



 ;====================================

;###################################################
    EXPORT   __main
__main PROC 

    mov r0, #2
    bl FLASH_SetWaitState

    mov r0, #16
    bl RCC_EnablePLL
    

    mov r0, #GPIOA
    ldr r1, =gpio_cfg
    bl GPIO_init
    
    ldr r0, =gpio_cfg
    mov r1, #GPIO_PIN0
    bl GPIO_cfg_out_alt_pp


    bl PWM_Out

    ldr r0, =gpio_cfg
    mov r1, #GPIO_PIN6
    push {r1}
    bl GPIO_cfg_in_float
    pop {r1}
;    ldr r0, =capt_pin 
;    str r1, [r0]


    mov r0, #TIM3
    ldr r1, =TIM3_CFG
    bl TIM_Init 
    
    ldr r0, =TIM3_CFG
    mov r1, #TRUE
    bl TIM_EnableNVICInterrupt
        
    ;ldr r0, =TIM3_CFG
    ;mov r1, #TRUE
    ;bl TIM_EnableUpdateInterrupt
        
    ldr r0, =TIM3_CFG
    mov r1, #0
    bl TIM_SetCounterValue


    ldr r0, =TIM3_CFG
    mov r1, #0x300
    bl TIM_SetUpdatePeriod_us
    
    
    ldr r0, =TIM3_CFG
    mov r1, #1; channel
    bl Capture_Cfg

    ldr r0, =TIM3_CFG
    mov r1, #TRUE
    bl TIM_EnableCounter
    
    mov r0, #TRUE
    bl Debug_EnableCYCCNT

    ldr r0, =10000
    bl Debug_Delay_us

    IMPORT period_us
    IMPORT duty_us

    mov r4, #128 
    ldr r5, =period_array
    ldr r6, =duty_array
l
    ldr r0, =20000
    bl Debug_Delay_us
    ldr r8, =period_us 
    ldr r8, [r8]
    str r8, [r5], #4
    ldr r8, =duty_us
    ldr r8, [r8]
    str r8, [r6], #4
    subs r4, #1 
    bne l

l1  b l1

    ENDP

    ALIGN

    END