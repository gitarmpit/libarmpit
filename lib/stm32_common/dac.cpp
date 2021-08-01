#include "dac_cpp.h"


#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_XD_VL) ||\
 (defined(STM32F4) && !defined(STM32F401xC)  && !defined(STM32F401xE) && !defined(STM32F401xx) && !defined(STM32F411xE))

#include "nvic.h"

void TIM6_DAC_IRQHandler()
{
    DAC::GetInstance()->HandleIrq();
}

DAC::DAC()
: Peripheral (&RCC_APB1ENR, &RCC_APB1RSTR, RCC_APB1ENR_DACEN)

{
    _dacChannels[0]._pDAC_DHR12L = &DAC_DHR12L1;
    _dacChannels[0]._pDAC_DHR12R = &DAC_DHR12R1;
    _dacChannels[0]._pDAC_DHR8R = &DAC_DHR8R1;
    _dacChannels[0]._pDAC_DOR = &DAC_DOR1;
    _dacChannels[0]._channelNo = DAC_CHANNEL_1;
    _dacChannels[1]._pDAC_DHR12L = &DAC_DHR12L2;
    _dacChannels[1]._pDAC_DHR12R = &DAC_DHR12R2;
    _dacChannels[1]._pDAC_DHR8R = &DAC_DHR8R2;
    _dacChannels[1]._pDAC_DOR = &DAC_DOR2;
    _dacChannels[1]._channelNo = DAC_CHANNEL_2;
    _irqHandler = 0;
}


void DAC::HandleIrq()
{
    DAC_CHANNEL channelNo;
    DAC_IRQ_Handler* handler = 0;

    for (int i = 0; i < 2; ++i)
    {
        if (_dacChannels[i].IsUnderrun())
        {
            _dacChannels[i].ClearUnderrunInterruptFlag();
            channelNo = (DAC_CHANNEL) (i);
            handler = _irqHandler;
            break;
        }
    }

    if (handler)
    {
        handler->HandleInterrupt(channelNo);
    }
}

void DAC_Channel::EnableChannel()
{
    DAC_CR |= (DAC_CR_EN1 << (_channelNo * 16));
}

bool DAC_Channel::IsUnderrun()
{
    return DAC_SR & (DAC_SR_DMAUDR1 << (_channelNo * 16));
}

void DAC_Channel::EnableUnderrunInterrupt()
{
    DAC_CR |= (DAC_CR_DMAUDRIE1 << (_channelNo * 16));
    NVIC_SetEnable(IRQn_TIM6_DAC);
}

void DAC_Channel::ClearUnderrunInterruptFlag()
{
    DAC_CR &= ~(DAC_CR_DMAUDRIE1 << (_channelNo * 16));
}

void DAC_Channel::EnableDMA()
{
    DAC_CR |= (DAC_CR_DMAEN1 << (_channelNo * 16));
}

void DAC_Channel::EnableNoiseGeneration()
{
    DAC_CR &= ((3 << 6) << (_channelNo * 16));
    DAC_CR |= (DAC_CR_WAVE1_NOISE << (_channelNo * 16));
}

void DAC_Channel::EnableTriangleWaveGeneration()
{
    DAC_CR &= ((3 << 6) << (_channelNo * 16));
    DAC_CR |= (DAC_CR_WAVE1_TRI << (_channelNo * 16));
}

void DAC_Channel::_SelectTrigger(uint32_t trigger)
{
    DAC_CR &= ((3 << 3) << (_channelNo * 16));
    DAC_CR |= (trigger << (_channelNo * 16));
    DAC_CR |= (DAC_CR_TEN1 << (_channelNo * 16));
}

void DAC_Channel::SelectTriggerTimer6()
{
    _SelectTrigger(DAC_CR_TSEL1_TIM6);
}
#ifdef STM32F4
void DAC_Channel::SelectTriggerTimer8()
{
    _SelectTrigger(DAC_CR_TSEL1_TIM8);
}
#else
void DAC_Channel::SelectTriggerTimer3()
{
    _SelectTrigger(DAC_CR_TSEL1_TIM3);
}
#endif

void DAC_Channel::SelectTriggerTimer7()
{
    _SelectTrigger(DAC_CR_TSEL1_TIM7);
}

void DAC_Channel::SelectTriggerTimer5_15()
{
    _SelectTrigger(DAC_CR_TSEL1_TIM5);
}

void DAC_Channel::SelectTriggerTimer2()
{
    _SelectTrigger(DAC_CR_TSEL1_TIM2);
}

void DAC_Channel::SelectTriggerTimer4()
{
    _SelectTrigger(DAC_CR_TSEL1_TIM4);
}

void DAC_Channel::EnableOutputBuffer(bool enable)
{
    if (enable)
    {
        DAC_CR &= ~(DAC_CR_BOFF1 << (_channelNo * 16));
    }
    else
    {
        DAC_CR |= (DAC_CR_BOFF1 << (_channelNo * 16));
    }
}

void DAC_Channel::EnableSoftwareTrigger(bool enable)
{
    if (enable)
    {
        DAC_SWTRIGR &= ~(DAC_SWTRIGR_1 << (_channelNo * 16));
    }
    else
    {
        DAC_SWTRIGR |= (DAC_SWTRIGR_1 << (_channelNo * 16));
    }
}

void DAC_Channel::SelectDataRegister12R()
{
    _dataRegister = _pDAC_DHR12R;
    _dataShift = 0;
    _dataBitmask = 0x4ff;
}

void DAC_Channel::SelectDataRegister12L()
{
    _dataRegister = _pDAC_DHR12L;
    _dataShift = 4;
    _dataBitmask = 0xffc;
}

void DAC_Channel::SelectDataRegister8R()
{
    _dataRegister = _pDAC_DHR8R;
    _dataShift = 0;
    _dataBitmask = 0xff;
}

void DAC_Channel::Write(uint16_t data)
{
    *_dataRegister = _dataBitmask & (data << _dataShift);
}

void DAC_Channel::SetAmplitude(DAC_AMPLITUDE amplitude)
{
    DAC_CR |= amplitude << (DAC_CR_AMPL1 + _channelNo * 16);
}


#endif
