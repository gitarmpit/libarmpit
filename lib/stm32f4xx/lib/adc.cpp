#include "adc_cpp.h"

#ifdef __cplusplus
extern "C"
{
#endif

void ADC_IRQHandler()
{
    if (ADC1_SR & ADC_SR_EOC)
    {
        if (ADC1_SR & ADC_SR_AWD)
        {
            ADC1::GetInstance()->HandleWatchdogInterrupt();
        }
        else
        {
            ADC1::GetInstance()->HandleInterrupt();
        }
    }
    else if (ADC1_SR & ADC_SR_JEOC)
    {
        if (ADC1_SR & ADC_SR_AWD)
        {
            ADC1::GetInstance()->HandleJWatchdogInterrupt();
        }
        else
        {
            ADC1::GetInstance()->HandleJInterrupt();
        }
    }

    if (ADC2_SR & ADC_SR_EOC)
    {
        if (ADC2_SR & ADC_SR_AWD)
        {
            ADC2::GetInstance()->HandleWatchdogInterrupt();
        }
        else
        {
            ADC2::GetInstance()->HandleInterrupt();
        }
    }
    else if (ADC2_SR & ADC_SR_JEOC)
    {
        if (ADC2_SR & ADC_SR_AWD)
        {
            ADC2::GetInstance()->HandleJWatchdogInterrupt();
        }
        else
        {
            ADC2::GetInstance()->HandleJInterrupt();
        }
    }

    if (ADC3_SR & ADC_SR_EOC)
    {
        if (ADC3_SR & ADC_SR_AWD)
        {
            ADC3::GetInstance()->HandleWatchdogInterrupt();
        }
        else
        {
            ADC3::GetInstance()->HandleInterrupt();
        }
    }
    else if (ADC3_SR & ADC_SR_JEOC)
    {
        if (ADC3_SR & ADC_SR_AWD)
        {
            ADC3::GetInstance()->HandleJWatchdogInterrupt();
        }
        else
        {
            ADC3::GetInstance()->HandleJInterrupt();
        }
    }
}

#ifdef __cplusplus
}
#endif

void ADC::HandleInterrupt()
{
    *_pADC_SR &= ~ADC_SR_EOC;
    *_pADC_SR &= ~ADC_SR_STRT;

    //bool isOVR = IsOverrun();

    if (_interruptHandler)
    {
        _interruptHandler->Handle(this);
    }
    //ClearOverrun();
}

void ADC::HandleJInterrupt()
{
    *_pADC_SR &= ~ADC_SR_JEOC;
    *_pADC_SR &= ~ADC_SR_JSTRT;

    //bool isOVR = IsOverrun();

    if (_jInterruptHandler)
    {
        _jInterruptHandler->Handle(this);
    }
    //ClearOverrun();
}

void ADC::HandleWatchdogInterrupt()
{
    *_pADC_SR &= ~ADC_SR_EOC;
    *_pADC_SR &= ~ADC_SR_STRT;
    *_pADC_SR &= ~ADC_SR_AWD;

    if (_watchdogHandler)
    {
        _watchdogHandler->Handle(this);
    }
}

void ADC::HandleJWatchdogInterrupt()
{
    *_pADC_SR &= ~ADC_SR_JEOC;
    *_pADC_SR &= ~ADC_SR_JSTRT;
    *_pADC_SR &= ~ADC_SR_AWD;

    if (_jWatchdogHandler)
    {
        _jWatchdogHandler->Handle(this);
    }
}

void ADC::Enable(bool enable)
{
    EnablePeripheralClock(enable);

    if (enable)
    {
        *_pADC_CR2 |= ADC_CR2_ADON;
    }
    else
    {
        *_pADC_CR2 &= ~ADC_CR2_ADON;
        _emptySequence = true;
        _emptyJSequence = true;

    }
}


void ADC::SetInterruptHandler(ADC_IRQ_Handler* interruptHandler)
{
    _interruptHandler = interruptHandler;
}

void ADC::SetJInterruptHandler(ADC_IRQ_Handler* interruptHandler)
{
    _jInterruptHandler = interruptHandler;
}

void ADC::StartContinuousConversion()
{
    *_pADC_CR2 |= ADC_CR2_CONT;
    *_pADC_CR2 |= ADC_CR2_SWSTART;

}

void ADC::StartContinuousJConversion()
{
    *_pADC_CR2 |= ADC_CR2_CONT;
    *_pADC_CR2 |= ADC_CR2_JSWSTART;

}


uint32_t ADC::SingleConversion()
{
    *_pADC_CR2 &= ~ADC_CR2_CONT;
    *_pADC_CR2 |= ADC_CR2_SWSTART;

    while ((*_pADC_SR & ADC_SR_EOC) == 0)
        ;

    *_pADC_SR &= ~ADC_SR_EOC;  //not needed if DR is read

    return 0xffff & *_pADC_DR;
}

uint32_t ADC::SingleJConversion()
{
    *_pADC_CR2 &= ~ADC_CR2_CONT;
    *_pADC_CR2 |= ADC_CR2_JSWSTART;

    while ((*_pADC_SR & ADC_SR_JEOC) == 0)
        ;

    *_pADC_SR &= ~ADC_SR_JEOC;

    return 0xffff & *_pADC_JDR1;
}


void ADC::SetChannelSampleTime(ADC_CHANNEL channel, ADC_SAMPLE_TIME sampleTime)
{

    int smpr_shift = (channel % 10) * 3;
    volatile uint32_t* pADC_SMPR;

    if (channel < 10)
    {
        pADC_SMPR = _pADC_SMPR2;
    }
    else
    {
        pADC_SMPR = _pADC_SMPR1;
    }

    *pADC_SMPR |= ((0x7 & sampleTime) << smpr_shift);

}

void ADC::AddConversion(ADC_CHANNEL channel, ADC_SAMPLE_TIME sampleTime)
{
    SetChannelSampleTime(channel, sampleTime);
    int current = 0xf & (*_pADC_SQR1 >> 20);

    if (current == 0xf)
    {
        return;
    }

    int seqno = 0xf & (current + 1);

    if (_emptySequence)
    {
        seqno = 0;
        _emptySequence = false;
    }

    *_pADC_SQR1 &= ~(0xf << 20);
    *_pADC_SQR1 |= (seqno << 20);

    volatile uint32_t* pADC_SQR;
    int sqr = 3 - seqno / 6;
    int sqr_shift = (seqno % 6) * 5;

    if (sqr == 3)
    {
        pADC_SQR = _pADC_SQR3;
    }
    else if (sqr == 2)
    {
        pADC_SQR = _pADC_SQR2;
    }
    else if (sqr == 1)
    {
        pADC_SQR = _pADC_SQR1;
    }
    else
    {
        pADC_SQR = 0;
    }

    if (pADC_SQR != 0)
    {
        *pADC_SQR |= ((0xff & channel) << sqr_shift);
    }

}

void ADC::AddJConversion(ADC_CHANNEL channel, ADC_SAMPLE_TIME sampleTime)
{
    SetChannelSampleTime(channel, sampleTime);

    int current = 0x3 & (*_pADC_JSQR >> 20);

    if (current == 3)
    {
        return;
    }

    int seqno = 0x3 & (current + 1);

    if (_emptyJSequence)
    {
        seqno = 0;
        _emptyJSequence = false;
    }

    *_pADC_JSQR &= ~(0x3 << 20);
    *_pADC_JSQR |= (seqno << 20);

    int sqr_shift = (3 - seqno) * 5;

    *_pADC_JSQR |= ((0xff & channel) << sqr_shift);

}

void ADC::EnableInterrupt(bool enable)
{
    _EnableInterrupt(ADC_CR1_EOCIE, enable);
}

void ADC::EnableJInterrupt(bool enable)
{
    _EnableInterrupt(ADC_CR1_JEOCIE, enable);
}

void ADC::_EnableInterrupt(uint16_t intMask, bool enable)
{
    if (enable)
    {
        *_pADC_CR1 |= intMask; //enable interrupt on EOC
        NVIC_SetEnable(IRQn_ADC);
    }
    else
    {
        *_pADC_CR1 &= ~intMask; //clear interrupt on EOC
    }
}

void ADC::SetLeftAlignment()
{
    *_pADC_CR2 |= ADC_CR2_ALIGN;
}

void ADC::SetRightAlignment()
{
    *_pADC_CR2 &= ~ADC_CR2_ALIGN;
}

void ADC::SetScanMode()
{
    *_pADC_CR1 |= ADC_CR1_SCAN;
}

void ADC::SetAutoInjection(bool on)
{
    if (on)
    {
        *_pADC_CR1 |= ADC_CR1_JAUTO;
    }
    else
    {
        *_pADC_CR1 &= ~ADC_CR1_JAUTO;
    }
}

void ADC::EnableWatchdog(float minV, float maxV,
        ADC_IRQ_Handler* watchdogHandler, ADC_CHANNEL channel)
{
    _EnableWatchdog(minV, maxV, watchdogHandler, channel, false);
}

void ADC::EnableJWatchdog(float minV, float maxV,
        ADC_IRQ_Handler* watchdogHandler, ADC_CHANNEL channel)
{
    _EnableWatchdog(minV, maxV, watchdogHandler, channel, true);
}

void ADC::_EnableWatchdog(float minV, float maxV,
        ADC_IRQ_Handler* watchdogHandler, ADC_CHANNEL channel, bool injected)
{
    int32_t min = 4096. / 3.3 * minV;
    int32_t max = 4096. / 3.3 * maxV;

    if (min >= 0 && min <= 4096 && max >= 0 && max <= 4096 && min < max)
    {
        *_pADC_LTR = min;
        *_pADC_HTR = max;
        *_pADC_CR1 |= injected ? ADC_CR1_JAWDEN : ADC_CR1_AWDEN;
        if (watchdogHandler != 0)
        {
            *_pADC_CR1 |= ADC_CR1_AWDIE;
            if (injected)
            {
                SetJWatchdogHandler(watchdogHandler);
                //EnableJInterrupt();
            }
            else
            {
                SetWatchdogHandler(watchdogHandler);
                //EnableInterrupt();
            }
        }

        if (channel != ADC_ALL)
        {
            *_pADC_CR1 |= ADC_CR1_AWDSGL;
            *_pADC_CR1 |= (0x1f & channel);
        }

    }

}

void ADC::SetWatchdogHandler(ADC_IRQ_Handler* watchdogHandler)
{
    _watchdogHandler = watchdogHandler;
}

void ADC::SetJWatchdogHandler(ADC_IRQ_Handler* jWatchdogHandler)
{
    _jWatchdogHandler = jWatchdogHandler;
}

void ADC::SetResolution12bit()
{
    *_pADC_CR1 &= ~ ADC_CR1_RES_CLEARMASK ;
    *_pADC_CR1 |= ADC_CR1_RES_12BIT;
}
void ADC::SetResolution10bit()
{
    *_pADC_CR1 &= ~ ADC_CR1_RES_CLEARMASK ;
    *_pADC_CR1 |= ADC_CR1_RES_10BIT;
}
void ADC::SetResolution8bit()
{
    *_pADC_CR1 &= ~ ADC_CR1_RES_CLEARMASK ;
    *_pADC_CR1 |= ADC_CR1_RES_8BIT;
}

void ADC::SetResolution6bit()
{
    *_pADC_CR1 &= ~ ADC_CR1_RES_CLEARMASK ;
    *_pADC_CR1 |= ADC_CR1_RES_6BIT;
}

void ADC::EnableOverrunInterrupt(bool enable)
{
    if (enable)
    {
        *_pADC_CR1 |= ADC_CR1_OVRIE;
    }
    else
    {
        *_pADC_CR1 &= ~ADC_CR1_OVRIE;
    }

}

bool ADC::IsOverrun()
{
    return (*_pADC_SR & ADC_SR_OVR);
}

void ADC::ClearOverrun()
{
    *_pADC_SR &= ~ADC_SR_OVR;
}

void ADC::SetPrescalerDIV2()
{
    ADC_CCR &= ~ADC_CCR_ADCPRE_CLEARMASK;
    ADC_CCR |= ADC_CCR_ADCPRE_DIV2;
}
void ADC::SetPrescalerDIV4()
{
    ADC_CCR &= ~ADC_CCR_ADCPRE_CLEARMASK;
    ADC_CCR |= ADC_CCR_ADCPRE_DIV4;
}

void ADC::SetPrescalerDIV6()
{
    ADC_CCR &= ~ADC_CCR_ADCPRE_CLEARMASK;
    ADC_CCR |= ADC_CCR_ADCPRE_DIV6;
}

void ADC::SetPrescalerDIV8()
{
    ADC_CCR &= ~ADC_CCR_ADCPRE_CLEARMASK;
    ADC_CCR |= ADC_CCR_ADCPRE_DIV8;
}


void ADC::EnableTempVref(bool enable)
{
    if (enable)
    {
        ADC_CCR |= ADC_CCR_TSVREFE;
    }
    else
    {
        ADC_CCR &= ~ADC_CCR_TSVREFE;
    }

}


void ADC::EnableVBAT (bool enable)
{
    if (enable)
    {
        ADC_CCR |= ADC_CCR_VBATE;
    }
    else
    {
        ADC_CCR &= ~ADC_CCR_VBATE;
    }

}

void ADC::EnableDMA(DMA_Stream* stream, uint8_t adc_channel_count, void* data_buffer)
{
    *_pADC_CR2 |= ADC_CR2_DMA;

    stream->SetPeripheralAddress((uint32_t)_pADC_DR);
    stream->SetMemory0Address((uint32_t) data_buffer);
    stream->SetCircularMode(true);
    stream->SetDirection_PeripheralToMemory();
    stream->SetMemorySize_16bits();
    stream->SetPeripheralSize_16bits();
    stream->SetNumberOfData(adc_channel_count);
    stream->SetMemoryIncrementMode(true);
    stream->SetPriorityHigh();

    stream->EnableStream(true);

}



