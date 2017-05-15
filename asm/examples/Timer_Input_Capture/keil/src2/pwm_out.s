    INCLUDE imp.s
    INCLUDE timer_regs.s

    AREA    DATA, DATA, READWRITE 
        
TIM_PWM_CFG  SPACE TIM_INIT_SIZE*4


    AREA CODE, CODE, READONLY 
    EXPORT PWM_Out
PWM_Out  PROC 
    push {lr}
    mov r0, #TIM2
    ldr r1, =TIM_PWM_CFG
    bl TIM_Init 
    
    
    ldr r0, =TIM_PWM_CFG
    mov r1, #1; channel
    mov r2, #0x27; period
    mov r3, #0x5; duty 
    bl TIM_SetupPWM1

    ldr r0, =TIM_PWM_CFG
    mov r1, #1; channel
    mov r2, #TRUE
    bl TIM_EnableChannel


    ldr r0, =TIM_PWM_CFG
    mov r1, #TRUE
    bl TIM_EnableCounter
    pop {lr}
    bx lr
    ENDP


        
    END    
