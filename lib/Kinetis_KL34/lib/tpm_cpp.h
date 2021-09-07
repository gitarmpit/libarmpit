#ifndef TMP_CPP_H
#define TMP_CPP_H 
#include "tpm.h"

class TPM_TOF_Handler
{
public:
    virtual void HandleInterrupt(TPM* tpm) = 0;
};

class TPM_Channel;

class TPM_Capture_Handler
{
public:
    virtual void HandleInterrupt (uint16_t val, TPM_Channel_t* ch) = 0;
};

class TPM_Base;

class TPM_Channel
{
public:
    friend class TPM_Base;
    TPM_Channel()
    {
        _ch = NULL;
        _captureHandler = NULL;
        _tpm = NULL;
    }

    void SetUpdatePeriod_us(uint32_t us);
    void SetUpdatePeriod_ms(uint32_t ms);
    void SetupPWM(uint32_t period_us, uint32_t duty_us);
    void UpdatePWMDuty(uint32_t duty_us);
    void ClearCHF();
    bool IsCHF();
    void EnableDMA(bool enable);
    void EnablePWM();

    void SetupInputCaptureRisingEdge();
    void SetupInputCaptureFallingEdge();
    void SetupInputCaptureEitherEdge();
    void SetupInterruptHandler();
    void EnableInterrupt(bool enable);
    void SetCaptureHandler (TPM_Capture_Handler* handler)
    {
        _captureHandler = handler;
    }

    TPM_Base* GetTPM() { return _tpm; }

private:
    TPM_Channel_t*       _ch;
    TPM_Capture_Handler* _captureHandler;
    TPM_Base*            _tpm;
};

class TPM_Base
{
protected:
    TPM*             _tpm;
    TPM_TOF_Handler* _TOF_handler;
    TPM_Channel      _channels[6];
    int              _nchannels;

    explicit TPM_Base(TPM* tpm, int nchannels)
    {
        _tpm = tpm;
        _TOF_handler = 0;
        _nchannels = nchannels;
        for (int i = 0; i < nchannels; ++i)
        {
            _channels[i]._ch = &_tpm->ch[i];
            _channels[i]._tpm = this;
        }
    }

public:

    static TPM_Base* GetInstance(TPM_N n);

    void HandleInterrupt();
    void SetInterruptHandler(TPM_TOF_Handler* handler) { _TOF_handler = handler; }

    virtual TPM_Channel* GetChannel(uint8_t channel)
    {
        if (channel >= _nchannels)
        {
            while(1)
                ;
        }
        return &_channels[channel];
    }

    void EnableClock (bool enable);
    void EnableDMA(bool enable);
    void ClearTOF();
    bool IsTOF();
    void EnableInterrupt(bool enable);
    void EnableCounter(bool enable);
    void SetPrescaler(TPM_PRESC presc);
    void ClearCounter ();
    uint16_t GetCounterValue ();
    void SetModulo(uint16_t mod);
    static void CalculateTimerValues (uint32_t us, uint16_t& count, uint16_t& presc);
    void SetTimerPeriod_us(uint32_t us);
    void SetTimerPeriod_ms(uint32_t ms) { SetTimerPeriod_us(ms*1000); }

};

class TPM0 : public TPM_Base
{
private:
    TPM0() : TPM_Base (TPM_GetInstance(TPM0_INSTANCE), 6)
    {
    }

public:

    static TPM0* GetInstance()
    {
        static TPM0 instance;
        return &instance;
    }

};

class TPM1 : public TPM_Base
{
private:

    TPM1() : TPM_Base (TPM_GetInstance(TPM1_INSTANCE), 2) {}

public:

    static TPM1* GetInstance()
    {
        static TPM1 instance;
        return &instance;
    }

};

class TPM2 : public TPM_Base
{
private:

    TPM2() : TPM_Base (TPM_GetInstance(TPM2_INSTANCE), 2)
    {
    }

public:

    static TPM2* GetInstance()
    {
        static TPM2 instance;
        return &instance;
    }
};


#endif
