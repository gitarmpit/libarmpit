#ifndef SMC_H 
#define SMC_H
#include "common.h"

/*

RUN  The MCU can be run at full speed and the internal supply is fully regulated, that is, in run regulation.
     This mode is also referred to as Normal Run mode.

WAIT The core clock is gated off. The system clock continues to operate. Bus clocks, if enabled, continue
     to operate. Run regulation is maintained.

STOP The core clock is gated off. System clocks to other masters and bus clocks are gated off after all
     stop acknowledge signals from supporting peripherals are valid.

VLPR The core, system, bus, and flash clock maximum frequencies are restricted in this mode. See the
     Power Management chapter for details about the maximum allowable frequencies.

VLPW The core clock is gated off. The system, bus, and flash clocks continue to operate, although their
     maximum frequency is restricted. See the Power Management chapter for details on the maximum
     allowable frequencies.

VLPS The core clock is gated off. System clocks to other masters and bus clocks are gated off after all
     stop acknowledge signals from supporting peripherals are valid.


LLS The core clock is gated off. System clocks to other masters and bus clocks are gated off after all
    stop acknowledge signals from supporting peripherals are valid. The MCU is placed in a low
    leakage mode by reducing the voltage to internal logic. All system RAM contents, internal logic and
    I/O states are retained.

VLLS3 The core clock is gated off. System clocks to other masters and bus clocks are gated off after all
      stop acknowledge signals from supporting peripherals are valid. The MCU is placed in a low
      leakage mode by powering down the internal logic. All system RAM contents are retained and I/O
      states are held. Internal logic states are not retained.

VLLS1 The core clock is gated off. System clocks to other masters and bus clocks are gated off after all
      stop acknowledge signals from supporting peripherals are valid. The MCU is placed in a low
      leakage mode by powering down the internal logic and all system RAM. I/O states are held. Internal
      logic states are not retained.

VLLS0 The core clock is gated off. System clocks to other masters and bus clocks are gated off after all
      stop acknowledge signals from supporting peripherals are valid. The MCU is placed in a low
      leakage mode by powering down the internal logic and all system RAM. I/O states are held. Internal
      logic states are not retained. The 1kHz LPO clock is disabled and the power on reset (POR) circuit
      can be optionally enabled using STOPCTRL[PORPO]

• Run
• Sleep       (Wait)  WFI
• Deep Sleep  (Stop)  WFE

*/

#ifndef SMC_BASE
#define SMC_BASE     0x4007E000
#endif

#define SMC_PMPROT   (*(volatile uint8_t*)(SMC_BASE + 0x0))
#define SMC_PMCTRL   (*(volatile uint8_t*)(SMC_BASE + 0x1))
#define SMC_STOPCTRL (*(volatile uint8_t*)(SMC_BASE + 0x2))
#define SMC_PMSTAT   (*(volatile uint8_t*)(SMC_BASE + 0x3))

// protection for entry into any low-power run or stop mode
#define SMC_PMPROT_AVLP  (1<<5) // Allow Very-Low-Power Modes (VLPR/W/S)
#define SMC_PMPROT_ALLS  (1<<3) // Allow Low-Leakage Stop Mode (LLS)
#define SMC_PMPROT_AVLLS (1<<1) // Allow Very-Low-Leakage Stop Mode (VLLS)

// controls entry into low-power run and stop modes
#define SMC_PMCTRL_RUNM_RUN         (0<<5)
#define SMC_PMCTRL_RUNM_VLPR        (2<<5)
#define SMC_PMCTRL_RUNM_CLEARMASK   (3<<5)

#define SMC_PMCTRL_STOPA  (1<<3) // Read-only, stop aborted due to interrupt or reset

// Stop Mode Control
#define SMC_PMCTRL_STOPM_STOP         (0<<0) // Normal STOP
#define SMC_PMCTRL_STOPM_VLPS         (2<<0)
#define SMC_PMCTRL_STOPM_VLLS         (4<<0)
#define SMC_PMCTRL_STOPM_CLEARMASK    (7<<0)

// Stop Control Register
// provides various control bits allowing the user to fine tune
// power consumption during the stop mode selected by the STOPM field
#define SMC_STOPCTRL_PSTOPO_STOP       (0<<6) // Normal Stop
#define SMC_STOPCTRL_PSTOPO_PSTOP1     (1<<6) // Partial Stop with both system and bus clocks disabled
#define SMC_STOPCTRL_PSTOPO_PSTOP2     (2<<6) // Partial Stop with system clock disabled and bus clock enabled
#define SMC_STOPCTRL_PSTOPO_CLEARMASK  (3<<6)

#define SMC_STOPCTRL_PORPRO  (1<<5) // POR detect circuit is disabled/enabled in VLLS0

// VLLS Mode Control
#define SMC_STOPCTRL_VLLSM_VLLS0      (0<<0)
#define SMC_STOPCTRL_VLLSM_VLLS1      (1<<0)
#define SMC_STOPCTRL_VLLSM_VLLS3      (3<<0)
#define SMC_STOPCTRL_VLLSM_CLEARMASK  (7<<0)

#define SMC_PMSTAT_RUN    0x00000001 
#define SMC_PMSTAT_STOP   0x00000010 
#define SMC_PMSTAT_VLPR   0x00000100 
#define SMC_PMSTAT_VLPW   0x00001000 
#define SMC_PMSTAT_VLPS   0x00010000 
#define SMC_PMSTAT_LLS    0x00100000 
#define SMC_PMSTAT_VLLS   0x01000000 


#ifdef __cplusplus
 extern "C" {
#endif

void SMC_AllowAllPowerModes();

uint32_t SMC_GetRunMode();

#ifdef __cplusplus
}
#endif


#endif
