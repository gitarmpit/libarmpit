#ifndef _TPM_CAPTURE_H
#define _TPM_CAPTURE_H

#include "tpm.h"
#include "gpio_helper.h"

#ifdef __cplusplus
 extern "C" {
#endif

 typedef struct
 {
     int32_t  val0;
     int32_t  period_us;
     int32_t  duty_us;
     uint16_t mod;
     uint16_t presc;
     TPM_Channel_t* ch;
     int32_t  missed_count;
     int32_t  max_missed_count;
     GPIO_PIN* pin;

 } TPM_Capture_Context;


void TPM_Capture_Setup(TPM_Capture_Context* ctx, TPM_Channel_t* ch, GPIO_PIN* pin, uint16_t maxPeriod_us);
void TPM_Capture_GetValue(TPM_Capture_Context* ctx, uint32_t* period_us, uint32_t* duty_us);

#ifdef __cplusplus
}
#endif


#endif
