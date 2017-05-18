.thumb

.syntax unified

.bss

//bytes:
//.skip 4

.section ro, "a"

ro_var:
.word  0x7777
.data




#include "_rcc.h"

.text 

.globl main
.globl _d

main :

//    bl RCC_GetSystemClock

    ldr r0, =g_pfnVectors
    ldr r0, =ro_var
    ldr r1, [r0]
    mov r1, #5
    str r1, [r0]
    //ldr r0, =bytes
    ldr r0, =_d
    ldr r1, [r0]
    //ldr r0, =_d2
    ldr r0, [r0]

    bl test64
    //bl RCC_EnablePeripheral

    mov r0, #1
    ldr r1, =RCC_APB1ENR
    ldr r2, =RCC_APB1ENR_PWREN
    ldr r3, =RCC_APB1RSTR
    ldr r4, =RCC_APB1RSTR_PWRRST
    push {r4}
    bl RCC_EnablePeripheral
    pop {r4}


forever:  b forever

.globl _test64
_test64:
    nop
    nop
    nop
    bx lr
