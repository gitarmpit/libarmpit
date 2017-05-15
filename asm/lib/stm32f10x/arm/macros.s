;    AREA    MACROS, CODE, READONLY

   INCLUDE common.s 
       
   IF :LNOT: :DEF: _MACROS
        GBLS _MACROS
            
   ; result in $nl
    MACRO 
    UDIV64 $nl, $nh, $d
    adds      $nl,$nl,$nl
    adc       $nh,$nh,$nh
    GBLA i

    while i < 31
        cmp     $nh,$d  
        subcs   $nh,$nh,$d  
        adcs    $nl,$nl,$nl
        adc     $nh,$nh,$nh
i   SETA i+1
    
    WEND

    cmp     $nh,$d
    subcs   $nh,$nh,$d
    adcs    $nl,$nl,$nl

    MEND


    MACRO 
    
    REG_SET $val, $r1, $r2 
    ;ldr $r1, $addr
    ldr $r2, [$r1]
    orr $r2, $val 
    str $r2, [$r1]
    
    MEND

    MACRO 
    
    REG_CLR $val, $r1, $r2 
    ;ldr $r1, $addr
    ldr $r2, [$r1]
    bic $r2, $val 
    str $r2, [$r1]
    
    MEND

    MACRO 
    
    REG_SETCLR $val, $r0, $r1, $r2

    ;ldr $r1, $addr
    ldr $r2, [$r1]
    cmp $r0, #1
    ITE EQ
    orreq $r2, $val 
    bicne $r2, $val 
    str $r2, [$r1]
    
    MEND

    MACRO
    REG_ISSET $mask, $r0, $r1, $r2

    ldr $r2, [$r1]
    ands $r2, $mask
    ITE EQ 
    moveq $r0, #FALSE 
    movne $r0, #TRUE
    
    MEND


    ENDIF
        
    END