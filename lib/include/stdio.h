#pragma once
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
EXTC
{
#endif

void vprintf(const char* fmt, va_list args);
void printf(const char* fmt, ...);

#ifdef __cplusplus
}
#endif