//
//
//#ifndef _RTC_CPP_H
//#define _RTC_CPP_H
//
//#include "rtc.h"
//
//
//
//class RTC_Interrupt_Handler
//{
//public:
//    virtual void HandleInterrupt (bool isOverrun, bool isAlarm, bool isSecond) = 0;
//    virtual ~RTC_Interrupt_Handler(){}
//
//};
//
//class RTC
//{
//private:
//    RTC()
//{
//        _overrunHandler = 0;
//        _alarmHandler = 0;
//        _secondHandler = 0;
//}
//
//    RTC(RTC const&);
//    void operator=(RTC const&);
//
//
//    RTC_Interrupt_Handler* _overrunHandler;
//    RTC_Interrupt_Handler* _alarmHandler;
//    RTC_Interrupt_Handler* _secondHandler;
//
//    void EnterConfigurationMode();
//    void ExitConfigurationMode();
//
//public:
//
//    static RTC* GetInstance()
//    {
//        static RTC instance;
//        return &instance;
//    }
//
//    void SetClockSourceHSE_DIV128();
//    void SetClockSourceLSE();
//    void SetClockSourceLSI();
//    void EnableClock(bool enable);
//    void WaitRSF();
//
//    void EnableOverrunInterrupt (bool enable, RTC_Interrupt_Handler* overrunHandler = 0);
//    void EnableAlarmInterrupt (bool enable, RTC_Interrupt_Handler* alarmHandler = 0);
//    void EnableSecondInterrupt (bool enable, RTC_Interrupt_Handler* secondHandler = 0);
//
//    void HandleInterrupt ();
//
//    bool IsOverrun();
//    bool IsAlarm();
//    bool IsSecond();
//
//    void ClearOverrun();
//    void ClearAlarm();
//    void ClearSecond();
//
//    void SetPrescaler (uint32_t val);
//    void SetCounter (uint32_t val);
//    uint32_t GetCounter();
//    void SetAlarm (uint32_t val);
//    void SetPrescaler_ms (uint32_t val);
//    void SetAlarm_ms (uint32_t val);
//
//    void OutputAlarmPulse (bool enable);
//    void OutputSecondPulse (bool enable);
//
//
//};
//
//#endif
