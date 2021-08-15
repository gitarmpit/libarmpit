#include "i2c.h"
#include "sim.h"

static I2C I2C_list[] =
{
    {
    		&SIM_SCGC4, SIM_SCGC4_I2C0, &I2C0_A1, &I2C0_F, &I2C0_C1, &I2C0_S, &I2C0_D, &I2C0_C2,
			&I2C0_FLT, &I2C0_RA, &I2C0_SLTH, &I2C0_SLTL, I2C0_IRQn, NULL
    },

    {
    		&SIM_SCGC4, SIM_SCGC4_I2C1, &I2C1_A1, &I2C1_F, &I2C1_C1, &I2C1_S, &I2C1_D, &I2C1_C2,
			&I2C1_FLT, &I2C1_RA, &I2C1_SLTH, &I2C1_SLTL, I2C1_IRQn, NULL
    },

};

I2C* I2C_GetInstance(I2C_N n)
{
    return &I2C_list[(uint8_t)n];
}




