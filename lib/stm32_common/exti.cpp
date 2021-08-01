#include "exti_cpp.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef USE_EXTI

#if defined(STM32F1) || defined(STM32F2) || defined(STM32F4)
void EXTI0_IRQHandler()
{
    EXTI0::GetInstance()->HandleInterrupt();
}
void EXTI1_IRQHandler()
{
    EXTI1::GetInstance()->HandleInterrupt();
}

void EXTI2_IRQHandler()
{
    EXTI2::GetInstance()->HandleInterrupt();
}

void EXTI3_IRQHandler()
{
    EXTI3::GetInstance()->HandleInterrupt();
}

void EXTI4_IRQHandler()
{
    EXTI4::GetInstance()->HandleInterrupt();
}

void EXTI9_5_IRQHandler()
{
    if (EXTI_PR & (1 << EXTI_LINE5))
        EXTI5::GetInstance()->HandleInterrupt();
    if (EXTI_PR & (1 << EXTI_LINE6))
        EXTI6::GetInstance()->HandleInterrupt();
    if (EXTI_PR & (1 << EXTI_LINE7))
        EXTI7::GetInstance()->HandleInterrupt();
    if (EXTI_PR & (1 << EXTI_LINE8))
        EXTI8::GetInstance()->HandleInterrupt();
    if (EXTI_PR & (1 << EXTI_LINE9))
        EXTI9::GetInstance()->HandleInterrupt();
}

void EXTI15_10_IRQHandler()
{
    if (EXTI_PR & (1 << EXTI_LINE10))
        EXTI10::GetInstance()->HandleInterrupt();
    if (EXTI_PR & (1 << EXTI_LINE11))
        EXTI11::GetInstance()->HandleInterrupt();
    if (EXTI_PR & (1 << EXTI_LINE12))
        EXTI12::GetInstance()->HandleInterrupt();
    if (EXTI_PR & (1 << EXTI_LINE13))
        EXTI13::GetInstance()->HandleInterrupt();
    if (EXTI_PR & (1 << EXTI_LINE14))
        EXTI14::GetInstance()->HandleInterrupt();
    if (EXTI_PR & (1 << EXTI_LINE15))
        EXTI15::GetInstance()->HandleInterrupt();
}

#elif defined(STM32L0)

void EXTI0_1_IRQHandler() 
{
    if (EXTI_PR & (1 << EXTI_LINE0))
        EXTI0::GetInstance()->HandleInterrupt();
    if (EXTI_PR & (1 << EXTI_LINE1))
        EXTI1::GetInstance()->HandleInterrupt();
}

void EXTI2_3_IRQHandler() 
{
    if (EXTI_PR & (1 << EXTI_LINE2))
        EXTI2::GetInstance()->HandleInterrupt();
    if (EXTI_PR & (1 << EXTI_LINE3))
        EXTI3::GetInstance()->HandleInterrupt();
}

void EXTI4_15_IRQHandler() 
{
    if (EXTI_PR & (1 << EXTI_LINE4))
        EXTI4::GetInstance()->HandleInterrupt();
    if (EXTI_PR & (1 << EXTI_LINE5))
        EXTI5::GetInstance()->HandleInterrupt();
    if (EXTI_PR & (1 << EXTI_LINE6))
        EXTI6::GetInstance()->HandleInterrupt();
    if (EXTI_PR & (1 << EXTI_LINE7))
        EXTI7::GetInstance()->HandleInterrupt();
    if (EXTI_PR & (1 << EXTI_LINE8))
        EXTI8::GetInstance()->HandleInterrupt();
    if (EXTI_PR & (1 << EXTI_LINE9))
        EXTI9::GetInstance()->HandleInterrupt();
    if (EXTI_PR & (1 << EXTI_LINE10))
        EXTI10::GetInstance()->HandleInterrupt();
    if (EXTI_PR & (1 << EXTI_LINE11))
        EXTI11::GetInstance()->HandleInterrupt();
    if (EXTI_PR & (1 << EXTI_LINE12))
        EXTI12::GetInstance()->HandleInterrupt();
    if (EXTI_PR & (1 << EXTI_LINE13))
        EXTI13::GetInstance()->HandleInterrupt();
    if (EXTI_PR & (1 << EXTI_LINE14))
        EXTI14::GetInstance()->HandleInterrupt();
    if (EXTI_PR & (1 << EXTI_LINE15))
        EXTI15::GetInstance()->HandleInterrupt();

}


#else
#error "Target device not defined"
#endif

#endif

#ifdef __cplusplus
}
#endif

EXTI* EXTI::GetInstance (EXTI_LINE line)
{
    switch (line)
    {
    case EXTI_LINE0:
        return EXTI0::GetInstance();
    case EXTI_LINE1:
        return EXTI1::GetInstance();
    case EXTI_LINE2:
        return EXTI2::GetInstance();
    case EXTI_LINE3:
        return EXTI3::GetInstance();
    case EXTI_LINE4:
        return EXTI4::GetInstance();
    case EXTI_LINE5:
        return EXTI5::GetInstance();
    case EXTI_LINE6:
        return EXTI6::GetInstance();
    case EXTI_LINE7:
        return EXTI7::GetInstance();
    case EXTI_LINE8:
        return EXTI8::GetInstance();
    case EXTI_LINE9:
        return EXTI9::GetInstance();
    case EXTI_LINE10:
        return EXTI10::GetInstance();
    case EXTI_LINE11:
        return EXTI11::GetInstance();
    case EXTI_LINE12:
        return EXTI0::GetInstance();
    case EXTI_LINE13:
        return EXTI13::GetInstance();
    case EXTI_LINE14:
        return EXTI14::GetInstance();
    case EXTI_LINE15:
        return EXTI15::GetInstance();
    case EXTI_LINE16:
        return EXTI16::GetInstance();
    case EXTI_LINE17:
        return EXTI17::GetInstance();
    case EXTI_LINE18:
        return EXTI18::GetInstance();
    case EXTI_LINE19:
        return EXTI19::GetInstance();
    };

    return 0;
}


void EXTI::HandleInterrupt()
{
    ClearPending();
    if (_irqHandler)
    {
        _irqHandler->HandleInterrupt(this);
    }

}

bool EXTI::IsPending()
{
    return (EXTI_PR & (1 << _line));
}
void EXTI::ClearPending()
{
    EXTI_PR |= (1 << _line);
}

void EXTI::Enable(bool enable, volatile uint32_t* edgeReg,
        volatile uint32_t* eventOrIntReg)
{
    if (enable)
    {
        if (!_isInternal)
        {
            NVIC_SetEnable(_irqnNo);
        }
        *edgeReg |= (1 << _line);
        *eventOrIntReg |= (1 << _line);
    }
    else
    {
        if (!_isInternal)
        {
            NVIC_ClearEnable(_irqnNo);
        }
        *edgeReg &= ~(1 << _line);
        *eventOrIntReg &= ~(1 << _line);
    }
}

void EXTI::EnableInterruptRisingEdge(bool enable)
{
    Enable(enable, &EXTI_RTSR, &EXTI_IMR);
}
void EXTI::EnableInterruptFallingEdge(bool enable)
{
    Enable(enable, &EXTI_FTSR, &EXTI_IMR);
}
void EXTI::EnableEventRisingEdge(bool enable)
{
    Enable(enable, &EXTI_RTSR, &EXTI_EMR);
}
void EXTI::EnableEventFallingEdge(bool enable)
{
    Enable(enable, &EXTI_FTSR, &EXTI_EMR);
}
void EXTI::GenerateSWI(bool enable)
{
    if (enable)
        EXTI_SWIER |= (1 << _line);
    else
        EXTI_SWIER &= ~(1 << _line);

}

