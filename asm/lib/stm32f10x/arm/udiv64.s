    AREA CODE, CODE, READONLY

    EXPORT _udiv64 
        
;========================================
_udiv64 PROC   ;r0: n.lo r1: n.hi r2: d
    adds      r0,r0,r0
    adc       r1,r1,r1

    mov r3, #31
ul 
    cmp     r1,r2  
    subcs   r1,r1,r2  
    adcs    r0,r0,r0
    adc     r1,r1,r1
    subs r3, #1 
    bne ul
 
    cmp     r1,r2
    subcs   r1,r1,r2
    adcs    r0,r0,r0

    bx lr
    ENDP
        
    END