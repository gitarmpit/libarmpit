#include "afio.h"


#ifdef __cplusplus
extern "C"
{
#endif



void AFIO_Remap_SPI1(BOOL remap)
{
    if (remap)
        AFIO_MAPR |= AFIO_MAPR_SPI1_REMAP;
    else
        AFIO_MAPR &= ~AFIO_MAPR_SPI1_REMAP;
}

void AFIO_Remap_I2C1(BOOL remap)
{
    if (remap)
        AFIO_MAPR |= AFIO_MAPR_I2C1_REMAP;
    else
        AFIO_MAPR &= ~AFIO_MAPR_I2C1_REMAP;
}
void AFIO_Remap_USART1(BOOL remap)
{
    if (remap)
        AFIO_MAPR |= AFIO_MAPR_USART1_REMAP;
    else
        AFIO_MAPR &= ~AFIO_MAPR_USART1_REMAP;
}
void AFIO_Remap_USART2(BOOL remap)
{
    if (remap)
        AFIO_MAPR |= AFIO_MAPR_USART2_REMAP;
    else
        AFIO_MAPR &= ~AFIO_MAPR_USART2_REMAP;
}
void AFIO_Remap_USART3(BOOL remap, BOOL full_remap)
{
    AFIO_MAPR &= ~AFIO_MAPR_USART3_REMAP_CLEARMASK;
    if (remap)
    {
        if (full_remap)
        {
            AFIO_MAPR |= AFIO_MAPR_USART3_REMAP_FULL;
        }
        else
        {
            AFIO_MAPR |= AFIO_MAPR_USART3_REMAP_PARTIAL;
        }
    }
    else
    {
        AFIO_MAPR |= AFIO_MAPR_USART3_REMAP_NONE;
    }

}
void AFIO_Remap_TIM1(BOOL remap, BOOL full_remap)
{
    AFIO_MAPR &= ~AFIO_MAPR_TIM1_REMAP_CLEARMASK;
    if (remap)
    {
        if (full_remap)
        {
            AFIO_MAPR |= AFIO_MAPR_TIM1_REMAP_FULL;
        }
        else
        {
            AFIO_MAPR |= AFIO_MAPR_TIM1_REMAP_PARTIAL;
        }
    }
    else
    {
        AFIO_MAPR |= AFIO_MAPR_TIM1_REMAP_NONE;
    }
}
void AFIO_Remap_TIM2(BOOL remap, BOOL full_remap)
{
    AFIO_MAPR &= ~AFIO_MAPR_TIM2_REMAP_CLEARMASK;
    if (remap)
    {
        if (full_remap)
        {
            AFIO_MAPR |= AFIO_MAPR_TIM2_REMAP_FULL;
        }
        else
        {
            AFIO_MAPR |= AFIO_MAPR_TIM2_REMAP_1;
        }
    }
    else
    {
        AFIO_MAPR |= AFIO_MAPR_TIM2_REMAP_2;
    }
}
void AFIO_Remap_TIM3(BOOL remap, BOOL full_remap)
{
    AFIO_MAPR &= ~AFIO_MAPR_TIM3_REMAP_CLEARMASK;
    if (remap)
    {
        if (full_remap)
        {
            AFIO_MAPR |= AFIO_MAPR_TIM3_REMAP_FULL;
        }
        else
        {
            AFIO_MAPR |= AFIO_MAPR_TIM3_REMAP_PARTIAL;
        }
    }
    else
    {
        AFIO_MAPR |= AFIO_MAPR_TIM3_REMAP_NONE;
    }
}
void AFIO_Remap_TIM4(BOOL remap)
{
    if (remap)
        AFIO_MAPR |= AFIO_MAPR_TIM4_REMAP;
    else
        AFIO_MAPR &= ~AFIO_MAPR_TIM4_REMAP;
}

void AFIO_Remap_CAN(BOOL remap, BOOL full_remap)
{
    UNUSED(remap);
    UNUSED(full_remap);
}

void AFIO_Remap_PD01(BOOL remap)
{
    if (remap)
        AFIO_MAPR |= AFIO_MAPR_PD01_REMAP;
    else
        AFIO_MAPR &= ~AFIO_MAPR_PD01_REMAP;
}
void AFIO_Remap_TIM5CH4(BOOL remap)
{
    if (remap)
        AFIO_MAPR |= AFIO_MAPR_TIM5CH4_REMAP;
    else
        AFIO_MAPR &= ~AFIO_MAPR_TIM5CH4_REMAP;
}
void AFIO_Remap_ADC1_ETRGINJ(BOOL remap)
{
    if (remap)
        AFIO_MAPR |= AFIO_MAPR_ADC1_ETRGINJ_REMAP;
    else
        AFIO_MAPR &= ~AFIO_MAPR_ADC1_ETRGINJ_REMAP;
}
void AFIO_Remap_ADC1_ETRGREG(BOOL remap)
{
    if (remap)
        AFIO_MAPR |= AFIO_MAPR_ADC1_ETRGREG_REMAP;
    else
        AFIO_MAPR &= ~AFIO_MAPR_ADC1_ETRGREG_REMAP;
}
void AFIO_Remap_ADC2_ETRGINJ(BOOL remap)
{
    if (remap)
        AFIO_MAPR |= AFIO_MAPR_ADC2_ETRGINJ_REMAP;
    else
        AFIO_MAPR &= ~AFIO_MAPR_ADC2_ETRGINJ_REMAP;
}
void AFIO_Remap_ADC2_ETRGREG_(BOOL remap)
{
    if (remap)
        AFIO_MAPR |= AFIO_MAPR_ADC2_ETRGREG_REMAP;
    else
        AFIO_MAPR &= ~AFIO_MAPR_ADC2_ETRGREG_REMAP;
}

void AFIO_Remap_SWJ_CFG(BOOL enableJTAG, BOOL enableSW, BOOL enableNJRST)
{
    AFIO_MAPR &= ~AFIO_MAPR_SWJ_CFG_CLEARMASK;
    if (enableJTAG && enableSW && enableNJRST)
    {
        AFIO_MAPR |= AFIO_MAPR_SWJ_CFG_FULL_SWJ;
    }
    else if (enableJTAG && enableSW && !enableNJRST)
    {
        AFIO_MAPR |= AFIO_MAPR_SWJ_CFG_FULL_SWJ_NO_NJTRST;
    }
    else if (!enableJTAG && enableSW)
    {
        AFIO_MAPR |= AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON;
    }
    else if (!enableJTAG && !enableSW)
    {
        AFIO_MAPR |= AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_OFF;
    }

}

void AFIO_SetEventOutput(BOOL enable, GPIO_PORT_N port, GPIO_PIN_N pin)
{
    AFIO_EVCR = 0;
    if (enable)
    {
        AFIO_EVCR = AFIO_EVCR_EVOE | port | pin;
    }
}

void AFIO_RemapEXTI(EXTI_LINE line, GPIO_PORT_N port)
{
    (&AFIO_EXTICR1)[line / 4] |= (port << (4 * (line % 4)));

}

void AFIO_Remap_TIM9(BOOL remap)
{
    if (remap)
        AFIO_MAPR2 |= AFIO_MAPR2_TIM9_REMAP;
    else
        AFIO_MAPR2 &= ~AFIO_MAPR2_TIM9_REMAP;
}

void AFIO_Remap_TIM10(BOOL remap)
{
    if (remap)
        AFIO_MAPR2 |= AFIO_MAPR2_TIM10_REMAP;
    else
        AFIO_MAPR2 &= ~AFIO_MAPR2_TIM10_REMAP;
}

void AFIO_Remap_TIM13(BOOL remap)
{
    if (remap)
        AFIO_MAPR2 |= AFIO_MAPR2_TIM11_REMAP;
    else
        AFIO_MAPR2 &= ~AFIO_MAPR2_TIM13_REMAP;
}

void AFIO_Remap_TIM14(BOOL remap)
{
    if (remap)
        AFIO_MAPR2 |= AFIO_MAPR2_TIM14_REMAP;
    else
        AFIO_MAPR2 &= ~AFIO_MAPR2_TIM14_REMAP;
}


#ifdef __cplusplus
}
#endif

