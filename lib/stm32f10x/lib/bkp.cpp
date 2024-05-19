

#include "bkp.h"


#include "rcc.h"
#include "nvic.h"
#include "pwr.h"

#ifdef __cplusplus
extern "C"
{
#endif

void TAMPER_IRQHandler(void)
{
    BKP::GetInstance()->HandleInterrupt();
}

#ifdef __cplusplus
}
#endif

void BKP::HandleInterrupt()
{
    if (_intHandler)
    {
        if (IsTamperInterrupt())
        {
            ClearTamperInterrupt();
            _intHandler->HandleInterrupt(false);
        }
        if (IsTamperEvent())
        {
            ClearTamperEvent();
            _intHandler->HandleInterrupt(true);
        }
    }
}


void BKP::EnableWriteAccess(bool enable)
{
    PWR::GetInstance()->EnableBKPWriteAccess(enable);
}

void BKP::ResetBackupDomain()
{
    RCC_ResetBKP(true);
    RCC_ResetBKP(false);
}

void BKP::OutputRTCClock_div64(bool enable)
{

}

void BKP::OutputAlarmPulse(bool enable)
{
    if (enable)
    {
        BKP_RTCCR |= BKP_RTCCR_ASOE;
        BKP_RTCCR &= ~BKP_RTCCR_ASOS;
    }
    else
    {
        BKP_RTCCR &= ~BKP_RTCCR_ASOE;
    }
}
void BKP::OutputSecondPulse(bool enable)
{
    if (enable)
    {
        BKP_RTCCR |= BKP_RTCCR_ASOE;
        BKP_RTCCR |= BKP_RTCCR_ASOS;
    }
    else
    {
        BKP_RTCCR &= ~BKP_RTCCR_ASOE;
    }
}
void BKP::SetRTCCalibrationValue(uint8_t val)
{
    BKP_RTCCR |= (val & 0x3f);
}

volatile uint32_t* BKP::GetBackupRegAddr(uint8_t regno)
{
    uint32_t addr_offset =
            (regno <= 10) ? 0x4 * regno : 0x40 + 0x4 * (regno - 11);
    return (volatile uint32_t*) (BKP_BASE + addr_offset);
}

void BKP::WriteBackupData(uint16_t data, uint8_t regno)
{
    if (regno > 0 && regno <= 42)
    {
        *GetBackupRegAddr(regno) = data;
    }
}

uint16_t BKP::ReadBackupData(uint8_t regno)
{
    uint16_t res = 0;

    if (regno > 0 && regno <= 42)
    {
        res = *GetBackupRegAddr(regno);
    }

    return res;
}

void BKP::EnableTamperPin(bool enable, bool isActiveLow)
{
    if (enable)
    {
        if (isActiveLow)
            BKP_CR |= BKP_CR_TPAL;
        else
            BKP_CR &= ~BKP_CR_TPAL;

        BKP_CR |= BKP_CR_TPE;
    }
    else
    {
        BKP_CR &= ~BKP_CR_TPE;
    }
}

void BKP::EnableTamperInterrupt(bool enable,
        BKP_TamperInterrupt_Handler* handler)
{
    if (enable)
    {
        _intHandler = handler;
        BKP_CSR |= BKP_CSR_TPIE;
        NVIC_SetEnable(IRQn_TAMPER);
    }
    else
    {
        BKP_CSR &= ~BKP_CSR_TPIE;
        NVIC_ClearEnable(IRQn_TAMPER);
    }
}

bool BKP::IsTamperEvent()
{
    return (BKP_CSR & BKP_CSR_TEF);
}
bool BKP::IsTamperInterrupt()
{
    return (BKP_CSR & BKP_CSR_TIF);
}

void BKP::ClearTamperInterrupt()
{
    BKP_CSR |= BKP_CSR_CTI;
}

void BKP::ClearTamperEvent()
{
    BKP_CSR |= BKP_CSR_CTE;
}


