
#ifndef GPIO_CPP_H
#define GPIO_CPP_H

#include "gpio.h"
#include "rcc.h"
#include "peripheral.h"

class GPIO_PORT;

class GPIO_PIN
{
    friend class GPIO_PORT;
private:
    GPIO_PORT* gpioPort;
    GPIO_PIN_N pin;

    GPIO_PIN()
    {
        gpioPort = 0;
        pin = GPIO_PIN0;
    }

    void _init (GPIO_PORT* gpioPort, GPIO_PIN_N pin)
    {
        this->gpioPort = gpioPort;
        this->pin = pin;
    }
public:
    void Set ();
    void Toggle();
    void Reset ();
    bool IsSet ();
    void SetupGPIO_InFloat ();
    void SetupGPIO_InAnalog ();
    void SetupGPIO_InPullUp ();
    void SetupGPIO_InPullDown ();
    void SetupGPIO_OutPP ();
    void SetupGPIO_OutOD ();
    void SetupGPIO_OutAltOD ();
    void SetupGPIO_OutAltPP ();
    void SetSpeedLow ();
    void SetSpeedMedium ();
    void SetSpeedHigh ();
    void PullUp();
    void PullDown();

    GPIO_PIN_N GetPinNumber() { return pin; }

    GPIO_PORT* GetPort()
    {   return gpioPort;}

};

class GPIO_PORT : public Peripheral
{
private:

    GPIO_PIN _pins[16];

    volatile uint64_t* _pGPIO_CR;
    volatile uint32_t* _pGPIO_IDR;
    volatile uint32_t* _pGPIO_ODR;
    volatile uint32_t* _pGPIO_BSRR;
    volatile uint32_t* _pGPIO_BRR;
    volatile uint32_t* _pGPIO_LCKR;

protected:
    GPIO_PORT(volatile uint64_t* pGPIO_CR,
            volatile uint32_t* pGPIO_IDR,
            volatile uint32_t* pGPIO_ODR,
            volatile uint32_t* pGPIO_BSRR,
            volatile uint32_t* pGPIO_BRR,
            volatile uint32_t* pGPIO_LCK,
            volatile uint32_t* pRCCEnableReg,
            volatile uint32_t* pRCCResetReg,
            uint32_t rccBit);
public:

    void SetupGPIO (GPIO_PIN_N pin, uint32_t cnf, GPIO_MODE mode);

    void SetupGPIO_Cnf(GPIO_PIN_N pin, uint32_t cnf);
    void SetupGPIO_Mode(GPIO_PIN_N pin, uint32_t mode);

    void SetupGPIO_InFloat (GPIO_PIN_N pin);
    void SetupGPIO_InAnalog (GPIO_PIN_N pin);
    void SetupGPIO_InPullUp (GPIO_PIN_N pin);
    void SetupGPIO_InPullDown (GPIO_PIN_N pin);
    void SetupGPIO_OutPP (GPIO_PIN_N pin);
    void SetupGPIO_OutOD (GPIO_PIN_N pin);
    void SetupGPIO_OutAltOD (GPIO_PIN_N pin);
    void SetupGPIO_OutAltPP (GPIO_PIN_N pin);

    void SetSpeedLow (GPIO_PIN_N pin);
    void SetSpeedMedium (GPIO_PIN_N pin);
    void SetSpeedHigh (GPIO_PIN_N pin);


    void SetOutput (uint16_t value);
    uint16_t GetOutput();
    void SetPin (GPIO_PIN_N pin);
    void TogglePin (GPIO_PIN_N pin);
    void ResetPin (GPIO_PIN_N pin);
    uint32_t GetInput();
    bool IsSet (GPIO_PIN_N pin);
    void Lock (uint16_t bitMask);

    GPIO_PIN* GetPin(GPIO_PIN_N pin);
    volatile uint64_t* GetGPIO_CR() { return _pGPIO_CR; }
    volatile uint32_t* GetGPIO_BRR() { return _pGPIO_BRR; }
    volatile uint32_t* GetGPIO_BSRR() { return _pGPIO_BSRR; }
    volatile uint32_t* GetGPIO_ODR() { return _pGPIO_ODR; }

};

class GPIOA : public GPIO_PORT
{
public:

    static GPIOA* GetInstance()
    {
        static GPIOA instance;
        return &instance;
    }


private:

    GPIOA() :
    GPIO_PORT (&GPIOA_CR, &GPIOA_IDR, &GPIOA_ODR, &GPIOA_BSRR, &GPIOA_BRR, &GPIOA_LCKR,
            &RCC_APB2ENR, &RCC_APB2RSTR, RCC_APB2ENR_IOPAEN)
    {
    }

    GPIOA(GPIOA const&);
    void operator=(GPIOA const&);
};

class GPIOB : public GPIO_PORT
{
public:

    static GPIOB* GetInstance()
    {
        static GPIOB instance;
        return &instance;
    }


private:

    GPIOB() :
    GPIO_PORT (&GPIOB_CR, &GPIOB_IDR, &GPIOB_ODR, &GPIOB_BSRR, &GPIOB_BRR, &GPIOB_LCKR,
            &RCC_APB2ENR, &RCC_APB2RSTR, RCC_APB2ENR_IOPBEN)

    {
    }

    GPIOB(GPIOB const&);
    void operator=(GPIOB const&);
};

class GPIOC : public GPIO_PORT
{
public:

    static GPIOC* GetInstance()
    {
        static GPIOC instance;
        return &instance;
    }


private:

    GPIOC() :
    GPIO_PORT (&GPIOC_CR, &GPIOC_IDR, &GPIOC_ODR, &GPIOC_BSRR, &GPIOC_BRR, &GPIOC_LCKR,
            &RCC_APB2ENR, &RCC_APB2RSTR, RCC_APB2ENR_IOPCEN)

    {
    }

    GPIOC(GPIOC const&);
    void operator=(GPIOC const&);
};

class GPIOD : public GPIO_PORT
{
public:

    static GPIOD* GetInstance()
    {
        static GPIOD instance;
        return &instance;
    }


private:

    GPIOD() :
    GPIO_PORT (&GPIOD_CR, &GPIOD_IDR, &GPIOD_ODR, &GPIOD_BSRR, &GPIOD_BRR, &GPIOD_LCKR,
            &RCC_APB2ENR, &RCC_APB2RSTR, RCC_APB2ENR_IOPDEN)

    {
    }

    GPIOD(GPIOD const&);
    void operator=(GPIOD const&);
};

class GPIOE : public GPIO_PORT
{
public:

    static GPIOE* GetInstance()
    {
        static GPIOE instance;
        return &instance;
    }


private:

    GPIOE() :
    GPIO_PORT (&GPIOE_CR, &GPIOE_IDR, &GPIOE_ODR, &GPIOE_BSRR, &GPIOE_BRR, &GPIOE_LCKR,
            &RCC_APB2ENR, &RCC_APB2RSTR, RCC_APB2ENR_IOPEEN)

    {
    }

    GPIOE(GPIOE const&);
    void operator=(GPIOE const&);
};

class GPIOF : public GPIO_PORT
{
public:

    static GPIOF* GetInstance()
    {
        static GPIOF instance;
        return &instance;
    }


private:

    GPIOF() :
    GPIO_PORT (&GPIOF_CR, &GPIOF_IDR, &GPIOF_ODR, &GPIOF_BSRR, &GPIOF_BRR, &GPIOF_LCKR,
            &RCC_APB2ENR, &RCC_APB2RSTR, RCC_APB2ENR_IOPFEN)

    {
    }

    GPIOF(GPIOF const&);
    void operator=(GPIOF const&);
};

class GPIOG : public GPIO_PORT
{
public:

    static GPIOG* GetInstance()
    {
        static GPIOG instance;
        return &instance;
    }


private:

    GPIOG() :
    GPIO_PORT (&GPIOG_CR, &GPIOG_IDR, &GPIOG_ODR, &GPIOG_BSRR, &GPIOG_BRR, &GPIOG_LCKR,
            &RCC_APB2ENR, &RCC_APB2RSTR, RCC_APB2ENR_IOPGEN)

    {
    }

    GPIOG(GPIOG const&);
    void operator=(GPIOG const&);
};

#endif

