#ifndef _DAC_CPP_H
#define _DAC_CPP_H

#include "dac.h"


#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_XD_VL) ||\
 (defined(STM32F4) && !defined(STM32F401xC)  && !defined(STM32F401xE) && !defined(STM32F401xx) && !defined(STM32F411xE))


#include "rcc.h"
#include "peripheral.h"

enum DAC_CHANNEL
{
    DAC_CHANNEL_1 = 0, DAC_CHANNEL_2 = 1
};

class DAC_IRQ_Handler;

class DAC_Channel
{
    friend class DAC;
private:
    volatile uint32_t* _pDAC_DHR12R;
    volatile uint32_t* _pDAC_DHR12L;
    volatile uint32_t* _pDAC_DHR8R;
    volatile uint32_t* _pDAC_DOR;
    volatile uint32_t* _dataRegister;
    uint8_t _dataShift;
    uint32_t _dataBitmask;

    DAC_CHANNEL _channelNo;

    DAC_Channel(DAC_Channel const&);
    void operator=(DAC_Channel const&);

    void _SelectTrigger(uint32_t timer);

protected:

    DAC_Channel()
    {
        _dataRegister = _pDAC_DHR12R;
        _dataShift = 0;
        _dataBitmask = 0x4ff;
    }

public:

    void EnableChannel();
    bool IsUnderrun();
    void EnableUnderrunInterrupt();
    void ClearUnderrunInterruptFlag();
    void EnableDMA();
    void EnableNoiseGeneration();
    void EnableTriangleWaveGeneration();
    void SelectTriggerTimer6();
#ifdef STM32F4
    void SelectTriggerTimer8();
#else
    void SelectTriggerTimer3();
#endif
    void SelectTriggerTimer7();
    void SelectTriggerTimer2();
    void SelectTriggerTimer4();
    void SelectTriggerTimer5_15();
    void EnableOutputBuffer(bool enable);
    void EnableSoftwareTrigger(bool enable);
    void SelectDataRegister12R();
    void SelectDataRegister12L();
    void SelectDataRegister8R();
    void Write(uint16_t data);
    void SetAmplitude(DAC_AMPLITUDE amplitude);

    volatile uint32_t* GetDataRegisterAddress()
    {
        return _dataRegister;
    }

};

class DAC: public Peripheral
{
public:

    DAC_Channel _dacChannels[2];

    static DAC* GetInstance()
    {
        static DAC instance;
        return &instance;
    }

private:

    DAC_IRQ_Handler* _irqHandler;

    DAC();

    DAC(DAC const&);
    void operator=(DAC const&);

public:

    DAC_Channel* GetChannel(DAC_CHANNEL channelNo)
    {
        return &_dacChannels[channelNo];
    }
    void SetIrqHandler(DAC_IRQ_Handler* handler)
    {
        _irqHandler = handler;
    }
    void HandleIrq();
};

class DAC_IRQ_Handler
{
public:
    virtual void HandleInterrupt(DAC_CHANNEL channelNo) = 0;
};

#endif

#endif
