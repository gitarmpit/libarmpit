    INCLUDE common.s
    INCLUDE imp.s
    INCLUDE timer_regs.s

    AREA DATA, DATA, READWRITE
;capt_pin   DCD 0 
tim_ch     DCD 0 
last_cnt   DCD 0 
clk_freq   DCD 0 
presc      DCD 0 
ar         DCD 0 
missed_cnt DCD 0 
period_tick  DCD 0 
duty_tick    DCD 0 
falling_edge DCB 1    

TIM_CFG  SPACE TIM_INIT_SIZE*4

    AREA    CODE, CODE, READONLY

    IMPORT TIM_SetHandler


    ;EXPORT ar 
    ;EXPORT presc
    ;EXPORT clk_freq 
    ;EXPORT tim_ch 
    ;EXPORT TIMx_IRQHandler
    ;EXPORT TIM_CFG
    EXPORT CaptureGetPeriod
    EXPORT CaptureGetDuty
    EXPORT Capture_Cfg

    ;EXPORT period_tick
    ;EXPORT duty_tick

;===============================================
TIMx_IRQHandler  PROC
    push {r4-r5, lr}
    ;ldr r0, =TIM_CFG
    ;ldr r1, =tim_ch
    ;ldr r1,[r1]
    ;mov r2, #TRUE; check and clear if set
    ;bl TIM_IsCCFlag
    ;cmp r0, #FALSE
    ;beq end_capt
    
    ldr r0, =TIM_CFG
    ldr r1, =tim_ch
    ldr r1, [r1]
    bl TIM_GetCCR
    mov r4, r0; cnt
    
    ldr r0, =TIM_CFG
    ldr r1, =tim_ch
    ldr r1, [r1]
    ldr r2, =falling_edge
    ldrb r5, [r2]
    cmp r5, #TRUE; isFalling 
    beq ris
    bl TIM_SetICCFallingEdge
    b done_edge
ris
    bl TIM_SetICCRisingEdge
done_edge
    ldr r2, =falling_edge
    eor r5, #1 
    strb r5, [r2]

    ldr r2, =missed_cnt
    mov r1, #0
    str r1, [r2]
    
    ldr r2, =last_cnt
    ldr r2, [r2]
    cmp r2, #0 
    beq end_capt
    
    ; smth captured 
    sub r0, r4, r2; r0: tick = cnt - last 
    cmp r0, #0
    ITTTT LT
    ldrlt r2, =ar
    ldrlt r2, [r2]
    addlt r2, #1 
    addlt r0, r2

    cmp r5, #TRUE
    beq is_set
    ldr r1, =duty_tick
    str r0, [r1]; duty = tick_us
    b end_capt
    
is_set 
    ldr r1, =duty_tick
    ldr r1, [r1]
    add r0, r1 ; tick_us + duty_us 
    ldr r1, =period_tick 
    str r0, [r1]
    
    ; end capture calc

end_capt

    ldr r2, =last_cnt
    str r4, [r2]    

    pop {r4-r5, lr}
    bx lr
    ENDP


;========================
;r0: cfg r1: ch
Capture_Cfg PROC
    push {r4, r5, lr}
    
    sub sp, #8 
    str r0, [sp, #4]
    str r1, [sp]
    ldr r4, =tim_ch
    str r1, [r4]


    ldr r4, =TIM_CFG
    mov r1, #TIM_INIT_SIZE
cpy
    ldr r2, [r0], #4
    str r2, [r4], #4
    subs r1, #1 
    bne cpy
    
    
    ldr r0, [sp, #4]
    ldr r1, [sp]
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
    
    ;ldr r4, =falling_edge
    ;mov r5, #1 
    ;str r5, [r4]

    
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

    ldr r0, [sp, #4]
    ldr r1, =TIMx_IRQHandler
    bl TIM_SetHandler


    add sp, #8
    pop {r4, r5, lr}
    bx lr
    ENDP


;=================================
CaptureCalc PROC
    push {lr}
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
    
    pop {lr}
    bx lr
    ENDP

;==================================
CaptureGetPeriod PROC
    push {lr}
    ldr r0, =period_tick
    ldr r0, [r0]
    bl CaptureCalc
    pop {lr}
    bx lr
    ENDP
        
;==================================        
CaptureGetDuty PROC
    push {lr}
    ldr r0, =duty_tick
    ldr r0, [r0]
    bl CaptureCalc
    pop {lr}
    bx lr
    ENDP

    ALIGN 
    END
