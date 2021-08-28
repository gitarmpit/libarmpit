#include "tpm_cpp.h"

#if !defined (TPM_C_HANDLER)

extern "C"
{

void TPM0_IRQHandler()
{
	TPM0::GetInstance()->HandleInterrupt();
}

void TPM1_IRQHandler()
{
	TPM1::GetInstance()->HandleInterrupt();
}

void TPM2_IRQHandler()
{
	TPM2::GetInstance()->HandleInterrupt();
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

    static uint32_t mask;
	static uint16_t status;

	status = *_tpm->TPM_STATUS;

    for (int i = 0; i <_tpm->channel_count; ++i)
    {
        mask = 1<<i;
		if ((status & mask) == mask)
        {
            *_tpm->TPM_STATUS |= mask;
            if (_channels[i]._captureHandler != NULL)
            {
            	_channels[i]._captureHandler->HandleInterrupt(*_channels[i]._ch->TPM_CnV, _channels[i]._ch);
                break;
            }
        }
    }

}


TPM_Base* TPM_Base::GetInstance(TPM_N n)
{
	if (n == TPM0_INSTANCE)
	{
		return TPM0::GetInstance();
	}
	else if (n == TPM1_INSTANCE)
	{
		return TPM1::GetInstance();
	}
	else if (n == TPM2_INSTANCE)
	{
		return TPM2::GetInstance();
	}
	else
	{
		while(1);
	}
}

void TPM_Base::EnableClock (bool enable)
{
	TPM_EnableClock(_tpm, enable);
}
void TPM_Base::EnableDMA(bool enable)
{
	TPM_EnableDMA(_tpm, enable);
}
void TPM_Base::ClearTOF()
{
	TPM_ClearTOF(_tpm);
}
bool TPM_Base::IsTOF()
{
	return TPM_IsTOF(_tpm);
}
void TPM_Base::EnableInterrupt(bool enable)
{
	TPM_EnableInterrupt(_tpm, enable);
}
void TPM_Base::EnableCounter(bool enable)
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

void TPM_Channel::SetUpdatePeriod_us(uint32_t us)
{
	TPM_Channel_SetUpdatePeriod_us(_ch->tpm, us);
}
void TPM_Channel::SetUpdatePeriod_ms(uint32_t ms)
{
	TPM_Channel_SetUpdatePeriod_ms(_ch->tpm, ms);
}
void TPM_Channel::SetupPWM(uint32_t period_us, uint32_t duty_us)
{
	TPM_Channel_SetupPWM(_ch, period_us, duty_us);
}
void TPM_Channel::UpdatePWMDuty(uint32_t duty_us)
{
	TPM_Channel_UpdatePWMDuty(_ch, duty_us);
}
void TPM_Channel::ClearCHF()
{
	TPM_Channel_ClearCHF(_ch);
}
bool TPM_Channel::IsCHF()
{
	return TPM_Channel_IsCHF(_ch);
}
void TPM_Channel::EnableDMA(bool enable)
{
	TPM_Channel_EnableDMA(_ch, enable);
}
void TPM_Channel::EnablePWM()
{
	TPM_Channel_EnablePWM(_ch);
}

void TPM_Channel::SetupInputCaptureRisingEdge()
{
	TPM_Channel_SetupInputCaptureRisingEdge(_ch);
}
void TPM_Channel::SetupInputCaptureFallingEdge()
{
	TPM_Channel_SetupInputCaptureFallingEdge(_ch);
}
void TPM_Channel::SetupInputCaptureEitherEdge()
{
	TPM_Channel_SetupInputCaptureEitherEdge(_ch);
}
void TPM_Channel::SetupInterruptHandler()
{

}
void TPM_Channel::EnableInterrupt(bool enable)
{
	TPM_Channel_EnableInterrupt(_ch, enable);
}

