#include "spi.h"
#include "sim.h"

static SPI SPI_list[] =
{
    { &SIM_SCGC4, SIM_SCGC4_SPI0, &SPI0_S, &SPI0_BR, &SPI0_C2, &SPI0_C1, &SPI0_DL, &SPI0_DH, SPI0_IRQn, NULL },
    { &SIM_SCGC4, SIM_SCGC4_SPI1, &SPI1_S, &SPI1_BR, &SPI1_C2, &SPI1_C1, &SPI1_DL, &SPI1_DH, SPI1_IRQn, NULL },
};

SPI* SPI_GetInstance(SPI_N n)
{
    return &SPI_list[(uint8_t)n];
}

void SPI_Enable(SPI* spi,BOOL enable)
{
	SIM_Enable(spi->SIM_enableReg, spi->SIM_enableMask, TRUE);
	if (enable)
	{
		*spi->SPI_C1 |= SPI_C1_SPE;
	}
	else
	{
		*spi->SPI_C1 &= ~SPI_C1_SPE;
	}
}

void   SPI_Initialize(SPI* spi, BOOL isMaster, uint8_t presc, SPI_BRDIV brdiv, BOOL isPhaseStart, BOOL isPolarityLow, BOOL isLSB)
{
	SPI_Enable(spi, TRUE);

	if (isMaster)
	{
		*spi->SPI_C1 |= SPI_C1_MSTR;
	}
	else
	{
		*spi->SPI_C1 &= ~SPI_C1_MSTR;
	}

	if (isPhaseStart)
	{
		*spi->SPI_C1 |= SPI_C1_CPHA;
	}
	else
	{
		*spi->SPI_C1 &= ~SPI_C1_CPHA;
	}

	if (isPolarityLow)
	{
		*spi->SPI_C1 |= SPI_C1_CPOL;
	}
	else
	{
		*spi->SPI_C1 &= ~SPI_C1_CPOL;
	}

	if (isLSB)
	{
		*spi->SPI_C1 |= SPI_C1_LSBFE;
	}
	else
	{
		*spi->SPI_C1 &= ~SPI_C1_LSBFE;
	}

	if (presc < 1 || presc > 8)
	{
		while(1)
			;
	}

	*spi->SPI_BR &= ~SPI_BR_SPPR_CLEARMASK;
	*spi->SPI_BR |= ((presc-1)<<SPI_BR_SPPR_BIT);

	*spi->SPI_BR &= ~SPI_BR_SPR_CLEARMASK;
	*spi->SPI_BR |= ((uint8_t)brdiv<<SPI_BR_SPR_BIT);

}

uint8_t SPI_TransmitByte (SPI* spi, uint8_t byte)
{
    while (!(*spi->SPI_S & SPI_S_SPTEF))
        ;

    *spi->SPI_DL = byte;

    while (!(*spi->SPI_S & SPI_S_SPRF))
        ;

    return *spi->SPI_DL;
}

uint8_t SPI_TransmitRegister (SPI* spi, uint8_t reg, uint8_t data)
{
    SPI_TransmitByte(spi, reg);
    return SPI_TransmitByte(spi, data);
}

