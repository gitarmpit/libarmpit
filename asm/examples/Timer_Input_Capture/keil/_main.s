    PRESERVE8
    THUMB

    INCLUDE stm32.s

    IF :LNOT: :DEF:STM32
        ASSERT {FALSE}
    ENDIF


    AREA  RO, DATA, READONLY 
MAX_MISSED_COUNT  DCD 20        

    INCLUDE timer_regs.s
    INCLUDE gpio_regs.s

    AREA    DATA2, DATA, READWRITE 

period_array SPACE  128*4 
duty_array  SPACE  128*4



TIM2_CFG  SPACE TIM_INIT_SIZE*4
TIM3_CFG  SPACE TIM_INIT_SIZE*4



gpio_cfg  SPACE GPIO_CFG_SIZE
capt_pin   DCD 0 
tim_ch     DCD 0 
last_cnt   DCD 0 
clk_freq   DCD 0 
presc      DCD 0 
ar         DCD 0 
missed_cnt DCD 0 
period_us  DCD 0 
duty_us    DCD 0 
falling_edge DCB 0    


    AREA    CODE, CODE, READONLY

;    INCLUDE imp.s

    IMPORT  TIM_Init
    IMPORT TIM_EnableCounter
    IMPORT TIM_EnableChannel
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
    IMPORT TIM_SetICCFallingEdge
    IMPORT TIM_SetICCRisingEdge
    IMPORT TIM_SetupPWM1

    IMPORT GPIO_init              ;GPIO_cfg: crl crh idr odr bsrr brr
    IMPORT GPIO_cfg_out_alt_pp        ;GPIO_cfg  pin 
    IMPORT GPIO_set_pin           ;GPIO_cfg  pin
    IMPORT GPIO_reset_pin         ;GPIO_cfg  pin
    IMPORT GPIO_toggle_pin           ;GPIO_cfg  pin
    IMPORT GPIO_cfg_in_float
    IMPORT GPIO_is_set

    IMPORT RCC_SetAPB1Prescaler

    IMPORT FLASH_SetWaitState
    IMPORT RCC_EnablePLL
    IMPORT TIM_EnableCCInterrupt
    IMPORT TIM_SetModeInputDirect
    IMPORT TIM_GetCCR
    IMPORT TIM_IsCCFlag
    IMPORT TIM_GetTimerClockFreq

    IMPORT _udiv64 
    IMPORT Debug_EnableCYCCNT
    IMPORT Debug_Delay_us


    EXPORT TIM3_IRQHandler


TIM3_IRQHandler  PROC
    push {r4 -r6, lr}
    ldr r0, =TIM3_CFG
    ldr r1, =tim_ch
    ldr r1,[r1]
    mov r2, #TRUE; check and clear if set
    bl TIM_IsCCFlag
    cmp r0, #FALSE
    beq end_capt
    
    ldr r0, =TIM3_CFG
    ldr r1, =tim_ch
    ldr r1, [r1]
    bl TIM_GetCCR
    mov r4, r0; cnt
    ldr r0, =TIM3_CFG
    ldr r1, =tim_ch
    ldr r1, [r1]
    ldr r2, =falling_edge
    ldr r6, [r2]
    cmp r6, #TRUE; isFalling 
    beq ris
    bl TIM_SetICCFallingEdge
    b done_edge
ris
    bl TIM_SetICCRisingEdge
done_edge
    ldr r2, =falling_edge
    eor r6, #1 
    str r6, [r2]

    ldr r2, =missed_cnt
    mov r3, #0
    str r3, [r2]
    
    ldr r2, =last_cnt
    ldr r2, [r2]
    cmp r2, #0 
    beq end_capt
    
    ; smth captured 
    ldr r2, =last_cnt
    ldr r2, [r2]
    sub r0, r4, r2; r0: tick = cnt - last 
    cmp r0, #0
    ITTT LT
    ldrlt r2, =ar
    ldrlt r2, [r2]
    ;addlt r2, #1 
    addlt r0, r2
    
    ;;;;;;;;;;;;;;;;;;;;;;;;;;
    ldr r1, =1000000
    umull r0, r1, r0, r1
    ldr r2, =presc
    ldr r2, [r2]
    mul r1, r1, r2 
    umull r0, r3, r0, r2
    add r1, r3
    
    ldr r2, =clk_freq
    ldr r2, [r2]
    bl _udiv64
    mov r5, r0  ; r5: tick_us
    
    ;;;;;;
    
    ;ldr r0, =gpio_cfg
    ;ldr r1, =capt_pin
    ;ldr r1, [r1]
    ;bl GPIO_is_set
    ;cmp r0, #TRUE 
    cmp r6, #TRUE
    beq is_set
    ldr r0, =duty_us
    str r5, [r0]; duty = tick_us
    b end_capt
    
is_set 
    ldr r0, =duty_us
    ldr r0, [r0]
    add r0, r5 ; tick_us + duty_us 
    ldr r1, =period_us 
    str r0, [r1]
    
    ; end capture calc

end_capt

    ldr r2, =last_cnt
    str r4, [r2]    

    pop {r4 - r6, lr}
    bx lr
    ENDP



; r0: TIMER_CFG, r1: pin, 
TIM_Capture PROC
    push {r4, r5, lr}
    
    sub sp, #8 
    str r0, [sp, #4]
    str r1, [sp]

    ldr r4, =tim_ch
    str r1, [r4]
    
    mov r2, #TRUE
    bl TIM_EnableCCInterrupt

    ldr r0, [sp, #4]
    ldr r1, [sp]
    mov r2, #TRUE
    bl TIM_SetModeInputDirect
    ldr r0, [sp, #4]
    ldr r1, [sp]
    mov r2, #TRUE
    bl TIM_SetICCFallingEdge
    
    ldr r4, =falling_edge
    mov r5, #1 
    str r5, [r4]

    
    ldr r0, [sp, #4]
    bl TIM_GetPrescalerValue
    add r0, #1
    ldr r4, =presc
    str r0, [r4]

    ldr r0, [sp, #4]
    bl TIM_GetAutoReloadValue
    ldr r4, =ar
    str r0, [r4]
    
    ldr r0, [sp, #4]
    bl TIM_GetTimerClockFreq
    ldr r4, =clk_freq
    str r0, [r4]   

    ldr r0, [sp, #4]
    ldr r1, [sp]
    mov r2, #TRUE
    bl TIM_EnableChannel


    add sp, #8
    pop {r4, r5, lr}
    bx lr
    ENDP

 ;====================================
PWM_Out  PROC 
    push {lr}
    mov r0, #TIM2
    ldr r1, =TIM2_CFG
    bl TIM_Init 
    
    
    ldr r0, =TIM2_CFG
    mov r1, #1; channel
    mov r2, #0x17; period
    mov r3, #0x3; duty 
    bl TIM_SetupPWM1

    ldr r0, =TIM2_CFG
    mov r1, #1; channel
    mov r2, #TRUE
    bl TIM_EnableChannel


    ldr r0, =TIM2_CFG
    mov r1, #TRUE
    bl TIM_EnableCounter
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
    
    ldr r0, =capt_pin

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
    ldr r0, =capt_pin 
    str r1, [r0]


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
    mov r1, #200
    bl TIM_SetUpdatePeriod_us
    
    
    ldr r0, =TIM3_CFG
    mov r1, #1; channel
    bl TIM_Capture

    ldr r0, =TIM3_CFG
    mov r1, #TRUE
    bl TIM_EnableCounter
    
    mov r0, #TRUE
    bl Debug_EnableCYCCNT

    ldr r0, =10000
    bl Debug_Delay_us

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