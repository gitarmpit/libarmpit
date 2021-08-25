#ifndef _TPM_CAPTURE_H
#define _TPM_CAPTURE_H

#include "tpm.h"
#include "gpio_helper.h"

void TPM_Capture_GetValue(TPM_Capture_Context* ctx, uint32_t* period_us, uint32_t* duty_us);
void TPM_Capture_Setup(TPM_Capture_Context* ctx, TPM_Channel* ch, uint16_t maxPeriod_us);


#endif
