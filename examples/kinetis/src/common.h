#ifndef _COMMON_H
#define _COMMON_H

#include <stdint.h>
#include <stddef.h>

typedef int BOOL;

#define TRUE 1 
#define FALSE 0


// Macros to extract selected bitmask from a register
#define RANGE_MASK(e, s) ((1<<(e-s+1))-1)
#define EXTRACT_VALUE(v, e, s) ( (v >> s) & (RANGE_MASK(e, s)) )


extern uint32_t CORE_FREQ;
extern uint32_t BUS_FREQ;
extern uint32_t TPM_FREQ;
extern uint32_t XTAL_FREQ;

void delay_ms (uint16_t ms);

#endif