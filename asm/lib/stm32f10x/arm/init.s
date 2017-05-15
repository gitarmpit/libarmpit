    PRESERVE8
    THUMB


    IMPORT |Image$$RO$$Base|
    IMPORT |Image$$RO$$Limit|
    IMPORT |Image$$ZI$$Base|        
    IMPORT |Image$$ZI$$Limit|        
    IMPORT |Image$$RW$$Base|        
    IMPORT |Image$$RW$$Limit|        

    AREA INIT, CODE, READONLY

    EXPORT SystemInit 
SystemInit PROC 

    ;push {lr}

    ;Copy the data segment initializers from flash to SRAM  
    ldr r0, =|Image$$RO$$Limit| 
    ldr r1, =|Image$$RW$$Base|
    ldr r2, =|Image$$RW$$Limit|
    sub r3, r2, r1
    lsr r3, #2
loop_data
    cmp r3, #0
    beq end_data
    ldr r2, [r0], #4
    str r2, [r1], #4
    subs r3, #1
    b loop_data

end_data
; Zero bss 
    ldr r0, =|Image$$ZI$$Base| 
    ldr r1, =|Image$$ZI$$Limit| 
    sub r2, r1, r0
    lsr r2, #2
loop_bss
    cmp r2, #0
    beq end_bss
    mov r1, #0
    str r1, [r0], #4
    subs r2, #1
    b loop_bss

end_bss

    ;pop {lr}
    bx lr
    ENDP


    END