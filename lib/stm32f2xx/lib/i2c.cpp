
#include "i2c_cpp.h"


#include "rcc.h"
#include "gpio_cpp.h"

#ifdef __cplusplus
extern "C"
{
#endif

void I2C1_EV_IRQHandler()
{
    I2C1::GetInstance()->HandleEventInterrupt();
}

void I2C1_ER_IRQHandler()
{

    I2C1::GetInstance()->HandleErrorInterrupt();

}



void I2C2_EV_IRQHandler()
{
    I2C2::GetInstance()->HandleEventInterrupt();
}

void I2C2_ER_IRQHandler()
{
    I2C2::GetInstance()->HandleErrorInterrupt();

}


#ifdef __cplusplus
}
#endif

void I2C::HandleErrorInterrupt()
{
    if (_irqErrorHandler)
    {
        I2C_IRQ_Error_Info info;

        if (IsBusError())
        {
            info.BusError = true;
            ClearBusError();
        }

        if (IsARLO())
        {
            info.ArbitrationLoss = true;
            ClearARLO();
        }

        if (IsAF())
        {
            info.AckFailure = true;
            ClearAF();
        }

        if (IsOVR())
        {
            info.OverrunUnderrun = true;
            ClearOVR();
        }

        if (IsPECError())
        {
            info.PECError = true;
            ClearPECERR();
        }

        if (IsTimeout())
        {
            info.Timeout = true;
            ClearTimeout();
        }

        if (IsSMBAlert())
        {
            info.SMBAlert = true;
            ClearSMBAlert();
        }

        _irqErrorHandler->HandleErrorInterrupt(this, info);
    }

}

void I2C::HandleEventInterrupt()
{
    if (_irqEventHandler)
    {
        I2C_IRQ_Event_Info info;

        info.StartBitSent = IsSB();
        info.Addr10HeaderSent = IsADD10();
        info.AddressSentMatched = IsADDR();
        info.ByteTransferFinished = IsBTF();
        info.RxNE = IsRxNE();
        info.TxE = IsTxE();
        info.StopBitReceived = IsStOPF();

        _irqEventHandler->HandleEventInterrupt(this, info);
    }

}


I2C::I2C(volatile uint32_t* pI2C_CR1, volatile uint32_t* pI2C_CR2,
        volatile uint32_t* pI2C_OAR1, volatile uint32_t* pI2C_OAR2,
        volatile uint32_t* pI2C_DR, volatile uint32_t* pI2C_SR1,
        volatile uint32_t* pI2C_SR2, volatile uint32_t* pI2C_CCR,
        volatile uint32_t* pI2C_TRISE, IRQn_Type eventIrqNo,
        IRQn_Type errorIrqNo,
        volatile uint32_t* pRCCEnableReg,
        volatile uint32_t* pRCCResetReg,
        uint32_t           rccBit) : Peripheral (pRCCEnableReg, pRCCResetReg, rccBit)

{
    _pI2C_CR1 = pI2C_CR1;
    _pI2C_CR2 = pI2C_CR2;
    _pI2C_OAR1 = pI2C_OAR1;
    _pI2C_OAR2 = pI2C_OAR2;
    _pI2C_DR = pI2C_DR;
    _pI2C_SR1 = pI2C_SR1;
    _pI2C_SR2 = pI2C_SR2;
    _pI2C_CCR = pI2C_CCR;
    _pI2C_TRISE = pI2C_TRISE;
    _eventIrqNo = eventIrqNo;
    _errorIrqNo = errorIrqNo;
    _retryCount = I2C_MAX_RETRY;
    _irqErrorHandler = 0;
    _irqEventHandler = 0;
    _errorCount = 0;
    _errorCountAF = 0;
    _errorCountBE = 0;
    _errorCountTO = 0;
    _errorCountARLO = 0;
    _errorCountRetry = 0;
    _errorDMARX = 0;
    _errorDMATX = 0;
    _dmaTxStream = 0;
    _dmaRxStream = 0;
    _errorCallBack = 0;
    _errorCallBackArg = 0;
    _errorHandler = 0;
}



void I2C::Initialize (uint32_t clockSpeed, bool isDuty16_9, bool isFmMode)
{
    EnablePeripheralClock(true);
    Enable(true);
    
    SetClock(clockSpeed, isDuty16_9, isFmMode);

}


void I2C::Enable (bool enable)
{
    if (enable)
    {
        *_pI2C_CR1 &= ~I2C_CR1_PE;
        *_pI2C_CR1 |= I2C_CR1_PE;
    }
    else
    {
        *_pI2C_CR1 &= ~I2C_CR1_PE;
    }

}


void I2C::SetClock(uint32_t clockSpeed, bool isDuty16_9, bool isFmMode)
{
    uint32_t apb1_clk = RCC_GetAPB1_CLK();
    uint16_t result;

    *_pI2C_CR2 |= (apb1_clk / 1000000);

    if (clockSpeed <= 100000)
    {
        result = (uint16_t)(apb1_clk / (clockSpeed*2));
        if (result < 0x04)
        {
            result = 0x04;
        }

        *_pI2C_CCR = result;

        *_pI2C_TRISE = apb1_clk / 1000000 + 1;
    }
    else // clockSpeed >100000
    {
        if (!isDuty16_9)
        {
            result = (uint16_t)(apb1_clk / (clockSpeed * 3));
        }
        else
        {
            result = (uint16_t)(apb1_clk / (clockSpeed * 25));
            result |= I2C_CCR_DUTY;
        }

        if ((result & 0x0fff) == 0)
        {
            result |= (uint16_t)0x0001;        /* Set minimum allowed value */
        }

        *_pI2C_CCR = result;

        if (isFmMode)
        {
            SetModeFm();
        }

        *_pI2C_TRISE = (uint16_t)((((apb1_clk / 1000000) * 300) / 1000) + 1);
    }


}

void I2C::SetSoftwareReset (bool on)
{
    if (on)
    {
        *_pI2C_CR1 |= I2C_CR1_SWRST;
    }
    else
    {
        *_pI2C_CR1 &= ~I2C_CR1_SWRST;
    }
}

void I2C::SetSMBAlert (bool on)
{
    if (on)
    {
        *_pI2C_CR1 |= I2C_CR1_ALERT;
    }
    else
    {
        *_pI2C_CR1 &= ~I2C_CR1_ALERT;
    }

}


void I2C::SetPEC (bool on)
{
    if (on)
    {
        *_pI2C_CR1 |= I2C_CR1_PEC;
    }
    else
    {
        *_pI2C_CR1 &= ~I2C_CR1_PEC;
    }

}

void I2C::SetPOS (bool on)
{
    if (on)
    {
        *_pI2C_CR1 |= I2C_CR1_POS;
    }
    else
    {
        *_pI2C_CR1 &= ~I2C_CR1_POS;
    }

}

void I2C::EnableACK (bool on)
{
    if (on)
    {
        *_pI2C_CR1 |= I2C_CR1_ACK;
    }
    else
    {
        *_pI2C_CR1 &= ~I2C_CR1_ACK;
    }

}

void I2C::SendStop (bool on)
{
    if (on)
    {
        *_pI2C_CR1 |= I2C_CR1_STOP;
    }
    else
    {
        *_pI2C_CR1 &= ~I2C_CR1_STOP;
    }

}

void I2C::SendStart (bool on)
{
    if (on)
    {
        *_pI2C_CR1 |= I2C_CR1_START;
    }
    else
    {
        *_pI2C_CR1 &= ~I2C_CR1_START;
    }

}

void I2C::SetNoStretch (bool on)
{
    if (on)
    {
        *_pI2C_CR1 |= I2C_CR1_NOSTRETCH;
    }
    else
    {
        *_pI2C_CR1 &= ~I2C_CR1_NOSTRETCH;
    }

}

void I2C::EnableGeneralCall (bool enable)
{
    if (enable)
    {
        *_pI2C_CR1 |= I2C_CR1_ENGC;
    }
    else
    {
        *_pI2C_CR1 &= ~I2C_CR1_ENGC;
    }

}

void I2C::EnablePEC (bool enable)
{
    if (enable)
    {
        *_pI2C_CR1 |= I2C_CR1_PEC;
    }
    else
    {
        *_pI2C_CR1 &= ~I2C_CR1_PEC;
    }

}


void I2C::EnableARP (bool enable)
{
    if (enable)
    {
        *_pI2C_CR1 |= I2C_CR1_ENARP;
    }
    else
    {
        *_pI2C_CR1 &= ~I2C_CR1_ENARP;
    }

}


void I2C::SetModeI2C()
{
    *_pI2C_CR1 &= ~I2C_CR1_SMBUS;
}

void I2C::SetModeSMBHost()
{
    *_pI2C_CR1 |= I2C_CR1_SMBUS;
    *_pI2C_CR1 |= I2C_CR1_SMBTYPE;

}
void I2C::SetModeSMBDevice()
{
    *_pI2C_CR1 |= I2C_CR1_SMBUS;
    *_pI2C_CR1 &= ~I2C_CR1_SMBTYPE;

}



void I2C::EnableDMA (bool enable)
{
    if (enable)
    {
        *_pI2C_CR2 |= I2C_CR2_DMAEN;
    }
    else
    {
        *_pI2C_CR2 &= ~I2C_CR2_DMAEN;
    }

}


void I2C::EnableAutoNACK (bool enable)
{
    if (enable)
    {
        *_pI2C_CR2 |= I2C_CR2_LAST;
    }
    else
    {
        *_pI2C_CR2 &= ~I2C_CR2_LAST;
    }

}

bool I2C::IsError()
{
    bool isError = false;
    if (IsARLO())
    {
        ClearARLO();
        isError = true;
        ++_errorCountARLO;
    }
    if (IsAF())
    {
        ClearAF();
        isError = true;
        ++_errorCountAF;
    }
    if (IsTimeout())
    {
        ClearTimeout();
        isError = true;
        ++_errorCountTO;
    }
    if (IsBusError())
    {
        ClearBusError();
        isError = true;
        ++_errorCountBE;
    }

    return isError;

}


bool I2C::ReceiveDMA (void* data_ptr, uint32_t data_size)
{
    EnableDMA(true);
    EnableAutoNACK(true);


    _dmaRxStream->SetPeripheralAddress ((uint32_t)_pI2C_DR);
    _dmaRxStream->SetMemory0Address((uint32_t)data_ptr);
    _dmaRxStream->SetDirection_PeripheralToMemory();
    _dmaRxStream->SetMemorySize_8bits();
    _dmaRxStream->SetPeripheralSize_8bits();
    _dmaRxStream->SetNumberOfData(data_size);
    _dmaRxStream->SetMemoryIncrementMode(true);
    _dmaRxStream->SetPriorityHigh();

    _dmaRxStream->EnableStream(true);

    bool rc = true;

    while (!_dmaRxStream->IsTransferComplete())
    {
        if  (_dmaRxStream->IsTransferError())
        {
            ++_errorCount;
            ++_errorDMARX;
            rc = false;
            _dmaRxStream->ClearTransferError();
            break;
        }
        if (IsError())
        {
            ++_errorCount;
            rc = false;
            break;
        }
    }

    SendStop(true);

    _dmaRxStream->EnableStream(false);
    _dmaRxStream->ClearTransferComplete();

    EnableDMA(false);
    EnableAutoNACK(false);

    return rc;

}

bool I2C::SendDMA (void* data_ptr, uint32_t data_size)
{
    if (!_dmaTxStream)
    {
        return false;
    }

    EnableDMA(true);

    DMA1* dma1 = DMA1::GetInstance();
    if (!dma1->IsClockEnabled())
    {
        dma1->EnablePeripheralClock(true);
    }

    _dmaTxStream->SetPeripheralAddress ((uint32_t)_pI2C_DR);
    _dmaTxStream->SetMemory0Address((uint32_t)data_ptr);
    _dmaTxStream->SetDirection_MemoryToPeripheral();
    _dmaTxStream->SetMemorySize_8bits();
    _dmaTxStream->SetPeripheralSize_8bits();
    _dmaTxStream->SetNumberOfData(data_size);
    _dmaTxStream->SetMemoryIncrementMode(true);
    _dmaTxStream->SetPriorityHigh();

    _dmaTxStream->EnableStream(true);

    bool rc = true;
    while (!_dmaTxStream->IsTransferComplete())
    {
        if  (_dmaTxStream->IsTransferError())
        {
            rc = false;
            ++_errorDMATX;
            ++_errorCount;
            _dmaTxStream->ClearTransferError();
            break;
        }

    }

    if (!WaitStatus(MASTER_EV8_2, true))  //TRA, BUSY, MSL, TXE and BTF
        rc = false;


    SendStop(true);

    _dmaTxStream->EnableStream(false);
    _dmaTxStream->ClearTransferComplete();
    EnableDMA(false);

    return rc;
}


void I2C::EnableBufferInterrupt (bool enable)
{
    if (enable)
    {
        *_pI2C_CR2 |= I2C_CR2_ITBUFEN;
    }
    else
    {
        *_pI2C_CR2 &= ~I2C_CR2_ITBUFEN;
    }
}

void I2C::EnableEventInterrupt (bool enable)
{
    if (enable)
    {
        *_pI2C_CR2 |= I2C_CR2_ITEVTEN;
        NVIC_SetEnable(_eventIrqNo);
    }
    else
    {
        *_pI2C_CR2 &= ~I2C_CR2_ITEVTEN;
        NVIC_ClearEnable(_eventIrqNo);
    }
}

void I2C::EnableErrorInterrupt (bool enable)
{
    if (enable)
    {
        *_pI2C_CR2 |= I2C_CR2_ITERREN;
        NVIC_SetEnable(_errorIrqNo);
    }
    else
    {
        *_pI2C_CR2 &= ~I2C_CR2_ITERREN;
        NVIC_ClearEnable(_errorIrqNo);
    }

}

void I2C::SetOwnAddr7Bit (uint8_t addr)
{
    *_pI2C_OAR1 = (addr & 0x7f) << 1;
    *_pI2C_OAR2 &= ~I2C_OAR2_ENDUAL;
}

void I2C::SetOwnAddr10bit (uint16_t addr)
{
    *_pI2C_OAR1 = (addr & 0x3ff);
    *_pI2C_OAR2 &= ~I2C_OAR2_ENDUAL;
}

void I2C::SetOwnAddr7bitDual (uint8_t addr1, uint8_t addr2)
{
    SetOwnAddr7Bit(addr1);
    *_pI2C_OAR2 = (addr2 & 0x7f) << 1;
    *_pI2C_OAR2 |= I2C_OAR2_ENDUAL;
}

bool I2C::IsSMBAlert()
{
    return *_pI2C_SR1 & I2C_SR1_SMBALERT;
}


bool I2C::IsTimeout()
{
    return *_pI2C_SR1 & I2C_SR1_TIMEOUT;

}

bool I2C::IsPECError()
{
    return *_pI2C_SR1 & I2C_SR1_PECERR;
}

bool I2C::IsOVR()
{
    return *_pI2C_SR1 & I2C_SR1_OVR;
}

bool I2C::IsAF()
{
    return *_pI2C_SR1 & I2C_SR1_AF;
}

bool I2C::IsARLO()
{
    return *_pI2C_SR1 & I2C_SR1_ARLO;
}
bool I2C::IsBusError()
{
    return *_pI2C_SR1 & I2C_SR1_BERR;
}


void I2C::ClearSMBAlert()
{
    *_pI2C_SR1 &= ~I2C_SR1_SMBALERT;
}
void I2C::ClearTimeout()
{
    *_pI2C_SR1 &= ~I2C_SR1_TIMEOUT;
}
void I2C::ClearPECERR()
{
    *_pI2C_SR1 &= ~I2C_SR1_PECERR;
}
void I2C::ClearOVR()
{
    *_pI2C_SR1 &= ~I2C_SR1_OVR;
}
void I2C::ClearAF()
{
    *_pI2C_SR1 &= ~I2C_SR1_AF;
}
void I2C::ClearARLO()
{
    *_pI2C_SR1 &= ~I2C_SR1_ARLO;
}
void I2C::ClearBusError()
{
    *_pI2C_SR1 &= ~I2C_SR1_BERR;
}


void I2C::ClearSTOPF()
{
    volatile uint32_t tmp = *_pI2C_SR1;
    ((void)(tmp));
    ++tmp;
    *_pI2C_CR1 &= 0xffff;

    //*_pI2C_CR1 |= I2C_CR1_PE;
}


bool I2C::IsStop() 
{
    return *_pI2C_CR1 & I2C_CR1_STOP;
}


bool I2C::IsTxE()
{
    return *_pI2C_SR1 & I2C_SR1_TxE;
}
bool I2C::IsRxNE()
{
    return *_pI2C_SR1 & I2C_SR1_RxNE;
}
bool I2C::IsStOPF()
{
    return *_pI2C_SR1 & I2C_SR1_STOPF;
}
bool I2C::IsADD10()
{
    return *_pI2C_SR1 & I2C_SR1_ADD10;
}
bool I2C::IsBTF()
{
    return *_pI2C_SR1 & I2C_SR1_BTF;
}
bool I2C::IsADDR()
{
    return *_pI2C_SR1 & I2C_SR1_ADDR;
}
bool I2C::IsSB()
{
    return *_pI2C_SR1 & I2C_SR1_SB;
}

void I2C::SetModeSm()
{
    *_pI2C_CCR &= ~I2C_CCR_FS;
}
void I2C::SetModeFm()
{
    *_pI2C_CCR |= I2C_CCR_FS;
}

void I2C::SetFmDuty (bool set)
{
    if (set)
    {
        *_pI2C_CCR |= I2C_CCR_DUTY;
    }
    else
    {
        *_pI2C_CCR &= ~I2C_CCR_DUTY;
    }
}

void I2C::SetTRISE(uint8_t val)
{
    *_pI2C_TRISE = (val & 0x1f);
}


uint8_t I2C::GetPEC()
{
    return (*_pI2C_SR2 & (I2C_SR2_PEC<<8));
}

bool I2C::IsDualF()
{
    return (*_pI2C_SR2 & I2C_SR2_DUALF);
}

bool I2C::IsSMBHostAddr()
{   return (*_pI2C_SR2 & I2C_SR2_SMBHOST);

}
bool I2C::IsSMBDefaultAddr()
{
    return (*_pI2C_SR2 & I2C_SR2_SMBDEFAULT);
}
bool I2C::IsGenCallAddr()
{
    return (*_pI2C_SR2 & I2C_SR2_GENCALL);
}
bool I2C::IsTransmitter()
{
    return (*_pI2C_SR2 & I2C_SR2_TRA);
}
bool I2C::IsBusy()
{
    return (*_pI2C_SR2 & I2C_SR2_BUSY);
}
bool I2C::IsMasterMode()
{
    return (*_pI2C_SR2 & I2C_SR2_MSL);
}


void I2C::Send7bitAddress (uint8_t slaveAddr, bool isWrite)
{
    if (isWrite)
    {
        slaveAddr &= ~0x1;
    }
    else
    {
        slaveAddr |= 0x1;
    }
    *_pI2C_DR = slaveAddr;
}

void I2C::SendByte  (uint8_t data)
{
    *_pI2C_DR = data;
}

uint8_t I2C::ReceiveByte ()
{
    return *_pI2C_DR;
}

//#define FLAG_Mask               ((uint32_t)0x00FFFFFF)

bool I2C::WaitStatus (I2C_STATUS_MASK status_mask, bool set)
{

    //uint32_t lastevent = 0;
    //uint32_t flag1 = 0, flag2 = 0;
    bool rc = false;
    uint32_t retryCount = _retryCount;

    while(--retryCount)
    {
        //flag1 = *_pI2C_SR1;
        //flag2 = *_pI2C_SR2;
        //flag2 = (flag2 & 0xff) << 16;

        //lastevent = (flag1 | flag2);

        uint32_t status = *_pI2C_SR1 | (*_pI2C_SR2 & 0xff) << 16;

        bool is_set = (status & status_mask) == status_mask;
        if (set == is_set)
        {
            rc = true;
            break;
        }

        if (IsError())
        {
            break;
        }

    }

    if (!rc)
    {
        ++_errorCount;
        if (retryCount == 0)
        {
            ++_errorCountRetry;
        }
    }

    return rc;
}


bool I2C::MasterWrite(uint8_t slave_addr, uint8_t reg_addr, uint32_t length, uint8_t* data)
{
    bool rc = _MasterWrite (slave_addr<<1, reg_addr, length, data);

    if (!rc)
    {
        if (_errorCallBack)
        {
            _errorCallBack(_errorCallBackArg);
        }

        if (_errorHandler)
        {
            _errorHandler->HandleError();
        }
    }
    return rc;
}


bool I2C::_MasterWrite(uint8_t slave_addr, uint8_t reg_addr, uint32_t length, uint8_t* data)
{
      if (length == 0)
        {
            return true;
        }
        
    if (!WaitStatus(I2C_BUSY, false))
    {
        return false;
    }

    SendStart(true);

    if (!WaitStatus(MASTER_EV5, true))
        return false;

    Send7bitAddress(slave_addr, true);

    if (!WaitStatus(MASTER_EV6_TRA, true)) //MSL | BUSY | ADDR | TXE | TRA
        return false;

    ////////////////////////////////////////////////////////////////////////////
    SendByte(reg_addr);
    if (!WaitStatus(MASTER_EV8_2, true))  //TRA, BUSY, MSL, TXE and BTF
    	return false;
    ////////////////////////////////////////////////////////////////////////////

    if (length == 1 || !_dmaTxStream)
    {
        SendByte(*data);

        if (!WaitStatus(MASTER_EV8, true))  //TRA, BUSY, MSL, TXE and BTF
            return false;

        SendStop(true);

        return true;

    }
    else
    {
        return SendDMA (data, length);
    }

}




bool I2C::MasterRead(uint8_t slave_addr,  uint8_t reg_addr, uint32_t length, uint8_t* data)
{
    bool rc =  _MasterRead (slave_addr<<1, reg_addr, length, data);
    if (!rc)
    {
        if (_errorCallBack)
        {
            _errorCallBack(_errorCallBackArg);
        }

        if (_errorHandler)
        {
            _errorHandler->HandleError();
        }
    }
    return rc;
}

bool I2C::_MasterRead (uint8_t slave_addr, uint8_t reg_addr, uint32_t length, uint8_t* data)
{
    if (length == 0)
    {
        return true;
    }

    if (!WaitStatus(I2C_BUSY, false))
    {
        return false;
    }


    SendStart(true);

    if (!WaitStatus(MASTER_EV5, true)) //BSY MSL SB
            return false;

    Send7bitAddress(slave_addr, true);

    if (!WaitStatus(MASTER_EV6_TRA, true)) //MSL | BUSY | ADDR | TXE | TRA
        return false;

    SendByte(reg_addr);

    if (!WaitStatus(MASTER_EV8_2, true))  //TRA, BUSY, MSL, TXE and BTF
        return false;


    SendStart(true);

    if (!WaitStatus(MASTER_EV5, true)) //BSY MSL SB
        return false;

    Send7bitAddress(slave_addr, false);

    if (!WaitStatus(MASTER_EV6_RECV, true)) //MSL | BUSY | ADDR
        return false;

    //////////////////////////////////////////////
    if (length > 1 && _dmaRxStream)
    {
        return ReceiveDMA(data, length);
    }
    else
    {
        while (length)
        {
            if (length == 1)
            {
                EnableACK(false);
                SendStop(true);
            }
            if (!WaitStatus(RXNE, true)) //or I2C::RXNE //MASTER_EV7
            {
                return false;
            }

            *data = ReceiveByte();
            data++;
            length--;

        }


        EnableACK(true);
        return true;
    }


}



bool I2C::SlaveReceiveData(uint8_t* data, uint8_t data_size)
{
    if (!WaitStatus(SLAVE_RECV_EV1, true))
    {
        return false;
    }

    for (uint8_t i = 0; i < data_size; ++i)
    {
        if (!WaitStatus(SLAVE_RECV_EV2, true))
        {
            return false;
        }
        data[i] = ReceiveByte();

    }

    if (!WaitStatus(SLAVE_RECV_EV4, true))
    {
        return false;
    }


    ClearSTOPF();


    return true;
}

bool I2C::SlaveSendData(uint8_t* data, uint8_t data_size)
{
    if (!WaitStatus(SLAVE_TRA_EV1, true))
    {
        return false;
    }

    for (uint8_t i = 0; i < data_size; ++i)
    {
        if (!WaitStatus(SLAVE_TRA_EV3, true))
        {
            return false;
        }

        SendByte (data[i]);
    }

    if (!WaitStatus(BTF, true))
    {
        return false;
    }


    if (!WaitStatus(SLAVE_TRA_EV3_2, true))
    {
        return false;
    }


    ClearAF();

    return true;
}


void I2C::SetDMA_TxStream (DMA_Stream* stream)
{
    _dmaTxStream = stream;
}

void I2C::SetDMA_RxStream (DMA_Stream* stream)
{
    _dmaRxStream = stream;
}

void I2C::Reinitialize()
{
    uint16_t save_cr1 = *_pI2C_CR1;
    uint16_t save_cr2 = *_pI2C_CR2;
    uint16_t save_ccr = *_pI2C_CCR;
    uint16_t save_trise = *_pI2C_TRISE;


    SetSoftwareReset(true);
    SetSoftwareReset(false);

    *_pI2C_CCR = save_ccr;
    *_pI2C_TRISE = save_trise;
    *_pI2C_CR2 = save_cr2;
    *_pI2C_CR1 = save_cr1;
}








