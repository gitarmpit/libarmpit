
#ifndef _BKP_H
#define _BKP_H

#include "common.h"


#include "rcc.h"
#include "peripheral.h"

/* BACKUP  APB1 ****************************************************/

#define BKP_OFFSET 0x6C00
#define BKP_BASE  (PERIPH_BASE + BKP_OFFSET)
#define BKP_RTCCR  (*(volatile uint32_t*)(BKP_BASE + 0x2c))
#define BKP_CR    (*(volatile uint32_t*)(BKP_BASE + 0x30))
#define BKP_CSR    (*(volatile uint32_t*)(BKP_BASE + 0x34))

#define BKP_RTCCR_CCO  (1<<7) //Outputs the RTC clock with a frequency divided by 64 on the TAMPER pin
#define BKP_RTCCR_ASOE (1<<8) //Alarm / second output enable
#define BKP_RTCCR_ASOS (1<<9) //Alarm or second output selection if ASOE is set 
//0: RTC Alarm pulse output selected
//1: RTC Second pulse output selected

#define BKP_CR_TPE  (1<<0) //TAMPER pin enable
#define BKP_CR_TPAL (1<<1)

#define BKP_CSR_CTE   (1<<0)
#define BKP_CSR_CTI   (1<<1)
#define BKP_CSR_TPIE  (1<<2)
#define BKP_CSR_TEF   (1<<8)
#define BKP_CSR_TIF   (1<<9)

class BKP_TamperInterrupt_Handler
{
public:
    virtual void HandleInterrupt(bool isEvent) = 0;
    virtual ~BKP_TamperInterrupt_Handler() {}
};

class BKP: public Peripheral
{
private:
    BKP() :  Peripheral(&RCC_APB1ENR, &RCC_APB1RSTR, RCC_APB1ENR_BKPEN)
    {
        _intHandler = 0;
    }


    BKP_TamperInterrupt_Handler* _intHandler;

    BKP(BKP const&);
    void operator=(BKP const&);

    volatile uint32_t* GetBackupRegAddr(uint8_t regno);

public:

    static BKP* GetInstance()
    {
        static BKP instance;
        return &instance;
    }

    void EnableWriteAccess(bool enable);
    void ResetBackupDomain();
    void OutputRTCClock_div64(bool enable);
    void OutputAlarmPulse(bool enable);
    void OutputSecondPulse(bool enable);
    void SetRTCCalibrationValue(uint8_t val);
    void WriteBackupData(uint16_t data, uint8_t regno);  //1-42
    uint16_t ReadBackupData(uint8_t regno);

    void EnableTamperPin(bool enable, bool isActiveLow = false);
    void EnableTamperInterrupt(bool enable,
            BKP_TamperInterrupt_Handler* handler);

    void HandleInterrupt();

    bool IsTamperEvent();
    bool IsTamperInterrupt();
    void ClearTamperInterrupt();
    void ClearTamperEvent();

};

#endif
