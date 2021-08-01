#include "nvic.h"

#ifdef __cplusplus
extern "C"
{
#endif


void NVIC_SetEnable (IRQn_Type irqNo) 
{
    (&NVIC_ISER)[irqNo / 32] |= (1<<(irqNo % 32));
}

void NVIC_ClearEnable (IRQn_Type irqNo) 
{
    (&NVIC_ICER)[irqNo / 32] |= (1<<(irqNo % 32));
}

void NVIC_SetPending (IRQn_Type irqNo) 
{
    (&NVIC_ISPR)[irqNo / 32] |= (1<<(irqNo % 32));
}

void NVIC_ClearPending (IRQn_Type irqNo) 
{
    (&NVIC_ICPR)[irqNo / 32] |= (1<<(irqNo % 32));
}

int NVIC_IsActive (IRQn_Type irqNo) 
{
    return ((&NVIC_ISER)[irqNo / 32] & (1<<(irqNo % 32)));
}

void NVIC_SetPriority (IRQn_Type irqNo, uint8_t priority) 
{
    (&NVIC_IPR)[irqNo / 4] |= (priority<<4)<<((irqNo % 4)*8);
}

int NVIC_GetPriority (IRQn_Type irqNo)
{   
    return 0xf & (((&NVIC_IPR)[irqNo / 4]) >> (4 + (irqNo % 4)*8));
}


#ifdef __cplusplus
}
#endif



