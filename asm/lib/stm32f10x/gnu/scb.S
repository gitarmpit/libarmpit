.thumb
.syntax unified

.data 

#include "scb.h"
#include "macros.S"

.text
   
.globl SCB_EnableDWT      @r0 true/false
.globl FPU_Enable         @r0 true/false
.globl SCB_SetSleepDeep   @r0 true/false   
.globl SCB_SetSleepOnExit @r0 true/false
.globl SCB_SystemReset
        
SCB_EnableDWT:
    
    push {r4, r5, lr}

    REG_SETCLR SCB_DEMCR, SCB_DEMCR_TRCENA
    
    pop {r4, r5, lr}
    
    bx lr


        
FPU_Enable:
    
    push {r4, r5, lr}

    REG_CLR CPACR, CPACR_CP10_CLEARMASK

    cmp r0, #1 
    ITTEE EQ
    moveq r1, #CPACR_CP10_FULL_ACCESS
    orreq r1, #CPACR_CP11_FULL_ACCESS
    movne r1, #CPACR_CP10_ACCESS_DENIED
    orrne r1, #CPACR_CP11_ACCESS_DENIED

    REG_SET CPACR, r1
    pop {r4, r5, lr}
    
    bx lr


        
SCB_SetSleepDeep:      
    push {r4, r5, lr}

    REG_SETCLR SCB_SCR, SCB_SCR_SLEEPDEEP

    pop {r4, r5, lr}
    
    bx lr


        
SCB_SetSleepOnExit:      
    push {r4, r5, lr}

    REG_SETCLR SCB_SCR, SCB_SCR_SLEEPONEXIT

    pop {r4, r5, lr}
    
    bx lr


        
SCB_SystemReset:     
    push {r4, r5, lr}

    ldr r4,=SCB_AIRCR
    ldr r5, [r4]
    ldr r3, =0xffff0000
    bic r5, r3
    ldr r3, =0x5FA
    lsl r3, #16
    orr r5, r3
    orr r5, #SCB_AIRCR_SYSRESETREQ    
    str r5, [r4]

    pop {r4, r5, lr}
    
    bx lr
   
