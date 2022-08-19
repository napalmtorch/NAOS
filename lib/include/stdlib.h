#pragma once
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
EXTC
{
#endif

typedef enum
{
    ALLOCTYPE_INVALID,
    ALLOCTYPE_FREE,
    ALLOCTYPE_UNSPECIFIED,
    ALLOCTYPE_OBJ,
    ALLOCTYPE_ARRAY,
    ALLOCTYPE_STRING,
} ALLOCTYPE;

void*    malloc(size_t size);
void*    tmalloc(size_t size, ALLOCTYPE type);
bool     free(void* ptr);
bool     freearray(void** ptr, uint32_t count);
int      atoi(const char* str);
uint32_t atol(const char* str);
uint32_t atox(const char* str);
float    atof(const char* str);
int      rand(int max);
void     exit(int code);
char*    itoa(int num, char* str, int base);
char*    ltoa(size_t num, char* str, int base);
char*    ftoa(float n, char* res, int afterpoint);
char*    strhex(uint32_t value, char* result, bool prefix, uint8_t bytes);
int      system(char* command);
char*    strerror(int errnum);
void     perror(const char* str, ...);

#ifdef __cplusplus
}
#endif