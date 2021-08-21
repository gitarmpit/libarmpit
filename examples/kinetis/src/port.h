#ifndef _PORT_H
#define _PORT_H

#define PORT_BASE     0x40049000

#define PORTA_BASE    PORT_BASE + 0x0
#define PORTB_BASE    PORT_BASE + 0x1000
#define PORTC_BASE    PORT_BASE + 0x2000
#define PORTD_BASE    PORT_BASE + 0x3000
#define PORTE_BASE    PORT_BASE + 0x4000

#define PORTA_PCR_BASE     PORTA_BASE
#define PORTB_PCR_BASE     PORTB_BASE
#define PORTC_PCR_BASE     PORTC_BASE
#define PORTD_PCR_BASE     PORTD_BASE
#define PORTE_PCR_BASE     PORTE_BASE

#define PORTA_GPCLR   (*(volatile uint32_t*)(PORTA_BASE + 0x80)) 
#define PORTB_GPCLR   (*(volatile uint32_t*)(PORTB_BASE + 0x80)) 
#define PORTC_GPCLR   (*(volatile uint32_t*)(PORTC_BASE + 0x80)) 
#define PORTD_GPCLR   (*(volatile uint32_t*)(PORTD_BASE + 0x80)) 
#define PORTE_GPCLR   (*(volatile uint32_t*)(PORTE_BASE + 0x80)) 

#define PORTA_GPCHR   (*(volatile uint32_t*)(PORTA_BASE + 0x84)) 
#define PORTB_GPCHR   (*(volatile uint32_t*)(PORTB_BASE + 0x84)) 
#define PORTC_GPCHR   (*(volatile uint32_t*)(PORTC_BASE + 0x84)) 
#define PORTD_GPCHR   (*(volatile uint32_t*)(PORTD_BASE + 0x84)) 
#define PORTE_GPCHR   (*(volatile uint32_t*)(PORTE_BASE + 0x84)) 

// interrupt per pin
#define PORTA_ISFR    (*(volatile uint32_t*)(PORTA_BASE + 0xA0)) 
#define PORTB_ISFR    (*(volatile uint32_t*)(PORTB_BASE + 0xA0)) 
#define PORTC_ISFR    (*(volatile uint32_t*)(PORTC_BASE + 0xA0)) 
#define PORTD_ISFR    (*(volatile uint32_t*)(PORTD_BASE + 0xA0)) 
#define PORTE_ISFR    (*(volatile uint32_t*)(PORTE_BASE + 0xA0)) 

#define PORT_PCR_ISF  (1<<24)

#define PORT_PCR_IRQC_OFF         (0<<16)
#define PORT_PCR_IRQC_DMA_RISING  (1<<16)
#define PORT_PCR_IRQC_DMA_FALLING (2<<16)
#define PORT_PCR_IRQC_DMA_EITHER  (3<<16)
#define PORT_PCR_IRQC_INT_ZERO    (8<<16)
#define PORT_PCR_IRQC_INT_RISING  (9<<16)
#define PORT_PCR_IRQC_INT_FALLING (10<<16)
#define PORT_PCR_IRQC_INT_EITHER  (11<<16)
#define PORT_PCR_IRQC_INT_ONE     (12<<16)
#define PORT_PCR_IRQC_CLEARMASK   (0xf<<16)

#define PORT_PCR_MUX_OFF          (0<<8)
#define PORT_PCR_MUX_ALT1         (1<<8)
#define PORT_PCR_MUX_ALT2         (2<<8)
#define PORT_PCR_MUX_ALT3         (3<<8)
#define PORT_PCR_MUX_ALT4         (4<<8)
#define PORT_PCR_MUX_ALT5         (5<<8)
#define PORT_PCR_MUX_ALT6         (6<<8)
#define PORT_PCR_MUX_ALT7         (7<<8)
#define PORT_PCR_MUX_CLEARMASK    (7<<8)

#define PORT_PCR_DSE (1<<6)
#define PORT_PCR_PFE (1<<4)
#define PORT_PCR_SRE (1<<2)
#define PORT_PCR_PE  (1<<1)
#define PORT_PCR_PS  (1<<0)




#endif
