#include "tpm_cpp.h"

#if !defined (TPM_C_HANDLER)

extern "C"
{

void TPM0_IRQHandler()
{
	TPM_0::GetInstance()->HandleInterrupt();
}

void TPM1_IRQHandler()
{
	TPM_1::GetInstance()->HandleInterrupt();
}

void TPM2_IRQHandler()
{
	TPM_2::GetInstance()->HandleInterrupt();
}

}
#endif

void TPM_Base::HandleInterrupt()
{
    if (*_tpm->TPM_SC & TPM_SC_TOF)
    {
        *_tpm->TPM_SC |= TPM_SC_TOF;
        if (_TOF_handler != NULL)
        {
            _TOF_handler->HandleInterrupt(_tpm);
        }
    }

    /*
    static uint32_t mask;

    for (int i = 0; i <6; ++i)
    {
        mask = 1<<i;
        if ((*tpm->TPM_STATUS & mask) == mask)
        {
            *tpm->TPM_STATUS |= mask;
            if (tpm->CHF_handler != NULL && tpm->captureCtx != NULL)
            {
                tpm->CHF_handler(tpm, *(volatile uint32_t*)(tpm->TPM_CnV_BASE + i*8));
                break;
            }
        }
    }
    */

}

void TPM_Base::EnableClock (BOOL enable)
{
	TPM_EnableClock(_tpm, enable);
}
void TPM_Base::EnableDMA(BOOL enable)
{
	TPM_EnableDMA(_tpm, enable);
}
void TPM_Base::ClearTOF()
{
	TPM_ClearTOF(_tpm);
}
BOOL TPM_Base::IsTOF()
{
	return TPM_IsTOF(_tpm);
}
void TPM_Base::EnableInterrupt(BOOL enable)
{
	TPM_EnableInterrupt(_tpm, enable);
}
void TPM_Base::EnableCounter(BOOL enable)
{
	TPM_EnableCounter(_tpm, enable);
}
void TPM_Base::SetPrescaler(TPM_PRESC presc)
{
	TPM_SetPrescaler(_tpm, presc);
}
void TPM_Base::ClearCounter ()
{
	TPM_ClearCounter(_tpm);
}
uint16_t TPM_Base::GetCounterValue ()
{
	return TPM_GetCounterValue(_tpm);
}
void TPM_Base::SetModulo(uint16_t mod)
{
	TPM_SetModulo(_tpm, mod);
}
void TPM_Base::CalculateTimerValues (uint32_t us, uint16_t& count, uint16_t& presc)
{
    TPM_CalculateTimerValues(us, &count, &presc);
}

void TPM_Base::SetTimerPeriod_us(uint32_t us)
{
	uint16_t count, presc;
    TPM_CalculateTimerValues(us, &count, &presc);
    TPM_SetModulo(_tpm, count);
    TPM_SetPrescaler(_tpm, (TPM_PRESC)presc);
}
