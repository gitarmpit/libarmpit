#ifndef _GPIO_H
#define _GPIO_H 
#include "common.h"
#include "port.h"
#define GPIOA_BASE 0x400FF000 
#define GPIOB_BASE 0x400FF040
#define GPIOC_BASE 0x400FF080
#define GPIOD_BASE 0x400FF0C0
#define GPIOE_BASE 0x400FF100

#define GPIOA_PDOR  (*(volatile uint32_t*)(GPIOA_BASE + 0x0))
#define GPIOB_PDOR  (*(volatile uint32_t*)(GPIOB_BASE + 0x0))
#define GPIOC_PDOR  (*(volatile uint32_t*)(GPIOC_BASE + 0x0))
#define GPIOD_PDOR  (*(volatile uint32_t*)(GPIOD_BASE + 0x0))
#define GPIOE_PDOR  (*(volatile uint32_t*)(GPIOE_BASE + 0x0))

#define GPIOA_PSOR  (*(volatile uint32_t*)(GPIOA_BASE + 0x4))
#define GPIOB_PSOR  (*(volatile uint32_t*)(GPIOB_BASE + 0x4))
#define GPIOC_PSOR  (*(volatile uint32_t*)(GPIOC_BASE + 0x4))
#define GPIOD_PSOR  (*(volatile uint32_t*)(GPIOD_BASE + 0x4))
#define GPIOE_PSOR  (*(volatile uint32_t*)(GPIOE_BASE + 0x4))

#define GPIOA_PCOR  (*(volatile uint32_t*)(GPIOA_BASE + 0x8))
#define GPIOB_PCOR  (*(volatile uint32_t*)(GPIOB_BASE + 0x8))
#define GPIOC_PCOR  (*(volatile uint32_t*)(GPIOC_BASE + 0x8))
#define GPIOD_PCOR  (*(volatile uint32_t*)(GPIOD_BASE + 0x8))
#define GPIOE_PCOR  (*(volatile uint32_t*)(GPIOE_BASE + 0x8))

#define GPIOA_PTOR  (*(volatile uint32_t*)(GPIOA_BASE + 0xC))
#define GPIOB_PTOR  (*(volatile uint32_t*)(GPIOB_BASE + 0xC))
#define GPIOC_PTOR  (*(volatile uint32_t*)(GPIOC_BASE + 0xC))
#define GPIOD_PTOR  (*(volatile uint32_t*)(GPIOD_BASE + 0xC))
#define GPIOE_PTOR  (*(volatile uint32_t*)(GPIOE_BASE + 0xC))

#define GPIOA_PDIR  (*(volatile uint32_t*)(GPIOA_BASE + 0x10))
#define GPIOB_PDIR  (*(volatile uint32_t*)(GPIOB_BASE + 0x10))
#define GPIOC_PDIR  (*(volatile uint32_t*)(GPIOC_BASE + 0x10))
#define GPIOD_PDIR  (*(volatile uint32_t*)(GPIOD_BASE + 0x10))
#define GPIOE_PDIR  (*(volatile uint32_t*)(GPIOE_BASE + 0x10))

#define GPIOA_PDDR  (*(volatile uint32_t*)(GPIOA_BASE + 0x14))
#define GPIOB_PDDR  (*(volatile uint32_t*)(GPIOB_BASE + 0x14))
#define GPIOC_PDDR  (*(volatile uint32_t*)(GPIOC_BASE + 0x14))
#define GPIOD_PDDR  (*(volatile uint32_t*)(GPIOD_BASE + 0x14))
#define GPIOE_PDDR  (*(volatile uint32_t*)(GPIOE_BASE + 0x14))

typedef enum
{
 GPIO_PIN0  = 0,
 GPIO_PIN1  = 1,
 GPIO_PIN2  = 2,
 GPIO_PIN3  = 3,
 GPIO_PIN4  = 4,
 GPIO_PIN5  = 5,
 GPIO_PIN6  = 6,
 GPIO_PIN7  = 7,
 GPIO_PIN8  = 8,
 GPIO_PIN9  = 9,
 GPIO_PIN10 = 10,
 GPIO_PIN11 = 11,
 GPIO_PIN12 = 12,
 GPIO_PIN13 = 13,
 GPIO_PIN14 = 14,
 GPIO_PIN15 = 15,
 GPIO_PIN16 = 16,
 GPIO_PIN17 = 17,
 GPIO_PIN18 = 18,
 GPIO_PIN19 = 19,
 GPIO_PIN20 = 20,
 GPIO_PIN21 = 21,
 GPIO_PIN22 = 22,
 GPIO_PIN23 = 23,
 GPIO_PIN24 = 24,
 GPIO_PIN25 = 25,
 GPIO_PIN26 = 26,
 GPIO_PIN27 = 27,
 GPIO_PIN28 = 28,
 GPIO_PIN29 = 29,
 GPIO_PIN30 = 30,
 GPIO_PIN31 = 31
} GPIO_PIN_N;

typedef enum
{
    PORTA = 0,
    PORTB = 1,
    PORTC = 2,
    PORTD = 3,
    PORTE = 4,
    PORTF = 5,
    PORTG = 6
} GPIO_PORT_N;


typedef struct
{
    volatile uint32_t* SIM_enableReg;
    uint32_t           SIM_enableMask;

    volatile uint32_t* GPIO_PDOR; // Data Output
    volatile uint32_t* GPIO_PSOR; // Port Set Output
    volatile uint32_t* GPIO_PCOR; // Port Clear Output
    volatile uint32_t* GPIO_PTOR; // Port Toggle Output
    volatile uint32_t* GPIO_PDIR; // Port Data Input
    volatile uint32_t* GPIO_PDDR; // Port Data Direction

    volatile uint32_t* PORT_ISFR;
    uint32_t           PORT_PCR_BASE; // Pin Control Register
    void (*interrupt_handler)(GPIO_PORT_N, GPIO_PIN_N);

} GPIO_PORT;


typedef struct
{
    GPIO_PORT* port;
    GPIO_PIN_N  pinN;
    uint32_t    pinMask;
    volatile uint32_t* PORT_PCR;
} GPIO_PIN;

#ifdef __cplusplus
extern "C"
{
#endif


GPIO_PORT* GPIO_GetInstance(GPIO_PORT_N port);

GPIO_PIN  GPIO_GetPin(GPIO_PORT* port, GPIO_PIN_N pin);

void GPIO_EnableClock(GPIO_PORT* port, BOOL enable);

void GPIO_SetupOut(GPIO_PIN* pin);
void GPIO_SetupIn(GPIO_PIN* pin);
void GPIO_SetupAnalog(GPIO_PIN* pin);

void GPIO_SetSlewRateFast(GPIO_PIN* pin);
void GPIO_SetSlewRateSlow(GPIO_PIN* pin);

void GPIO_EnableInterruptRisingEdge(GPIO_PIN* pin);
void GPIO_EnableInterruptFallingEdge(GPIO_PIN* pin);
void GPIO_EnableInterruptEitherEdge(GPIO_PIN* pin);

void GPIO_SetInterruptHandler(GPIO_PORT* port, void (*handler)(GPIO_PORT_N, GPIO_PIN_N));
BOOL GPIO_IsInterrupt(GPIO_PIN* pin);
void GPIO_ClearInterrupt(GPIO_PIN* pin);

void GPIO_SetPin(GPIO_PIN* pin);
void GPIO_ResetPin(GPIO_PIN* pin);
void GPIO_TogglePin(GPIO_PIN* pin);
BOOL GPIO_IsSet(GPIO_PIN* pin);
void GPIO_SetOutput(GPIO_PORT* port, uint32_t value);
uint32_t GPIO_GetInput(GPIO_PORT* port);
void GPIO_PullDown(GPIO_PIN* pin);
void GPIO_PullUp(GPIO_PIN* pin);
void GPIO_SetAF(GPIO_PIN* pin, uint8_t alt);

#ifdef __cplusplus
}
#endif


#endif
