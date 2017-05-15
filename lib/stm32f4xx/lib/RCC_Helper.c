#include "Rcc_Helper.h" 

#ifdef __cplusplus
extern "C"
{
#endif


void RCC_EnableHSI_SetMhz(uint8_t mhz)
{
    static const int nmin = 192; //, nmax = 432;

    FLASH_EnableDCache();
    FLASH_EnableICache();
    FLASH_EnablePrefetchBuffer();


    //loop on error
    if (mhz>180 || mhz<24)
    {
    	while(1)
    		;
    }


    FLASH_SetWaitState((mhz-1)/30);

    RCC_EnableHSI(TRUE);

    volatile uint8_t pllm = 16;
    volatile uint16_t plln = 0;  //vco= 16mHz(HSI) / m  * n;   192 < vco < 432
    volatile uint16_t pllp = 0;  // sysclock = vco / pllp
    while(1)
    {
        plln = mhz*pllp;
        if (plln >= nmin || pllp == 8)
        {
            break;
        }
        pllp += 2;
    }

    //loop on error
    if (16/pllm*plln/pllp != mhz)
    {
    	while(1)
    		;
    }

    RCC_EnablePLL(pllm, plln, pllp);

    //loop on error
    if (RCC_GetSystemClock() != mhz*1000000)
    {
    	while(1)
    		;
    }


}

void RCC_EnableHSE_168Mhz()
{
    FLASH_EnableDCache();
    FLASH_EnableICache();
    FLASH_EnablePrefetchBuffer();

    FLASH_SetWaitState(5);

    RCC_EnableHSE(TRUE);
    RCC_EnableHSI(FALSE);
    uint8_t pllm = 8;
    uint16_t plln = 336;  //vco= 8mHz(HSE) * n / m
    uint16_t pllp = 2; //vco out / pllp = sysclock
    RCC_EnablePLL(pllm, plln, pllp);
    //RCC_SetAHBPrescalerDiv2();
}



//MAX AHB=168, MAX APB2=84, MAX APB1=42
void RCC_EnableHSI_168Mhz()
{
    FLASH_EnableDCache();
    FLASH_EnableICache();
    FLASH_EnablePrefetchBuffer();

    FLASH_SetWaitState(5);

    RCC_EnableHSI(TRUE);
    uint8_t pllm = 16;
    uint16_t plln = 336;
    uint16_t pllp = 2; //vco out / pllp = sysclock
    RCC_EnablePLL(pllm, plln, pllp);
    //RCC_SetAHBPrescalerDiv2();
    //RCC_SetAPB2PrescalerDiv2();
    //RCC_SetAPB1PrescalerDiv4();
}



void RCC_EnableHSI_100Mhz()
{
    FLASH_EnableDCache();
    FLASH_EnableICache();
    FLASH_EnablePrefetchBuffer();

    FLASH_SetWaitState(3);

    RCC_EnableHSI(TRUE);
    uint8_t pllm = 8;
    uint16_t plln = 200;
    uint16_t pllp = 4;
    RCC_EnablePLL(pllm, plln, pllp);
    RCC_SetAHBPrescalerDiv2();

}

void RCC_EnableHSI_84Mhz()
{
    FLASH_EnableDCache();
    FLASH_EnableICache();
    FLASH_EnablePrefetchBuffer();

    FLASH_SetWaitState(2);

    RCC_EnableHSI(TRUE);
    uint8_t pllm = 16;
    uint16_t plln = 336;
    uint16_t pllp = 4;
    RCC_EnablePLL(pllm, plln, pllp);
    RCC_SetAHBPrescalerDiv2();

}

void RCC_EnableHSI_56Mhz()
{
    FLASH_EnableDCache();
    FLASH_EnableICache();
    FLASH_EnablePrefetchBuffer();

    FLASH_SetWaitState(1);

    RCC_EnableHSI(TRUE);
    uint8_t pllm = 16;
    uint16_t plln = 336;
    uint16_t pllp = 6;
    RCC_EnablePLL(pllm, plln, pllp);
    RCC_SetAHBPrescalerDiv2();

}
void RCC_EnableHSI_72Mhz()
{
    FLASH_EnableDCache();
    FLASH_EnableICache();
    FLASH_EnablePrefetchBuffer();

    FLASH_SetWaitState(2);

    RCC_EnableHSI(TRUE);
    uint8_t pllm = 8;
    uint16_t plln = 216;
    uint16_t pllp = 6;
    RCC_EnablePLL(pllm, plln, pllp);
    RCC_SetAHBPrescalerDiv2();

}

void RCC_EnableHSI_32Mhz()
{
    FLASH_EnableDCache();
    FLASH_EnableICache();
    FLASH_EnablePrefetchBuffer();

    FLASH_SetWaitState(1);

    RCC_EnableHSI(TRUE);
    uint8_t pllm = 16;
    uint16_t plln = 256;
    uint16_t pllp = 8;
    RCC_EnablePLL(pllm, plln, pllp);
    RCC_SetAHBPrescalerDiv2();

}

void RCC_EnableHSI_64Mhz()
{
    FLASH_EnableDCache();
    FLASH_EnableICache();
    FLASH_EnablePrefetchBuffer();

    FLASH_SetWaitState(2);

    RCC_EnableHSI(TRUE);
    uint8_t pllm = 8;
    uint16_t plln = 256;
    uint16_t pllp = 8;
    RCC_EnablePLL(pllm, plln, pllp);
    RCC_SetAHBPrescalerDiv2();

}

void RCC_EnableHSI_128Mhz()
{
    FLASH_EnableDCache();
    FLASH_EnableICache();
    FLASH_EnablePrefetchBuffer();

    FLASH_SetWaitState(4);

    RCC_EnableHSI(TRUE);
    uint8_t pllm = 8;
    uint16_t plln = 256;
    uint16_t pllp = 4;
    RCC_EnablePLL(pllm, plln, pllp);
    RCC_SetAHBPrescalerDiv2();

}
