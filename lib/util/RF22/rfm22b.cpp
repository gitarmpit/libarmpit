///* Copyright (c) 2013 Owen McAree
// *
// * Permission is hereby granted, free of charge, to any person obtaining a copy of
// * this software and associated documentation files (the "Software"), to deal in
// * the Software without restriction, including without limitation the rights to
// * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// * the Software, and to permit persons to whom the Software is furnished to do so,
// * subject to the following conditions:
// *
// * The above copyright notice and this permission notice shall be included in all
// * copies or substantial portions of the Software.
// *
// * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// */
//
//#include "rfm22b.h"
//#include "rcc.h"
//#include "system_time.h"
//
//RFM22B::RFM22B(SPI* spi, GPIO_PIN* ssPin, GPIO_PIN* sdnPin, GPIO_PIN* InterruptPin)
//{
//    _spi = spi;
//    _ssPin = ssPin;
//    _sdnPin = sdnPin;
//    _intPin = interruptPin;
//}
//
//
//// Set the frequency of the carrier wave
////  This function calculates the values of the registers 0x75-0x77 to achieve the
////  desired carrier wave frequency (without any hopping set)
////  Frequency should be passed in integer Hertz
//void RFM22B::setCarrierFrequency(unsigned int frequency) {
//    // Don't set a frequency outside the range specified in the datasheet
//    if (frequency < 240E6 || frequency > 960E6) {
//        return;
//    }
//
//    // The following determines the register values, see Section 3.5.1 of the datasheet
//
//    // Are we in the 'High Band'? (i.e. is hbsel == 1)
//    uint8_t hbsel = (frequency >= 480E6);
//
//    // What is the integer part of the frequency
//    uint8_t fb = frequency/10E6/(hbsel+1) - 24;
//
//    // Calculate register 0x75 from hbsel and fb. sbsel (bit 6) is always set
//    uint8_t fbs = (1<<6) | (hbsel<<5) | fb;
//
//    // Calculate the fractional part of the frequency
//    uint16_t fc = (frequency/(10E6f*(hbsel+1)) - fb - 24) * 64000;
//
//    // Split the fractional part in to most and least significant bits
//    // (Registers 0x76 and 0x77 respectively)
//    uint8_t ncf1 = (fc >> 8);
//    uint8_t ncf0 = fc & 0xff;
//
//    // Write the registers to the device
//    WriteRegister(FREQUENCY_BAND_SELECT, fbs);
//    WriteRegister(NOMINAL_CARRIER_FREQUENCY_1, ncf1);
//    WriteRegister(NOMINAL_CARRIER_FREQUENCY_0, ncf0);
//}
//
//// Get the frequency of the carrier wave in integer Hertz
////  Without any frequency hopping
//unsigned int RFM22B::getCarrierFrequency() {
//    // Read the register values
//    uint8_t fbs = ReadRegister(FREQUENCY_BAND_SELECT);
//    uint8_t ncf1 = ReadRegister(NOMINAL_CARRIER_FREQUENCY_1);
//    uint8_t ncf0 = ReadRegister(NOMINAL_CARRIER_FREQUENCY_0);
//
//    // The following calculations ceom from Section 3.5.1 of the datasheet
//
//    // Determine the integer part
//    uint8_t fb = fbs & 0x1F;
//
//    // Are we in the 'High Band'?
//    uint8_t hbsel = (fbs >> 5) & 1;
//
//    // Determine the fractional part
//    uint16_t fc = (ncf1 << 8) | ncf0;
//
//    // Return the frequency
//    return 10E6*(hbsel+1)*(fb+24+fc/64000.0);
//}
//
//// Get and set the frequency hopping step size
////  Values are in Hertz (to stay SI) but floored to the nearest 10kHz
//void RFM22B::setFrequencyHoppingStepSize(unsigned int step) {
//    if (step > 2550000) {
//        step = 2550000;
//    }
//    WriteRegister(FREQUENCY_HOPPING_STEP_SIZE, step/10000);
//}
//unsigned int RFM22B::getFrequencyHoppingStepSize() {
//    return ReadRegister(FREQUENCY_HOPPING_STEP_SIZE)*10000;
//}
//
//// Get and set the frequency hopping channel
//void RFM22B::setChannel(uint8_t channel) {
//    WriteRegister(FREQUENCY_HOPPING_CHANNEL_SELECT, channel);
//}
//uint8_t RFM22B::getChannel() {
//    return ReadRegister(FREQUENCY_HOPPING_CHANNEL_SELECT);
//}
//
//// Set or get the frequency deviation (in Hz, but floored to the nearest 625Hz)
//void RFM22B::setFrequencyDeviation(unsigned int deviation) {
//    if (deviation > 320000) {
//        deviation = 320000;
//    }
//    WriteRegister(FREQUENCY_DEVIATION, deviation/625);
//}
//unsigned int RFM22B::getFrequencyDeviation() {
//    return ReadRegister(FREQUENCY_DEVIATION)*625;
//}
//
//// Set or get the data rate (bps)
//void RFM22B::setDataRate(unsigned int rate) {
//    // Get the Modulation Mode Control 1 register (for scaling bit)
//    uint8_t mmc1 = ReadRegister(MODULATION_MODE_CONTROL_1);
//
//    uint16_t txdr;
//    // Set the scale bit (5th bit of 0x70) high if data rate is below 30kbps
//    // and calculate the value for txdr registers (0x6E and 0x6F)
//    if (rate < 30000) {
//        mmc1 |= (1<<5);
//        txdr = rate * ((1 << (16 + 5)) / 1E6);
//    } else {
//        mmc1 &= ~(1<<5);
//        txdr = rate * ((1 << (16)) / 1E6);
//    }
//
//    // Set the data rate bytes
//    BurstWrite(TX_DATA_RATE_1, (uint8_t *)&txdr, 2);
//
//    // Set the scaling byte
//    WriteRegister(MODULATION_MODE_CONTROL_1, mmc1);
//}
//unsigned int RFM22B::getDataRate() {
//    // Get the data rate scaling value (5th bit of 0x70)
//    uint8_t txdtrtscale = (ReadRegister(MODULATION_MODE_CONTROL_1) >> 5) & 1;
//
//    // Get the data rate registers
//    uint16_t txdr;
//
//    BurstRead(TX_DATA_RATE_1, (uint8_t *)&txdr, 2);
//
//    // Return the data rate (in bps, hence extra 1E3)
//    return ((unsigned int) txdr * 1E6) / (1 << (16 + 5 * txdtrtscale));
//
//}
//
//// Set or get the modulation type
//void RFM22B::setModulationType(RFM22B_Modulation_Type modulation) {
//    // Get the Modulation Mode Control 2 register
//    uint8_t mmc2 = ReadRegister(MODULATION_MODE_CONTROL_2);
//
//    // Clear the modtyp bits
//    mmc2 &= ~0x03;
//
//    // Set the desired modulation
//    mmc2 |= modulation;
//
//    // Set the register
//    WriteRegister(MODULATION_MODE_CONTROL_2, mmc2);
//}
//RFM22B::RFM22B_Modulation_Type RFM22B::getModulationType() {
//    // Get the Modulation Mode Control 2 register
//    uint8_t mmc2 = ReadRegister(MODULATION_MODE_CONTROL_2);
//
//    // Determine modtyp bits
//    uint8_t modtyp = mmc2 & 0x03;
//
//    // Ugly way to return correct enum
//    switch (modtyp) {
//        case 1:
//            return OOK;
//        case 2:
//            return FSK;
//        case 3:
//            return GFSK;
//        case 0:
//        default:
//            return UNMODULATED_CARRIER;
//    }
//}
//
//void RFM22B::setModulationDataSource(RFM22B_Modulation_Data_Source source) {
//    // Get the Modulation Mode Control 2 register
//    uint8_t mmc2 = ReadRegister(MODULATION_MODE_CONTROL_2);
//
//    // Clear the dtmod bits
//    mmc2 &= ~(0x03<<4);
//
//    // Set the desired data source
//    mmc2 |= source << 4;
//
//    // Set the register
//    WriteRegister(MODULATION_MODE_CONTROL_2, mmc2);
//}
//RFM22B::RFM22B_Modulation_Data_Source RFM22B::getModulationDataSource() {
//    // Get the Modulation Mode Control 2 register
//    uint8_t mmc2 = ReadRegister(MODULATION_MODE_CONTROL_2);
//
//    // Determine modtyp bits
//    uint8_t dtmod = (mmc2 >> 4) & 0x03;
//
//    // Ugly way to return correct enum
//    switch (dtmod) {
//        case 1:
//            return DIRECT_SDI;
//        case 2:
//            return FIFO;
//        case 3:
//            return PN9;
//        case 0:
//        default:
//            return DIRECT_GPIO;
//    }
//}
//
//void RFM22B::setDataClockConfiguration(RFM22B_Data_Clock_Configuration clock) {
//    // Get the Modulation Mode Control 2 register
//    uint8_t mmc2 = ReadRegister(MODULATION_MODE_CONTROL_2);
//
//    // Clear the trclk bits
//    mmc2 &= ~(0x03<<6);
//
//    // Set the desired data source
//    mmc2 |= clock << 6;
//
//    // Set the register
//    WriteRegister(MODULATION_MODE_CONTROL_2, mmc2);
//}
//RFM22B::RFM22B_Data_Clock_Configuration RFM22B::getDataClockConfiguration() {
//    // Get the Modulation Mode Control 2 register
//    uint8_t mmc2 = ReadRegister(MODULATION_MODE_CONTROL_2);
//
//    // Determine modtyp bits
//    uint8_t dtmod = (mmc2 >> 6) & 0x03;
//
//    // Ugly way to return correct enum
//    switch (dtmod) {
//        case 1:
//            return GPIO;
//        case 2:
//            return SDO;
//        case 3:
//            return NIRQ;
//        case 0:
//        default:
//            return NONE;
//    }
//}
//
//// Set or get the transmission power
//void RFM22B::setTransmissionPower(uint8_t power) {
//    // Saturate to maximum power
//    if (power > 20) {
//        power = 20;
//    }
//
//    // Get the TX power register
//    uint8_t txp = ReadRegister(TX_POWER);
//
//    // Clear txpow bits
//    txp &= ~(0x07);
//
//    // Calculate txpow bits (See Section 5.7.1 of datasheet)
//    uint8_t txpow = (power + 1) / 3;
//
//    // Set txpow bits
//    txp |= txpow;
//
//    // Set the register
//    WriteRegister(TX_POWER, txp);
//}
//uint8_t RFM22B::getTransmissionPower() {
//    // Get the TX power register
//    uint8_t txp = ReadRegister(TX_POWER);
//
//    // Get the txpow bits
//    uint8_t txpow = txp & 0x07;
//
//    // Calculate power (see Section 5.7.1 of datasheet)
//    if (txpow == 0) {
//        return 1;
//    } else {
//        return txpow * 3 - 1;
//    }
//}
//
//// Set or get the GPIO configuration
//void RFM22B::setGPIOFunction(RFM22B_GPIO gpio, RFM22B_GPIO_Function func) {
//    // Get the GPIO register
//    uint8_t gpioX = ReadRegister(gpio);
//
//    // Clear gpioX bits
//    gpioX &= ~((1<<5)-1);
//
//    // Set the gpioX bits
//    gpioX |= func;
//
//    // Set the register
//    WriteRegister(gpio, gpioX);
//}
//
//uint8_t RFM22B::getGPIOFunction(RFM22B_GPIO gpio) {
//    // Get the GPIO register
//    uint8_t gpioX = ReadRegister(gpio);
//
//    // Return the gpioX bits
//    // This should probably return an enum, but this needs a lot of cases
//    return gpioX & ((1<<5)-1);
//}
//
//// Enable or disable interrupts
//void RFM22B::setInterruptEnable(RFM22B_Interrupt interrupt, bool enable) {
//    // Get the (16 bit) register value
//    uint16_t intEnable;
//    BurstRead(INTERRUPT_ENABLE_1, (uint8_t *)&intEnable, 2);
//
//    // Either enable or disable the interrupt
//    if (enable) {
//        intEnable |= interrupt;
//    } else {
//        intEnable &= ~interrupt;
//    }
//
//    // Set the (16 bit) register value
//    BurstWrite(INTERRUPT_ENABLE_1, (uint8_t *)&intEnable, 2);
//}
//
//// Get the status of an interrupt
//bool RFM22B::getInterruptStatus(RFM22B_Interrupt interrupt) {
//    // Get the (16 bit) register value
//    uint16_t intStatus;
//    BurstRead(INTERRUPT_STATUS_1, (uint8_t *)&intStatus, 2);
//
//    // Determine if interrupt bit is set and return
//    if ((intStatus & interrupt) > 0) {
//        return true;
//    } else {
//        return false;
//    }
//}
//
//// Set the operating mode
////  This function does not toggle individual pins as with other functions
////  It expects a bitwise-ORed combination of the modes you want set
//void RFM22B::setOperatingMode(uint16_t mode) {
//    BurstWrite(OPERATING_MODE_AND_FUNCTION_CONTROL_1, (uint8_t *)&mode, 2);
//}
//
//// Get operating mode (bitwise-ORed)
//uint16_t RFM22B::getOperatingMode() {
//    uint16_t opMode;
//    BurstRead(OPERATING_MODE_AND_FUNCTION_CONTROL_1, (uint8_t *)&opMode, 2);
//    return opMode;
//}
//
//// Manuall enter RX or TX mode
//void RFM22B::enableRXMode() {
//    this->setOperatingMode(READY_MODE | RX_MODE);
//}
//void RFM22B::enableTXMode() {
//    this->setOperatingMode(READY_MODE | TX_MODE);
//}
//
//// Reset the device
//void RFM22B::reset() {
//    this->setOperatingMode(READY_MODE | RESET);
//}
//
//// Set or get the trasmit header
//void RFM22B::setTransmitHeader(uint32_t header) {
//    BurstWrite(TRANSMIT_HEADER_3, (uint8_t* )&header, 4);
//}
//uint32_t RFM22B::getTransmitHeader() {
//    uint32_t hdr;
//    BurstRead(TRANSMIT_HEADER_3, (uint8_t *)&hdr, 4);
//    return hdr;
//}
//
//// Set or get the check header
//void RFM22B::setCheckHeader(uint32_t header) {
//    BurstWrite(CHECK_HEADER_3, (uint8_t *)&header, 4);
//}
//uint32_t RFM22B::getCheckHeader() {
//    uint32_t hdr;
//    BurstRead(CHECK_HEADER_3, (uint8_t *)&hdr, 4);
//    return hdr;
//}
//
//// Set or get the CRC mode
//void RFM22B::setCRCMode(RFM22B::RFM22B_CRC_Mode mode) {
//    uint8_t dac = ReadRegister(DATA_ACCESS_CONTROL);
//
//    dac &= ~0x24;
//
//    switch (mode) {
//    case CRC_DISABLED:
//        break;
//    case CRC_DATA_ONLY:
//        dac |= 0x24;
//        break;
//    case CRC_NORMAL:
//    default:
//        dac |= 0x04;
//        break;
//    }
//
//    WriteRegister(DATA_ACCESS_CONTROL, dac);
//}
//RFM22B::RFM22B_CRC_Mode RFM22B::getCRCMode() {
//    uint8_t dac = ReadRegister(DATA_ACCESS_CONTROL);
//
//    if (! (dac & 0x04)) {
//        return CRC_DISABLED;
//    }
//    if (dac & 0x20) {
//        return CRC_DATA_ONLY;
//    }
//    return CRC_NORMAL;
//}
//
//// Set or get the CRC polynomial
//void RFM22B::setCRCPolynomial(RFM22B::RFM22B_CRC_Polynomial poly) {
//    uint8_t dac = ReadRegister(DATA_ACCESS_CONTROL);
//
//    dac &= ~0x03;
//
//    dac |= poly;
//
//    WriteRegister(DATA_ACCESS_CONTROL, dac);
//}
//RFM22B::RFM22B_CRC_Polynomial RFM22B::getCRCPolynomial() {
//    uint8_t dac = ReadRegister(DATA_ACCESS_CONTROL);
//
//    switch (dac & 0x03) {
//    case 0:
//        return CCITT;
//    case 1:
//        return CRC16;
//    case 2:
//        return IEC16;
//    case 3:
//        return BIACHEVA;
//    }
//    return CRC16;
//}
//
//// Get and set all the FIFO threshold
//void RFM22B::setTXFIFOAlmostFullThreshold(uint8_t thresh) {
//    this->setFIFOThreshold(TX_FIFO_CONTROL_1, thresh);
//}
//void RFM22B::setTXFIFOAlmostEmptyThreshold(uint8_t thresh) {
//    this->setFIFOThreshold(TX_FIFO_CONTROL_2, thresh);
//}
//void RFM22B::setRXFIFOAlmostFullThreshold(uint8_t thresh) {
//    this->setFIFOThreshold(RX_FIFO_CONTROL, thresh);
//}
//uint8_t RFM22B::getTXFIFOAlmostFullThreshold() {
//    return ReadRegister(TX_FIFO_CONTROL_1);
//}
//uint8_t RFM22B::getTXFIFOAlmostEmptyThreshold() {
//    return ReadRegister(TX_FIFO_CONTROL_2);
//}
//uint8_t RFM22B::getRXFIFOAlmostFullThreshold() {
//    return ReadRegister(RX_FIFO_CONTROL);
//}
//void RFM22B::setFIFOThreshold(RFM22B_Register reg, uint8_t thresh) {
//    thresh &= ((1 << 6) - 1);
//    (reg, thresh);
//}
//
//// Get RSSI value
//uint8_t RFM22B::getRSSI() {
//    return ReadRegister(RECEIVED_SIGNAL_STRENGTH_INDICATOR);
//}
//// Get input power (in dBm)
////  Coefficients approximated from the graph in Section 8.10 of the datasheet
//int8_t RFM22B::getInputPower() {
//    return 0.56*this->getRSSI()-128.8;
//}
//
//// Get length of last received packet
//uint8_t RFM22B::getReceivedPacketLength() {
//    return ReadRegister(RECEIVED_PACKET_LENGTH);
//}
//
//// Set length of packet to be transmitted
//void RFM22B::setTransmitPacketLength(uint8_t length) {
//    return WriteRegister(TRANSMIT_PACKET_LENGTH, length);
//}
//
//void RFM22B::clearRXFIFO() {
//    //Toggle ffclrrx bit high and low to clear RX FIFO
//   WriteRegister(OPERATING_MODE_AND_FUNCTION_CONTROL_2, 2);
//   WriteRegister(OPERATING_MODE_AND_FUNCTION_CONTROL_2, 0);
//}
//
//void RFM22B::clearTXFIFO() {
//    //Toggle ffclrtx bit high and low to clear TX FIFO
//    WriteRegister(OPERATING_MODE_AND_FUNCTION_CONTROL_2, 1);
//    WriteRegister(OPERATING_MODE_AND_FUNCTION_CONTROL_2, 0);
//}
//
//// Send data
//void RFM22B::send(uint8_t *data, int length) {
//    clearTXFIFO();
//
//    // Truncate data if its too long
//    if (length > MAX_PACKET_LENGTH) {
//        length = MAX_PACKET_LENGTH;
//    }
//
//    setTransmitPacketLength(length);
//
//    BurstWrite (FIFO_ACCESS, data, length);
//
//    enableTXMode();
//
//    // Loop until packet has been sent (device has left TX mode)
//    while (((ReadRegister(OPERATING_MODE_AND_FUNCTION_CONTROL_1)>>3) & 1)) {}
//
//    return;
//};
//
//// Receive data (blocking with timeout). Returns number of bytes received
//int RFM22B::receive(uint8_t *data, int length, int timeout) {
//
//    clearRXFIFO();
//    enableRXMode();
//
//    uint32_t start_time = millis();
//
//    // Timing for the interrupt loop timeout
//    uint32_t elapsed = 0;
//
//    // Loop endlessly on interrupt or timeout
//    //  Don't use interrupt registers here as these don't seem to behave consistently
//    //  Watch the operating mode register for the device leaving RX mode. This is indicitive
//    //  of a valid packet being received
//    while (((ReadRegister(OPERATING_MODE_AND_FUNCTION_CONTROL_1)>>2) & 1) && elapsed < timeout) {
//        elapsed = millis() - start_time;
//    }
//
//    if (elapsed >= timeout) {
//        return -1;
//    }
//
//    // Get length of packet received
//    uint8_t rxLength = getReceivedPacketLength();
//
//    if (rxLength > length) {
//        rxLength = length;
//    }
//
//    BurstRead (FIFO_ACCESS, data, length);
//
//    return rxLength;
//};
//
//uint8_t RFM22B::ReadRegister(uint8_t reg)
//{
//    uint8_t val = 0xFF;
//    BurstRead(reg, &val, 1);
//    return val;
//}
//
//void RFM22B::WriteRegister(uint8_t reg, uint8_t val)
//{
//    BurstWrite(reg, &val, 1);
//}
//
//void RFM22B::BurstWrite(uint8_t startReg, const uint8_t* value, uint8_t length)
//{
//
//    uint8_t regVal = (uint8_t) startReg | 0x80; // set MSB
//
//    _ssPin->Reset();
//    _spi->TransmitByte(regVal);
//
//    for (uint8_t i = 0; i < length; ++i)
//    {
//        _spi->TransmitByte(value[i]);
//    }
//
//
//    _ssPin->Set();
//}
//
//void RFM22B::BurstRead(uint8_t startReg, uint8_t* value, uint8_t length)
//{
//
//    uint8_t regVal = (uint8_t) startReg & 0x7F; // set MSB
//
//    _ssPin->Reset();
//    _spi->TransmitByte(regVal);
//
//    for (uint8_t i = 0; i < length; ++i)
//    {
//        value[i] = _spi->TransmitByte(0xFF);
//    }
//
//
//    _ssPin->Set();
//}
//
