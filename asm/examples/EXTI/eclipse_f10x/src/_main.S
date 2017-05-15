#if 0
.thumb

.syntax unified

.bss

//bytes:
//.skip 4

.section ro, "a"

ro_var:
.word  0x7777
.data


#include "gpio.h"
#include "../common.h"


GPIOA_CFG:
.word GPIO_CFG_SIZE
.text 

.globl main

main :

    
    bl RCC_GetSystemClock
    
    mov r0, #2
    bl FLASH_SetWaitState
    bl FLASH_GetWaitState

    mov r0, #16
    bl RCC_EnablePLL



    mov r0, #GPIOA
    ldr r1, =GPIOA_CFG
    bl GPIO_init
    
    ldr r0, =GPIOA_CFG
    mov r1, #GPIO_PIN0
    bl GPIO_cfg_out_pp

    mov r1, #GPIO_PIN0
    bl GPIO_set_pin
    bl GPIO_reset_pin
    bl GPIO_set_pin

    bl GPIO_toggle_pin
    bl GPIO_toggle_pin

    mov r0, #TRUE
    bl Debug_EnableCYCCNT

l2:
    ldr r0, =1000000
    bl Debug_Delay_us
    ldr r0, =GPIOA_CFG
    mov r1, #1
    lsl r1, #0
    bl GPIO_toggle_pin
    b l2

l:   b l




#endif
