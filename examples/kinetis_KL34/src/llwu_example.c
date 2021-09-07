#include "llwu.h"

void test_llwu()
{
	LLWU_SetWakeupPin_Any(LLWU_P15);
	LLWU_SetWakeupPin_Any(LLWU_P1);
	LLWU_SetWakeupPin_Any(LLWU_P10);
	LLWU_SetWakeupPin_Any(LLWU_P6);
	LLWU_SetWakeupPin_Any(LLWU_P14);
	LLWU_SetWakeupModule(LLWU_LPTMR0);
	LLWU_SetWakeupModule(LLWU_RTC_SEC);
	while(1)
		;

}
