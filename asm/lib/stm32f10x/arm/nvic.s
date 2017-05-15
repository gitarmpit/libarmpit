    PRESERVE8
    THUMB

    INCLUDE common.s
    INCLUDE irq.s

    AREA    DR, DATA, READONLY

NVIC_ISER   EQU 0xE000E100
NVIC_ICER   EQU 0xE000E180
NVIC_ISPR   EQU 0xE000E200
NVIC_ICPR   EQU 0xE000E280
NVIC_IABR   EQU 0xE000E300
NVIC_IPR    EQU 0xE000E400


    AREA    CODE, CODE, READONLY

    EXPORT NVIC_SetEnable
    EXPORT NVIC_ClearEnable
    EXPORT NVIC_SetPending
    EXPORT NVIC_ClearPending    
    EXPORT NVIC_IsActive
    EXPORT NVIC_SetPriority
    EXPORT NVIC_GetPriority

    MACRO 
    ; r0: irq no  r1: addr
    SET_NVIC $addr 
    ldr r1, =$addr
    
    lsr r2,r0, #5
    lsl r2, #2
    add r1, r2
    and r0, #31
    mov r3, #1
    lsl r3, r0
    ldr r2, [r1]
    orr r2, r3
    str r2, [r1]
    
    MEND



NVIC_SetEnable PROC 
    SET_NVIC NVIC_ISER
    bx lr
    
    ENDP
        
NVIC_ClearEnable PROC 
    SET_NVIC NVIC_ICER
    bx lr
    
    ENDP
        
NVIC_SetPending PROC 
    SET_NVIC NVIC_ISPR
    bx lr
    
    ENDP
        
NVIC_ClearPending PROC 
    SET_NVIC NVIC_ICPR
    bx lr
    
    ENDP

NVIC_IsActive PROC 
    ldr r1, =NVIC_ISER
    lsr r2,r0, #5
    lsl r2, #2
    add r1, r2

    and r0, #31
    mov r3, #1
    lsl r3, r0
    
    ldr r2, [r1]
    ands r2, r3
    ITE EQ
    moveq r0, #FALSE
    movne r0, #TRUE
    bx lr
    
    ENDP

NVIC_SetPriority PROC 
    ldr r2, =NVIC_IPR
    lsr r3,r0, #2
    lsl r3, #2
    add r2, r3
 
    and r0, #3
    lsl r0, #3
    add r0, #4
    lsl r1, r0
    
    ldr r3, [r2]
    orr r3, r1
    str r3, [r2]
    bx lr
    
    ENDP

NVIC_GetPriority PROC 
    ldr r2, =NVIC_IPR
    lsr r3,r0, #2
    lsl r3, #2
    add r2, r3
 
    and r0, #3
    lsl r0, #3
    add r0, #4
    
    ldr r3, [r2]
    lsr r0, r3, r0
    and r0, #0xf
    bx lr
    
    ENDP


    ALIGN

    END


