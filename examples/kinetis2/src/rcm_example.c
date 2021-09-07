#include "rcm.h"

void get_reset_source()
{
	RCM_SRC_t rstSrc;
	RCM_GetResetSource(&rstSrc);
	while(1)
		;

}
