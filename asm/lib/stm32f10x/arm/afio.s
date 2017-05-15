    PRESERVE8
    THUMB

    AREA DRO, DATA, READONLY

    INCLUDE common.s
    INCLUDE afio_regs.s
    INCLUDE macros.s
    INCLUDE rcc_regs.s
    
    AREA CODE, CODE, READONLY 

    EXPORT AFIO_Enable
    EXPORT AFIO_Remap_SPI1
    EXPORT AFIO_Remap_I2C1
    EXPORT AFIO_Remap_USART1    
    EXPORT AFIO_Remap_USART2    
    EXPORT AFIO_Remap_USART3    
    EXPORT AFIO_Remap_PD01
    EXPORT AFIO_Remap_SWJ_CFG
    EXPORT AFIO_Remap_TIM1
    EXPORT AFIO_Remap_TIM2_partial1
    EXPORT AFIO_Remap_TIM2_partial2
    EXPORT AFIO_Remap_TIM2_full
    EXPORT AFIO_Remap_TIM3
    EXPORT AFIO_Remap_TIM4
    EXPORT AFIO_Remap_TIM5_Ch4
    EXPORT AFIO_Remap_EXTI; r0: line r1: port no
    EXPORT AFIO_Remap_TIM9
    EXPORT AFIO_Remap_TIM10
    EXPORT AFIO_Remap_TIM13
    EXPORT AFIO_Remap_TIM14

    IMPORT RCC_EnablePeripheral


AFIO_Enable PROC
    push {lr}
    ldr r1, =RCC_APB2ENR
    ldr r2, =RCC_APB2RSTR
    mov r3, #RCC_APB2ENR_AFIOEN
    bl RCC_EnablePeripheral
    pop {lr}
    bx lr
    ENDP

AFIO_Remap_SPI1  PROC
    
    ldr r2, =AFIO_MAPR
    REG_SETCLR #AFIO_MAPR_SPI1_REMAP, r0, r2, r3
    bx lr
    ENDP

AFIO_Remap_I2C1  PROC
    
    ldr r2, =AFIO_MAPR
    REG_SETCLR #AFIO_MAPR_I2C1_REMAP, r0, r2, r3
    bx lr
    ENDP

AFIO_Remap_USART1  PROC
    
    ldr r2, =AFIO_MAPR
    REG_SETCLR #AFIO_MAPR_USART1_REMAP, r0, r2, r3
    bx lr
    ENDP

AFIO_Remap_USART2  PROC
    
    ldr r2, =AFIO_MAPR
    REG_SETCLR #AFIO_MAPR_USART2_REMAP, r0, r2, r3
    bx lr
    ENDP

; r0: remap r1: full_remap
AFIO_Remap_USART3  PROC
    
    ldr r2, =AFIO_MAPR
    
    REG_CLR #AFIO_MAPR_USART3_REMAP_CLEARMASK, r2, r3
    
    cmp r0, #TRUE 
    beq  u3_remap
    
    REG_SET #AFIO_MAPR_USART3_REMAP_NONE, r2, r3
    b u3_end
    
u3_remap    
    cmp r1, #TRUE
    ITE EQ 
    moveq r1, #AFIO_MAPR_USART3_REMAP_FULL
    movne r1, #AFIO_MAPR_USART3_REMAP_PARTIAL
    REG_SET r1, r2, r3
    
u3_end    
    bx lr
    ENDP

; r0: remap r1: full_remap
AFIO_Remap_TIM1  PROC
    
    ldr r2, =AFIO_MAPR
    
    REG_CLR #AFIO_MAPR_TIM1_REMAP_CLEARMASK, r2, r3
    
    cmp r0, #TRUE 
    beq  t1_remap 
    
    REG_SET #AFIO_MAPR_TIM1_REMAP_NONE, r2, r3
    b t1_end
    
t1_remap    
    cmp r1, #TRUE
    ITE EQ 
    moveq r1, #AFIO_MAPR_TIM1_REMAP_FULL
    movne r1, #AFIO_MAPR_TIM1_REMAP_PARTIAL
    REG_SET r1, r2, r3
    
t1_end    
    bx lr
    ENDP

; no args
AFIO_Remap_TIM2_partial1  PROC
    
    ldr r2, =AFIO_MAPR
    
    REG_CLR #AFIO_MAPR_TIM2_REMAP_CLEARMASK, r2, r3
    
    mov r1, #AFIO_MAPR_TIM2_REMAP_1
    REG_SET r1, r2, r3
    
    bx lr
    ENDP

AFIO_Remap_TIM2_partial2  PROC
    
    ldr r2, =AFIO_MAPR
    
    REG_CLR #AFIO_MAPR_TIM2_REMAP_CLEARMASK, r2, r3
    
    mov r1, #AFIO_MAPR_TIM2_REMAP_2
    REG_SET r1, r2, r3
    
    bx lr
    ENDP

AFIO_Remap_TIM2_full  PROC
    
    ldr r2, =AFIO_MAPR
    
    REG_CLR #AFIO_MAPR_TIM2_REMAP_CLEARMASK, r2, r3
    
    mov r1, #AFIO_MAPR_TIM2_REMAP_FULL
    REG_SET r1, r2, r3
    
    bx lr
    ENDP

; r0: remap r1: full_remap
AFIO_Remap_TIM3  PROC
    
    ldr r2, =AFIO_MAPR
    
    REG_CLR #AFIO_MAPR_TIM3_REMAP_CLEARMASK, r2, r3
    
    cmp r0, #TRUE 
    beq  t3_remap
    
    REG_SET #AFIO_MAPR_TIM3_REMAP_NONE, r2, r3
    b t3_end
    
t3_remap    
    cmp r1, #TRUE
    ITE EQ 
    moveq r1, #AFIO_MAPR_TIM3_REMAP_FULL
    movne r1, #AFIO_MAPR_TIM3_REMAP_PARTIAL
    REG_SET r1, r2, r3
    
t3_end    
    bx lr
    ENDP


AFIO_Remap_TIM4  PROC
    
    ldr r2, =AFIO_MAPR
    REG_SETCLR #AFIO_MAPR_TIM4_REMAP, r0, r2, r3
    bx lr
    ENDP

AFIO_Remap_PD01  PROC
    
    ldr r2, =AFIO_MAPR
    REG_SETCLR #AFIO_MAPR_PD01_REMAP, r0, r2, r3
    bx lr
    ENDP


AFIO_Remap_TIM5_Ch4  PROC
    
    ldr r2, =AFIO_MAPR
    REG_SETCLR #AFIO_MAPR_TIM5CH4_REMAP, r0, r2, r3
    bx lr
    ENDP

;r0: isJTAG r1: isSW  r2: isNJRST
AFIO_Remap_SWJ_CFG PROC
    push {r4}
    ldr r3, =AFIO_MAPR
    REG_CLR #AFIO_MAPR_SWJ_CFG_CLEARMASK, r3, r4
    cmp r0, #TRUE 
    beq is_jtag
    
    ;!jtag
    cmp r1, #TRUE ;isSW
    ITE EQ 
    moveq r2, #AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON
    movne r2, #AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_OFF
    REG_SET r2, r3, r4
    b j_end
is_jtag
    cmp r1, #TRUE 
    bne j_end
    cmp r2, #TRUE 
    ITE EQ 
    moveq r2, #AFIO_MAPR_SWJ_CFG_FULL_SWJ
    movne r2, #AFIO_MAPR_SWJ_CFG_FULL_SWJ_NO_NJTRST
    REG_SET r2, r3, r4
    
j_end    
    pop {r4}
    bx lr
    ENDP
    
;r0: line r1: port no    
AFIO_Remap_EXTI PROC 
 
    ldr r2, =AFIO_EXTICR1
    lsr r3, r0, #2 
    lsl r3, #2
    add r2, r3
    
    and r0, #3
    lsl r0, #2 
    lsl r1, r0
    
    str r1, [r2]
    
    bx lr
    ENDP

AFIO_Remap_TIM9  PROC
    
    ldr r2, =AFIO_MAPR2
    REG_SETCLR #AFIO_MAPR2_TIM9_REMAP, r0, r2, r3
    bx lr
    ENDP


AFIO_Remap_TIM10  PROC
    
    ldr r2, =AFIO_MAPR2
    REG_SETCLR #AFIO_MAPR2_TIM10_REMAP, r0, r2, r3
    bx lr
    ENDP


AFIO_Remap_TIM13  PROC
    
    ldr r2, =AFIO_MAPR2
    REG_SETCLR #AFIO_MAPR2_TIM13_REMAP, r0, r2, r3
    bx lr
    ENDP



AFIO_Remap_TIM14  PROC
    
    ldr r2, =AFIO_MAPR2
    REG_SETCLR #AFIO_MAPR2_TIM14_REMAP, r0, r2, r3
    bx lr
    ENDP


    ALIGN

    END