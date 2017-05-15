    PRESERVE8
    THUMB

    AREA    DR, DATA, READONLY

    INCLUDE gpio_regs.s 
    INCLUDE rcc_regs.s
    INCLUDE common.s


GPIO_ports  
    DCD     GPIOA_BASE, RCC_APB2ENR_IOPAEN
    DCD     GPIOB_BASE, RCC_APB2ENR_IOPBEN 
    DCD     GPIOC_BASE, RCC_APB2ENR_IOPCEN 
    DCD     GPIOD_BASE, RCC_APB2ENR_IOPDEN 
    DCD     GPIOE_BASE, RCC_APB2ENR_IOPEEN 
    DCD     GPIOF_BASE, RCC_APB2ENR_IOPFEN
    DCD     GPIOG_BASE, RCC_APB2ENR_IOPGEN

;####################################################################

    AREA    CODE1, CODE, READONLY
        
    IMPORT RCC_EnablePeripheral   ;0: true/false, 1: en_reg 2: en_flag 3: rst_reg  sp: rst_flag 

    EXPORT GPIO_init              ;0:port 1:GPIO_cfg: base crl crh idr odr bsrr brr
    EXPORT GPIO_cfg               ;GPIO_cfg  pin cnf mode 
    EXPORT GPIO_set_mode          ;GPIO_cfg  pin mode 
    EXPORT GPIO_cfg_in_float      ;GPIO_cfg  pin
    EXPORT GPIO_cfg_in_anal       ;GPIO_cfg  pin
    EXPORT GPIO_cfg_in_pulldown   ;GPIO_cfg  pin
    EXPORT GPIO_cfg_in_pullup     ;GPIO_cfg  pin
    EXPORT GPIO_cfg_out_pp        ;GPIO_cfg  pin 
    EXPORT GPIO_cfg_out_od        ;GPIO_cfg  pin
    EXPORT GPIO_cfg_out_alt_pp    ;GPIO_cfg  pin
    EXPORT GPIO_cfg_out_alt_od    ;GPIO_cfg  pin
    EXPORT GPIO_set_speed_low     ;GPIO_cfg  pin
    EXPORT GPIO_set_speed_medium  ;GPIO_cfg  pin
    EXPORT GPIO_set_speed_high    ;GPIO_cfg  pin

    EXPORT GPIO_set_pin           ;GPIO_cfg  pin
    EXPORT GPIO_reset_pin         ;GPIO_cfg  pin
    EXPORT GPIO_set_output        ;GPIO_cfg  pin 
    EXPORT GPIO_is_set            ;GPIO_cfg  pin
    EXPORT GPIO_toggle_pin           ;GPIO_cfg  pin

;####################################################################
GPIO_init PROC
    
    push {r4, lr}


    ldr r4, =GPIO_ports
    lsl r2, r0, #3
    ldr r0, [r4, r2]
    add r2, #4
    ldr r3, [r4, r2] ;port enable bitmask

    str r0, [r1, #0] ;base
    
    add r4, r0, #GPIO_OFF_CRL
    str r4, [r1, #4]
    add r4, r0, #GPIO_OFF_CRH
    str r4, [r1, #8]
    add r4, r0, #GPIO_OFF_IDR
    str r4, [r1, #12]
    add r4, r0, #GPIO_OFF_ODR
    str r4, [r1, #16]
    add r4, r0, #GPIO_OFF_BSRR
    str r4, [r1, #20]
    add r4, r0, #GPIO_OFF_BRR
    str r4, [r1, #24]

    mov r0, #TRUE ; enable
    ldr r1, =RCC_APB2ENR
    ldr r2, =RCC_APB2RSTR
    bl RCC_EnablePeripheral

    pop {r4, lr}
    bx lr

    ENDP

;##################################################################
;# r0      r1  r2  r3
;#GPIO_cfg pin cnf  mode
GPIO_cfg PROC
    push {r4 - r6, lr}

;# get pin number from pin mask
    clz r1, r1    ;//count leading zeros
    mov r4, #31
    sub r1, r4, r1

    ldr r6, [r0]

    cmp r1, #8
    blt cnt
    and r1, #7  ;@ pin % 8 for pin>7
    add r6, #GPIO_OFF_CRH
cnt
    lsl r1, r1, #2  ;@r1 : pin*4, r1 now contains number of bits to shift

    lsl r5, r2, #2  ;@  4bit cfg and mode
    orr  r5, r3     ;@ combined into a byte

    lsl r5, r5, r1  ;@r5 contains  cfg/mode byte shifted based on pin no

    mov r3, #0xf     ;@ clear mask: pin/cfg 4 bits each
    lsl r3, r3, r1  ;@ shift the clear mask according to pin no

    ldr r4, [r6]
    bic r4, r3    ;@ clear pin cfg/mode byte
    orr r4, r5    ;@  set pin cfg/mode byte
    str r4, [r6]

    pop {r4 - r6, lr}
    bx lr

    ENDP
        

;##################################################################
;# r0      r1  r2
;#GPIO_cfg pin mode
GPIO_set_mode PROC
        
    push {r4 - r6, lr}

;# get pin number from pin mask
    clz r1, r1
    mov r4, #31
    sub r1, r4, r1

    ldr r0, [r0]

    cmp r1, #8
    blt cnt2
    and r1, #7 ; @ pin % 8 for pin>7
    add r0, #GPIO_OFF_CRH
cnt2
    lsl r1, r1, #2  ;@r1 : pin*4, r1 now contains number of bits to shift

    mov r3, #0x3    ;@ cmode clear mask
    lsl r3, r3, r1  ;@ shift the clear mask according to pin no
    lsl r2, r2, r1  ;@ shift the mode according to pin no

    ldr r4, [r0]
    bic r4, r3    ;@ clear pin cfg/mode byte
    orr r4, r2    ;@  set pin cfg/mode byte
    str r4, [r0]

    pop {r4 - r6, lr}
    bx lr

    ENDP 
        
;##################################################################
;#r0: GPIO_CFG r1: pin
GPIO_cfg_in_float PROC
    push {lr}
    mov r2, #GPIO_CNF_IFLOAT
    mov r3, #GPIO_MODE_INP
    bl GPIO_cfg
    pop {lr}
    bx lr

    ENDP 
        
;##################################################################
GPIO_cfg_in_anal PROC
    push {lr}
    mov r2, #GPIO_CNF_IANAL
    mov r3, #GPIO_MODE_INP
    bl GPIO_cfg
    pop {lr}
    bx lr
    
    ENDP

;##################################################################
GPIO_cfg_in_pulldown PROC
    push {lr}
    mov r2, #GPIO_CNF_IPUPD
    mov r3, #GPIO_MODE_INP
    push {r0, r1}
    bl GPIO_cfg
    pop {r0, r1}
    bl GPIO_reset_pin
    pop {lr}
    bx lr
    
    ENDP
        
;##################################################################
GPIO_cfg_in_pullup PROC
    push {lr}
    mov r2, #GPIO_CNF_IPUPD
    mov r3, #GPIO_MODE_INP
    push {r0, r1}
    bl GPIO_cfg
    pop {r0, r1}
    bl GPIO_set_pin
    pop {lr}
    bx lr

    ENDP

;##################################################################
GPIO_cfg_out_pp PROC
    push {lr}
    mov r2, #GPIO_CNF_OPP
    mov r3, #GPIO_MODE_OUT_HIGH_SPEED
    bl GPIO_cfg
    pop {lr}
    bx lr

    ENDP

GPIO_cfg_out_od PROC
    push {lr}
    mov r2, #GPIO_CNF_OD
    mov r3, #GPIO_MODE_OUT_HIGH_SPEED
    bl GPIO_cfg
    pop {lr}
    bx lr
    
    ENDP

GPIO_cfg_out_alt_pp PROC
    push {lr}
    mov r2, #GPIO_CNF_ALT_PP
    mov r3, #GPIO_MODE_OUT_HIGH_SPEED
    bl GPIO_cfg
    pop {lr}
    bx lr
    
    ENDP

GPIO_cfg_out_alt_od PROC
    push {lr}
    mov r2, #GPIO_CNF_ALT_OD
    mov r3, #GPIO_MODE_OUT_HIGH_SPEED
    bl GPIO_cfg
    pop {lr}
    bx lr
    
    ENDP

GPIO_set_speed_low PROC
    push {lr}
    mov r2, #GPIO_MODE_OUT_LOW_SPEED
    bl GPIO_set_mode
    pop {lr}
    bx lr
    
    ENDP

GPIO_set_speed_medium PROC
    push {lr}
    mov r2, #GPIO_MODE_OUT_MEDIUM_SPEED
    bl GPIO_set_mode
    pop {lr}
    bx lr
    
    ENDP

GPIO_set_speed_high PROC
    push {lr}
    mov r2, #GPIO_MODE_OUT_HIGH_SPEED
    bl GPIO_set_mode
    pop {lr}
    bx lr
    
    ENDP

;##################################################################
GPIO_set_pin PROC
    push {r4, lr}
    ldr r4, [r0, #bsrr_off]
    str r1, [r4]
    pop {r4, lr}
    bx lr
    
    ENDP

;##################################################################
GPIO_reset_pin PROC
    push {r4, lr}
    ldr r4, [r0, #brr_off]
    str r1, [r4]
    pop {r4, lr}
    bx lr
    
    ENDP

;##################################################################
GPIO_toggle_pin PROC
    push {r4, r5, lr}
    ldr r4, [r0, #odr_off]
    ldr r5, [r4]
    eor r5, r1
    str r5, [r4]
    pop {r4, r5, lr}
    bx lr
    
    ENDP


;##################################################################
GPIO_set_output PROC
    push {r4, lr}
    ldr r4, [r0, #odr_off]
    str r1, [r4]
    pop {r4, lr}
    bx lr
    
    ENDP

;##################################################################
GPIO_is_set PROC
    push {r4, r5, lr}
    ldr r4, [r0, #idr_off]
    ldr r5, [r4]
    mov r0, #FALSE
    and r5, r1
    cbz r5, out
    mov r0, #TRUE
out
    pop {r4, r5, lr}
    bx lr

    ENDP
        
    ALIGN

    END
        