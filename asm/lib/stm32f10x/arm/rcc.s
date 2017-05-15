    PRESERVE8
    THUMB

    AREA    DR, DATA, READONLY


HSE_CLOCK_FREQ  EQU 4000000
HSI_CLOCK_FREQ  EQU 8000000    
    
    INCLUDE common.s
    INCLUDE rcc_regs.s 
    INCLUDE macros.s 

t_presc  DCD 2, 4, 8, 16, 64, 128, 256, 512



    AREA    CODE, CODE, READONLY

    EXPORT RCC_GetSystemClock 
    EXPORT RCC_GetAHB_CLK
    EXPORT RCC_GetAPBx_div
    EXPORT RCC_GetAPB1_CLK
    EXPORT RCC_GetAPB2_CLK
    EXPORT RCC_GetTIMx_CLK
    EXPORT RCC_EnableHSI
    EXPORT RCC_EnableHSE
    EXPORT RCC_BypassHSE
    EXPORT RCC_EnableLSI
    EXPORT RCC_EnableLSE
    EXPORT RCC_EnablePLL
    EXPORT RCC_DisablePLL
    EXPORT RCC_SetAHBPrescaler
    EXPORT RCC_SetAPB1Prescaler
    EXPORT RCC_SetAPB2Prescaler
    EXPORT RCC_SetADCPrescaler
    EXPORT RCC_EnableRTC
    EXPORT RCC_SetRTCClockSourceLSE
    EXPORT RCC_SetRTCClockSourceLSI
    EXPORT RCC_SetRTCClockSourceHSE_DIV128
    EXPORT RCC_EnableBKP
    EXPORT RCC_EnablePWR
    EXPORT RCC_EnablePeripheral  ;0: true/false, 1: en reg 2: rst reg  3: bitmask





RCC_GetSystemClock PROC
       
    push {r4 - r8, lr}

    mov r5, #1  ; freq multiplier
    
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
    
sclk_end

    mul r0, r0, r5

    pop {r4 - r8, lr}
    bx lr
        
    
    ENDP    


;###################################################
RCC_GetAHB_CLK PROC
    push {r4, lr}

;r0: sysclock
    bl RCC_GetSystemClock

    ldr r4, =RCC_CFGR
    ldr r1, [r4]
    lsr r1, r1, #4
    and r1, r1, #0xf
;r1: abh div         
    

    
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
    ENDP    

;###################################################
RCC_GetAPBx_div PROC
    push {r4, lr}

    ldr r4, =RCC_CFGR
    ldr r1, [r4]
    lsr r1, r1, r0   ; r0: passed as input
    and r1, r1, #0x7
;r1: apbx div         
    
    ldr r4, =t_presc
    
    ands r2, r1, #4 
    ITEEE EQ
    moveq r1, #1
    subne r1, #4
    lslne r1, #2
    ldrne r1, [r4, r1]

    mov r0, r1
    pop {r4, lr}
    bx lr
    
    ENDP    
        
;####################################################
RCC_GetAPB1_CLK  PROC

    push {r4, lr}

    bl RCC_GetAHB_CLK
    mov r4, r0

    mov r0, #8 ;APB1
    bl RCC_GetAPBx_div
    
    udiv r0, r4, r0

    pop {r4, lr}
    bx lr

    ENDP

;#####################################################
RCC_GetAPB2_CLK  PROC

    push {r4, lr}

    bl RCC_GetAHB_CLK
    mov r4, r0

    mov r0, #11 ;APB2
    bl RCC_GetAPBx_div
    
    udiv r0, r4, r0

    pop {r4, lr}
    bx lr

    ENDP


;#####################################################
;r0=1: APB1  r0=0: APB2

RCC_GetTIMx_CLK  PROC

    push {r4, r5, lr}

    mov r5, r0
    
    bl RCC_GetAHB_CLK
    mov r4, r0

;   r4 : ahb_clk 

    cmp r5, #0
    ITE EQ
    moveq r0, #11 ;APB2
    movne r0, #8 ;APB1
    
    bl RCC_GetAPBx_div
;   r0:  apbx_div  
    
    udiv r5, r4, r0

;  r5: timx_clk

    cmp r0, #1 
    lslgt r5, #1
    
    mov r0, r5

    pop {r4, r5, lr}
    bx lr

    ENDP

;##########################################
RCC_StartHSI  PROC

    push {r4, r5, lr}

    ldr r4, =RCC_CR
    ldr r5, [r4]
    ands r5, #RCC_CR_HSIRDY
    bne done_hsi

    orr r5, #RCC_CR_HSION
    str r5, [r4]

wait_hsi 
    ldr r5, [r4]
    ands r5, #RCC_CR_HSIRDY
    beq wait_hsi
    
done_hsi
    pop {r4, r5, lr}
    bx lr 
    ENDP

;##########################################
RCC_IsHSIActive  PROC

    push {r4, r5, lr}

    ldr r4, =RCC_CFGR
    ldr r4, [r4]


    ands r5, r4, #0xc
    cmp r5, #RCC_CFGR_SWS_HSI
    ITT EQ
    moveq r0, #TRUE
    beq ishsi_done


    cmp r5, #RCC_CFGR_SWS_PLL
    ITT NE
    movne r0, #FALSE
    bne ishsi_done
    
    and r5, r4, #RCC_CFGR_PLLSRC_CLEARMASK 
    cmp r5, #RCC_CFGR_PLLSRC_HSI_DIV2
    ITE EQ
    moveq r0, #TRUE
    movne r0, #FALSE

ishsi_done
    pop {r4, r5, lr}
    bx lr 
    ENDP

;#############################################
RCC_StopHSI PROC

    push {r4, r5, lr}

    ldr r4, =RCC_CR
    ldr r5, [r4]
    bic r5, #RCC_CR_HSION
    str r5, [r4]

wait_hsioff
    ldr r5, [r4]
    ands r5, #RCC_CR_HSIRDY
    bne wait_hsioff
    

    pop {r4, r5, lr}
    bx lr 

    ENDP

;##########################################
RCC_EnableHSI  PROC 

    push {r4, r5, lr}

    cmp r0, #TRUE
    beq hsi_start
    
    bl RCC_IsHSIActive
    cmp r0, #FALSE ;inactive, can stop
    IT EQ
    bleq RCC_StopHSI 
    b hsi_done

hsi_start
    ldr r4, =RCC_CFGR
    ldr r5, [r4]
    bic r5, #RCC_CFGR_SW_CLEARMASK
    str r5, [r4]

    bl RCC_StartHSI
    ldr r5, [r4]
    orr r5, #RCC_CFGR_SW_HSI
    str r5, [r4]
    

hsi_done

    pop {r4, r5, lr}
    bx lr 
    ENDP
        
;#############################################
RCC_StopHSE PROC

    push {r4, r5, lr}

    ldr r4, =RCC_CR
    ldr r5, [r4]
    bic r5, #RCC_CR_HSEON
    str r5, [r4]

wait_hseoff
    ldr r5, [r4]
    ands r5, #RCC_CR_HSERDY
    bne wait_hseoff
    

    pop {r4, r5, lr}
    bx lr 

    ENDP

;##########################################
RCC_StartHSE  PROC

    push {r4, r5, lr}

    ldr r4, =RCC_CR
    ldr r5, [r4]
    ands r5, #RCC_CR_HSERDY
    bne done_hse

    orr r5, #RCC_CR_HSEON
    str r5, [r4]

wait_hse 
    ldr r5, [r4]
    ands r5, #RCC_CR_HSERDY
    beq wait_hse
    
done_hse
    pop {r4, r5, lr}
    bx lr 
    ENDP

;##########################################
RCC_IsHSEActive  PROC

    push {r4, r5, lr}

    ldr r4, =RCC_CFGR
    ldr r4, [r4]


    ands r5, r4, #0xc
    cmp r5, #RCC_CFGR_SWS_HSE
    ITT EQ
    moveq r0, #TRUE
    beq ishse_done


    cmp r5, #RCC_CFGR_SWS_PLL
    ITT NE
    movne r0, #FALSE
    bne ishse_done

    and r5, r4, #RCC_CFGR_PLLSRC_CLEARMASK 
    cmp r5, #RCC_CFGR_PLLSRC_HSE
    ITE EQ
    moveq r0, #TRUE
    movne r0, #FALSE

ishse_done
    pop {r4, r5, lr}
    bx lr 
    ENDP


;##########################################
RCC_EnableHSE  PROC 

    push {r4 - r5, lr}

    cmp r0, #TRUE
    beq hse_start
    
    bl RCC_IsHSEActive
    cmp r0, #FALSE ;inactive, can stop
    IT EQ
    bleq RCC_StopHSE 
    b hse_done

hse_start
    ldr r4, =RCC_CFGR
    ldr r5, [r4]
    bic r5, #RCC_CFGR_SW_CLEARMASK
    str r5, [r4]

    bl RCC_StartHSE
    ldr r5, [r4]
    orr r5, #RCC_CFGR_SW_HSE
    str r5, [r4]
    

hse_done

    pop {r4 - r5, lr}
    bx lr 
    ENDP
        
;###########################################        
RCC_BypassHSE  PROC

    push {r4, r5, lr}
    ldr r4, =RCC_CR
    ldr r5, [r4]

    cmp r0, #TRUE
    ITE EQ 
    orreq r5, #RCC_CR_HSEBYP
    bicne r5, #RCC_CR_HSEBYP    

    pop {r4, r5, lr}
    bx lr
    ENDP


;##########################################
RCC_EnableLSI  PROC 

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

wait_lsi
    ldr r5, [r4]
    ands r5, #RCC_CSR_LSIRDY
    beq wait_lsi


    pop {r4 - r5, lr}
    bx lr 
    ENDP

;##########################################
RCC_EnableLSE  PROC 

    push {r4 - r5, lr}

    ldr r4, =RCC_CSR
    REG_CLR #RCC_CSR_LSION, r4, r5

    ldr r4, =RCC_BDCR
    REG_CLR #RCC_BDCR_RTC_LSI, r4, r5
    REG_SET #RCC_BDCR_RTC_LSE, r4, r5 
    REG_SET #RCC_BDCR_LSEON, r4, r5
    ; r4 -> BDCR

wait_lse
    ldr r5, [r4]
    ands r5, #RCC_BDCR_LSERDY
    beq wait_lse


    pop {r4 - r5, lr}
    bx lr 
    ENDP

;##########################################
RCC_IsPLLActive  PROC

    push {r4, r5, lr}

    ldr r4, =RCC_CFGR
    ldr r4, [r4]

    ands r5, r4, #RCC_CFGR_SWS_PLL
    ITE EQ
    moveq r0, #FALSE
    movne r0, #TRUE

    pop {r4, r5, lr}
    bx lr 
    ENDP


;#############################################
RCC_StopPLL PROC

    push {r4, r5, lr}

    ldr r4, =RCC_CR
    ldr r5, [r4]
    bic r5, #RCC_CR_PLLON
    str r5, [r4]

wait_plloff
    ldr r5, [r4]
    ands r5, #RCC_CR_PLLRDY
    bne wait_plloff
 
    pop {r4, r5, lr}
    bx lr 

    ENDP


;##########################################
RCC_StartPLL  PROC

    push {r4, r5, lr}

    ldr r4, =RCC_CR
    ldr r5, [r4]
    orr r5, #RCC_CR_PLLON
    str r5, [r4]

wait_pll
    ldr r5, [r4]
    ands r5, #RCC_CR_PLLRDY
    beq wait_pll

    pop {r4, r5, lr}
    bx lr 
    ENDP

;###########################################
;r0: pll_mul 2..16
RCC_EnablePLL PROC
    
    push {r4-r6, lr}
    cmp r0, #2 
    blt done_pll
    cmp r0, #16 
    bgt done_pll 
    
    mov r6, r0 
    
    bl RCC_IsPLLActive
    cmp r0, #TRUE
    beq done_pll
    
    bl RCC_StopPLL
    
    ldr r4, =RCC_CFGR
    REG_CLR #RCC_CFGR_PLLSRC_CLEARMASK, r4, r5
    
    bl RCC_IsHSIActive
    
    cmp r0, #TRUE
    bne pll1

    ldr r4, =RCC_CFGR
    REG_SET #RCC_CFGR_PLLSRC_HSI_DIV2, r4, r5
    b pll2

pll1

    bl RCC_IsHSEActive
    cmp r0, #TRUE
    bne pll2
    ldr r4, =RCC_CFGR
    REG_SET #RCC_CFGR_PLLSRC_HSE, r4, r5

pll2

    ldr r4, =RCC_CFGR
    REG_CLR #RCC_CFGR_SW_CLEARMASK, r4, r5
    REG_SET #RCC_CFGR_SW_PLL, r4, r5
    mov r3, #0xff 
    lsl r3, #18
    REG_CLR  r3, r4, r5
    
    sub r3, r6, #2 
    lsl r3, #18
    REG_SET r3, r4, r5
    
    bl RCC_StartPLL
    
 

done_pll 
    
    pop {r4-r6, lr}

    bx lr    
    ENDP    

RCC_DisablePLL PROC

    push {r4-r5, lr}
    bl RCC_IsPLLActive
    cmp r0, #FALSE 
    IT EQ 
    bleq RCC_StopPLL
    mov r3, #0xff 
    lsl r3, #18
    ldr r4, =RCC_CFGR
    REG_CLR r3, r4, r5

    pop {r4-r5, lr}
    bx lr
    ENDP

;===========================================
RCC_SetAHBPrescaler PROC
        
    push {r4 - r8, lr}    

    cmp r0, #1
    itt eq 
    moveq r0, #0
    beq ahb_set
    

    ldr r4, =t_presc 
    mov r5, #0

cmp_presc
    ldr r6, [r4], #4
    cmp r0, r6 
    itt eq 
    addeq r0, r5, #8 
    beq ahb_set

    add r5, #1
    cmp r5, #8
    bne cmp_presc
    
    b ahb_end

ahb_set

    ldr r4, =RCC_CFGR
    REG_CLR #RCC_CFGR_AHB_HCLK_PRE_SYSCLK_CLEARMASK, r4, r5    
    lsl r0, #4
    REG_SET r0, r4, r5

ahb_end
    pop {r4 - r8, lr}

    bx lr    
    
    ALIGN
    LTORG
    
    ENDP    



RCC_SetAPB1Prescaler PROC
        
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

apb1_set    
    ldr r4, =RCC_CFGR
    REG_CLR #RCC_CFGR_APB1_PCLK1_PRE_AHB_HCLK_CLEARMASK, r4, r5    
    REG_SET r0, r4, r5

apb1_end
    pop {r4, r5, lr}

    bx lr    
    ENDP    


RCC_SetAPB2Prescaler PROC
        
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
    
apb2_set
    ldr r4, =RCC_CFGR
    REG_CLR #RCC_CFGR_APB2_PCLK2_PRE_AHB_HCLK_CLEARMASK, r4, r5
    REG_SET r0, r4, r5
    
    
apb2_end    
    pop {r4, r5, lr}

    bx lr    
    ENDP    


RCC_SetADCPrescaler PROC
        
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

adc_set
    ldr r4, =RCC_CFGR
    REG_CLR #RCC_CFGR_ADCPRE_APB2_PCLK2_CLEARMASK, r4, r5    
    REG_SET r0, r4, r5
    
adc_end    
    pop {r4, r5, lr}

    bx lr    
    ENDP    


RCC_EnableRTC PROC
        
    push {r4, r5, lr}    

    ldr r4, =RCC_BDCR
    ldr r5, [r4]
    
    cmp r0, #TRUE 
    ITE EQ 
    orreq r5, #RCC_BDCR_RTCEN
    bicne r5, #RCC_BDCR_RTCEN

    str r5, [r4]
    
    pop {r4, r5, lr}
    bx lr
    ENDP
        
RCC_SetRTCClockSourceLSE PROC 

    ldr r2, =RCC_BDCR
    ldr r3, [r2]

    bic r3, #RCC_BDCR_RTC_CLEARMASK
    orr r3, #RCC_BDCR_RTC_LSE

    str r3, [r2]
    
    bx lr
    ENDP

RCC_SetRTCClockSourceLSI PROC 

    ldr r2, =RCC_BDCR
    ldr r3, [r2]

    bic r3, #RCC_BDCR_RTC_CLEARMASK
    orr r3, #RCC_BDCR_RTC_LSI

    str r3, [r2]
    
    bx lr
    ENDP

RCC_SetRTCClockSourceHSE_DIV128 PROC 


    ldr r2, =RCC_BDCR
    ldr r3, [r2]

    bic r3, #RCC_BDCR_RTC_CLEARMASK
    orr r3, #RCC_BDCR_RTC_HSE128

    str r3, [r2]
 
    bx lr
    ENDP

    MACRO 
    
$label   RCC_ENABLE $en_addr, $en_bit, $rst_addr, $rst_bit 

    cmp r0, #TRUE 
    beq $label.rcc_en

    ldr r4, $en_addr
    ldr r5, [r4]
    bic r5, $en_bit 
    str r5, [r4]

    ldr r4, $rst_addr
    ldr r5, [r4]
    orr r5, $rst_bit 
    str r5, [r4]
  
    b $label.rcc_en_end

$label.rcc_en    

    ldr r4, $rst_addr
    ldr r5, [r4]
    bic r5, $rst_bit 
    str r5, [r4]

    ldr r4, $en_addr
    ldr r5, [r4]
    orr r5, $en_bit 
    str r5, [r4]


$label.rcc_en_end     
    MEND
    
;####################################################################################
RCC_EnableBKP PROC
    push {r4, r5, lr}    

BKP    RCC_ENABLE =RCC_APB1ENR, #RCC_APB1ENR_BKPEN, =RCC_APB1RSTR, #RCC_APB1RSTR_BKPRST

    pop {r4, r5, lr}
    bx lr
    ENDP

;####################################################################################
RCC_EnablePWR PROC
    push {r4, r5, lr}    

PWR    RCC_ENABLE =RCC_APB1ENR, #RCC_APB1ENR_PWREN, =RCC_APB1RSTR, #RCC_APB1RSTR_PWRRST

    pop {r4, r5, lr}
    bx lr
    ENDP

;#####################################################################################
;0: true/false, 1: en reg 2: rst reg  3: bitmask
RCC_EnablePeripheral PROC
    push {r4, lr}

;    ldr r4, [sp, #12] ; 12: previous push 3 regs x 4 bytes each

    cmp r0, #TRUE
    beq rcc_en

    ldr r4, [r1]
    bic r4, r3
    str r4, [r1]

    ldr r4, [r2]
    orr r4, r3
    str r4, [r2]

    b rcc_en_end

rcc_en

    ldr r4, [r2]
    bic r4, r3
    str r4, [r2]

    ldr r4, [r1]
    orr r4, r3
    str r4, [r1]
  

rcc_en_end

    pop {r4, lr}
    bx lr
    ENDP

    ALIGN 
        
    END
