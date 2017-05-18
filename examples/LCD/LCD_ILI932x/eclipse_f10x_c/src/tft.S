.thumb

.syntax unified



.data

#include "_gpio.h"

.globl GPIO_CRL
.globl GPIO_CRH
.globl GPIO_IDR
.globl GPIO_ODR
.globl GPIO_BSRR
.globl GPIO_BRR

GPIO_CRL: .word GPIO_OFF_CRL+ 1
GPIO_CRH: .word GPIO_OFF_CRH
GPIO_IDR: .word GPIO_OFF_IDR
GPIO_ODR: .word GPIO_OFF_ODR
GPIO_BSRR: .word GPIO_OFF_BSRR
GPIO_BRR: .word 0



.text

.globl init_gpio


init_gpio:
    push {r4 - r6, lr}

    ldr r4, =GPIO_CRL
    add r5, r4, GPIO_OFF_CRL
    str  r5, [r4]
    ldr r4, =GPIO_CRH
    add r5, r4, GPIO_OFF_CRH
    str  r5, [r4]

    pop {r4 - r6, lr}

    bx lr
