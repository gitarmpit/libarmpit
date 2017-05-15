#include "syscfg.h"


void SYSCFG_RemapEXTI(EXTI_LINE line, GPIO_PORT_N port)
{
    (&SYSCFG_EXTICR1)[line / 4] |= (port << (4 * (line % 4)));
}

