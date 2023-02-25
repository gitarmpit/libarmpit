#ifndef _MCP47X6_H_
#define _MCP47X6_H_

#include <stdint.h>
#include "i2c.h"
#include "gpio_helper.h"

// I2C Address of device
// MCP4706, MCP4716 & MCP4726 are factory programed for any of 0x60 thru 0x67
//  commonly 0x60
#define MCP47X6_DEFAULT_ADDRESS	0x60

/** Config Data Defn
 * Config = 0bCCCVVPPG
 */

// Programmable Gain definitions
#define MCP47X6_GAIN_MASK	0xFE
#define MCP47X6_GAIN_1X	0x00
#define MCP47X6_GAIN_2X	0x01

// Power Down Mode definitions
#define MCP47X6_PWRDN_MASK     0xF9
#define MCP47X6_AWAKE          0x00
#define MCP47X6_PWRDN_1K       0x02
#define MCP47X6_PWRDN_100K     0x04
#define MCP47X6_PWRDN_500K     0x06

// Reference Voltage definitions
#define MCP47X6_VREF_MASK             0xE7
#define MCP47X6_VREF_VDD              0x00
#define MCP47X6_VREF_VREFPIN	      0x10
#define MCP47X6_VREF_VREFPIN_BUFFERED 0x18

// Command definitioins
#define MCP47X6_CMD_MASK       0x1F
#define MCP47X6_CMD_VOLDAC     0x00
#define MCP47X6_CMD_VOLALL     0x40
#define MCP47X6_CMD_VOLCONFIG  0x80
#define MCP47X6_CMD_ALL        0x60

class MCP47X6
{
public:
	MCP47X6(I2C* itc);


	void setVoltage(double v);
	void setVrefPin();
	void setVrefVcc();
	void powerDown(void);

private:

	void setDAC(uint16_t level);

	I2C* _i2c;
	uint8_t devAddr;
};

#endif /* _MCP47X6_H_ */
