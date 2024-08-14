#ifndef _EE_PRINTF_H
#define _EE_PRINTF_H

#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif
int ee_sprintf(char *buf, const char *fmt, ...);
int ee_vsprintf(char *buf, const char *fmt, va_list args);

#ifdef __cplusplus
}
#endif


#endif
