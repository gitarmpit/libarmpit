#include "smc.h"

void SMC_AllowAllPowerModes()
{
	SMC_PMPROT = SMC_PMPROT_ALLS | SMC_PMPROT_AVLLS | SMC_PMPROT_AVLP;
}

uint32_t SMC_GetRunMode()
{
	return SMC_PMSTAT;
}

