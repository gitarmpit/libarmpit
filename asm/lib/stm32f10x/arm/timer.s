
    PRESERVE8
    THUMB

    INCLUDE timer_regs.s
    INCLUDE macros.s

        
    AREA    DRW, DATA, READWRITE

TIMERS          ;isAPB1                                    ;advanced  ;irq handler func addr
    DCD TIM1_BASE, FALSE, IRQn_TIM1_UP, RCC_APB2ENR_TIM1EN, TRUE,      0
    DCD TIM2_BASE, TRUE,  IRQn_TIM2,    RCC_APB1ENR_TIM2EN, FALSE,     0
    DCD TIM3_BASE, TRUE,  IRQn_TIM3,    RCC_APB1ENR_TIM3EN, FALSE,     0
    DCD TIM4_BASE, TRUE,  IRQn_TIM4,    RCC_APB1ENR_TIM4EN, FALSE,     0        


;####################################################################

    AREA    TCODE, CODE, READONLY
        
    IMPORT RCC_EnablePeripheral   ;0: true/false, 1: en_reg 2: en_flag 3: rst_reg  sp: rst_flag 
    IMPORT NVIC_SetEnable
    IMPORT NVIC_ClearEnable
    IMPORT RCC_GetTIMx_CLK

    IMPORT _udiv64

    EXPORT TIM_Init
    EXPORT TIM_EnableCounter
    EXPORT TIM_EnableUpdateEvent
    EXPORT TIM_EnableAutoPreload    
    EXPORT TIM_EnableUpdateInterrupt
    EXPORT TIM_EnableNVICInterrupt
    EXPORT TIM_EnableTriggerInterrupt
    EXPORT TIM_SetCounterValue
    EXPORT TIM_SetPrescalerValue
    EXPORT TIM_SetAutoReloadValue
    EXPORT TIM_GetCounterValue
    EXPORT TIM_GetPrescalerValue
    EXPORT TIM_GetAutoReloadValue
    EXPORT TIM_CalculateTimerValues
    EXPORT TIM_SetUpdatePeriod_us
    EXPORT TIM_IsUIF
    EXPORT TIM_ClearUIF
    EXPORT TIM_GenerateRegisterUpdate
    EXPORT TIM_SetOCMode_PWM1    
    EXPORT TIM_SetOCMode_PWM2
    EXPORT TIM_SetCCR
    EXPORT TIM_GetCCR
    EXPORT TIM_EnableOCPreload
    EXPORT TIM_SetupPWM1
    EXPORT TIM_SetupPWM2
    EXPORT TIM_EnableChannel
    EXPORT TIM_UpdatePWMDuty
    EXPORT TIM_SetICCFallingEdge
    EXPORT TIM_SetICCRisingEdge
    EXPORT TIM_EnableCCInterrupt
    EXPORT TIM_SetModeInputDirect
    EXPORT TIM_IsCCFlag    
    EXPORT TIM_ClearCCFlag
    EXPORT TIM_GetTimerClockFreq


;======================================
TIM_Init PROC ; r0: TIM number r1: cfg addr

    push {r4-r6, lr}

    push {r0}
    mov r2, #TIM_INIT_SIZE
    lsl r2, #2 ; l2 = offfset (size*4) 
    mul r2, r0
    ldr r4, =TIMERS
    ldr r0, [r4, r2]
    str r0, [r1, #0] ; timer addr base
    add r2, #4
    ldr r5, [r4, r2]; isAPB1
    str r5, [r1, #4]; isAPB1

    add r2, #4
    ldr r0, [r4, r2]
    str r0, [r1, #8]; irq
    
    add r2, #4
    ldr r3, [r4, r2] ;port enable bitmask
    str r3, [r1, #12];port enable bitmask

    add r2, #4
    ldr r0, [r4, r2]
    str r0, [r1, #16]; isAdvanced
    
    pop {r0}
    str r0, [r1, #20]; TIM no
    

    mov r0, #TRUE ; enable
    cmp r5, #1
    ITTEE EQ
    ldreq r1, =RCC_APB1ENR
    ldreq r2, =RCC_APB1RSTR
    ldrne r1, =RCC_APB2ENR
    ldrne r2, =RCC_APB2RSTR
    ;r3 =bitmask
    bl RCC_EnablePeripheral

    pop {r4-r6, lr}
    bx lr

    ENDP

;======================================
TIM_EnableCounter  PROC
    
    ldr r2, [r0]
    add r2, #TIM_CR1_OFFSET
    ldr r3, [r2]
    
    cmp r1, #TRUE 
    ITE EQ 
    orreq r3, #TIM_CR1_CEN
    bicne r3, #TIM_CR1_CEN
    str r3, [r2]

    bx lr
    
    ENDP

;======================================
TIM_EnableUpdateEvent  PROC

    ldr r2, [r0]
    add r2, #TIM_CR1_OFFSET
    ldr r3, [r2]

    cmp r1, #TRUE 
    ITE EQ 
    biceq r3, #TIM_CR1_UDIS
    orrne r3, #TIM_CR1_UDIS
    str r3, [r2]
    
    bx lr

    ENDP    

;======================================
TIM_EnableAutoPreload  PROC

    ldr r2, [r0]
    add r2, #TIM_CR1_OFFSET
    ldr r3, [r2]

    cmp r1, #TRUE 
    ITE EQ 
    orreq r3, #TIM_CR1_ARPE
    bicne r3, #TIM_CR1_ARPE
    str r3, [r2]

    bx lr

    ENDP    

;======================================
TIM_EnableUpdateInterrupt  PROC
    ldr r2, [r0]
    add r2, #TIM_DIER_OFFSET
    ldr r3, [r2]

    cmp r1, #TRUE 
    ITE EQ 
    orreq r3, #TIM_DIER_UIE
    bicne r3, #TIM_DIER_UIE
    str r3, [r2]

    bx lr

    ENDP

;=================================
;r0: cfg, r1: ch, r2: bool
TIM_EnableCCInterrupt  PROC 
    push {r4, r5}

    ldr r3, [r0]
    add r3, #TIM_DIER_OFFSET

    mov r5, #TIM_DIER_CC1IE
    sub r1, #1
    lsl r5, r1
    REG_SETCLR r5, r2, r3, r4 
    pop {r4, r5}

    bx lr
    ENDP


;======================================
TIM_EnableNVICInterrupt  PROC
    
    push {lr}
    ldr r0, [r0, #8]  ;irq

    cmp r1, #TRUE 
    beq set_en
    bl NVIC_ClearEnable
    b end_nvic
set_en
    bl NVIC_SetEnable
end_nvic

        pop {lr}
    bx lr

    ENDP
    
;======================================
TIM_EnableTriggerInterrupt  PROC
    push {r4}

    ldr r2, [r0]
    add r2, #TIM_DIER_OFFSET
    ldr r3, [r2]

    cmp r1, #TRUE 
    ITE EQ 
    orreq r3, #TIM_DIER_TIE
    bicne r3, #TIM_DIER_TIE
    str r3, [r2]

    pop {r4}
    bx lr

    ENDP

;======================================
TIM_SetCounterValue PROC

    ldr r2, [r0]
    add r2, #TIM_CNT_OFFSET
    str r1, [r2]

    bx lr

    ENDP

;======================================
TIM_SetAutoReloadValue PROC

    ldr r2, [r0]
    add r2, #TIM_ARR_OFFSET
    str r1, [r2]

    bx lr

    ENDP

;======================================
TIM_SetPrescalerValue PROC

    ldr r2, [r0]
    add r2, #TIM_PSC_OFFSET
    str r1, [r2]

    bx lr

    ENDP

;======================================
TIM_GetCounterValue PROC

    ldr r2, [r0]
    add r2, #TIM_CNT_OFFSET
    ldr r0, [r2]

    bx lr

    ENDP

;======================================
TIM_GetPrescalerValue PROC

    ldr r2, [r0]
    add r2, #TIM_PSC_OFFSET
    ldr r0, [r2]

    bx lr

    ENDP

;======================================
TIM_GetAutoReloadValue PROC

    ldr r2, [r0]
    add r2, #TIM_ARR_OFFSET
    ldr r0, [r2]

    bx lr

    ENDP

;======================================
;r0: cfg, r1: us, r2: *count, r3: *presc
TIM_CalculateTimerValues  PROC
    push {r4, r5, lr}

    ldr r0, [r0, #4]  ;isAPB1
    push {r1, r2}
    bl RCC_GetTIMx_CLK
    pop {r1, r2}
    ;r0: tim clk hz
    ; r0 * r1 => r5/r4
    umull r4, r5, r0, r1
    mov r0, r4; lo
    mov r1, r5; hi
    ;r0 lo
    ;r1 hi
  
    ;count64 
    push {r2, r3}
    ldr r2, =1000000
    bl _udiv64 
    pop {r2, r3}

    cmp r0, #0
    IT EQ 
    moveq r0, #1

    ; r0=count64

    ; r5=presc
    mov r5, #1

    ldr r1, =0xffff

calc_loop

    cmp r0, r1
    blt calc_end
    lsr r0, #1
    lsl r5, #1
    b calc_loop

calc_end

    sub r5, #1
    str r0, [r2] ;*count
    str r5, [r3] ;*presc

    pop {r4, r5, lr}
    bx lr

    ENDP

;======================================
TIM_SetUpdatePeriod_us  PROC
        push {lr}
        
        push {r2}
        push {r3}
        mov r3, sp
        mov r2, sp
        add r2, #4
        
        push {r0}
        
        bl TIM_CalculateTimerValues
        
        pop {r0}
        
        ldr r1, [r2]; count
        bl TIM_SetAutoReloadValue
        
        ldr r1, [r3]; presc
        bl TIM_SetPrescalerValue        
        
        pop {r3}
        pop {r2}
        pop {lr}
        bx lr
        ENDP

;======================================
TIM_IsUIF PROC

    ldr r3, [r0]
    add r3, #TIM_SR_OFFSET
    
    REG_ISSET #TIM_SR_UIF, r0, r3, r3
    bx lr

    ENDP

;======================================
TIM_ClearUIF PROC 
    
    ldr r3, [r0]
    add r3, #TIM_SR_OFFSET
    ldr r2, [r3]
    bic r2, #TIM_SR_UIF
    str r2, [r3]

    bx lr
    ENDP
        
TIM_GenerateRegisterUpdate PROC

    ldr r2, [r0]
    add r2, #TIM_EGR_OFFSET
    REG_SETCLR #TIM_EGR_UG, r0, r2, r3
    bx lr
    ENDP


; channel operations  

    MACRO
    CALC_CCMR $r
    ldr $r, [r0]
    cmp r1, #3
    IT LT
    addlt $r,#TIM_CCMR1_OFFSET
    addge $r,#TIM_CCMR2_OFFSET
    
    sub r1, #1 
    and r1, #1 ; %2 
    lsl r1, #3 ; *8 
        
    MEND

    MACRO
    CALC_CCER $r
    ldr $r, [r0]
    add $r, #TIM_CCER_OFFSET
    sub r1, #1  ; (ch-1)*4 
    lsl r1, #2 

    MEND


;======================================
TIM_Set_CCMR PROC 

    push {r4, r5}
    ;r1: channel

    CALC_CCMR r4
    ;mov r3, $cmask ; shift this left
    lsl r3, r1
    REG_CLR r3,  r4, r5
    
    ;mov r2, $smask 
    lsl r2, r1
    REG_SET r2,  r4, r5
    pop {r4, r5}
    
    bx lr 
    ENDP
 
 

;======================================
;r0: cfg  r1: ch 1-4
TIM_SetOCMode_PWM1  PROC
    push {lr, r4, r5}
    mov r2, #TIM_CCMR1_OC1M_PWM1 
    mov r3, #TIM_CCMR1_OC1M_CLEARMASK
    bl TIM_Set_CCMR 
    pop {lr, r4, r5}

    bx lr
    ENDP

;======================================
;r0: cfg  r1: ch 1-4
TIM_SetOCMode_PWM2  PROC
    push {lr, r4, r5}
    mov r2, #TIM_CCMR1_OC1M_PWM2 
    mov r3, #TIM_CCMR1_OC1M_CLEARMASK
    bl TIM_Set_CCMR 
    pop {lr, r4, r5}
    
    bx lr
    ENDP

;========================================
;r0: cfg  r1: ch 1-4
TIM_SetModeInputDirect PROC
    push {lr, r4, r5}
    mov r2, #TIM_CCMR1_CC1S_IN_TI1 
    mov r3, #TIM_CCMR1_CC1S_CLEARMASK
    bl TIM_Set_CCMR 
    pop {lr, r4, r5}
    
    bx lr
    ENDP

        
;======================================
;r0: cfg  r1: ch 1-4, r2: ccr val       
TIM_SetCCR PROC
    ldr r3, [r0]
    add r3, #TIM_CCR1_OFFSET
    sub r1, #1
    add r3, r1, lsl #2
    strh r2, [r3]
    bx lr
    ENDP

;=====================================
TIM_GetCCR PROC 
    ldr r3, [r0]
    add r3, #TIM_CCR1_OFFSET
    sub r1, #1
    add r3, r1, lsl #2
    ldrh r0, [r3]
    bx lr
    ENDP

;======================================
; r0: cfg, r1: ch, r2: true/false
TIM_EnableOCPreload  PROC

    push {r4, r5}
    CALC_CCMR r4 ; r4: ccmr addr 
    mov r3, #TIM_CCMR1_OC1PE
    lsl r3, r1
    REG_SETCLR r3, r2, r4, r5
    pop {r4, r5}
    bx lr
    ALIGN
    LTORG
    ENDP

;======================================
; r0: cfg, r1: ch, r2: period us, r3: duty_us
TIM_SetupPWM PROC 
    push {lr, r4 - r9}
    
    mov r4, r0; cfg
    mov r5, r1; ch 
    mov r6, r2; period
    mov r8, r3; duty
    ldr r9, [sp, #28]
    
    push {r2}
    push {r3}
    mov r3, sp ;presc addr
    mov r2, sp ;count addr
    add r2, #4
    
    mov r1, r6    
    bl TIM_CalculateTimerValues
    ldr r1, [r2]; count
    push {r1}
    mov r0, r4
    bl TIM_SetAutoReloadValue
        
    ldr r1, [r3]; presc
    bl TIM_SetPrescalerValue        
        
    pop {r1}

    pop {r3}
    pop {r2}
    
    
    ;cnt*duty_us: r1 * r8  
    umull r2, r3, r1, r8
    ;r2 lo
    ;r3 high
  
    ;cnt*duty_us/period_us(r6)
    ;UDIV64 r2, r3, r6; (r3r2 / r6 => r2)
    
    mov r0, r2 
    mov r1, r3
    mov r2, r6
    bl _udiv64
    mov r2, r0
    
    mov r0, r4
    mov r1, r5
    bl TIM_SetCCR 
    
    mov r1, r5
    cmp r9, #TRUE; isPWM1
    beq pwm1
    bl TIM_SetOCMode_PWM2
    b pwm_done
pwm1 bl TIM_SetOCMode_PWM1

pwm_done
    ldr r1, [r0, #16]; isAdvanced
    cmp r1, #TRUE
    bne adv_done
    ldr r1, [r0]
    add r1, #TIM_BDTR_OFFSET
    REG_SET #TIM_BDTR_MOE, r1, r2

adv_done
    mov r0, r4
    mov r1, r5
    mov r2, #TRUE
    bl TIM_EnableOCPreload
    mov r1, r5
    mov r2, #TRUE
    bl TIM_EnableAutoPreload
    mov r1, r5
    mov r2, #TRUE
    bl TIM_GenerateRegisterUpdate

    pop {lr, r4 - r9}
    bx lr
    ENDP

;======================================
TIM_SetupPWM1 PROC 
    push {lr, r4}
    mov r4, #TRUE
    push {r4}
    bl TIM_SetupPWM
    pop {r4}
    pop {lr, r4}
    bx lr
    ENDP

;======================================
TIM_SetupPWM2 PROC 
    push {lr, r4}
    mov r4, #FALSE
    push {r4}
    bl TIM_SetupPWM
    pop {r4}
    pop {lr, r4}
    bx lr
    ENDP

;======================================
;r0: cfg, r1: ch, r2: bool
TIM_EnableChannel PROC
    push {r4, r5}
    CALC_CCER r3
    mov r5, #TIM_CCER_CC1E
    lsl r5, r1
    REG_SETCLR r5, r2, r3, r4 
    pop {r4, r5}
    bx lr
    ENDP


; r0: cfg, r1: ch, r2: duty_ms
;=======================================
TIM_UpdatePWMDuty PROC

    push {r4-r6, lr}


    sub sp, #16
    str r0, [sp, #12]
    str r1, [sp, #8]
    str r2, [sp, #4]

    ldr r0, [sp, #12]
    ldr r0, [r0, #4]  ;isAPB1
    bl RCC_GetTIMx_CLK
    mov r3, r0

    ldr r0, [sp, #12]
    bl TIM_GetPrescalerValue
    mov r6, r0 
    add r6, #1;  presc


    ; duty_us * timclk
    ldr r2, [sp, #4]
    umull r4, r5, r2, r3
    ;r4 lo
    ;r5 high   
    
    mov r0, r4; lo 
    mov r1, r5; hi
    ldr r2, =1000000
    bl _udiv64

    udiv r2, r0, r6; /presc

    ldr r0, [sp, #12]
    ldr r1, [sp, #8]
    bl TIM_SetCCR 

    add sp, #16
    pop {r4-r6, lr}
    bx lr
    ENDP

;================================
TIM_SetICCFallingEdge  PROC
    push {r4, r5}
    CALC_CCER r4
    mov r3, #TIM_CCER_CC1P
    lsl r3, r1
    REG_SET r3, r4, r5 
    pop {r4, r5}
    bx lr
    ENDP
        
;=============================
TIM_SetICCRisingEdge PROC
    push {r4, r5}
    CALC_CCER r4
    mov r3, #TIM_CCER_CC1P
    lsl r3, r1
    REG_CLR r3, r4, r5 
    pop {r4, r5}
    bx lr
    ENDP

;=============================
;r0: cfg, r1: ch, r2: bool (clear if set)
TIM_IsCCFlag PROC
    push {r4}
    ldr r3, [r0]
    add r3, #TIM_SR_OFFSET

    mov r4, #TIM_SR_CC1IF
    sub r1, #1
    lsl r4, r1
    REG_ISSET r4, r0, r3, r1
    cmp r2, #FALSE
    beq end_iscc
    
    REG_CLR r4, r3, r1 
    
    
end_iscc    
    pop {r4}
    bx lr
    ENDP

;r0: cfg, r1: ch
TIM_ClearCCFlag PROC
    ldr r3, [r0]
    add r3, #TIM_SR_OFFSET

    mov r2, #TIM_SR_CC1IF
    sub r1, #1
    lsl r2, r1
    ;
    mov r1, #0
    str r1, [r3]
    ; 

    REG_CLR r2, r3, r1 
    bx lr
    ENDP
        
       
TIM_GetTimerClockFreq  PROC

    push {lr}
    ldr r0, [r0, #4]  ;isAPB1
    bl RCC_GetTIMx_CLK
    pop {lr}
    bx lr
    ENDP

    EXPORT TIM3_IRQHandler
TIM3_IRQHandler  PROC
    push {lr}
    
    mov r2, #TIM_INIT_SIZE
    lsl r2, #2 ; l2 = offfset (size*4) 
    mov r0, #TIM3
    mul r2, r0
    ldr r0, =TIMERS
    add r0, r2

    push {r0}
    mov r1, #1
    mov r2, #TRUE; check and clear if set
    bl TIM_IsCCFlag
    
    pop {r0}
    
    ldr r0, [r0, #20] 
    cmp r0, #0 
    beq t3_done
    blx r0
t3_done    
    pop {lr}
    bx lr
    ENDP

    EXPORT TIM_SetHandler
;r0: cfg, r1: handler addr 
TIM_SetHandler PROC 
    ldr r0, [r0, #20]
    mov r2, #TIM_INIT_SIZE
    lsl r2, #2 ; l2 = offfset (size*4)
    mul r2, r0;  r0: TIM no
    ldr r0, =TIMERS
    add r0, r2
    str r1, [r0, #20]


    bx lr
    ENDP

    ALIGN
        
    END    
        
        
                