#include "i2c.h"
#include "mcg.h"
#include "gpio_helper.h"
#include "barometer_bmp280.h"
#include "sim.h"
#include "system_time.h"
#include "pit.h"
#include <stdio.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

static void test1()
{
	InitClock_FEI_24Mhz_Bus_24Mhz();

	I2C* i2c = GPIO_Helper_SetupI2C_Master(I2C0_PE_24_25, 400000);

	uint8_t buf;
	size_t  sz = 1;
	/* Send start and slave address. */
	uint8_t slave_address = 0x76;
	BOOL rc = I2C_MasterStart(i2c, slave_address, FALSE);
	if (!rc)
		goto err;

	buf = 0xd0;

	rc = I2C_MasterWrite(i2c, &buf, sz);
	if (!rc)
		goto err;

	I2C_MasterStop(i2c);

	rc = I2C_MasterStart(i2c, slave_address, TRUE);
	if (!rc)
		goto err;

	rc = I2C_MasterRead(i2c, &buf, sz);
	if (!rc)
		goto err;

	I2C_MasterStop(i2c);

	printf ("%x\n", buf);

err:
	while(1)
	{

	}
}

static void test2()
{
	InitClock_FEI_24Mhz_Bus_24Mhz();
	I2C* i2c = GPIO_Helper_SetupI2C_Master(I2C1_PE_1_0, 400000);
	uint8_t buf;
	uint8_t rc;
	while(1)
	{
		rc = I2C_MasterReadRegister(i2c, BMP280_I2C_ADDR, BMP280_CHIP_ID_REG, &buf, 1);
		if (rc == I2C_ERROR_SUCCESS)
		{
			printf ("0x%x\n", buf);
		}
		else
		{
			printf ("error: %d\n", rc);
		}
		delay_ms(1000);
	}

}


#define I2C_RELEASE_BUS_COUNT 100U

static void i2c_release_bus_delay(void)
{
    uint32_t i = 0;
    for (i = 0; i < I2C_RELEASE_BUS_COUNT; i++)
    {
        __nop();
    }
}

static void I2C_ReleaseBus(void)
{
	GPIO_PIN scl = GPIO_Helper_GetPin("E1");
	GPIO_PIN sda = GPIO_Helper_GetPin("E0");

    /* Config pin mux as gpio */
    //i2c_pin_config.pullSelect = kPORT_PullUp;
    //i2c_pin_config.mux = kPORT_MuxAsGpio;

    //pin_config.pinDirection = kGPIO_DigitalOutput;
    //pin_config.outputLogic = 1U;
	GPIO_SetupOut(&scl);
	GPIO_PullUp(&scl);
	GPIO_SetupOut(&sda);
	GPIO_PullUp(&sda);

    /* Drive SDA low first to simulate a start */
	GPIO_ResetPin(&sda);
	i2c_release_bus_delay();

    /* Send 9 pulses on SCL and keep SDA low */
    for (int i = 0; i < 9; i++)
    {
        //GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
    	GPIO_ResetPin(&scl);
        i2c_release_bus_delay();

        //GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
    	GPIO_SetPin(&sda);
        i2c_release_bus_delay();

        //GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
    	GPIO_SetPin(&scl);
        i2c_release_bus_delay();
        i2c_release_bus_delay();
    }

    /* Send stop */
    //GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
   	GPIO_ResetPin(&scl);
    i2c_release_bus_delay();

    //GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
   	GPIO_ResetPin(&sda);
    i2c_release_bus_delay();

    //GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
   	GPIO_SetPin(&scl);
    i2c_release_bus_delay();

    //GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
   	GPIO_SetPin(&sda);
    i2c_release_bus_delay();
    GPIO_SetAF(&sda, 6);
    GPIO_SetAF(&scl, 6);
}

static void test_async()
{
	//InitClock_FEI_24Mhz_Bus_24Mhz();
	 InitClock_FEI_48Mhz_Bus_24Mhz();
	//InitClock_FBI_Slow();

	// Lowest tested: about 2-3k
	// Upper limit: 800k - 1MB
	I2C* i2c = GPIO_Helper_SetupI2C_Master(I2C1_PE_1_0, 400000);
	I2C_Async_Context ctx;
	ctx.waitRetry = 1000;

	uint8_t buf;
	uint8_t rc;

	// rc = I2C_MasterReadRegisterAsync(i2c, BMP280_I2C_ADDR, BMP280_CHIP_ID_REG, &buf, 1, &ctx, 100000);

	while(1)
	{
		rc = I2C_MasterReadRegister(i2c, BMP280_I2C_ADDR, BMP280_CHIP_ID_REG, &buf, 1);
		if (rc == I2C_ERROR_SUCCESS)
		{
			printf ("sync: 0x%x\n", buf);
		}
		else
		{
			printf ("error: %d\n", rc);
			//I2C_ReleaseBus();
			I2C_MasterStop(i2c);
		}

		rc = I2C_MasterReadRegisterAsync(i2c, BMP280_I2C_ADDR, BMP280_CHIP_ID_REG, &buf, 1, &ctx, 100000);

		if (rc == I2C_ERROR_SUCCESS)
		{
			printf ("async: 0x%x\n", buf);
		}
		else
		{
			printf ("error: %d\n", rc);
			//I2C_ReleaseBus();
			I2C_MasterStop(i2c);
		}
		delay_ms(1000);
	}
}

static void test_async2()
{
	//InitClock_FBI_Slow();
	InitClock_FEI_48Mhz_Bus_24Mhz();

	// max speed seems to be around 560kbps at 1.2kkbps
	I2C* i2c = GPIO_Helper_SetupI2C_Master(I2C1_PE_1_0, 1200000);
	I2C_Async_Context ctx;
	ctx.waitRetry = 1000;

	uint8_t buf;
	systick_enable(TRUE);

	uint32_t n = 1000;
	while(1)
	{
		uint32_t t0 = micros();
		uint32_t sum = 0;
		for (uint32_t i = 0; i < n; ++i)
		{
			uint8_t rc = I2C_MasterReadRegisterAsync(i2c, BMP280_I2C_ADDR, BMP280_CHIP_ID_REG,
					&buf, 1, &ctx, 100000);
			if (rc != I2C_ERROR_SUCCESS)
			{
				printf ("error: %d\n", rc);
				I2C_ReleaseBus();
				I2C_MasterStop(i2c);
			}
			else
			{
				sum += buf;
			}
		}

		uint32_t t = micros() - t0;
		uint64_t kbps = (uint64_t)1000000*n/t*5*8/1024;
		printf ("u: %lu, kbps:%lu, id=0x%lx\n", t, (uint32_t)kbps, sum/n);
		delay_ms(1000);
	}
}


static void test_write()
{
	InitClock_FEI_24Mhz_Bus_24Mhz();
	I2C* i2c = GPIO_Helper_SetupI2C_Master(I2C1_PE_1_0, 100000);

	uint8_t buf;
	uint8_t rc;

	rc = I2C_MasterReadRegister(i2c, BMP280_I2C_ADDR, BMP280_CTRL_MEAS_REG, &buf, 1);
	printf ("read reg: rc=%d, ctrl:%0x\n", rc, buf);

	buf = 0x30;
	rc = I2C_MasterWriteRegister(i2c, BMP280_I2C_ADDR, BMP280_CTRL_MEAS_REG, &buf, 1);
	printf ("write: %d\n", rc);

	rc = I2C_MasterReadRegister(i2c, BMP280_I2C_ADDR, BMP280_CTRL_MEAS_REG, &buf, 1);
	printf ("read reg: rc=%d, ctrl:%0x\n", rc, buf);


	while(1);
}

static void test_write_async()
{
	InitClock_FEI_24Mhz_Bus_24Mhz();
	I2C* i2c = GPIO_Helper_SetupI2C_Master(I2C1_PE_1_0, 100000);
	I2C_Async_Context ctx;
	ctx.waitRetry = 1000;

	uint8_t buf;
	uint8_t rc;

	int cnt = 0;

	while (1)
	{
		buf = (cnt++ % 8) << 5;
		rc = I2C_MasterWriteRegisterAsync(i2c, BMP280_I2C_ADDR, BMP280_CTRL_MEAS_REG, &buf, 1, &ctx, 10000);
		printf ("write: %d\n", rc);

		rc = I2C_MasterReadRegister(i2c, BMP280_I2C_ADDR, BMP280_CTRL_MEAS_REG, &buf, 1);
		printf ("read reg: rc=%d, ctrl:%0x\n", rc, buf);

		delay_ms(1000);
	}

}

static uint8_t test_addr = 0x12;

static void PIT_InterruptHandler(void* ctx)
{
	UNUSED(ctx);
	I2C* i2c = GPIO_Helper_SetupI2C_Master(I2C1_PE_1_0, 100000);
	uint8_t rc = I2C_MasterStart(i2c, test_addr, FALSE);
	if (rc == I2C_ERROR_SUCCESS)
	{

	}
	else
	{
		printf ("Start error: %d\n", rc);
	}
}

static void test_slave()
{
	InitClock_FEI_48Mhz_Bus_24Mhz();
	SIM_Enable_PIT(TRUE);
	PIT_EnableClock(TRUE);
	PIT* pit = PIT_GetInstance(PIT0);
	PIT_SetPeriod_us(pit, 1000000);
	pit->interrupt_handler = PIT_InterruptHandler;
	PIT_EnableInterrupt(pit, TRUE);
	PIT_EnableTimer(pit, TRUE);
	while(1);
}

#pragma GCC diagnostic push


void test_i2c()
{
	// test_slave();
	test_async2();
	while(1);
}
