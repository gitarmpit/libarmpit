#include "Rcc_Helper.h" 

#ifdef __cplusplus
extern "C"
{
#endif

void RCC_EnableHSI_8Mhz(void)
{
	RCC_EnableHSI(TRUE);
}


void RCC_EnableHSI_16Mhz(void)
{
	RCC_EnableHSI(TRUE);
	RCC_EnablePLL(4);
}

void RCC_EnableHSI_24Mhz(void)
{
	FLASH_SetWaitState(1);
	RCC_EnableHSI(TRUE);
	RCC_EnablePLL(6);
}

// This is the max for HSI: 8Mhz/2 * 16 = 64
// AHB max: 72,  APB1 max: 36, APB2 max:  72
void RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz(void)
{
	FLASH_SetWaitState(2);
	RCC_EnableHSI(TRUE);
	RCC_EnablePLL(16);
	RCC_SetAPB1PrescalerDiv2();
}

void RCC_EnableHSE_72()
{
    FLASH_SetWaitState(2);
    RCC_EnableHSE(TRUE);
    RCC_EnablePLL(9);
    RCC_SetAPB1PrescalerDiv2();
}

void RCC_EnableHSI_64Mhz_AHB_32Mhz_APB1_16MHz_APB2_16MHz(void)
{
	FLASH_SetWaitState(2);
	RCC_EnableHSI(TRUE);
	RCC_EnablePLL(16);
	RCC_SetAHBPrescalerDiv2();
	RCC_SetAPB1PrescalerDiv2();
	RCC_SetAPB2PrescalerDiv2();
}



#ifdef __cplusplus
}
#endif
