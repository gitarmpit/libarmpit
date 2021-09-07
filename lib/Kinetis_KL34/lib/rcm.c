#include "rcm.h"

void RCM_GetResetSource(RCM_SRC_t* rstSrc)
{
	rstSrc->isPOR    = (RCM_SRS0 & RCM_SRS0_POR) == RCM_SRS0_POR;
	rstSrc->isPIN    = (RCM_SRS0 & RCM_SRS0_PIN) == RCM_SRS0_PIN;
	rstSrc->isWDOG   = (RCM_SRS0 & RCM_SRS0_WDOG) == RCM_SRS0_WDOG;
	rstSrc->isLOL    = (RCM_SRS0 & RCM_SRS0_LOL) == RCM_SRS0_LOL;
	rstSrc->isLOC    = (RCM_SRS0 & RCM_SRS0_LOC) == RCM_SRS0_LOC;
	rstSrc->isLVD    = (RCM_SRS0 & RCM_SRS0_LVD) == RCM_SRS0_LVD;
	rstSrc->isLOC    = (RCM_SRS0 & RCM_SRS0_LOC) == RCM_SRS0_LOC;
	rstSrc->isWAKEUP = (RCM_SRS0 & RCM_SRS0_WAKEUP) == RCM_SRS0_WAKEUP;

	rstSrc->isSACKERR = (RCM_SRS1 & RCM_SRS1_SACKERR) == RCM_SRS1_SACKERR;
	rstSrc->isMDM_AP  = (RCM_SRS1 & RCM_SRS1_MDM_AP) == RCM_SRS1_MDM_AP;
	rstSrc->isSW      = (RCM_SRS1 & RCM_SRS1_SW) == RCM_SRS1_SW;
	rstSrc->isLOCKUP  = (RCM_SRS1 & RCM_SRS1_LOCKUP) == RCM_SRS1_LOCKUP;
}
