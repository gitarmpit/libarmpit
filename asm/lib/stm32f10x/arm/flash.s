                PRESERVE8
                THUMB

                AREA    DR, DATA, READONLY

                INCLUDE macros.s
FLASH_BASE     EQU 0x08000000

FLASH_REG_BASE EQU 0x40022000

FLASH_ACR      EQU  FLASH_REG_BASE + 0x00   


FLASH_ACR_LATENCY_OFFSET EQU 0
FLASH_ACR_LATENCY_MASK EQU (0x7 << FLASH_ACR_LATENCY_OFFSET)
FLASH_ACR_HLFCYA EQU (1<<3)
FLASH_ACR_PRFTEN EQU (1<<4)
FLASH_ACR_PRFTBS EQU (1<<5)



FLASH_KEYR   EQU FLASH_REG_BASE + 0x04  

FLASH_KEY1 EQU 0x45670123
FLASH_KEY2 EQU 0xCDEF89AB


FLASH_OPTKEYR  EQU FLASH_REG_BASE + 0x08   
FLASH_OPTKEY1  EQU 0x08192A3B
FLASH_OPTKEY2  EQU 0x4C5D6E7F


FLASH_SR        EQU FLASH_REG_BASE + 0x0c   
FLASH_SR_BSY    EQU (1<<0)
FLASH_SR_PGPERR EQU (1<<2)
FLASH_SR_WRPERR EQU (1<<4)
FLASH_SR_EOP    EQU (1<<5)


FLASH_CR   EQU FLASH_REG_BASE + 0x10   

FLASH_CR_PG    EQU (1<<0)
FLASH_CR_PER   EQU (1<<1)
FLASH_CR_MER   EQU (1<<2)
FLASH_CR_OPTPG EQU (1<<4)
FLASH_CR_OPTER EQU (1<<5)
FLASH_CR_STRT  EQU (1<<6)
FLASH_CR_LOCK  EQU (1<<7)
FLASH_CR_OPTWRE EQU (1<<9)
FLASH_CR_ERRIE EQU (1<<10)
FLASH_CR_EOPIE EQU (1<<12)


FLASH_AR   EQU FLASH_REG_BASE + 0x14   
FLASH_OBR  EQU FLASH_REG_BASE + 0x1c   
FLASH_WRPR EQU FLASH_REG_BASE + 0x20   


           AREA    CODE, CODE, READONLY

           INCLUDE macros.s 
        
           EXPORT FLASH_SetWaitState 
           EXPORT FLASH_GetWaitState
           EXPORT FLASH_EnablePrefetchBuffer
           EXPORT FLASH_Unlock
           EXPORT FLASH_Lock
           EXPORT FLASH_IsLocked
           EXPORT FLASH_IsBusy
           EXPORT FLASH_GetStatus
           EXPORT FLASH_ErasePage
           EXPORT FLASH_MassErase
           EXPORT FLASH_Write ; addr data

FLASH_SetWaitState PROC 

    ldr r1, =FLASH_ACR
    REG_CLR #FLASH_ACR_LATENCY_MASK, r1, r2
    lsl r0, #FLASH_ACR_LATENCY_OFFSET
    REG_SET r0, r1, r2
    bx lr
                
    ENDP

FLASH_GetWaitState PROC 

    push {lr}
    ldr r0, =FLASH_ACR
    ldr r0, [r0]
    and r0, #0x7
    pop {lr}
    bx lr
    
    ENDP

FLASH_EnablePrefetchBuffer PROC
    ldr r1, =FLASH_ACR
    REG_SET #FLASH_ACR_PRFTEN, r1, r2
    bx lr
                
    ENDP

FLASH_Unlock PROC
    push {r4, r5, lr}

    ldr r4, =FLASH_KEYR
    
    ldr r5, =FLASH_KEY1
    str r5, [r4]
    ldr r5, =FLASH_KEY2
    str r5, [r4]

    pop {r4, r5, lr}
    bx lr
    
    ENDP

FLASH_Lock PROC
    ldr r1, =FLASH_CR
    REG_SET #FLASH_CR_LOCK, r1, r2
    bx lr
                
    ENDP
    
FLASH_IsLocked PROC 
    
    ldr r2, =FLASH_CR
    REG_ISSET  #FLASH_CR_LOCK, r0, r2, r2
    bx lr
                
    ENDP

FLASH_IsBusy PROC

    ldr r2, =FLASH_SR 
    REG_ISSET #FLASH_SR_BSY, r0, r2, r2
    bx lr
                
    ENDP

FLASH_GetStatus PROC 
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


end_stat
    pop {r4, r5, lr}
    bx lr

    ENDP

FLASH_WaitNotBusy PROC 
    push {lr}
wait_not_busy  
    bl FLASH_IsBusy
    cmp r0, #TRUE 
    beq wait_not_busy
    pop {lr}
    bx lr
    ENDP

FLASH_ErasePage PROC 
    
    push {r4 - r6, lr}

    mov r6, r0

    bl FLASH_WaitNotBusy

    ldr r4, =FLASH_CR
    REG_SET #FLASH_CR_PER, r4, r5

    ldr r4, =FLASH_AR
    lsl r6, #10
    add r6, #FLASH_BASE
    str r6, [r4]

    ldr r4, =FLASH_CR
    REG_SET #FLASH_CR_STRT, r4, r5
    
    bl FLASH_WaitNotBusy

    bl FLASH_GetStatus
    ldr r4, =FLASH_CR
    REG_CLR #FLASH_CR_PER, r4, r5
    ldr r4, =FLASH_SR 
    REG_SET #FLASH_SR_EOP, r4, r5

    pop {r4 - r6, lr}
    bx lr
    ENDP

FLASH_MassErase  PROC 

    bl FLASH_WaitNotBusy

    ldr r2, =FLASH_CR
    REG_SET #FLASH_CR_MER, r2, r3
    REG_SET #FLASH_CR_STRT, r2, r3
    
    bl FLASH_WaitNotBusy
    bl FLASH_GetStatus

    bx lr
    ENDP


FLASH_Write PROC 
    push {r4 - r5, lr}

    push {r0}
    bl FLASH_WaitNotBusy
    pop {r0}

    ldr r4, =FLASH_CR
    REG_SET #FLASH_CR_PG, r4, r5

    ldr r4, =0xffff
    and r1, r4
    strh r1, [r0]
    
    bl FLASH_WaitNotBusy
    bl FLASH_GetStatus

    ldr r4, =FLASH_CR
    REG_CLR #FLASH_CR_PG, r4, r5
    ldr r4, =FLASH_SR
    REG_SET #FLASH_SR_EOP, r4, r5

    pop {r4 - r5, lr}
    bx lr
    ENDP
    

    ALIGN

    END