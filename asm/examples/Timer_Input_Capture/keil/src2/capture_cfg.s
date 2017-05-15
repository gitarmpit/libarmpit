    INCLUDE common.s
    INCLUDE imp.s
        

    AREA CODE, CODE, READONLY
        
    IMPORT ar
    IMPORT presc
    IMPORT clk_freq 
    IMPORT tim_ch
    
    EXPORT Capture_Cfg

;========================
;r0: cfg r1: ch
Capture_Cfg PROC
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


    add sp, #8
    pop {r4, r5, lr}
    bx lr
    ENDP

    ALIGN 
    END