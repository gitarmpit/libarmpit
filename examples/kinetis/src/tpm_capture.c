#include "tpm_capture.h"

static uint16_t presc_array[] = {1,2,4,8,16,32,64,128};


static void tmp_capture_handler(TPM* tpm, uint16_t val)
{
	static int32_t diff;

	if (tpm->captureCtx->val0 != -1)
	{
		diff = val - tpm->captureCtx->val0;

		if (diff < 0)
		{
			diff += tpm->captureCtx->mod;
		}

		if (tpm->captureCtx->isRising)
		{
			tpm->captureCtx->duty_us = diff;
			tpm->captureCtx->isRising = FALSE;
		}
		else
		{
			tpm->captureCtx->period_us = tpm->captureCtx->duty_us + diff;
			tpm->captureCtx->isRising = TRUE;
		}

	}
	else
	{
		TPM_Channel_SetupInputCaptureEitherEdge(tpm->captureCtx->ch);
		tpm->captureCtx->isRising = TRUE;
	}

	tpm->captureCtx->val0 = val;
}

void TPM_Capture_GetValue(TPM_Capture_Context* ctx, uint32_t* period_us, uint32_t* duty_us)
{
	*period_us = (uint64_t)ctx->period_us * presc_array[ctx->presc] * 1000000llu / CORE_FREQ;
	*duty_us = (uint64_t)ctx->duty_us * presc_array[ctx->presc] * 1000000llu / CORE_FREQ;
}

void TPM_Capture_Setup(TPM_Capture_Context* ctx, TPM_Channel* ch, uint16_t maxPeriod_us)
{
	TPM* tpm = ch->tpm;
	tpm->CHF_handler = tmp_capture_handler;
	uint16_t mod, presc;
	TPM_CalculateTimerValues(maxPeriod_us, &mod, &presc);
	*tpm->TPM_CNT = 0;
	TPM_SetModulo(tpm, mod);
	TPM_SetPrescaler(tpm, presc);

	ctx->val0 = -1;
	ctx->period_us = 0;
	ctx->duty_us = 0;
	ctx->mod = mod;
	ctx->presc = presc;
	ctx->ch = ch;

	tpm->captureCtx = ctx;

	TPM_Channel_EnableInterrupt(ch, TRUE);
	TPM_Channel_SetupInputCaptureRisingEdge(ch);
	TPM_EnableCounter(tpm, TRUE);
}
