#ifndef _GPIO_H
#define _GPIO_H


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





#define GPIOG_LCKR_LCKK (1<<16)

//C only:
#ifndef __ASSEMBLY__


typedef enum
{
    GPIOA = 0,
    GPIOB = 1,
    GPIOC = 2,
    GPIOD = 3,
    GPIOE = 4,
    GPIOF = 5,
    GPIOG = 6
} GPIO_PORT ;

typedef enum
{
 GPIO_PIN0  = 1<<0,
 GPIO_PIN1  = 1<<1,
 GPIO_PIN2  = 1<<2,
 GPIO_PIN3  = 1<<3,
 GPIO_PIN4  = 1<<4,
 GPIO_PIN5  = 1<<5,
 GPIO_PIN6  = 1<<6,
 GPIO_PIN7  = 1<<7,
 GPIO_PIN8  = 1<<8,
 GPIO_PIN9  = 1<<9,
 GPIO_PIN10 = 1<<10,
 GPIO_PIN11 = 1<<11,
 GPIO_PIN12 = 1<<12,
 GPIO_PIN13 = 1<<13,
 GPIO_PIN14 = 1<<14,
 GPIO_PIN15 = 1<<15
} GPIO_PIN;

typedef struct
{
    uint32_t gpio_base;
    uint32_t crl;
    uint32_t crh;
    uint32_t idr;
    uint32_t odr;
    uint32_t bsrr;
    uint32_t brr;
} GPIO_CFG;

void gpio_init(GPIO_PORT port_no,  GPIO_CFG* cfg);
void gpio_cfg(GPIO_CFG* cfg, GPIO_PIN pin, uint8_t cnf, uint8_t mode);
void gpio_cfg_in_float (GPIO_CFG* cfg, GPIO_PIN pin);
void gpio_cfg_in_anal (GPIO_CFG* cfg, GPIO_PIN pin);
void gpio_cfg_in_pullup (GPIO_CFG* cfg, GPIO_PIN pin);
void gpio_cfg_in_pulldown (GPIO_CFG* cfg, GPIO_PIN pin);
void gpio_cfg_out_pp (GPIO_CFG* cfg, GPIO_PIN pin);
void gpio_cfg_out_od (GPIO_CFG* cfg, GPIO_PIN pin);
void gpio_cfg_out_alt_pp (GPIO_CFG* cfg, GPIO_PIN pin);
void gpio_cfg_out_alt_od (GPIO_CFG* cfg, GPIO_PIN pin);
void gpio_set_speed_low (GPIO_CFG* cfg, GPIO_PIN pin);
void gpio_set_speed_medium (GPIO_CFG* cfg, GPIO_PIN pin);
void gpio_set_speed_high (GPIO_CFG* cfg, GPIO_PIN pin);
void gpio_set_pin (GPIO_CFG* cfg, GPIO_PIN pin);
void gpio_reset_pin (GPIO_CFG* cfg, GPIO_PIN pin);
void gpio_set_output(GPIO_CFG* cfg, uint16_t out);
int  gpio_is_set(GPIO_CFG* cfg, GPIO_PIN pin);
void gpio_toggle_pin(GPIO_CFG* cfg, GPIO_PIN pin);

#endif

#endif
