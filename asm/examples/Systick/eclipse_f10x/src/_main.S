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


#include "gpio.h"
#include "../common.h"


GPIOA_CFG:
.word GPIO_CFG_SIZE

.text 

.thumb_func
.globl  SysTick_Handler

SysTick_Handler:
    push {lr}
    ldr r0, =GPIOA_CFG
    mov r1, #GPIO_PIN0
    bl GPIO_toggle_pin

    pop {lr}
    bx lr


.globl main

.thumb_func
main :


    
    bl RCC_GetSystemClock
    
    mov r0, #2
    bl FLASH_SetWaitState
    bl FLASH_GetWaitState

    mov r0, #4
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

    @SYSTICK SETUP
    bl RCC_GetAHB_CLK
    mov r1, #TRUE    @enable
    mov r2, #FALSE   @div8
    bl SystickEnable
    ldr r0, =GPIOA_CFG
    mov r1, #GPIO_PIN0


l:   b l




#endif
