.macro REG_SET addr, val

    ldr r4, =\addr
    ldr r5, [r4]
    orr r5, \val
    str r5, [r4]
    
.endm

.macro REG_CLR addr, val
    ldr r4, =\addr
    ldr r5, [r4]
    bic r5, \val
    str r5, [r4]
    
.endm

.macro REG_SETCLR addr, val
    ldr r4, =\addr
    ldr r5, [r4]
    cmp r0, #1
    ITE EQ
    orreq r5, \val
    bicne r5, \val
    str r5, [r4]

.endm
