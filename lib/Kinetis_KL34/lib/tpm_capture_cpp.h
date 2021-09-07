#ifndef _TPM_CAPTURE_CPP_H
#define _TPM_CAPTURE_CPP_H

#include "tpm_cpp.h"
#include "gpio.h"
#include "tpm_capture.h"

class TPM_PWM_Capture : public TPM_Capture_Handler, public TPM_TOF_Handler
{
public:
	TPM_PWM_Capture(TPM_Channel* ch, GPIO_PIN* pin);

	void SetUp(uint16_t maxPeriod_us);
	void GetValue(uint32_t& period, uint32_t& ds);
	virtual void HandleInterrupt (uint16_t val, TPM_Channel_t* ch);
	virtual void HandleInterrupt(TPM* tpm);

private:
	TPM_Channel* _ch;
	TPM_Capture_Context _ctx;

};


#endif
