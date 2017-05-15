#include "RCC_helper.h"

int main(void)
{
    RCC_EnableHSE_168Mhz();
    RCC_EnableHSI(false);
}
