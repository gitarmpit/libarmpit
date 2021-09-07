#include "gpio_helper.h"

TPM_Channel* GPIO_Helper_SetupTPM_Channel(TPM_Pins pin)
{
	TPM_Channel_t* ch = GPIO_Helper_SetupTPM_Channel_t(pin);
	TPM_Base* tpm = TPM_Base::GetInstance(ch->tpm->instance);
	return tpm->GetChannel(ch->channe_number);
}
