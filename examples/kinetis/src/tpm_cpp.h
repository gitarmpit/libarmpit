#ifndef TMP_CPP_H
#define TMP_CPP_H 
#include "tpm.h"

class PWM_Capture_Handler
{
public:
    virtual void Handle (uint32_t period, uint32_t duty) = 0;
};

class TPM_TOF_Handler
{
public:
    virtual void HandleInterrupt(TPM* tpm) = 0;
    ~TPM_TOF_Handler() {}
};


class TPM_Base
{
private:
    TPM* _tpm;
protected:
    TPM_TOF_Handler* _TOF_handler;

    explicit TPM_Base(TPM* tpm)
    {
        _tpm = tpm;
        _TOF_handler = 0;
    }

    virtual ~TPM_Base() {}

public:

    void HandleInterrupt();
    void SetInterruptHandler(TPM_TOF_Handler* handler) { _TOF_handler = handler; }

    // TPM_Channel TPM_GetChannel(uint8_t channel);
    void EnableClock (BOOL enable);
    void EnableDMA(BOOL enable);
    void ClearTOF();
    BOOL IsTOF();
    void EnableInterrupt(BOOL enable);
    void EnableCounter(BOOL enable);
    void SetPrescaler(TPM_PRESC presc);
    void ClearCounter ();
    uint16_t GetCounterValue ();
    void SetModulo(uint16_t mod);
    static void CalculateTimerValues (uint32_t us, uint16_t& count, uint16_t& presc);
    void SetTimerPeriod_us(uint32_t us);
    void SetTimerPeriod_ms(uint32_t ms) { SetTimerPeriod_us(ms*1000); }
};

class TPM_0 : public TPM_Base
{
private:
    TPM_0() : TPM_Base (TPM_GetInstance(TPM0))
    {
    }

public:

    static TPM_0* GetInstance()
    {
        static TPM_0 instance;
        return &instance;
    }

};

class TPM_1 : public TPM_Base
{
private:
    TPM_1() : TPM_Base (TPM_GetInstance(TPM1))
    {
    }

public:

    static TPM_1* GetInstance()
    {
        static TPM_1 instance;
        return &instance;
    }

};

class TPM_2 : public TPM_Base
{
private:
    TPM_2() : TPM_Base (TPM_GetInstance(TPM2))
    {
    }

public:

    static TPM_2* GetInstance()
    {
        static TPM_2 instance;
        return &instance;
    }

};

#endif
