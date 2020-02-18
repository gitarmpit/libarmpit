.thumb
.syntax unified

.data
DATA      .req r0
LEN       .req r1
BRR       .req r2
BSRR      .req r3
ODR       .req r4
CS        .req r5
WRT       .req r6
RS        .req r7
TMP       .req r8

.text

.globl pushColorsAsm2  @comment
.globl pushColorsAsm   @comment

pushColorsAsm2:
    push {r5-r7, lr}

    ldr r4, [r0], #4
    ldr r5, [sp, #16]

    pop {r5-r7, lr}
    bx lr

pushColorsAsm:
    push {r4-r9, lr}

    ldr ODR, [sp, #28]
    ldr CS,  [sp, #32]
    ldr WRT, [sp, #36]
    ldr RS,  [sp, #40]

    str CS,  [BRR]
loop:
    ldrb TMP, [DATA, #1]
    str  TMP, [ODR]
    str  WRT, [BRR]
    @@ nop
    str  WRT, [BSRR]

    ldrb TMP, [DATA], #2
    str  TMP, [ODR]
    str  WRT, [BRR]
    @@ nop
    str  WRT, [BSRR]

    subs LEN,    #1
    bne loop

    STR CS, [BSRR]
    @@@@@@@@@@@@@@@@@@
    pop {r4-r9, lr}
    bx lr
