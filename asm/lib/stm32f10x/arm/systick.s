    PRESERVE8
    THUMB
    
    GBLS STM32

SYST_CSR   EQU 0xE000E010
SYST_LOAD  EQU 0xE000E014
SYST_VAL   EQU 0xE000E018
SYST_CALIB EQU 0xE000E01C

SYST_CSR_ENABLE   EQU (1<<0) 
SYST_CSR_TICKINT  EQU (1<<1) 

    IF      :DEF:STM32 
SYST_CSR_CLKSOURCE_AHB_DIV8  EQU  (0<<2) 
SYST_CSR_CLKSOURCE_AHB       EQU  (1<<2) 
    ELSE
SYST_CSR_CLKSOURCE_CORE      EQU  (1<<2) 
    ENDIF

SYST_CSR_CLKSOURCE_CLEARMASK  EQU (1<<2) 
SYST_CSR_COUNTFLAG            EQU (1<<16) 
    
    INCLUDE macros.s
    
    AREA DR, DATA, READONLY 
        
    AREA CODE, CODE, READONLY
        
    EXPORT SystickEnable          ;count, enable_int, isDIV8
    EXPORT SystickSetReloadValue  ;count
    EXPORT SystickDisable    


;#########################################################        
SystickEnable   PROC 
    
    push {r4-r5}
    ldr r4, =SYST_LOAD
    sub r0, #1 ;r0: tick count 
    ldr r3, =0xffffff
    and r0, r3
    str r0, [r4]
    
    ldr r4, =SYST_CSR
    REG_SETCLR #SYST_CSR_TICKINT, r1, r4, r5

    REG_CLR #SYST_CSR_CLKSOURCE_CLEARMASK, r4, r5
    
    IF :DEF:STM32 
        
    ldr r4, =SYST_CSR
    ldr r5, [r4]
    cmp r2, #1 ;r2: AHB(0) or AHB_DIV8 (1)
    ITE EQ 
    orreq r5, #SYST_CSR_CLKSOURCE_AHB_DIV8
    orrne r5, #SYST_CSR_CLKSOURCE_AHB
    
    ELSE
        
    orr r5, #SYST_CSR_CLKSOURCE_CORE
    
    ENDIF    

    str r5, [r4]

    REG_SET #SYST_CSR_ENABLE, r4, r5
    

    pop {r4-r5}
    bx lr
    ENDP
        
;#########################################################        
SystickDisable  PROC

    ldr r2, =SYST_CSR
    REG_CLR #SYST_CSR_ENABLE, r2, r3
    bx lr
    ENDP
        


;#########################################################        
SystickSetReloadValue  PROC
    
    ldr r1, =SYST_LOAD
    str r0, [r1]
    ldr r1, =SYST_VAL 
    mov r0, #0 
    str r0, [r1]

    bx lr
    ENDP

    ALIGN
    END
