#include "tpm_capture_cpp.h"


TPM_PWM_Capture::TPM_PWM_Capture(TPM_Channel* ch, GPIO_PIN* pin)
{
	_ch = ch;
	_ctx.val0 = -1;
	_ctx.period_us = 0;
	_ctx.duty_us = 0;
	_ctx.mod = 0;
	_ctx.presc = 0;
	_ctx.ch = NULL;
	_ctx.pin = pin;
	_ctx.missed_count = 0;
	_ctx.max_missed_count = 0; // 1000000/maxPeriod_us; // 1 second
}

void TPM_PWM_Capture::HandleInterrupt(TPM* tpm)
{
	UNUSED(tpm);
    if (_ctx.missed_count++ > _ctx.max_missed_count )
    {
        _ctx.val0 = -1;
        _ctx.period_us = 0;
        _ctx.duty_us = 0;
    }
}

void TPM_PWM_Capture::HandleInterrupt (uint16_t val, TPM_Channel_t* ch)
{
	UNUSED(ch);

	if (_ctx.val0 != -1)
	{
		int32_t diff = val - _ctx.val0;

		if (diff < 0)
		{
			diff += _ctx.mod + 1;
		}

		if (!GPIO_IsSet(_ctx.pin))
		{
			_ctx.duty_us = diff;
		}
		else if (_ctx.duty_us != 0)
		{
			_ctx.period_us = _ctx.duty_us + diff;
		}

	}

	_ctx.val0 = val;
	_ctx.missed_count = 0;

}

void TPM_PWM_Capture::SetUp(uint16_t maxPeriod_us)
{
	TPM_Base* tpm = _ch->GetTPM();
    uint16_t mod, presc;
    TPM_CalculateTimerValues(maxPeriod_us, &mod, &presc);
    tpm->ClearCounter();
    tpm->SetModulo(mod);
    tpm->SetPrescaler((TPM_PRESC)presc);

    tpm->SetInterruptHandler(this);

    tpm->EnableInterrupt(true);

    _ctx.mod = mod;
    _ctx.presc = presc;
    _ctx.max_missed_count = 1000000/maxPeriod_us; // 1 second

    _ch->SetCaptureHandler(this);
    _ch->EnableInterrupt(true);
    _ch->SetupInputCaptureEitherEdge();

}

static uint16_t presc_array[] = {1,2,4,8,16,32,64,128};

void TPM_PWM_Capture::GetValue(uint32_t& period_us, uint32_t& duty_us)
{
	period_us = (uint64_t)_ctx.period_us * presc_array[_ctx.presc] * 1000000llu / CORE_FREQ;
	duty_us = (uint64_t)_ctx.duty_us * presc_array[_ctx.presc] * 1000000llu / CORE_FREQ;

}
