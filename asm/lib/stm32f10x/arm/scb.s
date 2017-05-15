    PRESERVE8
    THUMB


    INCLUDE scb_regs.s
    INCLUDE macros.s
        
    AREA CODE, CODE, READONLY
        
   
    EXPORT SCB_EnableDWT    ;r0 true/false
    EXPORT FPU_Enable       ;r0 true/false
    EXPORT SCB_SetSleepDeep ;r0 true/false   
    EXPORT SCB_SetSleepOnExit ;r0 true/false
    EXPORT SCB_SystemReset
        
SCB_EnableDWT  PROC

    ldr r2, =SCB_DEMCR
    REG_SETCLR #SCB_DEMCR_TRCENA, r0, r2, r3
    bx lr

    ENDP
        
FPU_Enable  PROC
 
    ldr r2, =CPACR
    REG_CLR #CPACR_CP10_CLEARMASK, r2, r3

    cmp r0, #TRUE 
    ITTEE EQ
    moveq r1, #CPACR_CP10_FULL_ACCESS
    orreq r1, #CPACR_CP11_FULL_ACCESS
    movne r1, #CPACR_CP10_ACCESS_DENIED
    orrne r1, #CPACR_CP11_ACCESS_DENIED

    REG_SET r1, r2, r3
    
    bx lr

    ENDP
        
SCB_SetSleepDeep PROC      

    ldr r2, =SCB_SCR
    REG_SETCLR #SCB_SCR_SLEEPDEEP, r0, r2, r3
    bx lr

    ENDP
        
SCB_SetSleepOnExit PROC      

    ldr r2, =SCB_SCR
    REG_SETCLR #SCB_SCR_SLEEPONEXIT, r0, r2, r3

    bx lr

    ENDP
        
SCB_SystemReset PROC      
    push {r4, r5}

    ldr r4,=SCB_AIRCR
    ldr r5, [r4]
    ldr r3, =0xffff0000
    bic r5, r3
    mov r3, #0x5FA
    lsl r3, #16
    orr r5, r3
    orr r5, #SCB_AIRCR_SYSRESETREQ    
    str r5, [r4]
    
    pop {r4, r5}
    
    bx lr

    ENDP
        
    ALIGN    
        
    END    