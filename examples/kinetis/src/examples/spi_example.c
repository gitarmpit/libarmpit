#include "spi.h"

void test_spi()
{
	SPI* spi0 = SPI_GetInstance(SPI0);
	SPI_Initialize(spi0, TRUE, 2, SPI_DIV32, TRUE, TRUE, TRUE);
	uint8_t res = SPI_TransmitByte(spi0, 0xde);
	while(1)
		;
}
