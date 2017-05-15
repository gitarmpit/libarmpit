.thumb
.syntax unified

.data 

#include "systick.h"
#include "macros.S"

.text

.globl SystickEnable          @count, enable_int, isDIV8
.globl SystickSetReloadValue  @count
.globl SystickDisable    


#########################################################        
SystickEnable: 
    
    push {r4-r5, lr}
    ldr r4, =SYST_LOAD
    sub r0, #1 @r0: tick count
    ldr r3, =0xffffff
    and r0, r3
    str r0, [r4]
    
    mov r0, r1 @enable/disable int
    REG_SETCLR SYST_CSR, SYST_CSR_TICKINT

    REG_CLR SYST_CSR, SYST_CSR_CLKSOURCE_CLEARMASK
    
#ifdef STM32        
    ldr r4, =SYST_CSR
    ldr r5, [r4]
    cmp r2, #1 @r2: AHB(0) or AHB_DIV8 (1)
    ITE EQ 
    orreq r5, #SYST_CSR_CLKSOURCE_AHB_DIV8
    orrne r5, #SYST_CSR_CLKSOURCE_AHB
    
#else
        
    orr r5, #SYST_CSR_CLKSOURCE_CORE
    
#endif    

    str r5, [r4]

    REG_SET SYST_CSR, SYST_CSR_ENABLE
    

    pop {r4-r5, lr}
    bx lr
        
#########################################################        
SystickDisable:

    push {r4-r5, lr}
    REG_CLR SYST_CSR, SYST_CSR_ENABLE
    pop {r4-r5, lr}
    bx lr

#########################################################        
SystickSetReloadValue:
    push {lr}
    
    ldr r1, =SYST_LOAD
    str r0, [r1]
    ldr r1, =SYST_VAL 
    mov r0, #0 
    str r0, [r1]
    
    pop {lr}
