#ifndef __CORE_CM4_H_GENERIC
#define __CORE_CM4_H_GENERIC 

// Included from parts specific includes: CMSIS/Device/ST/STM32F4xx/Include/stm32f401xc.h etc 


#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

#include "cmsis_version.h"

#define __CM4_CMSIS_VERSION_MAIN (__CM_CMSIS_VERSION_MAIN)
#define __CM4_CMSIS_VERSION_SUB (__CM_CMSIS_VERSION_SUB)
#define __CM4_CMSIS_VERSION ((__CM4_CMSIS_VERSION_MAIN << 16U) | \
                                    __CM4_CMSIS_VERSION_SUB )

#define __CORTEX_M (4U)

#if defined ( __CC_ARM )
  #if defined __TARGET_FPU_VFP
    #if defined (__FPU_PRESENT) && (__FPU_PRESENT == 1U)
      #define __FPU_USED 1U
    #else
      #error "Compiler generates FPU instructions for a device without an FPU (check __FPU_PRESENT)"
      #define __FPU_USED 0U
    #endif
  #else
    #define __FPU_USED 0U
  #endif

#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #if defined __ARM_PCS_VFP
    #if defined (__FPU_PRESENT) && (__FPU_PRESENT == 1U)
      #define __FPU_USED 1U
    #else
      #warning "Compiler generates FPU instructions for a device without an FPU (check __FPU_PRESENT)"
      #define __FPU_USED 0U
    #endif
  #else
    #define __FPU_USED 0U
  #endif

#elif defined ( __GNUC__ )
  #if defined (__VFP_FP__) && !defined(__SOFTFP__)
    #if defined (__FPU_PRESENT) && (__FPU_PRESENT == 1U)
      #define __FPU_USED 1U
    #else
      #error "Compiler generates FPU instructions for a device without an FPU (check __FPU_PRESENT)"
      #define __FPU_USED 0U
    #endif
  #else
    #define __FPU_USED 0U
  #endif
#endif

#include "cmsis_compiler.h"

#ifdef __cplusplus
}
#endif

#endif

#ifndef __CMSIS_GENERIC

#ifndef __CORE_CM4_H_DEPENDANT
#define __CORE_CM4_H_DEPENDANT 

#ifdef __cplusplus
 extern "C" {
#endif

#if defined __CHECK_DEVICE_DEFINES
  #ifndef __CM4_REV
    #define __CM4_REV 0x0000U
    #warning "__CM4_REV not defined in device header file; using default!"
  #endif

  #ifndef __FPU_PRESENT
    #define __FPU_PRESENT 0U
    #warning "__FPU_PRESENT not defined in device header file; using default!"
  #endif

  #ifndef __MPU_PRESENT
    #define __MPU_PRESENT 0U
    #warning "__MPU_PRESENT not defined in device header file; using default!"
  #endif

  #ifndef __NVIC_PRIO_BITS
    #define __NVIC_PRIO_BITS 3U
    #warning "__NVIC_PRIO_BITS not defined in device header file; using default!"
  #endif

  #ifndef __Vendor_SysTickConfig
    #define __Vendor_SysTickConfig 0U
    #warning "__Vendor_SysTickConfig not defined in device header file; using default!"
  #endif
#endif

#ifdef __cplusplus
  #define __I volatile
#else
  #define __I volatile const
#endif
#define __O volatile
#define __IO volatile


#define __IM volatile const
#define __OM volatile
#define __IOM volatile

typedef union
{
  struct
  {
    uint32_t _reserved0:16;
    uint32_t GE:4;
    uint32_t _reserved1:7;
    uint32_t Q:1;
    uint32_t V:1;
    uint32_t C:1;
    uint32_t Z:1;
    uint32_t N:1;
  } b;
  uint32_t w;
} APSR_Type;


#define APSR_N_Pos 31U
#define APSR_N_Msk (1UL << APSR_N_Pos)

#define APSR_Z_Pos 30U
#define APSR_Z_Msk (1UL << APSR_Z_Pos)

#define APSR_C_Pos 29U
#define APSR_C_Msk (1UL << APSR_C_Pos)

#define APSR_V_Pos 28U
#define APSR_V_Msk (1UL << APSR_V_Pos)

#define APSR_Q_Pos 27U
#define APSR_Q_Msk (1UL << APSR_Q_Pos)

#define APSR_GE_Pos 16U
#define APSR_GE_Msk (0xFUL << APSR_GE_Pos)

typedef union
{
  struct
  {
    uint32_t ISR:9;
    uint32_t _reserved0:23;
  } b;
  uint32_t w;
} IPSR_Type;


#define IPSR_ISR_Pos 0U
#define IPSR_ISR_Msk (0x1FFUL )

typedef union
{
  struct
  {
    uint32_t ISR:9;
    uint32_t _reserved0:1;
    uint32_t ICI_IT_1:6;
    uint32_t GE:4;
    uint32_t _reserved1:4;
    uint32_t T:1;
    uint32_t ICI_IT_2:2;
    uint32_t Q:1;
    uint32_t V:1;
    uint32_t C:1;
    uint32_t Z:1;
    uint32_t N:1;
  } b;
  uint32_t w;
} xPSR_Type;


#define xPSR_N_Pos 31U
#define xPSR_N_Msk (1UL << xPSR_N_Pos)

#define xPSR_Z_Pos 30U
#define xPSR_Z_Msk (1UL << xPSR_Z_Pos)

#define xPSR_C_Pos 29U
#define xPSR_C_Msk (1UL << xPSR_C_Pos)

#define xPSR_V_Pos 28U
#define xPSR_V_Msk (1UL << xPSR_V_Pos)

#define xPSR_Q_Pos 27U
#define xPSR_Q_Msk (1UL << xPSR_Q_Pos)

#define xPSR_ICI_IT_2_Pos 25U
#define xPSR_ICI_IT_2_Msk (3UL << xPSR_ICI_IT_2_Pos)

#define xPSR_T_Pos 24U
#define xPSR_T_Msk (1UL << xPSR_T_Pos)

#define xPSR_GE_Pos 16U
#define xPSR_GE_Msk (0xFUL << xPSR_GE_Pos)

#define xPSR_ICI_IT_1_Pos 10U
#define xPSR_ICI_IT_1_Msk (0x3FUL << xPSR_ICI_IT_1_Pos)

#define xPSR_ISR_Pos 0U
#define xPSR_ISR_Msk (0x1FFUL )

typedef union
{
  struct
  {
    uint32_t nPRIV:1;
    uint32_t SPSEL:1;
    uint32_t FPCA:1;
    uint32_t _reserved0:29;
  } b;
  uint32_t w;
} CONTROL_Type;


#define CONTROL_FPCA_Pos 2U
#define CONTROL_FPCA_Msk (1UL << CONTROL_FPCA_Pos)

#define CONTROL_SPSEL_Pos 1U
#define CONTROL_SPSEL_Msk (1UL << CONTROL_SPSEL_Pos)

#define CONTROL_nPRIV_Pos 0U
#define CONTROL_nPRIV_Msk (1UL )

typedef struct
{
  __IOM uint32_t ISER[8U];
        uint32_t RESERVED0[24U];
  __IOM uint32_t ICER[8U];
        uint32_t RSERVED1[24U];
  __IOM uint32_t ISPR[8U];
        uint32_t RESERVED2[24U];
  __IOM uint32_t ICPR[8U];
        uint32_t RESERVED3[24U];
  __IOM uint32_t IABR[8U];
        uint32_t RESERVED4[56U];
  __IOM uint8_t IP[240U];
        uint32_t RESERVED5[644U];
  __OM uint32_t STIR;
} NVIC_Type;


#define NVIC_STIR_INTID_Pos 0U
#define NVIC_STIR_INTID_Msk (0x1FFUL )

typedef struct
{
  __IM uint32_t CPUID;
  __IOM uint32_t ICSR;
  __IOM uint32_t VTOR;
  __IOM uint32_t AIRCR;
  __IOM uint32_t SCR;
  __IOM uint32_t CCR;
  __IOM uint8_t SHP[12U];
  __IOM uint32_t SHCSR;
  __IOM uint32_t CFSR;
  __IOM uint32_t HFSR;
  __IOM uint32_t DFSR;
  __IOM uint32_t MMFAR;
  __IOM uint32_t BFAR;
  __IOM uint32_t AFSR;
  __IM uint32_t PFR[2U];
  __IM uint32_t DFR;
  __IM uint32_t ADR;
  __IM uint32_t MMFR[4U];
  __IM uint32_t ISAR[5U];
        uint32_t RESERVED0[5U];
  __IOM uint32_t CPACR;
} SCB_Type;


#define SCB_CPUID_IMPLEMENTER_Pos 24U
#define SCB_CPUID_IMPLEMENTER_Msk (0xFFUL << SCB_CPUID_IMPLEMENTER_Pos)

#define SCB_CPUID_VARIANT_Pos 20U
#define SCB_CPUID_VARIANT_Msk (0xFUL << SCB_CPUID_VARIANT_Pos)

#define SCB_CPUID_ARCHITECTURE_Pos 16U
#define SCB_CPUID_ARCHITECTURE_Msk (0xFUL << SCB_CPUID_ARCHITECTURE_Pos)

#define SCB_CPUID_PARTNO_Pos 4U
#define SCB_CPUID_PARTNO_Msk (0xFFFUL << SCB_CPUID_PARTNO_Pos)

#define SCB_CPUID_REVISION_Pos 0U
#define SCB_CPUID_REVISION_Msk (0xFUL )


#define SCB_ICSR_NMIPENDSET_Pos 31U
#define SCB_ICSR_NMIPENDSET_Msk (1UL << SCB_ICSR_NMIPENDSET_Pos)

#define SCB_ICSR_PENDSVSET_Pos 28U
#define SCB_ICSR_PENDSVSET_Msk (1UL << SCB_ICSR_PENDSVSET_Pos)

#define SCB_ICSR_PENDSVCLR_Pos 27U
#define SCB_ICSR_PENDSVCLR_Msk (1UL << SCB_ICSR_PENDSVCLR_Pos)

#define SCB_ICSR_PENDSTSET_Pos 26U
#define SCB_ICSR_PENDSTSET_Msk (1UL << SCB_ICSR_PENDSTSET_Pos)

#define SCB_ICSR_PENDSTCLR_Pos 25U
#define SCB_ICSR_PENDSTCLR_Msk (1UL << SCB_ICSR_PENDSTCLR_Pos)

#define SCB_ICSR_ISRPREEMPT_Pos 23U
#define SCB_ICSR_ISRPREEMPT_Msk (1UL << SCB_ICSR_ISRPREEMPT_Pos)

#define SCB_ICSR_ISRPENDING_Pos 22U
#define SCB_ICSR_ISRPENDING_Msk (1UL << SCB_ICSR_ISRPENDING_Pos)

#define SCB_ICSR_VECTPENDING_Pos 12U
#define SCB_ICSR_VECTPENDING_Msk (0x1FFUL << SCB_ICSR_VECTPENDING_Pos)

#define SCB_ICSR_RETTOBASE_Pos 11U
#define SCB_ICSR_RETTOBASE_Msk (1UL << SCB_ICSR_RETTOBASE_Pos)

#define SCB_ICSR_VECTACTIVE_Pos 0U
#define SCB_ICSR_VECTACTIVE_Msk (0x1FFUL )


#define SCB_VTOR_TBLOFF_Pos 7U
#define SCB_VTOR_TBLOFF_Msk (0x1FFFFFFUL << SCB_VTOR_TBLOFF_Pos)


#define SCB_AIRCR_VECTKEY_Pos 16U
#define SCB_AIRCR_VECTKEY_Msk (0xFFFFUL << SCB_AIRCR_VECTKEY_Pos)

#define SCB_AIRCR_VECTKEYSTAT_Pos 16U
#define SCB_AIRCR_VECTKEYSTAT_Msk (0xFFFFUL << SCB_AIRCR_VECTKEYSTAT_Pos)

#define SCB_AIRCR_ENDIANESS_Pos 15U
#define SCB_AIRCR_ENDIANESS_Msk (1UL << SCB_AIRCR_ENDIANESS_Pos)

#define SCB_AIRCR_PRIGROUP_Pos 8U
#define SCB_AIRCR_PRIGROUP_Msk (7UL << SCB_AIRCR_PRIGROUP_Pos)

#define SCB_AIRCR_SYSRESETREQ_Pos 2U
#define SCB_AIRCR_SYSRESETREQ_Msk (1UL << SCB_AIRCR_SYSRESETREQ_Pos)

#define SCB_AIRCR_VECTCLRACTIVE_Pos 1U
#define SCB_AIRCR_VECTCLRACTIVE_Msk (1UL << SCB_AIRCR_VECTCLRACTIVE_Pos)

#define SCB_AIRCR_VECTRESET_Pos 0U
#define SCB_AIRCR_VECTRESET_Msk (1UL )


#define SCB_SCR_SEVONPEND_Pos 4U
#define SCB_SCR_SEVONPEND_Msk (1UL << SCB_SCR_SEVONPEND_Pos)

#define SCB_SCR_SLEEPDEEP_Pos 2U
#define SCB_SCR_SLEEPDEEP_Msk (1UL << SCB_SCR_SLEEPDEEP_Pos)

#define SCB_SCR_SLEEPONEXIT_Pos 1U
#define SCB_SCR_SLEEPONEXIT_Msk (1UL << SCB_SCR_SLEEPONEXIT_Pos)


#define SCB_CCR_STKALIGN_Pos 9U
#define SCB_CCR_STKALIGN_Msk (1UL << SCB_CCR_STKALIGN_Pos)

#define SCB_CCR_BFHFNMIGN_Pos 8U
#define SCB_CCR_BFHFNMIGN_Msk (1UL << SCB_CCR_BFHFNMIGN_Pos)

#define SCB_CCR_DIV_0_TRP_Pos 4U
#define SCB_CCR_DIV_0_TRP_Msk (1UL << SCB_CCR_DIV_0_TRP_Pos)

#define SCB_CCR_UNALIGN_TRP_Pos 3U
#define SCB_CCR_UNALIGN_TRP_Msk (1UL << SCB_CCR_UNALIGN_TRP_Pos)

#define SCB_CCR_USERSETMPEND_Pos 1U
#define SCB_CCR_USERSETMPEND_Msk (1UL << SCB_CCR_USERSETMPEND_Pos)

#define SCB_CCR_NONBASETHRDENA_Pos 0U
#define SCB_CCR_NONBASETHRDENA_Msk (1UL )


#define SCB_SHCSR_USGFAULTENA_Pos 18U
#define SCB_SHCSR_USGFAULTENA_Msk (1UL << SCB_SHCSR_USGFAULTENA_Pos)

#define SCB_SHCSR_BUSFAULTENA_Pos 17U
#define SCB_SHCSR_BUSFAULTENA_Msk (1UL << SCB_SHCSR_BUSFAULTENA_Pos)

#define SCB_SHCSR_MEMFAULTENA_Pos 16U
#define SCB_SHCSR_MEMFAULTENA_Msk (1UL << SCB_SHCSR_MEMFAULTENA_Pos)

#define SCB_SHCSR_SVCALLPENDED_Pos 15U
#define SCB_SHCSR_SVCALLPENDED_Msk (1UL << SCB_SHCSR_SVCALLPENDED_Pos)

#define SCB_SHCSR_BUSFAULTPENDED_Pos 14U
#define SCB_SHCSR_BUSFAULTPENDED_Msk (1UL << SCB_SHCSR_BUSFAULTPENDED_Pos)

#define SCB_SHCSR_MEMFAULTPENDED_Pos 13U
#define SCB_SHCSR_MEMFAULTPENDED_Msk (1UL << SCB_SHCSR_MEMFAULTPENDED_Pos)

#define SCB_SHCSR_USGFAULTPENDED_Pos 12U
#define SCB_SHCSR_USGFAULTPENDED_Msk (1UL << SCB_SHCSR_USGFAULTPENDED_Pos)

#define SCB_SHCSR_SYSTICKACT_Pos 11U
#define SCB_SHCSR_SYSTICKACT_Msk (1UL << SCB_SHCSR_SYSTICKACT_Pos)

#define SCB_SHCSR_PENDSVACT_Pos 10U
#define SCB_SHCSR_PENDSVACT_Msk (1UL << SCB_SHCSR_PENDSVACT_Pos)

#define SCB_SHCSR_MONITORACT_Pos 8U
#define SCB_SHCSR_MONITORACT_Msk (1UL << SCB_SHCSR_MONITORACT_Pos)

#define SCB_SHCSR_SVCALLACT_Pos 7U
#define SCB_SHCSR_SVCALLACT_Msk (1UL << SCB_SHCSR_SVCALLACT_Pos)

#define SCB_SHCSR_USGFAULTACT_Pos 3U
#define SCB_SHCSR_USGFAULTACT_Msk (1UL << SCB_SHCSR_USGFAULTACT_Pos)

#define SCB_SHCSR_BUSFAULTACT_Pos 1U
#define SCB_SHCSR_BUSFAULTACT_Msk (1UL << SCB_SHCSR_BUSFAULTACT_Pos)

#define SCB_SHCSR_MEMFAULTACT_Pos 0U
#define SCB_SHCSR_MEMFAULTACT_Msk (1UL )


#define SCB_CFSR_USGFAULTSR_Pos 16U
#define SCB_CFSR_USGFAULTSR_Msk (0xFFFFUL << SCB_CFSR_USGFAULTSR_Pos)

#define SCB_CFSR_BUSFAULTSR_Pos 8U
#define SCB_CFSR_BUSFAULTSR_Msk (0xFFUL << SCB_CFSR_BUSFAULTSR_Pos)

#define SCB_CFSR_MEMFAULTSR_Pos 0U
#define SCB_CFSR_MEMFAULTSR_Msk (0xFFUL )


#define SCB_CFSR_MMARVALID_Pos (SCB_SHCSR_MEMFAULTACT_Pos + 7U)
#define SCB_CFSR_MMARVALID_Msk (1UL << SCB_CFSR_MMARVALID_Pos)

#define SCB_CFSR_MLSPERR_Pos (SCB_SHCSR_MEMFAULTACT_Pos + 5U)
#define SCB_CFSR_MLSPERR_Msk (1UL << SCB_CFSR_MLSPERR_Pos)

#define SCB_CFSR_MSTKERR_Pos (SCB_SHCSR_MEMFAULTACT_Pos + 4U)
#define SCB_CFSR_MSTKERR_Msk (1UL << SCB_CFSR_MSTKERR_Pos)

#define SCB_CFSR_MUNSTKERR_Pos (SCB_SHCSR_MEMFAULTACT_Pos + 3U)
#define SCB_CFSR_MUNSTKERR_Msk (1UL << SCB_CFSR_MUNSTKERR_Pos)

#define SCB_CFSR_DACCVIOL_Pos (SCB_SHCSR_MEMFAULTACT_Pos + 1U)
#define SCB_CFSR_DACCVIOL_Msk (1UL << SCB_CFSR_DACCVIOL_Pos)

#define SCB_CFSR_IACCVIOL_Pos (SCB_SHCSR_MEMFAULTACT_Pos + 0U)
#define SCB_CFSR_IACCVIOL_Msk (1UL )


#define SCB_CFSR_BFARVALID_Pos (SCB_CFSR_BUSFAULTSR_Pos + 7U)
#define SCB_CFSR_BFARVALID_Msk (1UL << SCB_CFSR_BFARVALID_Pos)

#define SCB_CFSR_LSPERR_Pos (SCB_CFSR_BUSFAULTSR_Pos + 5U)
#define SCB_CFSR_LSPERR_Msk (1UL << SCB_CFSR_LSPERR_Pos)

#define SCB_CFSR_STKERR_Pos (SCB_CFSR_BUSFAULTSR_Pos + 4U)
#define SCB_CFSR_STKERR_Msk (1UL << SCB_CFSR_STKERR_Pos)

#define SCB_CFSR_UNSTKERR_Pos (SCB_CFSR_BUSFAULTSR_Pos + 3U)
#define SCB_CFSR_UNSTKERR_Msk (1UL << SCB_CFSR_UNSTKERR_Pos)

#define SCB_CFSR_IMPRECISERR_Pos (SCB_CFSR_BUSFAULTSR_Pos + 2U)
#define SCB_CFSR_IMPRECISERR_Msk (1UL << SCB_CFSR_IMPRECISERR_Pos)

#define SCB_CFSR_PRECISERR_Pos (SCB_CFSR_BUSFAULTSR_Pos + 1U)
#define SCB_CFSR_PRECISERR_Msk (1UL << SCB_CFSR_PRECISERR_Pos)

#define SCB_CFSR_IBUSERR_Pos (SCB_CFSR_BUSFAULTSR_Pos + 0U)
#define SCB_CFSR_IBUSERR_Msk (1UL << SCB_CFSR_IBUSERR_Pos)


#define SCB_CFSR_DIVBYZERO_Pos (SCB_CFSR_USGFAULTSR_Pos + 9U)
#define SCB_CFSR_DIVBYZERO_Msk (1UL << SCB_CFSR_DIVBYZERO_Pos)

#define SCB_CFSR_UNALIGNED_Pos (SCB_CFSR_USGFAULTSR_Pos + 8U)
#define SCB_CFSR_UNALIGNED_Msk (1UL << SCB_CFSR_UNALIGNED_Pos)

#define SCB_CFSR_NOCP_Pos (SCB_CFSR_USGFAULTSR_Pos + 3U)
#define SCB_CFSR_NOCP_Msk (1UL << SCB_CFSR_NOCP_Pos)

#define SCB_CFSR_INVPC_Pos (SCB_CFSR_USGFAULTSR_Pos + 2U)
#define SCB_CFSR_INVPC_Msk (1UL << SCB_CFSR_INVPC_Pos)

#define SCB_CFSR_INVSTATE_Pos (SCB_CFSR_USGFAULTSR_Pos + 1U)
#define SCB_CFSR_INVSTATE_Msk (1UL << SCB_CFSR_INVSTATE_Pos)

#define SCB_CFSR_UNDEFINSTR_Pos (SCB_CFSR_USGFAULTSR_Pos + 0U)
#define SCB_CFSR_UNDEFINSTR_Msk (1UL << SCB_CFSR_UNDEFINSTR_Pos)


#define SCB_HFSR_DEBUGEVT_Pos 31U
#define SCB_HFSR_DEBUGEVT_Msk (1UL << SCB_HFSR_DEBUGEVT_Pos)

#define SCB_HFSR_FORCED_Pos 30U
#define SCB_HFSR_FORCED_Msk (1UL << SCB_HFSR_FORCED_Pos)

#define SCB_HFSR_VECTTBL_Pos 1U
#define SCB_HFSR_VECTTBL_Msk (1UL << SCB_HFSR_VECTTBL_Pos)


#define SCB_DFSR_EXTERNAL_Pos 4U
#define SCB_DFSR_EXTERNAL_Msk (1UL << SCB_DFSR_EXTERNAL_Pos)

#define SCB_DFSR_VCATCH_Pos 3U
#define SCB_DFSR_VCATCH_Msk (1UL << SCB_DFSR_VCATCH_Pos)

#define SCB_DFSR_DWTTRAP_Pos 2U
#define SCB_DFSR_DWTTRAP_Msk (1UL << SCB_DFSR_DWTTRAP_Pos)

#define SCB_DFSR_BKPT_Pos 1U
#define SCB_DFSR_BKPT_Msk (1UL << SCB_DFSR_BKPT_Pos)

#define SCB_DFSR_HALTED_Pos 0U
#define SCB_DFSR_HALTED_Msk (1UL )

typedef struct
{
        uint32_t RESERVED0[1U];
  __IM uint32_t ICTR;
  __IOM uint32_t ACTLR;
} SCnSCB_Type;


#define SCnSCB_ICTR_INTLINESNUM_Pos 0U
#define SCnSCB_ICTR_INTLINESNUM_Msk (0xFUL )


#define SCnSCB_ACTLR_DISOOFP_Pos 9U
#define SCnSCB_ACTLR_DISOOFP_Msk (1UL << SCnSCB_ACTLR_DISOOFP_Pos)

#define SCnSCB_ACTLR_DISFPCA_Pos 8U
#define SCnSCB_ACTLR_DISFPCA_Msk (1UL << SCnSCB_ACTLR_DISFPCA_Pos)

#define SCnSCB_ACTLR_DISFOLD_Pos 2U
#define SCnSCB_ACTLR_DISFOLD_Msk (1UL << SCnSCB_ACTLR_DISFOLD_Pos)

#define SCnSCB_ACTLR_DISDEFWBUF_Pos 1U
#define SCnSCB_ACTLR_DISDEFWBUF_Msk (1UL << SCnSCB_ACTLR_DISDEFWBUF_Pos)

#define SCnSCB_ACTLR_DISMCYCINT_Pos 0U
#define SCnSCB_ACTLR_DISMCYCINT_Msk (1UL )

typedef struct
{
  __IOM uint32_t CTRL;
  __IOM uint32_t LOAD;
  __IOM uint32_t VAL;
  __IM uint32_t CALIB;
} SysTick_Type;


#define SysTick_CTRL_COUNTFLAG_Pos 16U
#define SysTick_CTRL_COUNTFLAG_Msk (1UL << SysTick_CTRL_COUNTFLAG_Pos)

#define SysTick_CTRL_CLKSOURCE_Pos 2U
#define SysTick_CTRL_CLKSOURCE_Msk (1UL << SysTick_CTRL_CLKSOURCE_Pos)

#define SysTick_CTRL_TICKINT_Pos 1U
#define SysTick_CTRL_TICKINT_Msk (1UL << SysTick_CTRL_TICKINT_Pos)

#define SysTick_CTRL_ENABLE_Pos 0U
#define SysTick_CTRL_ENABLE_Msk (1UL )


#define SysTick_LOAD_RELOAD_Pos 0U
#define SysTick_LOAD_RELOAD_Msk (0xFFFFFFUL )


#define SysTick_VAL_CURRENT_Pos 0U
#define SysTick_VAL_CURRENT_Msk (0xFFFFFFUL )


#define SysTick_CALIB_NOREF_Pos 31U
#define SysTick_CALIB_NOREF_Msk (1UL << SysTick_CALIB_NOREF_Pos)

#define SysTick_CALIB_SKEW_Pos 30U
#define SysTick_CALIB_SKEW_Msk (1UL << SysTick_CALIB_SKEW_Pos)

#define SysTick_CALIB_TENMS_Pos 0U
#define SysTick_CALIB_TENMS_Msk (0xFFFFFFUL )

typedef struct
{
  __OM union
  {
    __OM uint8_t u8;
    __OM uint16_t u16;
    __OM uint32_t u32;
  } PORT [32U];
        uint32_t RESERVED0[864U];
  __IOM uint32_t TER;
        uint32_t RESERVED1[15U];
  __IOM uint32_t TPR;
        uint32_t RESERVED2[15U];
  __IOM uint32_t TCR;
        uint32_t RESERVED3[29U];
  __OM uint32_t IWR;
  __IM uint32_t IRR;
  __IOM uint32_t IMCR;
        uint32_t RESERVED4[43U];
  __OM uint32_t LAR;
  __IM uint32_t LSR;
        uint32_t RESERVED5[6U];
  __IM uint32_t PID4;
  __IM uint32_t PID5;
  __IM uint32_t PID6;
  __IM uint32_t PID7;
  __IM uint32_t PID0;
  __IM uint32_t PID1;
  __IM uint32_t PID2;
  __IM uint32_t PID3;
  __IM uint32_t CID0;
  __IM uint32_t CID1;
  __IM uint32_t CID2;
  __IM uint32_t CID3;
} ITM_Type;


#define ITM_TPR_PRIVMASK_Pos 0U
#define ITM_TPR_PRIVMASK_Msk (0xFFFFFFFFUL )


#define ITM_TCR_BUSY_Pos 23U
#define ITM_TCR_BUSY_Msk (1UL << ITM_TCR_BUSY_Pos)

#define ITM_TCR_TraceBusID_Pos 16U
#define ITM_TCR_TraceBusID_Msk (0x7FUL << ITM_TCR_TraceBusID_Pos)

#define ITM_TCR_GTSFREQ_Pos 10U
#define ITM_TCR_GTSFREQ_Msk (3UL << ITM_TCR_GTSFREQ_Pos)

#define ITM_TCR_TSPrescale_Pos 8U
#define ITM_TCR_TSPrescale_Msk (3UL << ITM_TCR_TSPrescale_Pos)

#define ITM_TCR_SWOENA_Pos 4U
#define ITM_TCR_SWOENA_Msk (1UL << ITM_TCR_SWOENA_Pos)

#define ITM_TCR_DWTENA_Pos 3U
#define ITM_TCR_DWTENA_Msk (1UL << ITM_TCR_DWTENA_Pos)

#define ITM_TCR_SYNCENA_Pos 2U
#define ITM_TCR_SYNCENA_Msk (1UL << ITM_TCR_SYNCENA_Pos)

#define ITM_TCR_TSENA_Pos 1U
#define ITM_TCR_TSENA_Msk (1UL << ITM_TCR_TSENA_Pos)

#define ITM_TCR_ITMENA_Pos 0U
#define ITM_TCR_ITMENA_Msk (1UL )


#define ITM_IWR_ATVALIDM_Pos 0U
#define ITM_IWR_ATVALIDM_Msk (1UL )


#define ITM_IRR_ATREADYM_Pos 0U
#define ITM_IRR_ATREADYM_Msk (1UL )


#define ITM_IMCR_INTEGRATION_Pos 0U
#define ITM_IMCR_INTEGRATION_Msk (1UL )


#define ITM_LSR_ByteAcc_Pos 2U
#define ITM_LSR_ByteAcc_Msk (1UL << ITM_LSR_ByteAcc_Pos)

#define ITM_LSR_Access_Pos 1U
#define ITM_LSR_Access_Msk (1UL << ITM_LSR_Access_Pos)

#define ITM_LSR_Present_Pos 0U
#define ITM_LSR_Present_Msk (1UL )
typedef struct
{
  __IOM uint32_t CTRL;
  __IOM uint32_t CYCCNT;
  __IOM uint32_t CPICNT;
  __IOM uint32_t EXCCNT;
  __IOM uint32_t SLEEPCNT;
  __IOM uint32_t LSUCNT;
  __IOM uint32_t FOLDCNT;
  __IM uint32_t PCSR;
  __IOM uint32_t COMP0;
  __IOM uint32_t MASK0;
  __IOM uint32_t FUNCTION0;
        uint32_t RESERVED0[1U];
  __IOM uint32_t COMP1;
  __IOM uint32_t MASK1;
  __IOM uint32_t FUNCTION1;
        uint32_t RESERVED1[1U];
  __IOM uint32_t COMP2;
  __IOM uint32_t MASK2;
  __IOM uint32_t FUNCTION2;
        uint32_t RESERVED2[1U];
  __IOM uint32_t COMP3;
  __IOM uint32_t MASK3;
  __IOM uint32_t FUNCTION3;
} DWT_Type;


#define DWT_CTRL_NUMCOMP_Pos 28U
#define DWT_CTRL_NUMCOMP_Msk (0xFUL << DWT_CTRL_NUMCOMP_Pos)

#define DWT_CTRL_NOTRCPKT_Pos 27U
#define DWT_CTRL_NOTRCPKT_Msk (0x1UL << DWT_CTRL_NOTRCPKT_Pos)

#define DWT_CTRL_NOEXTTRIG_Pos 26U
#define DWT_CTRL_NOEXTTRIG_Msk (0x1UL << DWT_CTRL_NOEXTTRIG_Pos)

#define DWT_CTRL_NOCYCCNT_Pos 25U
#define DWT_CTRL_NOCYCCNT_Msk (0x1UL << DWT_CTRL_NOCYCCNT_Pos)

#define DWT_CTRL_NOPRFCNT_Pos 24U
#define DWT_CTRL_NOPRFCNT_Msk (0x1UL << DWT_CTRL_NOPRFCNT_Pos)

#define DWT_CTRL_CYCEVTENA_Pos 22U
#define DWT_CTRL_CYCEVTENA_Msk (0x1UL << DWT_CTRL_CYCEVTENA_Pos)

#define DWT_CTRL_FOLDEVTENA_Pos 21U
#define DWT_CTRL_FOLDEVTENA_Msk (0x1UL << DWT_CTRL_FOLDEVTENA_Pos)

#define DWT_CTRL_LSUEVTENA_Pos 20U
#define DWT_CTRL_LSUEVTENA_Msk (0x1UL << DWT_CTRL_LSUEVTENA_Pos)

#define DWT_CTRL_SLEEPEVTENA_Pos 19U
#define DWT_CTRL_SLEEPEVTENA_Msk (0x1UL << DWT_CTRL_SLEEPEVTENA_Pos)

#define DWT_CTRL_EXCEVTENA_Pos 18U
#define DWT_CTRL_EXCEVTENA_Msk (0x1UL << DWT_CTRL_EXCEVTENA_Pos)

#define DWT_CTRL_CPIEVTENA_Pos 17U
#define DWT_CTRL_CPIEVTENA_Msk (0x1UL << DWT_CTRL_CPIEVTENA_Pos)

#define DWT_CTRL_EXCTRCENA_Pos 16U
#define DWT_CTRL_EXCTRCENA_Msk (0x1UL << DWT_CTRL_EXCTRCENA_Pos)

#define DWT_CTRL_PCSAMPLENA_Pos 12U
#define DWT_CTRL_PCSAMPLENA_Msk (0x1UL << DWT_CTRL_PCSAMPLENA_Pos)

#define DWT_CTRL_SYNCTAP_Pos 10U
#define DWT_CTRL_SYNCTAP_Msk (0x3UL << DWT_CTRL_SYNCTAP_Pos)

#define DWT_CTRL_CYCTAP_Pos 9U
#define DWT_CTRL_CYCTAP_Msk (0x1UL << DWT_CTRL_CYCTAP_Pos)

#define DWT_CTRL_POSTINIT_Pos 5U
#define DWT_CTRL_POSTINIT_Msk (0xFUL << DWT_CTRL_POSTINIT_Pos)

#define DWT_CTRL_POSTPRESET_Pos 1U
#define DWT_CTRL_POSTPRESET_Msk (0xFUL << DWT_CTRL_POSTPRESET_Pos)

#define DWT_CTRL_CYCCNTENA_Pos 0U
#define DWT_CTRL_CYCCNTENA_Msk (0x1UL )


#define DWT_CPICNT_CPICNT_Pos 0U
#define DWT_CPICNT_CPICNT_Msk (0xFFUL )


#define DWT_EXCCNT_EXCCNT_Pos 0U
#define DWT_EXCCNT_EXCCNT_Msk (0xFFUL )


#define DWT_SLEEPCNT_SLEEPCNT_Pos 0U
#define DWT_SLEEPCNT_SLEEPCNT_Msk (0xFFUL )


#define DWT_LSUCNT_LSUCNT_Pos 0U
#define DWT_LSUCNT_LSUCNT_Msk (0xFFUL )


#define DWT_FOLDCNT_FOLDCNT_Pos 0U
#define DWT_FOLDCNT_FOLDCNT_Msk (0xFFUL )


#define DWT_MASK_MASK_Pos 0U
#define DWT_MASK_MASK_Msk (0x1FUL )


#define DWT_FUNCTION_MATCHED_Pos 24U
#define DWT_FUNCTION_MATCHED_Msk (0x1UL << DWT_FUNCTION_MATCHED_Pos)

#define DWT_FUNCTION_DATAVADDR1_Pos 16U
#define DWT_FUNCTION_DATAVADDR1_Msk (0xFUL << DWT_FUNCTION_DATAVADDR1_Pos)

#define DWT_FUNCTION_DATAVADDR0_Pos 12U
#define DWT_FUNCTION_DATAVADDR0_Msk (0xFUL << DWT_FUNCTION_DATAVADDR0_Pos)

#define DWT_FUNCTION_DATAVSIZE_Pos 10U
#define DWT_FUNCTION_DATAVSIZE_Msk (0x3UL << DWT_FUNCTION_DATAVSIZE_Pos)

#define DWT_FUNCTION_LNK1ENA_Pos 9U
#define DWT_FUNCTION_LNK1ENA_Msk (0x1UL << DWT_FUNCTION_LNK1ENA_Pos)

#define DWT_FUNCTION_DATAVMATCH_Pos 8U
#define DWT_FUNCTION_DATAVMATCH_Msk (0x1UL << DWT_FUNCTION_DATAVMATCH_Pos)

#define DWT_FUNCTION_CYCMATCH_Pos 7U
#define DWT_FUNCTION_CYCMATCH_Msk (0x1UL << DWT_FUNCTION_CYCMATCH_Pos)

#define DWT_FUNCTION_EMITRANGE_Pos 5U
#define DWT_FUNCTION_EMITRANGE_Msk (0x1UL << DWT_FUNCTION_EMITRANGE_Pos)

#define DWT_FUNCTION_FUNCTION_Pos 0U
#define DWT_FUNCTION_FUNCTION_Msk (0xFUL )
typedef struct
{
  __IM uint32_t SSPSR;
  __IOM uint32_t CSPSR;
        uint32_t RESERVED0[2U];
  __IOM uint32_t ACPR;
        uint32_t RESERVED1[55U];
  __IOM uint32_t SPPR;
        uint32_t RESERVED2[131U];
  __IM uint32_t FFSR;
  __IOM uint32_t FFCR;
  __IM uint32_t FSCR;
        uint32_t RESERVED3[759U];
  __IM uint32_t TRIGGER;
  __IM uint32_t FIFO0;
  __IM uint32_t ITATBCTR2;
        uint32_t RESERVED4[1U];
  __IM uint32_t ITATBCTR0;
  __IM uint32_t FIFO1;
  __IOM uint32_t ITCTRL;
        uint32_t RESERVED5[39U];
  __IOM uint32_t CLAIMSET;
  __IOM uint32_t CLAIMCLR;
        uint32_t RESERVED7[8U];
  __IM uint32_t DEVID;
  __IM uint32_t DEVTYPE;
} TPI_Type;


#define TPI_ACPR_PRESCALER_Pos 0U
#define TPI_ACPR_PRESCALER_Msk (0x1FFFUL )


#define TPI_SPPR_TXMODE_Pos 0U
#define TPI_SPPR_TXMODE_Msk (0x3UL )


#define TPI_FFSR_FtNonStop_Pos 3U
#define TPI_FFSR_FtNonStop_Msk (0x1UL << TPI_FFSR_FtNonStop_Pos)

#define TPI_FFSR_TCPresent_Pos 2U
#define TPI_FFSR_TCPresent_Msk (0x1UL << TPI_FFSR_TCPresent_Pos)

#define TPI_FFSR_FtStopped_Pos 1U
#define TPI_FFSR_FtStopped_Msk (0x1UL << TPI_FFSR_FtStopped_Pos)

#define TPI_FFSR_FlInProg_Pos 0U
#define TPI_FFSR_FlInProg_Msk (0x1UL )


#define TPI_FFCR_TrigIn_Pos 8U
#define TPI_FFCR_TrigIn_Msk (0x1UL << TPI_FFCR_TrigIn_Pos)

#define TPI_FFCR_EnFCont_Pos 1U
#define TPI_FFCR_EnFCont_Msk (0x1UL << TPI_FFCR_EnFCont_Pos)


#define TPI_TRIGGER_TRIGGER_Pos 0U
#define TPI_TRIGGER_TRIGGER_Msk (0x1UL )


#define TPI_FIFO0_ITM_ATVALID_Pos 29U
#define TPI_FIFO0_ITM_ATVALID_Msk (0x3UL << TPI_FIFO0_ITM_ATVALID_Pos)

#define TPI_FIFO0_ITM_bytecount_Pos 27U
#define TPI_FIFO0_ITM_bytecount_Msk (0x3UL << TPI_FIFO0_ITM_bytecount_Pos)

#define TPI_FIFO0_ETM_ATVALID_Pos 26U
#define TPI_FIFO0_ETM_ATVALID_Msk (0x3UL << TPI_FIFO0_ETM_ATVALID_Pos)

#define TPI_FIFO0_ETM_bytecount_Pos 24U
#define TPI_FIFO0_ETM_bytecount_Msk (0x3UL << TPI_FIFO0_ETM_bytecount_Pos)

#define TPI_FIFO0_ETM2_Pos 16U
#define TPI_FIFO0_ETM2_Msk (0xFFUL << TPI_FIFO0_ETM2_Pos)

#define TPI_FIFO0_ETM1_Pos 8U
#define TPI_FIFO0_ETM1_Msk (0xFFUL << TPI_FIFO0_ETM1_Pos)

#define TPI_FIFO0_ETM0_Pos 0U
#define TPI_FIFO0_ETM0_Msk (0xFFUL )


#define TPI_ITATBCTR2_ATREADY2_Pos 0U
#define TPI_ITATBCTR2_ATREADY2_Msk (0x1UL )

#define TPI_ITATBCTR2_ATREADY1_Pos 0U
#define TPI_ITATBCTR2_ATREADY1_Msk (0x1UL )


#define TPI_FIFO1_ITM_ATVALID_Pos 29U
#define TPI_FIFO1_ITM_ATVALID_Msk (0x3UL << TPI_FIFO1_ITM_ATVALID_Pos)

#define TPI_FIFO1_ITM_bytecount_Pos 27U
#define TPI_FIFO1_ITM_bytecount_Msk (0x3UL << TPI_FIFO1_ITM_bytecount_Pos)

#define TPI_FIFO1_ETM_ATVALID_Pos 26U
#define TPI_FIFO1_ETM_ATVALID_Msk (0x3UL << TPI_FIFO1_ETM_ATVALID_Pos)

#define TPI_FIFO1_ETM_bytecount_Pos 24U
#define TPI_FIFO1_ETM_bytecount_Msk (0x3UL << TPI_FIFO1_ETM_bytecount_Pos)

#define TPI_FIFO1_ITM2_Pos 16U
#define TPI_FIFO1_ITM2_Msk (0xFFUL << TPI_FIFO1_ITM2_Pos)

#define TPI_FIFO1_ITM1_Pos 8U
#define TPI_FIFO1_ITM1_Msk (0xFFUL << TPI_FIFO1_ITM1_Pos)

#define TPI_FIFO1_ITM0_Pos 0U
#define TPI_FIFO1_ITM0_Msk (0xFFUL )


#define TPI_ITATBCTR0_ATREADY2_Pos 0U
#define TPI_ITATBCTR0_ATREADY2_Msk (0x1UL )

#define TPI_ITATBCTR0_ATREADY1_Pos 0U
#define TPI_ITATBCTR0_ATREADY1_Msk (0x1UL )


#define TPI_ITCTRL_Mode_Pos 0U
#define TPI_ITCTRL_Mode_Msk (0x3UL )


#define TPI_DEVID_NRZVALID_Pos 11U
#define TPI_DEVID_NRZVALID_Msk (0x1UL << TPI_DEVID_NRZVALID_Pos)

#define TPI_DEVID_MANCVALID_Pos 10U
#define TPI_DEVID_MANCVALID_Msk (0x1UL << TPI_DEVID_MANCVALID_Pos)

#define TPI_DEVID_PTINVALID_Pos 9U
#define TPI_DEVID_PTINVALID_Msk (0x1UL << TPI_DEVID_PTINVALID_Pos)

#define TPI_DEVID_MinBufSz_Pos 6U
#define TPI_DEVID_MinBufSz_Msk (0x7UL << TPI_DEVID_MinBufSz_Pos)

#define TPI_DEVID_AsynClkIn_Pos 5U
#define TPI_DEVID_AsynClkIn_Msk (0x1UL << TPI_DEVID_AsynClkIn_Pos)

#define TPI_DEVID_NrTraceInput_Pos 0U
#define TPI_DEVID_NrTraceInput_Msk (0x1FUL )


#define TPI_DEVTYPE_SubType_Pos 4U
#define TPI_DEVTYPE_SubType_Msk (0xFUL )

#define TPI_DEVTYPE_MajorType_Pos 0U
#define TPI_DEVTYPE_MajorType_Msk (0xFUL << TPI_DEVTYPE_MajorType_Pos)




#if defined (__MPU_PRESENT) && (__MPU_PRESENT == 1U)
typedef struct
{
  __IM uint32_t TYPE;
  __IOM uint32_t CTRL;
  __IOM uint32_t RNR;
  __IOM uint32_t RBAR;
  __IOM uint32_t RASR;
  __IOM uint32_t RBAR_A1;
  __IOM uint32_t RASR_A1;
  __IOM uint32_t RBAR_A2;
  __IOM uint32_t RASR_A2;
  __IOM uint32_t RBAR_A3;
  __IOM uint32_t RASR_A3;
} MPU_Type;

#define MPU_TYPE_RALIASES 4U


#define MPU_TYPE_IREGION_Pos 16U
#define MPU_TYPE_IREGION_Msk (0xFFUL << MPU_TYPE_IREGION_Pos)

#define MPU_TYPE_DREGION_Pos 8U
#define MPU_TYPE_DREGION_Msk (0xFFUL << MPU_TYPE_DREGION_Pos)

#define MPU_TYPE_SEPARATE_Pos 0U
#define MPU_TYPE_SEPARATE_Msk (1UL )


#define MPU_CTRL_PRIVDEFENA_Pos 2U
#define MPU_CTRL_PRIVDEFENA_Msk (1UL << MPU_CTRL_PRIVDEFENA_Pos)

#define MPU_CTRL_HFNMIENA_Pos 1U
#define MPU_CTRL_HFNMIENA_Msk (1UL << MPU_CTRL_HFNMIENA_Pos)

#define MPU_CTRL_ENABLE_Pos 0U
#define MPU_CTRL_ENABLE_Msk (1UL )


#define MPU_RNR_REGION_Pos 0U
#define MPU_RNR_REGION_Msk (0xFFUL )


#define MPU_RBAR_ADDR_Pos 5U
#define MPU_RBAR_ADDR_Msk (0x7FFFFFFUL << MPU_RBAR_ADDR_Pos)

#define MPU_RBAR_VALID_Pos 4U
#define MPU_RBAR_VALID_Msk (1UL << MPU_RBAR_VALID_Pos)

#define MPU_RBAR_REGION_Pos 0U
#define MPU_RBAR_REGION_Msk (0xFUL )


#define MPU_RASR_ATTRS_Pos 16U
#define MPU_RASR_ATTRS_Msk (0xFFFFUL << MPU_RASR_ATTRS_Pos)

#define MPU_RASR_XN_Pos 28U
#define MPU_RASR_XN_Msk (1UL << MPU_RASR_XN_Pos)

#define MPU_RASR_AP_Pos 24U
#define MPU_RASR_AP_Msk (0x7UL << MPU_RASR_AP_Pos)

#define MPU_RASR_TEX_Pos 19U
#define MPU_RASR_TEX_Msk (0x7UL << MPU_RASR_TEX_Pos)

#define MPU_RASR_S_Pos 18U
#define MPU_RASR_S_Msk (1UL << MPU_RASR_S_Pos)

#define MPU_RASR_C_Pos 17U
#define MPU_RASR_C_Msk (1UL << MPU_RASR_C_Pos)

#define MPU_RASR_B_Pos 16U
#define MPU_RASR_B_Msk (1UL << MPU_RASR_B_Pos)

#define MPU_RASR_SRD_Pos 8U
#define MPU_RASR_SRD_Msk (0xFFUL << MPU_RASR_SRD_Pos)

#define MPU_RASR_SIZE_Pos 1U
#define MPU_RASR_SIZE_Msk (0x1FUL << MPU_RASR_SIZE_Pos)

#define MPU_RASR_ENABLE_Pos 0U
#define MPU_RASR_ENABLE_Msk (1UL )


#endif
typedef struct
{
        uint32_t RESERVED0[1U];
  __IOM uint32_t FPCCR;
  __IOM uint32_t FPCAR;
  __IOM uint32_t FPDSCR;
  __IM uint32_t MVFR0;
  __IM uint32_t MVFR1;
} FPU_Type;


#define FPU_FPCCR_ASPEN_Pos 31U
#define FPU_FPCCR_ASPEN_Msk (1UL << FPU_FPCCR_ASPEN_Pos)

#define FPU_FPCCR_LSPEN_Pos 30U
#define FPU_FPCCR_LSPEN_Msk (1UL << FPU_FPCCR_LSPEN_Pos)

#define FPU_FPCCR_MONRDY_Pos 8U
#define FPU_FPCCR_MONRDY_Msk (1UL << FPU_FPCCR_MONRDY_Pos)

#define FPU_FPCCR_BFRDY_Pos 6U
#define FPU_FPCCR_BFRDY_Msk (1UL << FPU_FPCCR_BFRDY_Pos)

#define FPU_FPCCR_MMRDY_Pos 5U
#define FPU_FPCCR_MMRDY_Msk (1UL << FPU_FPCCR_MMRDY_Pos)

#define FPU_FPCCR_HFRDY_Pos 4U
#define FPU_FPCCR_HFRDY_Msk (1UL << FPU_FPCCR_HFRDY_Pos)

#define FPU_FPCCR_THREAD_Pos 3U
#define FPU_FPCCR_THREAD_Msk (1UL << FPU_FPCCR_THREAD_Pos)

#define FPU_FPCCR_USER_Pos 1U
#define FPU_FPCCR_USER_Msk (1UL << FPU_FPCCR_USER_Pos)

#define FPU_FPCCR_LSPACT_Pos 0U
#define FPU_FPCCR_LSPACT_Msk (1UL )


#define FPU_FPCAR_ADDRESS_Pos 3U
#define FPU_FPCAR_ADDRESS_Msk (0x1FFFFFFFUL << FPU_FPCAR_ADDRESS_Pos)


#define FPU_FPDSCR_AHP_Pos 26U
#define FPU_FPDSCR_AHP_Msk (1UL << FPU_FPDSCR_AHP_Pos)

#define FPU_FPDSCR_DN_Pos 25U
#define FPU_FPDSCR_DN_Msk (1UL << FPU_FPDSCR_DN_Pos)

#define FPU_FPDSCR_FZ_Pos 24U
#define FPU_FPDSCR_FZ_Msk (1UL << FPU_FPDSCR_FZ_Pos)

#define FPU_FPDSCR_RMode_Pos 22U
#define FPU_FPDSCR_RMode_Msk (3UL << FPU_FPDSCR_RMode_Pos)


#define FPU_MVFR0_FP_rounding_modes_Pos 28U
#define FPU_MVFR0_FP_rounding_modes_Msk (0xFUL << FPU_MVFR0_FP_rounding_modes_Pos)

#define FPU_MVFR0_Short_vectors_Pos 24U
#define FPU_MVFR0_Short_vectors_Msk (0xFUL << FPU_MVFR0_Short_vectors_Pos)

#define FPU_MVFR0_Square_root_Pos 20U
#define FPU_MVFR0_Square_root_Msk (0xFUL << FPU_MVFR0_Square_root_Pos)

#define FPU_MVFR0_Divide_Pos 16U
#define FPU_MVFR0_Divide_Msk (0xFUL << FPU_MVFR0_Divide_Pos)

#define FPU_MVFR0_FP_excep_trapping_Pos 12U
#define FPU_MVFR0_FP_excep_trapping_Msk (0xFUL << FPU_MVFR0_FP_excep_trapping_Pos)

#define FPU_MVFR0_Double_precision_Pos 8U
#define FPU_MVFR0_Double_precision_Msk (0xFUL << FPU_MVFR0_Double_precision_Pos)

#define FPU_MVFR0_Single_precision_Pos 4U
#define FPU_MVFR0_Single_precision_Msk (0xFUL << FPU_MVFR0_Single_precision_Pos)

#define FPU_MVFR0_A_SIMD_registers_Pos 0U
#define FPU_MVFR0_A_SIMD_registers_Msk (0xFUL )


#define FPU_MVFR1_FP_fused_MAC_Pos 28U
#define FPU_MVFR1_FP_fused_MAC_Msk (0xFUL << FPU_MVFR1_FP_fused_MAC_Pos)

#define FPU_MVFR1_FP_HPFP_Pos 24U
#define FPU_MVFR1_FP_HPFP_Msk (0xFUL << FPU_MVFR1_FP_HPFP_Pos)

#define FPU_MVFR1_D_NaN_mode_Pos 4U
#define FPU_MVFR1_D_NaN_mode_Msk (0xFUL << FPU_MVFR1_D_NaN_mode_Pos)

#define FPU_MVFR1_FtZ_mode_Pos 0U
#define FPU_MVFR1_FtZ_mode_Msk (0xFUL )
typedef struct
{
  __IOM uint32_t DHCSR;
  __OM uint32_t DCRSR;
  __IOM uint32_t DCRDR;
  __IOM uint32_t DEMCR;
} CoreDebug_Type;


#define CoreDebug_DHCSR_DBGKEY_Pos 16U
#define CoreDebug_DHCSR_DBGKEY_Msk (0xFFFFUL << CoreDebug_DHCSR_DBGKEY_Pos)

#define CoreDebug_DHCSR_S_RESET_ST_Pos 25U
#define CoreDebug_DHCSR_S_RESET_ST_Msk (1UL << CoreDebug_DHCSR_S_RESET_ST_Pos)

#define CoreDebug_DHCSR_S_RETIRE_ST_Pos 24U
#define CoreDebug_DHCSR_S_RETIRE_ST_Msk (1UL << CoreDebug_DHCSR_S_RETIRE_ST_Pos)

#define CoreDebug_DHCSR_S_LOCKUP_Pos 19U
#define CoreDebug_DHCSR_S_LOCKUP_Msk (1UL << CoreDebug_DHCSR_S_LOCKUP_Pos)

#define CoreDebug_DHCSR_S_SLEEP_Pos 18U
#define CoreDebug_DHCSR_S_SLEEP_Msk (1UL << CoreDebug_DHCSR_S_SLEEP_Pos)

#define CoreDebug_DHCSR_S_HALT_Pos 17U
#define CoreDebug_DHCSR_S_HALT_Msk (1UL << CoreDebug_DHCSR_S_HALT_Pos)

#define CoreDebug_DHCSR_S_REGRDY_Pos 16U
#define CoreDebug_DHCSR_S_REGRDY_Msk (1UL << CoreDebug_DHCSR_S_REGRDY_Pos)

#define CoreDebug_DHCSR_C_SNAPSTALL_Pos 5U
#define CoreDebug_DHCSR_C_SNAPSTALL_Msk (1UL << CoreDebug_DHCSR_C_SNAPSTALL_Pos)

#define CoreDebug_DHCSR_C_MASKINTS_Pos 3U
#define CoreDebug_DHCSR_C_MASKINTS_Msk (1UL << CoreDebug_DHCSR_C_MASKINTS_Pos)

#define CoreDebug_DHCSR_C_STEP_Pos 2U
#define CoreDebug_DHCSR_C_STEP_Msk (1UL << CoreDebug_DHCSR_C_STEP_Pos)

#define CoreDebug_DHCSR_C_HALT_Pos 1U
#define CoreDebug_DHCSR_C_HALT_Msk (1UL << CoreDebug_DHCSR_C_HALT_Pos)

#define CoreDebug_DHCSR_C_DEBUGEN_Pos 0U
#define CoreDebug_DHCSR_C_DEBUGEN_Msk (1UL )


#define CoreDebug_DCRSR_REGWnR_Pos 16U
#define CoreDebug_DCRSR_REGWnR_Msk (1UL << CoreDebug_DCRSR_REGWnR_Pos)

#define CoreDebug_DCRSR_REGSEL_Pos 0U
#define CoreDebug_DCRSR_REGSEL_Msk (0x1FUL )


#define CoreDebug_DEMCR_TRCENA_Pos 24U
#define CoreDebug_DEMCR_TRCENA_Msk (1UL << CoreDebug_DEMCR_TRCENA_Pos)

#define CoreDebug_DEMCR_MON_REQ_Pos 19U
#define CoreDebug_DEMCR_MON_REQ_Msk (1UL << CoreDebug_DEMCR_MON_REQ_Pos)

#define CoreDebug_DEMCR_MON_STEP_Pos 18U
#define CoreDebug_DEMCR_MON_STEP_Msk (1UL << CoreDebug_DEMCR_MON_STEP_Pos)

#define CoreDebug_DEMCR_MON_PEND_Pos 17U
#define CoreDebug_DEMCR_MON_PEND_Msk (1UL << CoreDebug_DEMCR_MON_PEND_Pos)

#define CoreDebug_DEMCR_MON_EN_Pos 16U
#define CoreDebug_DEMCR_MON_EN_Msk (1UL << CoreDebug_DEMCR_MON_EN_Pos)

#define CoreDebug_DEMCR_VC_HARDERR_Pos 10U
#define CoreDebug_DEMCR_VC_HARDERR_Msk (1UL << CoreDebug_DEMCR_VC_HARDERR_Pos)

#define CoreDebug_DEMCR_VC_INTERR_Pos 9U
#define CoreDebug_DEMCR_VC_INTERR_Msk (1UL << CoreDebug_DEMCR_VC_INTERR_Pos)

#define CoreDebug_DEMCR_VC_BUSERR_Pos 8U
#define CoreDebug_DEMCR_VC_BUSERR_Msk (1UL << CoreDebug_DEMCR_VC_BUSERR_Pos)

#define CoreDebug_DEMCR_VC_STATERR_Pos 7U
#define CoreDebug_DEMCR_VC_STATERR_Msk (1UL << CoreDebug_DEMCR_VC_STATERR_Pos)

#define CoreDebug_DEMCR_VC_CHKERR_Pos 6U
#define CoreDebug_DEMCR_VC_CHKERR_Msk (1UL << CoreDebug_DEMCR_VC_CHKERR_Pos)

#define CoreDebug_DEMCR_VC_NOCPERR_Pos 5U
#define CoreDebug_DEMCR_VC_NOCPERR_Msk (1UL << CoreDebug_DEMCR_VC_NOCPERR_Pos)

#define CoreDebug_DEMCR_VC_MMERR_Pos 4U
#define CoreDebug_DEMCR_VC_MMERR_Msk (1UL << CoreDebug_DEMCR_VC_MMERR_Pos)

#define CoreDebug_DEMCR_VC_CORERESET_Pos 0U
#define CoreDebug_DEMCR_VC_CORERESET_Msk (1UL )
#define _VAL2FLD(field,value) (((uint32_t)(value) << field ## _Pos) & field ## _Msk)

#define _FLD2VAL(field,value) (((uint32_t)(value) & field ## _Msk) >> field ## _Pos)
#define SCS_BASE (0xE000E000UL)
#define ITM_BASE (0xE0000000UL)
#define DWT_BASE (0xE0001000UL)
#define TPI_BASE (0xE0040000UL)
#define CoreDebug_BASE (0xE000EDF0UL)
#define SysTick_BASE (SCS_BASE + 0x0010UL)
#define NVIC_BASE (SCS_BASE + 0x0100UL)
#define SCB_BASE (SCS_BASE + 0x0D00UL)

#define SCnSCB ((SCnSCB_Type *) SCS_BASE )
#define SCB ((SCB_Type *) SCB_BASE )
#define SysTick ((SysTick_Type *) SysTick_BASE )
#define NVIC ((NVIC_Type *) NVIC_BASE )
#define ITM ((ITM_Type *) ITM_BASE )
#define DWT ((DWT_Type *) DWT_BASE )
#define TPI ((TPI_Type *) TPI_BASE )
#define CoreDebug ((CoreDebug_Type *) CoreDebug_BASE)

#if defined (__MPU_PRESENT) && (__MPU_PRESENT == 1U)
  #define MPU_BASE (SCS_BASE + 0x0D90UL)
  #define MPU ((MPU_Type *) MPU_BASE )
#endif

#define FPU_BASE (SCS_BASE + 0x0F30UL)
#define FPU ((FPU_Type *) FPU_BASE )
#ifdef CMSIS_NVIC_VIRTUAL
  #ifndef CMSIS_NVIC_VIRTUAL_HEADER_FILE
    #define CMSIS_NVIC_VIRTUAL_HEADER_FILE "cmsis_nvic_virtual.h"
  #endif
  #include CMSIS_NVIC_VIRTUAL_HEADER_FILE
#else
  #define NVIC_SetPriorityGrouping __NVIC_SetPriorityGrouping
  #define NVIC_GetPriorityGrouping __NVIC_GetPriorityGrouping
  #define NVIC_EnableIRQ __NVIC_EnableIRQ
  #define NVIC_GetEnableIRQ __NVIC_GetEnableIRQ
  #define NVIC_DisableIRQ __NVIC_DisableIRQ
  #define NVIC_GetPendingIRQ __NVIC_GetPendingIRQ
  #define NVIC_SetPendingIRQ __NVIC_SetPendingIRQ
  #define NVIC_ClearPendingIRQ __NVIC_ClearPendingIRQ
  #define NVIC_GetActive __NVIC_GetActive
  #define NVIC_SetPriority __NVIC_SetPriority
  #define NVIC_GetPriority __NVIC_GetPriority
  #define NVIC_SystemReset __NVIC_SystemReset
#endif

#ifdef CMSIS_VECTAB_VIRTUAL
  #ifndef CMSIS_VECTAB_VIRTUAL_HEADER_FILE
   #define CMSIS_VECTAB_VIRTUAL_HEADER_FILE "cmsis_vectab_virtual.h"
  #endif
  #include CMSIS_VECTAB_VIRTUAL_HEADER_FILE
#else
  #define NVIC_SetVector __NVIC_SetVector
  #define NVIC_GetVector __NVIC_GetVector
#endif

#define NVIC_USER_IRQ_OFFSET 16



#define EXC_RETURN_HANDLER (0xFFFFFFF1UL)
#define EXC_RETURN_THREAD_MSP (0xFFFFFFF9UL)
#define EXC_RETURN_THREAD_PSP (0xFFFFFFFDUL)
#define EXC_RETURN_HANDLER_FPU (0xFFFFFFE1UL)
#define EXC_RETURN_THREAD_MSP_FPU (0xFFFFFFE9UL)
#define EXC_RETURN_THREAD_PSP_FPU (0xFFFFFFEDUL)

__STATIC_INLINE void __NVIC_SetPriorityGrouping(uint32_t PriorityGroup)
{
  uint32_t reg_value;
  uint32_t PriorityGroupTmp = (PriorityGroup & (uint32_t)0x07UL);

  reg_value = SCB->AIRCR;
  reg_value &= ~((uint32_t)(SCB_AIRCR_VECTKEY_Msk | SCB_AIRCR_PRIGROUP_Msk));
  reg_value = (reg_value |
                ((uint32_t)0x5FAUL << SCB_AIRCR_VECTKEY_Pos) |
                (PriorityGroupTmp << SCB_AIRCR_PRIGROUP_Pos) );
  SCB->AIRCR = reg_value;
}

__STATIC_INLINE uint32_t __NVIC_GetPriorityGrouping(void)
{
  return ((uint32_t)((SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) >> SCB_AIRCR_PRIGROUP_Pos));
}

__STATIC_INLINE void __NVIC_EnableIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    NVIC->ISER[(((uint32_t)IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL));
  }
}

__STATIC_INLINE uint32_t __NVIC_GetEnableIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    return((uint32_t)(((NVIC->ISER[(((uint32_t)IRQn) >> 5UL)] & (1UL << (((uint32_t)IRQn) & 0x1FUL))) != 0UL) ? 1UL : 0UL));
  }
  else
  {
    return(0U);
  }
}

__STATIC_INLINE void __NVIC_DisableIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    NVIC->ICER[(((uint32_t)IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL));
    __DSB();
    __ISB();
  }
}

__STATIC_INLINE uint32_t __NVIC_GetPendingIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    return((uint32_t)(((NVIC->ISPR[(((uint32_t)IRQn) >> 5UL)] & (1UL << (((uint32_t)IRQn) & 0x1FUL))) != 0UL) ? 1UL : 0UL));
  }
  else
  {
    return(0U);
  }
}

__STATIC_INLINE void __NVIC_SetPendingIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    NVIC->ISPR[(((uint32_t)IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL));
  }
}

__STATIC_INLINE void __NVIC_ClearPendingIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    NVIC->ICPR[(((uint32_t)IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL));
  }
}

__STATIC_INLINE uint32_t __NVIC_GetActive(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    return((uint32_t)(((NVIC->IABR[(((uint32_t)IRQn) >> 5UL)] & (1UL << (((uint32_t)IRQn) & 0x1FUL))) != 0UL) ? 1UL : 0UL));
  }
  else
  {
    return(0U);
  }
}

__STATIC_INLINE void __NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
{
  if ((int32_t)(IRQn) >= 0)
  {
    NVIC->IP[((uint32_t)IRQn)] = (uint8_t)((priority << (8U - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL);
  }
  else
  {
    SCB->SHP[(((uint32_t)IRQn) & 0xFUL)-4UL] = (uint8_t)((priority << (8U - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL);
  }
}

__STATIC_INLINE uint32_t __NVIC_GetPriority(IRQn_Type IRQn)
{

  if ((int32_t)(IRQn) >= 0)
  {
    return(((uint32_t)NVIC->IP[((uint32_t)IRQn)] >> (8U - __NVIC_PRIO_BITS)));
  }
  else
  {
    return(((uint32_t)SCB->SHP[(((uint32_t)IRQn) & 0xFUL)-4UL] >> (8U - __NVIC_PRIO_BITS)));
  }
}

__STATIC_INLINE uint32_t NVIC_EncodePriority (uint32_t PriorityGroup, uint32_t PreemptPriority, uint32_t SubPriority)
{
  uint32_t PriorityGroupTmp = (PriorityGroup & (uint32_t)0x07UL);
  uint32_t PreemptPriorityBits;
  uint32_t SubPriorityBits;

  PreemptPriorityBits = ((7UL - PriorityGroupTmp) > (uint32_t)(__NVIC_PRIO_BITS)) ? (uint32_t)(__NVIC_PRIO_BITS) : (uint32_t)(7UL - PriorityGroupTmp);
  SubPriorityBits = ((PriorityGroupTmp + (uint32_t)(__NVIC_PRIO_BITS)) < (uint32_t)7UL) ? (uint32_t)0UL : (uint32_t)((PriorityGroupTmp - 7UL) + (uint32_t)(__NVIC_PRIO_BITS));

  return (
           ((PreemptPriority & (uint32_t)((1UL << (PreemptPriorityBits)) - 1UL)) << SubPriorityBits) |
           ((SubPriority & (uint32_t)((1UL << (SubPriorityBits )) - 1UL)))
         );
}

__STATIC_INLINE void NVIC_DecodePriority (uint32_t Priority, uint32_t PriorityGroup, uint32_t* const pPreemptPriority, uint32_t* const pSubPriority)
{
  uint32_t PriorityGroupTmp = (PriorityGroup & (uint32_t)0x07UL);
  uint32_t PreemptPriorityBits;
  uint32_t SubPriorityBits;

  PreemptPriorityBits = ((7UL - PriorityGroupTmp) > (uint32_t)(__NVIC_PRIO_BITS)) ? (uint32_t)(__NVIC_PRIO_BITS) : (uint32_t)(7UL - PriorityGroupTmp);
  SubPriorityBits = ((PriorityGroupTmp + (uint32_t)(__NVIC_PRIO_BITS)) < (uint32_t)7UL) ? (uint32_t)0UL : (uint32_t)((PriorityGroupTmp - 7UL) + (uint32_t)(__NVIC_PRIO_BITS));

  *pPreemptPriority = (Priority >> SubPriorityBits) & (uint32_t)((1UL << (PreemptPriorityBits)) - 1UL);
  *pSubPriority = (Priority ) & (uint32_t)((1UL << (SubPriorityBits )) - 1UL);
}

__STATIC_INLINE void __NVIC_SetVector(IRQn_Type IRQn, uint32_t vector)
{
  uint32_t *vectors = (uint32_t *)SCB->VTOR;
  vectors[(int32_t)IRQn + NVIC_USER_IRQ_OFFSET] = vector;
}

__STATIC_INLINE uint32_t __NVIC_GetVector(IRQn_Type IRQn)
{
  uint32_t *vectors = (uint32_t *)SCB->VTOR;
  return vectors[(int32_t)IRQn + NVIC_USER_IRQ_OFFSET];
}


__NO_RETURN __STATIC_INLINE void __NVIC_SystemReset(void)
{
  __DSB();

  SCB->AIRCR = (uint32_t)((0x5FAUL << SCB_AIRCR_VECTKEY_Pos) |
                           (SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) |
                            SCB_AIRCR_SYSRESETREQ_Msk );
  __DSB();

  for(;;)
  {
    __NOP();
  }
}


#if defined (__MPU_PRESENT) && (__MPU_PRESENT == 1U)

#include "mpu_armv7.h"

#endif
__STATIC_INLINE uint32_t SCB_GetFPUType(void)
{
  uint32_t mvfr0;

  mvfr0 = FPU->MVFR0;
  if ((mvfr0 & (FPU_MVFR0_Single_precision_Msk | FPU_MVFR0_Double_precision_Msk)) == 0x020U)
  {
    return 1U;
  }
  else
  {
    return 0U;
  }
}

#if defined (__Vendor_SysTickConfig) && (__Vendor_SysTickConfig == 0U)
__STATIC_INLINE uint32_t SysTick_Config(uint32_t ticks)
{
  if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk)
  {
    return (1UL);
  }

  SysTick->LOAD = (uint32_t)(ticks - 1UL);
  NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);
  SysTick->VAL = 0UL;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk |
                   SysTick_CTRL_ENABLE_Msk;
  return (0UL);
}

#endif
extern volatile int32_t ITM_RxBuffer;
#define ITM_RXBUFFER_EMPTY ((int32_t)0x5AA55AA5U)
__STATIC_INLINE uint32_t ITM_SendChar (uint32_t ch)
{
  if (((ITM->TCR & ITM_TCR_ITMENA_Msk) != 0UL) &&
      ((ITM->TER & 1UL ) != 0UL) )
  {
    while (ITM->PORT[0U].u32 == 0UL)
    {
      __NOP();
    }
    ITM->PORT[0U].u8 = (uint8_t)ch;
  }
  return (ch);
}
__STATIC_INLINE int32_t ITM_ReceiveChar (void)
{
  int32_t ch = -1;

  if (ITM_RxBuffer != ITM_RXBUFFER_EMPTY)
  {
    ch = ITM_RxBuffer;
    ITM_RxBuffer = ITM_RXBUFFER_EMPTY;
  }

  return (ch);
}
__STATIC_INLINE int32_t ITM_CheckChar (void)
{

  if (ITM_RxBuffer == ITM_RXBUFFER_EMPTY)
  {
    return (0);
  }
  else
  {
    return (1);
  }
}

#ifdef __cplusplus
}
#endif

#endif

#endif
