#ifndef _GPIO_H
#define _GPIO_H


#define    GPIOA 0
#define    GPIOB 1
#define    GPIOC 2
#define    GPIOD 3
#define    GPIOE 4
#define    GPIOF 5
#define    GPIOG 6

#define GPIO_PIN0  1<<0
#define GPIO_PIN1  1<<1
#define GPIO_PIN2  1<<2
#define GPIO_PIN3  1<<3
#define GPIO_PIN4  1<<4
#define GPIO_PIN5  1<<5
#define GPIO_PIN6  1<<6
#define GPIO_PIN7  1<<7
#define GPIO_PIN8  1<<8
#define GPIO_PIN9  1<<9
#define GPIO_PIN10 1<<10
#define GPIO_PIN11 1<<11
#define GPIO_PIN12 1<<12
#define GPIO_PIN13 1<<13
#define GPIO_PIN14 1<<14
#define GPIO_PIN15 1<<15

#define GPIO_CFG_SIZE 28

#if defined (__GNUC__) && defined(__ASSEMBLY__)

#define GPIOA_BASE 0x40010800
#define GPIOB_BASE 0x40010C00
#define GPIOC_BASE 0x40011000
#define GPIOD_BASE 0x40011400
#define GPIOE_BASE 0x40011800
#define GPIOF_BASE 0x40011C00
#define GPIOG_BASE 0x40012000

#define GPIO_OFF_CRL   0x00
#define GPIO_OFF_CRH   0x04
#define GPIO_OFF_IDR  0x08
#define GPIO_OFF_ODR  0x0c
#define GPIO_OFF_BSRR 0x10
#define GPIO_OFF_BRR  0x14
#define GPIO_OFF_LCKR 0x18



#define GPIO_MODE_INP              0
#define GPIO_MODE_OUT_MEDIUM_SPEED 1  //10MHZ
#define GPIO_MODE_OUT_LOW_SPEED    2    //2MHZ
#define GPIO_MODE_OUT_HIGH_SPEED   3          //50MHZ

#define GPIO_CNF_IANAL   0
#define GPIO_CNF_IFLOAT  1
#define GPIO_CNF_IPUPD   2

#define     GPIO_CNF_OPP     0 
#define     GPIO_CNF_OD      1
#define     GPIO_CNF_ALT_PP  2
#define     GPIO_CNF_ALT_OD  3

#define     GPIOG_LCKR_LCKK (1<<16)



#else

#include <stdint.h>


typedef struct
{
    uint32_t GPIO_base;
    uint32_t crl;
    uint32_t crh;
    uint32_t idr;
    uint32_t odr;
    uint32_t bsrr;
    uint32_t brr;
} GPIO_CFG;


void GPIO_init(uint8_t port_no,  GPIO_CFG* cfg);
void GPIO_cfg(GPIO_CFG* cfg, uint16_t pin, uint8_t cnf, uint8_t mode);
void GPIO_cfg_in_float (GPIO_CFG* cfg, uint16_t pin);
void GPIO_cfg_in_anal (GPIO_CFG* cfg, uint16_t pin);
void GPIO_cfg_in_pullup (GPIO_CFG* cfg, uint16_t pin);
void GPIO_cfg_in_pulldown (GPIO_CFG* cfg, uint16_t pin);
void GPIO_cfg_out_pp (GPIO_CFG* cfg, uint16_t pin);
void GPIO_cfg_out_od (GPIO_CFG* cfg, uint16_t pin);
void GPIO_cfg_out_alt_pp (GPIO_CFG* cfg, uint16_t pin);
void GPIO_cfg_out_alt_od (GPIO_CFG* cfg, uint16_t pin);
void GPIO_set_speed_low (GPIO_CFG* cfg, uint16_t pin);
void GPIO_set_speed_medium (GPIO_CFG* cfg, uint16_t pin);
void GPIO_set_speed_high (GPIO_CFG* cfg, uint16_t pin);
void GPIO_set_pin (GPIO_CFG* cfg, uint16_t pin);
void GPIO_reset_pin (GPIO_CFG* cfg, uint16_t pin);
void GPIO_set_output(GPIO_CFG* cfg, uint16_t out);
int  GPIO_is_set(GPIO_CFG* cfg, uint16_t pin);
int  GPIO_toggle_pin(GPIO_CFG* cfg, uint16_t pin);
#endif

#endif
