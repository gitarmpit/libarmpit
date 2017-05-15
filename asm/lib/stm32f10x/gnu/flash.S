.thumb

.syntax unified


.data

#include "flash.h"

.text

#include "macros.S"

.globl FLASH_SetWaitState 
.globl FLASH_GetWaitState
.globl FLASH_EnablePrefetchBuffer
.globl FLASH_Unlock
.globl FLASH_Lock
.globl FLASH_IsLocked
.globl FLASH_IsBusy
.globl FLASH_GetStatus
.globl FLASH_ErasePage
.globl FLASH_MassErase
.globl FLASH_Write @ addr data

FLASH_SetWaitState: 

    push {r4, r5, lr}
    REG_CLR FLASH_ACR, FLASH_ACR_LATENCY_MASK
    lsl r0, #FLASH_ACR_LATENCY_OFFSET
    REG_SET FLASH_ACR, r0
    pop {r4, r5, lr}
    bx lr
                


FLASH_GetWaitState: 

    push {lr}
    ldr r0, =FLASH_ACR
    ldr r0, [r0]
    and r0, #0x7
    pop {lr}
    bx lr
    


FLASH_EnablePrefetchBuffer:
    push {r4, r5, lr}
    REG_SET FLASH_ACR, FLASH_ACR_PRFTEN
    pop {r4, r5, lr}
    bx lr
                


FLASH_Unlock:
    push {r4, r5, lr}

    ldr r4, =FLASH_KEYR
    
    ldr r5, =FLASH_KEY1
    str r5, [r4]
    ldr r5, =FLASH_KEY2
    str r5, [r4]

    pop {r4, r5, lr}
    bx lr
    


FLASH_Lock:
    push {r4, r5, lr}
    REG_SET FLASH_CR, FLASH_CR_LOCK
    pop {r4, r5, lr}
    bx lr
                

    
FLASH_IsLocked: 
    push {r4, lr}
    ldr r4, =FLASH_CR
    ldr r4, [r4]
    ands r4, #FLASH_CR_LOCK
    ITE EQ 
    moveq r0, #1 
    movne r0, #0
    pop {r4, lr}
    bx lr
                


FLASH_IsBusy: 
    push {r4, lr}
    ldr r4, =FLASH_SR
    ldr r4, [r4]
    ands r4, #FLASH_SR_BSY
    ITE EQ 
    moveq r0, #0 
    movne r0, #1
    pop {r4, lr}
    bx lr
                


FLASH_GetStatus: 
    push {r4, r5, lr}
    mov r0, #0
    ldr r4, =FLASH_SR
    ldr r4, [r4]


    ands r5, r4, #FLASH_SR_WRPERR
    ITT GT 
    movgt r0, r5 
    bgt end_stat

    ands r5, r4, #FLASH_SR_PGPERR
    ITT GT 
    movgt r0, r5 
    bgt end_stat


end_stat:
    pop {r4, r5, lr}
    bx lr



FLASH_WaitNotBusy: 
    push {lr}
wait_not_busy:
    bl FLASH_IsBusy
    cmp r0, #1 
    beq wait_not_busy
    pop {lr}
    bx lr


FLASH_ErasePage: 
    
    push {r4 - r6, lr}

    mov r6, r0

    bl FLASH_WaitNotBusy

    REG_SET FLASH_CR, FLASH_CR_PER

    ldr r4, =FLASH_AR
    lsl r6, #10
    add r6, #FLASH_BASE
    str r6, [r4]

    REG_SET FLASH_CR, FLASH_CR_STRT
    
    bl FLASH_WaitNotBusy

    bl FLASH_GetStatus
    REG_CLR FLASH_CR, FLASH_CR_PER
    REG_SET FLASH_SR, FLASH_SR_EOP

    pop {r4 - r6, lr}
    bx lr


FLASH_MassErase : 
    push {r4 - r5, lr}

    bl FLASH_WaitNotBusy

    REG_SET FLASH_CR, FLASH_CR_MER

    REG_SET FLASH_CR, FLASH_CR_STRT
    
    bl FLASH_WaitNotBusy
    bl FLASH_GetStatus

    pop {r4 - r5, lr}
    bx lr

FLASH_Write: 
    push {r4 - r5, lr}

    push {r0}
    bl FLASH_WaitNotBusy
    pop {r0}

    REG_SET FLASH_CR, FLASH_CR_PG

    ldr r4, =0xffff
    and r1, r4
    strh r1, [r0]
    
    bl FLASH_WaitNotBusy
    bl FLASH_GetStatus

    REG_CLR FLASH_CR, FLASH_CR_PG
    REG_SET FLASH_SR, FLASH_SR_EOP

    pop {r4 - r5, lr}
    bx lr
