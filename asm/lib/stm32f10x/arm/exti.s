    PRESERVE8
    THUMB

    INCLUDE stm32.s
    INCLUDE macros.s
    INCLUDE irq.s

    AREA    DR, DATA, READONLY


        
    IF :DEF:STM32F1 || :DEF:STM32L0        
EXTI_BASE  EQU 0x40010400        
    ELIF :DEF:STM32F2 || :DEF:STM32F4    
EXTI_BASE  EQU 0x40013C00    
    ELSE
    ASSERT {FALSE}
    
    ENDIF

EXTI_IMR   EQU EXTI_BASE          ;//interrupt mask
EXTI_EMR   EQU EXTI_BASE + 0x4    ;//event mask
EXTI_RTSR  EQU EXTI_BASE + 0x8    ;//rising  edge
EXTI_FTSR  EQU EXTI_BASE + 0xc    ;//falling edge
EXTI_SWIER EQU EXTI_BASE + 0x10   ;//software int event
EXTI_PR    EQU EXTI_BASE + 0x14   ;//pending

EXTI0  EQU 0
EXTI1  EQU 1
EXTI2  EQU 2
EXTI3  EQU 3
EXTI4  EQU 4
EXTI5  EQU 5
EXTI6  EQU 6
EXTI7  EQU 7
EXTI8  EQU 8
EXTI9  EQU 9
EXTI10 EQU 10
EXTI11 EQU 11
EXTI12 EQU 12
EXTI13 EQU 13
EXTI14 EQU 14
EXTI15 EQU 15
EXTI16 EQU 16
EXTI17 EQU 17
EXTI18 EQU 18
EXTI19 EQU 19
    
    IF :DEF:STM32F2 || :DEF:STM32F4 || :DEF:STM32L0

EXTI20 EQU 20
EXTI21 EQU 21
EXTI22 EQU 22
    
        IF :DEF:STM32L0

EXTI23 EQU 23
EXTI24 EQU 24
EXTI25 EQU 25
EXTI26 EQU 26
EXTI28 EQU 28
EXTI29 EQU 29

        ENDIF
            
    ENDIF

    
EXTI_IRQ DCB IRQn_EXTI0, IRQn_EXTI1, IRQn_EXTI2, IRQn_EXTI3, IRQn_EXTI4, \
             IRQn_EXTI9_5, IRQn_EXTI9_5, IRQn_EXTI9_5, IRQn_EXTI9_5, IRQn_EXTI9_5, \
             IRQn_EXTI15_10, IRQn_EXTI15_10, IRQn_EXTI15_10, IRQn_EXTI15_10, IRQn_EXTI15_10, IRQn_EXTI15_10, \
             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0   
    
    
    AREA CODE, CODE, READONLY

    IMPORT NVIC_ClearEnable
    IMPORT NVIC_SetEnable

    EXPORT EXTI_IsPending
    EXPORT EXTI_ClearPending
    EXPORT EXTI_EnableEventFallingEdge
    EXPORT EXTI_EnableEventRisingEdge
    EXPORT EXTI_EnableInterruptFallingEdge
    EXPORT EXTI_EnableInterruptRisingEdge

;r0: line, r1: true=clear if set
EXTI_IsPending  PROC
    push {r4}
    ldr r3, =EXTI_PR
    mov r2, #1
    lsl r2, r0
    REG_ISSET r2, r0, r3, r4
    cmp r1, #FALSE
    beq end_isp
    
    REG_SET r2, r3, r4 
    
end_isp
   
    pop {r4}
    bx lr
    ENDP

EXTI_ClearPending PROC
    mov r2, #1
    lsl r2, r0
    ldr r1, =EXTI_PR
    REG_SET r2, r1, r3
    bx lr
    ENDP

;r0: exti line  r1: enable, r2: edge_reg, r3: event/int reg
EXTI_Enable PROC 
    push {r4-r5, lr}
    push {r2, r3}
    mov r4, #1
    lsl r4, r0
    ldr r5, =EXTI_IRQ
    ldrb r0, [r5, r0]
    cmp r1, #TRUE
    beq set_en
    cmp r1, #16
    IT LE
    blle NVIC_ClearEnable
    pop {r2, r3}
    REG_CLR r4, r2, r5 
    REG_CLR r4, r3, r5 
    b end_en
    

set_en
    cmp r1, #16
    IT LE
    blle NVIC_SetEnable
    pop {r2, r3}
    REG_SET r4, r2, r5 
    REG_SET r4, r3, r5 


end_en
    pop {r4-r5, lr}
    bx lr
    ENDP

;r0: exti line  r1: enable
EXTI_EnableInterruptRisingEdge  PROC 
    push {lr}
    ldr r2, =EXTI_RTSR
    ldr r3, =EXTI_IMR
    bl EXTI_Enable
    pop {lr}
    bx lr
    ENDP

;r0: exti line  r1: enable
EXTI_EnableInterruptFallingEdge PROC
    push {lr}
    ldr r2, =EXTI_FTSR
    ldr r3, =EXTI_IMR
    bl EXTI_Enable
    pop {lr}
    bx lr
    ENDP 
        
;r0: exti line  r1: enable
EXTI_EnableEventRisingEdge PROC
    push {lr}
    ldr r2, =EXTI_RTSR
    ldr r3, =EXTI_EMR
    bl EXTI_Enable
    pop {lr}
    bx lr
    ENDP 
        
;r0: exti line  r1: enable
EXTI_EnableEventFallingEdge PROC
    push {lr}
    ldr r2, =EXTI_FTSR
    ldr r3, =EXTI_EMR
    bl EXTI_Enable
    pop {lr}
    bx lr
    ENDP

    ALIGN
    END