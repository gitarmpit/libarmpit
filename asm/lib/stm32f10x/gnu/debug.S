.thumb

.syntax unified


.data

#include "debug.h"

g_ahb_clock_mhz:
.word 0

.equ g_fudge, 46

.text
    
@    IMPORT SCB_EnableDWT
@    IMPORT RCC_GetAHB_CLK

.globl Debug_EnableCYCCNT  @r0 true/false
.globl Debug_Delay_cycles  @cnt
.globl Debug_Delay_us      @us


Debug_EnableCYCCNT: 

    push {r4-r5, lr}

    ldr r4, =DWT_CTRL
    ldr r5, [r4]
    ands r5, #DWT_CTRL_NOCYCCNT
    bne en_end
    
    ldr r5, [r4]
    cmp r0, #0
    ITTT EQ
    biceq r5, #DWT_CTRL_CYCCNTENA
    streq r5, [r4]
    beq en_end

    mov r0, #1
    bl SCB_EnableDWT
    ldr r5, [r4]
    orr r5, #DWT_CTRL_CYCCNTENA
    str r5, [r4]
    bl RCC_GetAHB_CLK
    ldr r1, =1000000
    udiv r0, r1
    ldr r1, =g_ahb_clock_mhz
    str r0, [r1]
    

en_end:
    pop {r4-r5, lr}
    bx lr


##################################################################3
Debug_Delay_cycles: 
    push {lr}

    ldr r1, =DWT_CYCCNT
    mov r2, #0 
    str r2, [r1]
    
loop_ddc: 
    ldr r2, [r1]
    cmp r0, r2 
    bgt loop_ddc

    pop {lr}
    bx lr


##################################################################3
Debug_Delay_us: 
    push {lr}

    ldr r3, =DWT_CYCCNT
    mov r2, #0 
    str r2, [r3]
    ldr r1, =g_ahb_clock_mhz
    ldr r1, [r1]
    mul r0, r1
    sub r0, #g_fudge 
loop_ddus: 
    ldr r2, [r3]
    cmp r0, r2 
    bgt loop_ddus

    pop {lr}
    bx lr
