#if 1
.thumb

.syntax unified

.bss

//bytes:
//.skip 4

.section ro, "a"

ro_var:
.word  0x7777
.data


#include "rcc.h"

.text 

.globl main

main :

    

    bl RCC_GetSystemClock
    
    mov r0, #2
    bl FLASH_SetWaitState
    bl FLASH_GetWaitState
 

    mov r0, #16
    bl RCC_EnablePLL

    mov r0, #1
    ldr r1, =RCC_APB1ENR
    ldr r2, =RCC_APB1ENR_PWREN
    ldr r3, =RCC_APB1RSTR
    ldr r4, =RCC_APB1RSTR_PWRRST
    push {r4}
    bl RCC_EnablePeripheral
    pop {r4}

    bl FLASH_Unlock
    bl FLASH_Lock
    bl FLASH_Unlock

 
    mov r0, #63
    bl FLASH_ErasePage
 
    ldr r0, =0x800FC00
    ldr r1, =0x1234
    bl FLASH_Write

    mov r0, #63
    bl FLASH_ErasePage

l:   b l




#endif
