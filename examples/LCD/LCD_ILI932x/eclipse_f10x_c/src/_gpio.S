.thumb

.syntax unified


.data

#include "_gpio.h"

.equ idr_off,   0x0c
.equ odr_off,   0x10
.equ set_off,   0x14
.equ reset_off, 0x18


#reg1 .req r1

gpio_ports:
.word GPIOA_BASE
.word GPIOB_BASE
.word GPIOC_BASE
.word GPIOD_BASE
.word GPIOE_BASE
.word GPIOF_BASE
.word GPIOG_BASE


.align

array:
.word 0x1, 0x2, 0x3, 0x4
.equ alen, (.-array) / 4

.text

.extern g_ar

.globl test_array

.globl gpio_init

.globl gpio_cfg
.globl gpio_cfg_in_float
.globl gpio_cfg_in_anal
.globl gpio_cfg_in_pullup
.globl gpio_cfg_in_pulldown
.globl gpio_cfg_out_pp
.globl gpio_cfg_out_od
.globl gpio_cfg_out_alt_pp
.globl gpio_cfg_out_alt_od
.globl gpio_set_speed_low
.globl gpio_set_speed_medium
.globl gpio_set_speed_high

.globl gpio_set_pin
.globl gpio_reset_pin
.globl gpio_set_output
.globl gpio_is_set


# w10 = *(uint32_t*)(x11 + (int32_t)w12)
#ldr w10, [x11, w12, SXTW]



#r0: port number (a=0, etc)   r1: addr of GPIO_CFG
gpio_init:
    push {r4 - r6, lr}

    lsl r2, r0, #2
    ldr r4, =gpio_ports
    ldr r0, [r4, r2]

    str r0, [r1, #0]
    add r4, r0, GPIO_OFF_CRL
    str r4, [r1, #4]
    add r4, r0, GPIO_OFF_CRH
    str r4, [r1, #8]
    add r4, r0, GPIO_OFF_IDR
    str r4, [r1, #12]
    add r4, r0, GPIO_OFF_ODR
    str r4, [r1, #16]
    add r4, r0, GPIO_OFF_BSRR
    str r4, [r1, #20]
    add r4, r0, GPIO_OFF_BRR
    str r4, [r1, #24]

    pop {r4 - r6, lr}
    bx lr


##################################################################
# r0      r1  r2  r3
#gpio_cfg pin cfg  mode
gpio_cfg:
    push {r4 - r6, lr}

# get pin number from pin mask
    clz r1, r1
    mov r4, 31
    sub r1, r4, r1

    ldr r0, [r0]

    cmp r1, #8
    blt cnt
    and r1, #7  @ pin % 8 for pin>7
    add r0, GPIO_OFF_CRH
cnt:
    lsl r1, r1, #2  @r1 : pin*4, r1 now contains number of bits to shift

    lsl r5, r2, #2  @  4bit cfg and mode
    orr  r5, r3     @ combined into a byte

    lsl r5, r5, r1  @r5 contains  cfg/mode byte shifted based on pin no

    mov r3, 0xf     @ clear mask: pin/cfg 4 bits each
    lsl r3, r3, r1  @ shift the clear mask according to pin no

    ldr r4, [r0]
    bic r4, r3    @ clear pin cfg/mode byte
    orr r4, r5    @  set pin cfg/mode byte
    str r4, [r0]

    pop {r4 - r6, lr}
    bx lr

##################################################################
# r0      r1  r2
#gpio_cfg pin mode
gpio_set_mode:
    push {r4 - r6, lr}

# get pin number from pin mask
    clz r1, r1
    mov r4, 31
    sub r1, r4, r1

    ldr r0, [r0]

    cmp r1, #8
    blt cnt2
    and r1, #7  @ pin % 8 for pin>7
    add r0, GPIO_OFF_CRH
cnt2:
    lsl r1, r1, #2  @r1 : pin*4, r1 now contains number of bits to shift

    mov r3, #0x3    @ cmode clear mask
    lsl r3, r3, r1  @ shift the clear mask according to pin no
    lsl r2, r2, r1  @ shift the mode according to pin no

    ldr r4, [r0]
    bic r4, r3    @ clear pin cfg/mode byte
    orr r4, r2    @  set pin cfg/mode byte
    str r4, [r0]

    pop {r4 - r6, lr}
    bx lr

##################################################################
#r0: GPIO_CFG r1: pin
gpio_cfg_in_float:
    push {lr}
    mov r2, GPIO_CNF_IFLOAT
    mov r3, GPIO_MODE_INP
    bl gpio_cfg
    pop {lr}
    bx lr

##################################################################
gpio_cfg_in_anal:
    push {lr}
    mov r2, GPIO_CNF_IANAL
    mov r3, GPIO_MODE_INP
    bl gpio_cfg
    pop {lr}
    bx lr

##################################################################
gpio_cfg_in_pulldown:
    push {lr}
    mov r2, GPIO_CNF_IPUPD
    mov r3, GPIO_MODE_INP
    push {r0, r1}
    bl gpio_cfg
    pop {r0, r1}
    bl gpio_reset_pin
    pop {lr}
    bx lr

##################################################################
gpio_cfg_in_pullup:
    push {lr}
    mov r2, GPIO_CNF_IPUPD
    mov r3, GPIO_MODE_INP
    push {r0, r1}
    bl gpio_cfg
    pop {r0, r1}
    bl gpio_set_pin
    pop {lr}
    bx lr

##################################################################
gpio_cfg_out_pp:
    push {lr}
    mov r2, GPIO_CNF_OPP
    mov r3, GPIO_MODE_OUT_HIGH_SPEED
    bl gpio_cfg
    pop {lr}
    bx lr

gpio_cfg_out_od:
    push {lr}
    mov r2, GPIO_CNF_OD
    mov r3, GPIO_MODE_OUT_HIGH_SPEED
    bl gpio_cfg
    pop {lr}
    bx lr

gpio_cfg_out_alt_pp:
    push {lr}
    mov r2, GPIO_CNF_ALT_PP
    mov r3, GPIO_MODE_OUT_HIGH_SPEED
    bl gpio_cfg
    pop {lr}
    bx lr

gpio_cfg_out_alt_od:
    push {lr}
    mov r2, GPIO_CNF_ALT_OD
    mov r3, GPIO_MODE_OUT_HIGH_SPEED
    bl gpio_cfg
    pop {lr}
    bx lr

gpio_set_speed_low:
    push {lr}
    mov r2, GPIO_MODE_OUT_LOW_SPEED
    bl gpio_set_mode
    pop {lr}
    bx lr

gpio_set_speed_medium:
    push {lr}
    mov r2, GPIO_MODE_OUT_MEDIUM_SPEED
    bl gpio_set_mode
    pop {lr}
    bx lr

gpio_set_speed_high:
    push {lr}
    mov r2, GPIO_MODE_OUT_HIGH_SPEED
    bl gpio_set_mode
    pop {lr}
    bx lr

##################################################################
gpio_set_pin:
    push {r4, lr}
    ldr r4, [r0, set_off]
    str r1, [r4]
    pop {r4, lr}
    bx lr

##################################################################
gpio_reset_pin:
    push {r4, lr}
    ldr r4, [r0, reset_off]
    str r1, [r4]
    pop {r4, lr}
    bx lr

##################################################################
gpio_set_output:
    push {r4, lr}
    ldr r4, [r0, odr_off]
    str r1, [r4]
    pop {r4, lr}
    bx lr

##################################################################
gpio_is_set:
    push {r4, lr}
    ldr r4, [r0, idr_off]
    ldr r5, [r4]
    mov r0, 0
    and r5, r1
    cbz r5, out
    mov r0, 1
out:
    pop {r4, lr}
    bx lr


