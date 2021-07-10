#include "system_control.h" 

#ifdef __cplusplus
extern "C"
{
#endif



void SCB_ReadCPUID (uint16_t* implementer_id, uint16_t* variant_id, uint16_t* architecture_id, uint16_t* partno_id, uint16_t* revision_id, BOOL* is_bigendian)
{
    *revision_id = EXTRACT_VALUE(SCB_CPUID, SCB_CPUID_REVISION_E, SCB_CPUID_REVISION_S);
    *partno_id =  EXTRACT_VALUE(SCB_CPUID, SCB_CPUID_PARTNO_E, SCB_CPUID_PARTNO_S);
    *architecture_id = EXTRACT_VALUE(SCB_CPUID, SCB_CPUID_ARCHITECTURE_E, SCB_CPUID_ARCHITECTURE_S);
    *variant_id = EXTRACT_VALUE(SCB_CPUID, SCB_CPUID_VARIANT_E, SCB_CPUID_VARIANT_S);
    *implementer_id = EXTRACT_VALUE(SCB_CPUID, SCB_CPUID_IMPLEMENTER_E, SCB_CPUID_IMPLEMENTER_S);
 
    *is_bigendian = (SCB_AIRCR & SCB_AIRCR_ENDIANNESS);

}

int8_t SCB_GetPendingIRQNo(void)
{
    return ((SCB_ICSR & SCB_ICSR_VECTPENDING_MASK) >> SCB_ICSR_VECTPENDING_SHIFT) -  16;
}

void SCB_SystemReset(void) 
{
    SCB_AIRCR |= 0x5FA<<16;
    SCB_AIRCR |= SCB_AIRCR_SYSRESETREQ;
}

void SCB_SetSleepOnExit(BOOL set)
{
    if (set)
        SCB_SCR |= SCB_SCR_SLEEPONEXIT;
    else
        SCB_SCR &= ~SCB_SCR_SLEEPONEXIT;

}

void SCB_SetSleepDeep(BOOL set)
{
    if (set)
        SCB_SCR |= SCB_SCR_SLEEPDEEP;
    else
        SCB_SCR &= ~SCB_SCR_SLEEPDEEP;
}

void SCB_SetSEVONPEND(BOOL set)
{
    if (set)
        SCB_SCR |= SCB_SCR_SEVONPEND;
    else
        SCB_SCR &= ~SCB_SCR_SEVONPEND;
}

void SCB_EnableDivByZeroTrap()
{
}
void SCB_EnableUnalignedTrap()
{
}

void SCB_EnableMemoryFault()
{
    SCB_SHCSR |= SCB_SHCSR_MEMFAULTENA;
}
void SCB_EnableUsageFault()
{
    SCB_SHCSR |= SCB_SHCSR_USGFAULTENA;
}
void SCB_EnableBusFault()
{
    SCB_SHCSR |= SCB_SHCSR_BUSFAULTENA;
}


void SCB_GetFaulStatus (volatile SCB_SHCSR_def* shcsr, volatile SCB_CFSR_def* cfsr)
{
    shcsr->busFaultActive = (SCB_SHCSR & SCB_SHCSR_BUSFAULTACT);
    shcsr->memFaultActive = (SCB_SHCSR & SCB_SHCSR_MEMFAULTACT);
    shcsr->usageFaultActive = (SCB_SHCSR & SCB_SHCSR_USGFAULTACT);
    shcsr->busFaultPending = (SCB_SHCSR & SCB_SHCSR_BUSFAULTPENDED);
    shcsr->memFaultPending = (SCB_SHCSR & SCB_SHCSR_MEMFAULTPENDED);
    shcsr->usageFaultPending = (SCB_SHCSR & SCB_SHCSR_USGFAULTPENDED);
    
    cfsr->IACCVIOL = (SCB_CFSR & SCB_CFSR_IACCVIOL);
    cfsr->DACCVIOL = (SCB_CFSR & SCB_CFSR_DACCVIOL);
    cfsr->MUNSTKERR = (SCB_CFSR & SCB_CFSR_MUNSTKERR);
    cfsr->MSTKERR = (SCB_CFSR & SCB_CFSR_MSTKERR);
    cfsr->MLSPERR = (SCB_CFSR & SCB_CFSR_MLSPERR);
    cfsr->MMARVALID = (SCB_CFSR & SCB_CFSR_MMARVALID);
    cfsr->IBUSERR = (SCB_CFSR & SCB_CFSR_IBUSERR);
    cfsr->PRECISERR = (SCB_CFSR & SCB_CFSR_PRECISERR);
    cfsr->IMPRECISERR = (SCB_CFSR & SCB_CFSR_IMPRECISERR);
    cfsr->UNSTKERR = (SCB_CFSR & SCB_CFSR_UNSTKERR);
    cfsr->STKERR = (SCB_CFSR & SCB_CFSR_STKERR);
    cfsr->LSPERR = (SCB_CFSR & SCB_CFSR_LSPERR);
    cfsr->BFARVALID = (SCB_CFSR & SCB_CFSR_BFARVALID);
    cfsr->UNDEFINSTR = (SCB_CFSR & SCB_CFSR_UNDEFINSTR);
    cfsr->INVSTATE = (SCB_CFSR & SCB_CFSR_INVSTATE);
    cfsr->INVPC = (SCB_CFSR & SCB_CFSR_INVPC);
    cfsr->NOCP = (SCB_CFSR & SCB_CFSR_NOCP);
    cfsr->UNALIGNED = (SCB_CFSR & SCB_CFSR_UNALIGNED);
    cfsr->DIVBYZERO = (SCB_CFSR & SCB_CFSR_DIVBYZERO);
    
    
}


void SCB_EnableDWT(BOOL enable)
{
   if (enable) 
   {
       SCB_DEMCR |= SCB_DEMCR_TRCENA; 
   }
   else
   {
       SCB_DEMCR &= ~SCB_DEMCR_TRCENA; 
   }
}



#ifdef __cplusplus
}
#endif

