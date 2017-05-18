.thumb
.syntax unified

.data

#include "_rcc.h"


t_presc: .word 2, 4, 8, 16, 64, 128, 256, 512

.text

.globl RCC_GetSystemClock
.globl RCC_GetAHB_CLK
.globl RCC_GetAPBx_div
.globl RCC_GetAPB1_CLK
.globl RCC_GetAPB2_CLK
.globl RCC_GetTIMx_CLK
.globl RCC_EnableHSI
.globl RCC_EnableHSE
.globl RCC_BypassHSE
.globl RCC_EnableLSI
.globl RCC_EnableLSE
.globl RCC_EnablePLL
.globl RCC_DisablePLL
.globl RCC_SetAHBPrescaler
.globl RCC_SetAPB1Prescaler
.globl RCC_SetAPB2Prescaler
.globl RCC_SetADCPrescaler
.globl RCC_EnableRTC
.globl RCC_SetRTCClockSourceLSE
.globl RCC_SetRTCClockSourceLSI
.globl RCC_SetRTCClockSourceHSE_DIV128
.globl RCC_EnableBKP
.globl RCC_EnablePWR
.globl RCC_EnablePeripheral

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



 RCC_GetSystemClock:
       
    push {r4-r6, r8, lr}

    mov r5, #1  @freq multiplier

    ldr r4, =RCC_CFGR
    ldr r4, [r4]

    mov r0, #0
    and r8, r4, #0xc 
    
    cmp r8, #RCC_CFGR_SWS_HSI
    itt eq 
    ldreq r0, =HSI_CLOCK_FREQ
    beq sclk_end

    cmp r8, #RCC_CFGR_SWS_HSE
    itt eq 
    ldreq r0, =HSE_CLOCK_FREQ
    beq sclk_end

    cmp r8, #RCC_CFGR_SWS_PLL
    bne sclk_end

    lsr r5, r4, #18 
    and r5, #0xff 
    add r5, #2
    
    ands r6, r4, #RCC_CFGR_PLLSRC_HSE
    ITTT EQ
    ldreq r0, =HSI_CLOCK_FREQ
    lsreq r0, #1
    beq sclk_end
    
    ldr r0, =HSE_CLOCK_FREQ

    ands r6, r4, #RCC_CFGR_PLLXTPRE
    IT NE
    lsrne r0, #1
    
sclk_end:

    mul r0, r0, r5

    pop {r4-r6, r8, lr}
    bx lr

####################################################
RCC_GetAHB_CLK:

    push {r4, lr}

#r0: sysclock
    bl RCC_GetSystemClock

    ldr r4, =RCC_CFGR
    ldr r1, [r4]
    lsr r1, r1, #4
    and r1, r1, #0xf
#r1: abh div

    ldr r4, =t_presc
    
    ands r2, r1, #8 
    ITEEE EQ
    moveq r1, #1
    subne r1, #8
    lslne r1, #2
    ldrne r1, [r4, r1]

    udiv r0, r0, r1
    
    pop {r4, lr}
    bx lr

###################################################
RCC_GetAPBx_div:

    push {r4, lr}

    ldr r4, =RCC_CFGR
    ldr r1, [r4]
    lsr r1, r1, r0  @r0: passed as input
    and r1, r1, #0x7
#r1: apbx div
    
    ldr r4, =t_presc
    
    ands r2, r1, #4 
    ITEEE EQ
    moveq r1, #1
    subne r1, #4
    lslne r1, #1
    ldrne r1, [r4, r1]

    mov r0, r1
    pop {r4, lr}
    bx lr
        
#####################################################
RCC_GetAPB1_CLK:

    push {r4, lr}

    bl RCC_GetAHB_CLK
    mov r4, r0

    mov r0, #8 @APB1
    bl RCC_GetAPBx_div
    
    udiv r0, r4, r0

    pop {r4, lr}
    bx lr


######################################################
RCC_GetAPB2_CLK:

    push {r4, lr}

    bl RCC_GetAHB_CLK
    mov r4, r0

    mov r0, #11 @APB2
    bl RCC_GetAPBx_div
    
    udiv r0, r4, r0

    pop {r4, lr}
    bx lr

######################################################
#r0=1: APB1  r0=0: APB2
RCC_GetTIMx_CLK:

    push {r4, r5, lr}

    mov r5, r0
    
    bl RCC_GetAHB_CLK
    mov r4, r0

#   r4 : ahb_clk

    cmp r5, #0
    ITE EQ
    moveq r0, #11 @APB2
    movne r0, #8 @APB1
    
    bl RCC_GetAPBx_div
#   r0:  apbx_div
    
    udiv r5, r4, r0

#  r5: timx_clk

    cmp r0, #1 
    IT GT
    lslgt r5, #1
    
    mov r0, r5

    pop {r4, r5, lr}
    bx lr


###########################################
RCC_StartHSI:

    push {r4, r5, lr}

    ldr r4, =RCC_CR
    ldr r5, [r4]
    ands r5, #RCC_CR_HSIRDY
    bne done_hsi

    orr r5, #RCC_CR_HSION
    str r5, [r4]

wait_hsi:
    ldr r5, [r4]
    ands r5, #RCC_CR_HSIRDY
    beq wait_hsi
    
done_hsi:
    pop {r4, r5, lr}
    bx lr 

###########################################
RCC_IsHSIActive:

    push {r4, r5, lr}

    ldr r4, =RCC_CFGR
    ldr r4, [r4]

    ands r5, r4, #0xc
    cmp r5, #RCC_CFGR_SWS_HSI
    ITT EQ
    moveq r0, #1
    beq ishsi_done

    cmp r5, #RCC_CFGR_SWS_PLL
    ITT NE
    movne r0, #0
    bne ishsi_done
    
    and r5, r4, #RCC_CFGR_PLLSRC_CLEARMASK 
    cmp r5, #RCC_CFGR_PLLSRC_HSI_DIV2
    ITE EQ
    moveq r0, #1
    movne r0, #0

ishsi_done:
    pop {r4, r5, lr}
    bx lr 


##############################################
RCC_StopHSI:

    push {r4, r5, lr}

    ldr r4, =RCC_CR
    ldr r5, [r4]
    bic r5, #RCC_CR_HSION
    str r5, [r4]

wait_hsioff:
    ldr r5, [r4]
    ands r5, #RCC_CR_HSIRDY
    bne wait_hsioff


    pop {r4, r5, lr}
    bx lr 


###########################################
RCC_EnableHSI:

    push {r4, r5, lr}

    cmp r0, #1
    beq hsi_start
    
    bl RCC_IsHSIActive
    cmp r0, #0 @inactive, can stop
    IT EQ
    bleq RCC_StopHSI
    b hsi_done

hsi_start:
    ldr r4, =RCC_CFGR
    ldr r5, [r4]
    bic r5, #RCC_CFGR_SW_CLEARMASK
    str r5, [r4]

    bl RCC_StartHSI
    ldr r5, [r4]
    orr r5, #RCC_CFGR_SW_HSI
    str r5, [r4]


hsi_done:

    pop {r4, r5, lr}
    bx lr 
        
##############################################

RCC_StopHSE:

    push {r4, r5, lr}

    ldr r4, =RCC_CR
    ldr r5, [r4]
    bic r5, #RCC_CR_HSEON
    str r5, [r4]

wait_hseoff:
    ldr r5, [r4]
    ands r5, #RCC_CR_HSERDY
    bne wait_hseoff

    pop {r4, r5, lr}
    bx lr 

###########################################

RCC_StartHSE:

    push {r4, r5, lr}

    ldr r4, =RCC_CR
    ldr r5, [r4]
    ands r5, #RCC_CR_HSERDY
    bne done_hse

    orr r5, #RCC_CR_HSEON
    str r5, [r4]

wait_hse:
    ldr r5, [r4]
    ands r5, #RCC_CR_HSERDY
    beq wait_hse

done_hse:
    pop {r4, r5, lr}
    bx lr 

###########################################
RCC_IsHSEActive:

    push {r4, r5, lr}

    ldr r4, =RCC_CFGR
    ldr r4, [r4]

    ands r5, r4, #0xc
    cmp r5, #RCC_CFGR_SWS_HSE
    ITT EQ
    moveq r0, #1
    beq ishse_done

    cmp r5, #RCC_CFGR_SWS_PLL
    ITT NE
    movne r0, #0
    bne ishse_done

    and r5, r4, #RCC_CFGR_PLLSRC_CLEARMASK 
    cmp r5, #RCC_CFGR_PLLSRC_HSE
    ITE EQ
    moveq r0, #1
    movne r0, #0

ishse_done:
    pop {r4, r5, lr}
    bx lr 


###########################################

RCC_EnableHSE:

    push {r4-r6, r8, lr}

    mov r5, r0 
    
    bl RCC_IsHSEActive
    cmp r0, #1
    beq hse_done

    cmp r5, #1
    beq hse_start
    bl RCC_StopHSE 
    b hse_done

hse_start:
    ldr r4, =RCC_CFGR
    ldr r8, [r4]
    bic r8, #RCC_CFGR_SW_CLEARMASK
    str r8, [r4]

    bl RCC_StartHSE
    ldr r8, [r4]
    orr r8, #RCC_CFGR_SW_HSE
    str r8, [r4]
    

hse_done:

    pop {r4-r6, r8, lr}
    bx lr 



RCC_BypassHSE:

    push {r4, r5, lr}
    ldr r4, =RCC_CR
    ldr r5, [r4]

    cmp r0, #1
    ITE EQ 
    orreq r5, #RCC_CR_HSEBYP
    bicne r5, #RCC_CR_HSEBYP    

    pop {r4, r5, lr}
    bx lr



###########################################

RCC_EnableLSI:

    push {r4 - r5, lr}
    
    ldr r4, =RCC_BDCR
    ldr r5, [r4]
    bic r5, #RCC_BDCR_LSEON 
    bic r5, #RCC_BDCR_RTC_LSE
    orr r5, #RCC_BDCR_RTC_LSI
    str r5, [r4]

    ldr r4, =RCC_CSR
    ldr r5, [r4]
    orr r5, #RCC_CSR_LSION
    str r5, [r4]

wait_lsi:
    ldr r5, [r4]
    ands r5, #RCC_CSR_LSIRDY
    beq wait_lsi


    pop {r4 - r5, lr}
    bx lr 


###########################################

RCC_EnableLSE:

    push {r4 - r5, lr}

    REG_CLR RCC_CSR, #RCC_CSR_LSION
    
    REG_CLR RCC_BDCR, #RCC_BDCR_RTC_LSI
    REG_SET RCC_BDCR, #RCC_BDCR_RTC_LSE
    REG_SET RCC_BDCR, #RCC_BDCR_LSEON
    # r4 -> BDCR

wait_lse:
    ldr r5, [r4]
    ands r5, #RCC_BDCR_LSERDY
    beq wait_lse


    pop {r4 - r5, lr}
    bx lr 



###########################################
RCC_IsPLLActive:

    push {r4, r5, lr}

    ldr r4, =RCC_CFGR
    ldr r4, [r4]

    ands r5, r4, #RCC_CFGR_SWS_PLL
    ITE EQ
    moveq r0, #0
    movne r0, #1

    pop {r4, r5, lr}
    bx lr 



##############################################
RCC_StopPLL:

    push {r4, r5, lr}

    ldr r4, =RCC_CR
    ldr r5, [r4]
    bic r5, #RCC_CR_PLLON
    str r5, [r4]

wait_plloff:
    ldr r5, [r4]
    ands r5, #RCC_CR_PLLRDY
    bne wait_plloff
 
    pop {r4, r5, lr}
    bx lr 

###########################################
RCC_StartPLL:

    push {r4, r5, lr}

    ldr r4, =RCC_CR
    ldr r5, [r4]
    orr r5, #RCC_CR_PLLON
    str r5, [r4]

wait_pll:
    ldr r5, [r4]
    ands r5, #RCC_CR_PLLRDY
    beq wait_pll

    pop {r4, r5, lr}
    bx lr 


############################################
#r0: pll_mul 2..16
RCC_EnablePLL:
    
    push {r4 - r6, r8, r9, lr}
    cmp r0, #2 
    blt done_pll
    cmp r0, #16 
    bgt done_pll 
    
    mov r6, r0 
    
    bl RCC_IsPLLActive
    cmp r0, #1 
    beq done_pll
    
    bl RCC_StopPLL
    
    REG_CLR RCC_CFGR, #RCC_CFGR_PLLSRC_CLEARMASK
    
    bl RCC_IsHSIActive
    
    cmp r0, #1
    bne pll1

    REG_SET RCC_CFGR, #RCC_CFGR_PLLSRC_HSI_DIV2
    b pll2

pll1:

    bl RCC_IsHSEActive
    cmp r0, #1
    bne pll2
    REG_SET RCC_CFGR, #RCC_CFGR_PLLSRC_HSE

pll2:

    REG_CLR RCC_CFGR, #RCC_CFGR_SW_CLEARMASK
    REG_SET RCC_CFGR, #RCC_CFGR_SW_PLL
    mov r9, #0xff
    lsl r9, #18
    REG_CLR RCC_CFGR, r9
    sub r9, r6, #2 
    lsl r9, #18
    REG_SET RCC_CFGR, r9
    
    bl RCC_StartPLL

done_pll:
    
    pop {r4-r6, r8, r9, lr}

    bx lr    

######################################################
RCC_DisablePLL:

    push {r9, lr}

    bl RCC_IsPLLActive
    cmp r0, #0
    IT EQ
    bleq RCC_StopPLL
    mov r9, #0xff
    lsl r9, #18
    REG_CLR RCC_CFGR, r9

    pop {r9, lr}


######################################################
RCC_SetAHBPrescaler:
        
    push {r4-r6, r8, lr}

    cmp r0, #1 
    itt eq 
    moveq r0, #0 
    beq ahb_set
    

    ldr r4, =t_presc 
    mov r5, #0

cmp_presc:
    ldr r6, [r4], #4
    cmp r0, r6 
    itt eq 
    addeq r0, r5, #8 
    beq ahb_set

    add r5, #1
    cmp r5, #8
    bne cmp_presc
    
    b ahb_end

ahb_set:

    lsl r0, #4
    REG_CLR RCC_CFGR, #RCC_CFGR_AHB_HCLK_PRE_SYSCLK_CLEARMASK
    REG_SET RCC_CFGR, r0

ahb_end:
    pop {r4-r6, r8, lr}

    bx lr    
    
#################################################################
RCC_SetAPB1Prescaler:
        
    push {r4, r5, lr}    

    cmp r0, #1
    itt eq
    moveq r0, #RCC_CFGR_APB1_PCLK1_PRE_AHB_HCLK_DIV1
    beq apb1_set

    cmp r0, #2
    itt eq
    moveq r0, #RCC_CFGR_APB1_PCLK1_PRE_AHB_HCLK_DIV2
    beq apb1_set

    cmp r0, #4
    itt eq
    moveq r0, #RCC_CFGR_APB1_PCLK1_PRE_AHB_HCLK_DIV4
    beq apb1_set

    cmp r0, #8
    itt eq
    moveq r0, #RCC_CFGR_APB1_PCLK1_PRE_AHB_HCLK_DIV8
    beq apb1_set

    cmp r0, #16
    itt eq
    moveq r0, #RCC_CFGR_APB1_PCLK1_PRE_AHB_HCLK_DIV16
    beq apb1_set

    b apb1_end

apb1_set:
    REG_CLR RCC_CFGR, #RCC_CFGR_APB1_PCLK1_PRE_AHB_HCLK_CLEARMASK
    REG_SET RCC_CFGR, r0

apb1_end:
    pop {r4, r5, lr}

    bx lr    


#####################################################################
RCC_SetAPB2Prescaler:
        
    push {r4, r5, lr}

    cmp r0, #1
    itt eq
    moveq r0, #RCC_CFGR_APB2_PCLK2_PRE_AHB_HCLK_DIV1
    beq apb2_set

    cmp r0, #2
    itt eq
    moveq r0, #RCC_CFGR_APB2_PCLK2_PRE_AHB_HCLK_DIV2
    beq apb2_set

    cmp r0, #4
    itt eq
    moveq r0, #RCC_CFGR_APB2_PCLK2_PRE_AHB_HCLK_DIV4
    beq apb2_set

    cmp r0, #8
    itt eq
    moveq r0, #RCC_CFGR_APB2_PCLK2_PRE_AHB_HCLK_DIV8
    beq apb2_set

    cmp r0, #16
    itt eq
    moveq r0, #RCC_CFGR_APB2_PCLK2_PRE_AHB_HCLK_DIV16
    beq apb2_set

    b apb2_end
    
apb2_set:
    REG_CLR RCC_CFGR, #RCC_CFGR_APB2_PCLK2_PRE_AHB_HCLK_CLEARMASK
    REG_SET RCC_CFGR, r0
    
    
apb2_end:
    pop {r4, r5, lr}

    bx lr    

##############################################
RCC_SetADCPrescaler:
        
    push {r4, r5, lr} 

    cmp r0, #2
    itt eq
    moveq r0, #RCC_CFGR_ADCPRE_APB2_PCLK2_DIV2
    beq adc_set

    cmp r0, #4
    itt eq
    moveq r0, #RCC_CFGR_ADCPRE_APB2_PCLK2_DIV4
    beq adc_set

    cmp r0, #6
    itt eq
    moveq r0, #RCC_CFGR_ADCPRE_APB2_PCLK2_DIV6
    beq adc_set

    cmp r0, #8
    itt eq
    moveq r0, #RCC_CFGR_ADCPRE_APB2_PCLK2_DIV8
    beq adc_set

    b adc_end

adc_set:
    REG_CLR RCC_CFGR, #RCC_CFGR_ADCPRE_APB2_PCLK2_CLEARMASK
    REG_SET RCC_CFGR, r0
    
adc_end:
    pop {r4, r5, lr}

    bx lr    

##############################################
RCC_EnableRTC:
        
    push {r4, r5, lr}    

    ldr r4, =RCC_BDCR
    ldr r5, [r4]
    
    cmp r0, #1 
    ITE EQ 
    orreq r5, #RCC_BDCR_RTCEN
    bicne r5, #RCC_BDCR_RTCEN

    str r5, [r4]
    
    pop {r4, r5, lr}
    bx lr

######################################################
RCC_SetRTCClockSourceLSE:

    push {r4, r5, lr}    

    ldr r4, =RCC_BDCR
    ldr r5, [r4]

    bic r5, #RCC_BDCR_RTC_CLEARMASK
    orr r5, #RCC_BDCR_RTC_LSE

    str r5, [r4]
    
    pop {r4, r5, lr}
    bx lr

#####################################################
RCC_SetRTCClockSourceLSI:

    push {r4, r5, lr}    

    ldr r4, =RCC_BDCR
    ldr r5, [r4]

    bic r5, #RCC_BDCR_RTC_CLEARMASK
    orr r5, #RCC_BDCR_RTC_LSI

    str r5, [r4]
    
    pop {r4, r5, lr}
    bx lr



##############################################
RCC_SetRTCClockSourceHSE_DIV128:

    push {r4, r5, lr}    

    ldr r4, =RCC_BDCR
    ldr r5, [r4]

    bic r5, #RCC_BDCR_RTC_CLEARMASK
    orr r5, #RCC_BDCR_RTC_HSE128

    str r5, [r4]
 
    pop {r4, r5, lr}
    bx lr

    
.macro RCC_ENABLE en_addr, en_bit, rst_addr, rst_bit

    cmp r0, #1 
    beq rcc_en\@

    ldr r4, =\en_addr
    ldr r5, [r4]
    bic r5, \en_bit
    str r5, [r4]

    ldr r4, =\rst_addr
    ldr r5, [r4]
    orr r5, \rst_bit
    str r5, [r4]
  
    b rcc_en_end\@

rcc_en\@:

    ldr r4, =\rst_addr
    ldr r5, [r4]
    bic r5, \rst_bit
    str r5, [r4]

    ldr r4, =\en_addr
    ldr r5, [r4]
    orr r5, \en_bit
    str r5, [r4]


rcc_en_end\@:

.endm
    
##############################################
RCC_EnableBKP:
    push {r4, r5, lr}    

    RCC_ENABLE RCC_APB1ENR, #RCC_APB1ENR_BKPEN, RCC_APB1RSTR, #RCC_APB1RSTR_BKPRST

    pop {r4, r5, lr}
    bx lr



##############################################
RCC_EnablePWR:
    push {r4, r5, lr}    

    RCC_ENABLE RCC_APB1ENR, #RCC_APB1ENR_PWREN, RCC_APB1RSTR, #RCC_APB1RSTR_PWRRST

    pop {r4, r5, lr}
    bx lr

##############################################
RCC_EnablePeripheral:
    push {r4, r5, lr}

    ldr r4, [sp, #12]

    cmp r0, #1
    beq rcc_en

    ldr r5, [r1]
    bic r5, r2
    str r5, [r1]

    ldr r5, [r3]
    orr r5, r4
    str r5, [r3]

    b rcc_en_end

rcc_en:

    ldr r5, [r3]
    bic r5, r4
    str r5, [r3]

    ldr r5, [r1]
    orr r5, r2
    str r5, [r1]


rcc_en_end:

    pop {r4, r5, lr}
    bx lr



