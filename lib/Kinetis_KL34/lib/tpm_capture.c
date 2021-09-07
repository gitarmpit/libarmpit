#include "tpm_capture.h"

static uint16_t presc_array[] = {1,2,4,8,16,32,64,128};


static void tmp_capture_handler(TPM_Channel_t* ch, uint16_t val)
{
	//static int32_t diff;
	//static TPM_Capture_Context* ctx;

	TPM_Capture_Context* ctx = (TPM_Capture_Context*)ch->capture_ctx;

	if (ctx->val0 != -1)
	{
		int32_t diff = val - ctx->val0;

		if (diff < 0)
		{
			diff += ctx->mod + 1;
		}

		if (!GPIO_IsSet(ctx->pin))
		{
			ctx->duty_us = diff;
		}
		else if (ctx->duty_us != 0)
		{
			ctx->period_us = ctx->duty_us + diff;
		}

	}

	ctx->val0 = val;
	ctx->missed_count = 0;
}


void TPM_Capture_GetValue(TPM_Capture_Context* ctx, uint32_t* period_us, uint32_t* duty_us)
{
	*period_us = (uint64_t)ctx->period_us * presc_array[ctx->presc] * 1000000llu / CORE_FREQ;
	*duty_us = (uint64_t)ctx->duty_us * presc_array[ctx->presc] * 1000000llu / CORE_FREQ;
}

static void TOF_handler(void* ctx)
{
	TPM_Capture_Context* capture_ctx = (TPM_Capture_Context*)ctx;
	if (capture_ctx && capture_ctx->missed_count++ > capture_ctx->max_missed_count )
	{
		capture_ctx->val0 = -1;
		capture_ctx->period_us = 0;
		capture_ctx->duty_us = 0;
	}
}

void TPM_Capture_Setup(TPM_Capture_Context* ctx, TPM_Channel_t* ch, GPIO_PIN* pin, uint16_t maxPeriod_us)
{
	TPM* tpm = ch->tpm;
	uint16_t mod, presc;
	TPM_CalculateTimerValues(maxPeriod_us, &mod, &presc);
	*tpm->TPM_CNT = 0;
	TPM_SetModulo(tpm, mod);
	TPM_SetPrescaler(tpm, presc);

	tpm->TOF_handler = TOF_handler;
	tpm->handler_ctx = ctx;
	TPM_EnableInterrupt(tpm, TRUE);

	ctx->val0 = -1;
	ctx->period_us = 0;
	ctx->duty_us = 0;
	ctx->mod = mod;
	ctx->presc = presc;
	ctx->ch = ch;
	ctx->pin = pin;
	ctx->missed_count = 0;
	ctx->max_missed_count = 1000000/maxPeriod_us; // 1 second

	ch->capture_ctx = ctx;
	ch->CHF_handler = tmp_capture_handler;

	TPM_Channel_EnableInterrupt(ch, TRUE);
	TPM_Channel_SetupInputCaptureEitherEdge(ch);
}
