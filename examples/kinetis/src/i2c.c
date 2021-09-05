#include "i2c.h"
#include "sim.h"
#include "nvic.h"

static I2C I2C_list[] =
{
		{ &SIM_SCGC4, SIM_SCGC4_I2C0, &I2C0_A1, &I2C0_F, &I2C0_C1, &I2C0_S,
				&I2C0_D, &I2C0_C2, &I2C0_FLT, &I2C0_RA, &I2C0_SLTH, &I2C0_SLTL,
				I2C0_IRQn, NULL },

		{ &SIM_SCGC4, SIM_SCGC4_I2C1, &I2C1_A1, &I2C1_F, &I2C1_C1, &I2C1_S,
				&I2C1_D, &I2C1_C2, &I2C1_FLT, &I2C1_RA, &I2C1_SLTH, &I2C1_SLTL,
				I2C1_IRQn, NULL },

};

static const uint16_t I2C_DividerTable[] =
{ 20, 22, 24, 26, 28, 30, 34, 40, 28, 32, 36, 40, 44, 48, 56, 68, 48, 56, 64,
		72, 80, 88, 104, 128, 80, 96, 112, 128, 144, 160, 192, 240, 160, 192,
		224, 256, 288, 320, 384, 480, 320, 384, 448, 512, 576, 640, 768, 960,
		640, 768, 896, 1024, 1152, 1280, 1536, 1920, 1280, 1536, 1792, 2048,
		2304, 2560, 3072, 3840 };

static uint8_t I2C_MasterStartAsync(I2C *i2c, uint8_t address, BOOL isRead);
static void I2C_MasterSetBaudRate(I2C* i2c, uint32_t baudRate_Bps,
		uint32_t srcClock_Hz);

static void I2C_Handler(I2C* i2c)
{
	*i2c->I2C_S = I2C_S_IICIF;
	if (!i2c->ctx)
	{
		return;
	}

	switch (i2c->ctx->state)
	{
	case I2C_ASYNC_STATE_START_SENT:
	{
		if (*i2c->I2C_S & I2C_S_RXAK)
		{
			*i2c->I2C_S = I2C_S_RXAK;
			i2c->ctx->error = I2C_ERROR_RXAK;
		}
		else if (*i2c->I2C_S & I2C_S_ARBL)
		{
			*i2c->I2C_S = I2C_S_ARBL;
			i2c->ctx->error = I2C_ERROR_ARBL;
		}
		else
		{
			i2c->ctx->state = I2C_ASYNC_STATE_REG_SENT;
			uint32_t retry = i2c->ctx->waitRetry;
			if (retry < 100)
			{
				retry = 100;
			}
			i2c->ctx->error = I2C_ERROR_SUCCESS;
			while (!(*i2c->I2C_S & I2C_S_TCF))
			{
				__nop();
				if (retry-- == 0)
				{
					i2c->ctx->error = I2C_ERROR_TIMEOUT;
				}
			}
			if (i2c->ctx->error == I2C_ERROR_SUCCESS)
			{
				*i2c->I2C_C1 |= I2C_C1_TX;
				*i2c->I2C_D = i2c->ctx->reg;
			}
			//        	return;
		}
		break;
	}
	case I2C_ASYNC_STATE_REG_SENT:
	{
		if (*i2c->I2C_S & I2C_S_ARBL)
		{
			*i2c->I2C_S = I2C_S_ARBL;
			i2c->ctx->error = I2C_ERROR_ARBL;
		}
		else if (i2c->ctx->isRead)
		{
			i2c->ctx->error = I2C_MasterStartAsync(i2c, i2c->ctx->slaveAddr, TRUE);
			if (i2c->ctx->error == I2C_ERROR_SUCCESS)
			{
				i2c->ctx->state = I2C_ASYNC_STATE_REPEATSTART_SENT;
				//return;
			}
		}
		break;
	}
	case I2C_ASYNC_STATE_REPEATSTART_SENT:
	{
		*i2c->I2C_C1 &= ~(I2C_C1_TX | I2C_C1_TXAK);
		if (i2c->ctx->dataSize == 1)
		{
			/* Issue NACK on read. */
			*i2c->I2C_C1 |= I2C_C1_TXAK;
		}
		volatile uint8_t dummy = 0;
		UNUSED(dummy);
		// Dummy read to clear TCF
		dummy = *i2c->I2C_D;
		i2c->ctx->state = I2C_ASYNC_STATE_READ_STARTED;
		//return;
		break;
	}
	case I2C_ASYNC_STATE_READ_STARTED:
	{
		i2c->ctx->error = I2C_ERROR_SUCCESS;
		i2c->ctx->dataSize--;
		if (i2c->ctx->dataSize == 0)
		{
			i2c->ctx->error = I2C_MasterStop(i2c);
			i2c->ctx->state = I2C_ASYNC_STATE_DONE;
		}
		else if (i2c->ctx->dataSize == 1)
		{
			/* Issue NACK on read. */
			*i2c->I2C_C1 |= I2C_C1_TXAK;
		}

		*i2c->ctx->data++ = *i2c->I2C_D;
		break;
	}
	case I2C_ASYNC_STATE_INIT:
		break;
	default:
	{
		i2c->ctx->error = I2C_ERROR_UNEXPECTED_STATE;
		break;
	}
	}

	if (i2c->ctx->error != I2C_ERROR_SUCCESS)
	{
		i2c->ctx->state = I2C_ASYNC_STATE_DONE;
	}

	if (i2c->ctx->state == I2C_ASYNC_STATE_DONE)
	{
		I2C_EnableInterrupt(i2c, FALSE);
	}
}

void I2C0_IRQHandler()
{
	I2C_Handler(&I2C_list[(uint8_t) I2C0]);
}

void I2C1_IRQHandler()
{
	I2C_Handler(&I2C_list[(uint8_t) I2C1]);
}

I2C* I2C_GetInstance(I2C_N n)
{
	return &I2C_list[(uint8_t) n];
}

void I2C_EnableClock(I2C* i2c, BOOL enable)
{
	SIM_Enable(i2c->SIM_enableReg, i2c->SIM_enableMask, enable);
}

void I2C_MasterInit(I2C* i2c, uint32_t baudRate_Bps)
{
	I2C_EnableClock(i2c, TRUE);

	*i2c->I2C_A1 = 0;
	*i2c->I2C_F = 0;
	*i2c->I2C_C1 = 0;
	*i2c->I2C_S = 0xFFU;
	*i2c->I2C_C2 = 0;
	*i2c->I2C_FLT = 0; // 0x40U; // for STOP detect
	*i2c->I2C_RA = 0;

	/* Disable I2C prior to configuring it. */
	*i2c->I2C_C1 &= ~(I2C_C1_IICEN);

	I2C_MasterSetBaudRate(i2c, baudRate_Bps, CORE_FREQ);

	*i2c->I2C_C1 = (I2C_C1_IICEN | I2C_C1_IICIE);
}

void I2C_Reset(I2C* i2c)
{
	*i2c->I2C_C1 = 0;
	I2C_EnableClock(i2c, FALSE);
}

void I2C_EnableInterrupt(I2C* i2c, BOOL enable)
{
	if (enable)
	{
		NVIC_ClearPending(i2c->irq);
		NVIC_SetEnable(i2c->irq);
	}
	else
	{
		NVIC_ClearPending(i2c->irq);
		NVIC_ClearEnable(i2c->irq);
	}
}

static void I2C_MasterSetBaudRate(I2C* i2c, uint32_t baudRate_Bps,
		uint32_t srcClock_Hz)
{
	uint32_t multiplier;
	uint32_t computedRate;
	uint32_t absError;
	uint32_t bestError = UINT32_MAX;
	uint32_t bestMult = 0u;
	uint32_t bestIcr = 0u;
	uint8_t mult;
	uint8_t i;

	/* Search for the settings with the lowest error. Mult is the MULT field of the I2C_F register,
	 * and ranges from 0-2. It selects the multiplier factor for the divider. */
	for (mult = 0u; (mult <= 2u) && (bestError != 0); ++mult)
	{
		multiplier = 1u << mult;

		/* Scan table to find best match. */
		for (i = 0u; i < sizeof(I2C_DividerTable) / sizeof(uint16_t); ++i)
		{
			computedRate = srcClock_Hz / (multiplier * I2C_DividerTable[i]);
			absError =
					baudRate_Bps > computedRate ?
							(baudRate_Bps - computedRate) :
							(computedRate - baudRate_Bps);

			if (absError < bestError)
			{
				bestMult = mult;
				bestIcr = i;
				bestError = absError;

				if (absError == 0)
				{
					break;
				}
			}
		}
	}

	printf ("mul: %u, icr: %u\n", bestMult, bestIcr);
	*i2c->I2C_F = (bestMult << I2C_F_MULT_POS) | bestIcr;
}

// Slave

static uint8_t I2C_MasterStartAsync(I2C *i2c, uint8_t address, BOOL isRead)
{
	if (*i2c->I2C_C1 & I2C_C1_MST)
	{
		// Repeated start
		*i2c->I2C_C1 |= I2C_C1_RSTA | I2C_C1_TX;
	}
	else if (*i2c->I2C_S & I2C_S_BUSY)
	{
		// Not a repeated start and bus is busy
		return I2C_ERROR_BUSY;
	}
	else
	{
		// Normal start
		*i2c->I2C_C1 |= I2C_C1_MST | I2C_C1_TX;
	}

	// Send Addr +1 r/w bit
	*i2c->I2C_D = (((uint32_t) address) << 1U | ((isRead) ? 1U : 0U));
	return I2C_ERROR_SUCCESS;
}

uint8_t I2C_MasterStart(I2C *i2c, uint8_t address, BOOL isRead)
{
	uint8_t rc = I2C_MasterStartAsync(i2c, address, isRead);
	if (rc != I2C_ERROR_SUCCESS)
	{
		return rc;
	}

	/* Wait until data transfer complete. */
	while (!(*i2c->I2C_S & I2C_S_IICIF))
		;

	*i2c->I2C_S = I2C_S_IICIF;

	rc = I2C_ERROR_SUCCESS;

	if (*i2c->I2C_S & I2C_S_RXAK)
	{
		*i2c->I2C_S = I2C_S_RXAK;
		rc = I2C_ERROR_RXAK;
	}

	return rc;
}

uint8_t I2C_MasterStop(I2C *i2c)
{
	*i2c->I2C_C1 &= ~(I2C_C1_MST | I2C_C1_TX | I2C_C1_TXAK);

	uint32_t retry = 100;
	if (i2c->ctx)
	{
		retry = i2c->ctx->waitRetry;
	}
	if (retry < 100)
	{
		retry = 100;
	}
	while (*i2c->I2C_S & I2C_S_BUSY)
	{
		__nop();
		if (--retry == 0)
		{
			return I2C_ERROR_BUSY;
		}
	}

	return I2C_ERROR_SUCCESS;
}

uint8_t I2C_MasterWrite(I2C *i2c, const uint8_t *txBuff, size_t txSize)
{
	uint8_t rc = I2C_ERROR_SUCCESS;
	uint8_t statusFlags = 0;

	while (!(*i2c->I2C_S & I2C_S_TCF))
		;

	*i2c->I2C_S = I2C_S_IICIF;

	*i2c->I2C_C1 |= I2C_C1_TX;

	while (txSize--)
	{
		/* Send a byte of data. */
		*i2c->I2C_D = *txBuff++;

		/* Wait until data transfer complete. */
		while (!(*i2c->I2C_S & I2C_S_IICIF))
			;

		statusFlags = *i2c->I2C_S;

		*i2c->I2C_S = I2C_S_IICIF;

		if (statusFlags & I2C_S_ARBL)
		{
			*i2c->I2C_S = I2C_S_ARBL;
			rc = I2C_ERROR_ARBL;
		}

		if ((statusFlags & I2C_S_RXAK) && txSize)
		{
			*i2c->I2C_S = I2C_S_RXAK;
			rc = I2C_ERROR_RXAK;
		}

		if (rc != I2C_ERROR_SUCCESS)
		{
			break;
		}
	}

	return rc;
}

uint8_t I2C_MasterRead(I2C* i2c, uint8_t *rxBuff, size_t rxSize)
{
	uint8_t rc = I2C_ERROR_SUCCESS;
	volatile uint8_t dummy = 0;
	UNUSED(dummy);

	while (!(*i2c->I2C_S & I2C_S_TCF))
		;

	*i2c->I2C_S = I2C_S_IICIF;

	*i2c->I2C_C1 &= ~(I2C_C1_TX | I2C_C1_TXAK);

	/* If rxSize equals 1, configure to send NAK. */
	if (rxSize == 1)
	{
		/* Issue NACK on read. */
		*i2c->I2C_C1 |= I2C_C1_TXAK;
	}

	// Dummy read to clear TCF
	dummy = *i2c->I2C_D;

	while ((rxSize--))
	{
		/* Wait until data transfer complete. */
		while (!(*i2c->I2C_S & I2C_S_IICIF))
			;

		/* Clear the IICIF flag. */
		*i2c->I2C_S = I2C_S_IICIF;

		/* Single byte use case. */
		if (rxSize == 0)
		{
			rc = I2C_MasterStop(i2c);
		}

		if (rxSize == 1)
		{
			/* Issue NACK on read. */
			*i2c->I2C_C1 |= I2C_C1_TXAK;
		}

		/* Read from the data register. */
		*rxBuff++ = *i2c->I2C_D;
	}

	if (*i2c->I2C_S & I2C_S_ARBL)
	{
		*i2c->I2C_S = I2C_S_ARBL;
		rc = I2C_ERROR_ARBL;
	}

	return rc;
}

uint8_t I2C_MasterReadRegister(I2C* i2c, uint8_t addr, uint8_t reg,
		uint8_t *rxBuff, size_t rxSize)
{
	*i2c->I2C_S = I2C_S_IICIF;

	uint8_t rc = I2C_MasterStart(i2c, addr, FALSE);
	if (rc != I2C_ERROR_SUCCESS)
		return rc;

	rc = I2C_MasterWrite(i2c, &reg, 1);
	if (rc != I2C_ERROR_SUCCESS)
		return rc;

	rc = I2C_MasterStart(i2c, addr, TRUE);
	if (rc != I2C_ERROR_SUCCESS)
		return rc;

	rc = I2C_MasterRead(i2c, rxBuff, rxSize);
	if (rc != I2C_ERROR_SUCCESS)
		return rc;

	return I2C_MasterStop(i2c);

}

uint8_t I2C_MasterReadRegisterAsync(I2C* i2c, uint8_t addr, uint8_t reg,
		uint8_t *rxBuff, size_t rxSize, I2C_Async_Context* ctx, uint32_t timeout)
{
	if (*i2c->I2C_S & I2C_S_BUSY)
	{
		return I2C_ERROR_BUSY;
	}

	*i2c->I2C_S = I2C_S_IICIF;

	ctx->isMaster = TRUE;
	ctx->slaveAddr = addr;
	ctx->reg = reg;
	ctx->data = rxBuff;
	ctx->dataSize = rxSize;
	ctx->error = I2C_ERROR_SUCCESS;
	ctx->state = I2C_ASYNC_STATE_INIT;
	ctx->isRead = TRUE;

	i2c->ctx = ctx;
	ctx->state = I2C_ASYNC_STATE_START_SENT;
	I2C_EnableInterrupt(i2c, TRUE);
	uint8_t rc = I2C_MasterStartAsync(i2c, addr, FALSE);

	if (rc != I2C_ERROR_SUCCESS)
	{
		I2C_EnableInterrupt(i2c, FALSE);
		ctx->state = I2C_ASYNC_STATE_DONE;
		ctx->error = rc;
		return rc;
	}
	else if (timeout > 0)
	{
		while(timeout--)
		{
			if (ctx->state == I2C_ASYNC_STATE_DONE)
			{
				//printf ("to: %d\n", 100000 - timeout);
				return ctx->error;
			}
			__nop();
		}
		ctx->state = I2C_ASYNC_STATE_DONE;
		ctx->error = I2C_ERROR_TIMEOUT;
		rc = ctx->error;
	}

	return rc;
}

uint8_t I2C_MasterWriteRegister(I2C* i2c, uint8_t addr, uint8_t reg,
		uint8_t *txBuff, size_t txSize)
{
	uint8_t rc = I2C_MasterStart(i2c, addr, FALSE);
	if (rc != I2C_ERROR_SUCCESS)
		return rc;

	rc = I2C_MasterWrite(i2c, &reg, 1);
	if (rc != I2C_ERROR_SUCCESS)
		return rc;

	rc = I2C_MasterWrite(i2c, txBuff, txSize);
	if (rc != I2C_ERROR_SUCCESS)
		return rc;

	return I2C_MasterStop(i2c);

}

/////////////////////////// SLAVE ///////////////////////////////////////////

static void I2C_SlaveSetHoldTime(I2C *i2c, uint32_t sclStopHoldTime_ns,
		uint32_t sourceClock_Hz)
{
	uint32_t multiplier;
	uint32_t computedSclHoldTime;
	uint32_t absError;
	uint32_t bestError = UINT32_MAX;
	uint32_t bestMult = 0u;
	uint32_t bestIcr = 0u;
	uint8_t mult;
	uint8_t i;

	/* Search for the settings with the lowest error. Mult is the MULT field of the I2C_F register,
	 * and ranges from 0-2. It selects the multiplier factor for the divider. */
	/* SDA hold time = bus period (s) * mul * SDA hold value. */
	/* SCL start hold time = bus period (s) * mul * SCL start hold value. */
	/* SCL stop hold time = bus period (s) * mul * SCL stop hold value. */

	for (mult = 0u; (mult <= 2u) && (bestError != 0); ++mult)
	{
		multiplier = 1u << mult;

		/* Scan table to find best match. */
		for (i = 0u; i < sizeof(I2C_DividerTable) / sizeof(I2C_DividerTable[0]);
				++i)
		{
			/* Assume SCL hold(stop) value = s_i2cDividerTable[i]/2. */
			computedSclHoldTime = ((multiplier * I2C_DividerTable[i]) * 500000U)
					/ (sourceClock_Hz / 1000U);
			absError =
					sclStopHoldTime_ns > computedSclHoldTime ?
							(sclStopHoldTime_ns - computedSclHoldTime) :
							(computedSclHoldTime - sclStopHoldTime_ns);

			if (absError < bestError)
			{
				bestMult = mult;
				bestIcr = i;
				bestError = absError;

				/* If the error is 0, then we can stop searching because we won't find a better match. */
				if (absError == 0)
				{
					break;
				}
			}
		}
	}

	*i2c->I2C_F = (bestMult << I2C_F_MULT_POS) | bestIcr;
}

void I2C_SlaveInit(I2C *i2c, uint32_t slaveAddress)
{
	*i2c->I2C_A1 = 0;
	*i2c->I2C_F = 0;
	*i2c->I2C_C1 = 0;
	*i2c->I2C_S = 0xFFU;
	*i2c->I2C_C2 = 0;
	*i2c->I2C_FLT = 0x40U; // for STOP detect
	*i2c->I2C_RA = 0;

	*i2c->I2C_A1 = ((uint32_t) (slaveAddress)) << 1U;

	/* Configure low power wake up feature. */
	//uint8_t tmpReg;
	// tmpReg = *i2c->I2C_C1;
	// tmpReg &= ~I2C_C1_WUEN;
	// *i2c->I2C_C1 = tmpReg | I2C_C1_WUEN(slaveConfig->enableWakeUp) | I2C_C1_IICEN(slaveConfig->enableSlave);
	/* Configure general call & baud rate control. */
	//tmpReg = *i2c->I2C_C2;
	//tmpReg &= ~(I2C_C2_SBRC | I2C_C2_GCAEN);
	//tmpReg |= I2C_C2_SBRC(slaveConfig->enableBaudRateCtl) | I2C_C2_GCAEN(slaveConfig->enableGeneralCall);
	//*i2c->I2C_C2 = tmpReg;
	uint32_t sclStopHoldTime_ns = 4000; // 4us, the spec minimum
	I2C_SlaveSetHoldTime(i2c, sclStopHoldTime_ns, CORE_FREQ);
}

BOOL I2C_SlaveWrite(I2C *i2c, const uint8_t *txBuff, size_t txSize)
{
	BOOL rc = TRUE;
	volatile uint8_t dummy = 0;
	UNUSED(dummy);

	/* Wait for address match flag. */
	while (!(*i2c->I2C_S & I2C_S_IAAS))
	{
	}

	/* Read dummy to release bus. */
	dummy = *i2c->I2C_D;

	rc = I2C_MasterWrite(i2c, txBuff, txSize);

	/* Switch to receive mode. */
	*i2c->I2C_C1 &= ~(I2C_C1_TX | I2C_C1_TXAK);

	/* Read dummy to release bus. */
	dummy = *i2c->I2C_D;

	return rc;
}

BOOL I2C_SlaveRead(I2C *i2c, uint8_t *rxBuff, size_t rxSize)
{
	BOOL rc = TRUE;
	volatile uint8_t dummy = 0;

	/* Add this to avoid build warning. */
	dummy++;

	/* Wait for address match and int pending flag. */
	while (!(*i2c->I2C_S & I2C_S_IAAS))
	{
	}
	while (!(*i2c->I2C_S & I2C_S_IICIF))
	{
	}

	/* Read dummy to release bus. */
	dummy = *i2c->I2C_D;

	/* Clear the IICIF flag. */
	*i2c->I2C_S = I2C_S_IICIF;

	/* Setup the I2C peripheral to receive data. */
	*i2c->I2C_C1 &= ~(I2C_C1_TX);

	while (rxSize--)
	{
		/* Wait until data transfer complete. */
		while (!(*i2c->I2C_S & I2C_S_IICIF))
		{
		}
		/* Clear the IICIF flag. */
		*i2c->I2C_S = I2C_S_IICIF;

		/* Read from the data register. */
		*rxBuff++ = *i2c->I2C_D;
	}

	return rc;
}
