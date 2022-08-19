#include <stdlib.h>
#include <core/kernel.h>

#ifdef __cplusplus
EXTC
{
#endif

const char HEX_VALUES[]  = "0123456789ABCDEF";

void* malloc(size_t size) { return tmalloc(size, ALLOCTYPE_UNSPECIFIED); }

void* tmalloc(size_t size, ALLOCTYPE type)
{
    return NULL;
}

bool free(void* ptr)
{
    return false;
}

bool freearray(void** ptr, uint32_t count)
{
    if (ptr == NULL) { return false; }
    for (size_t i = 0; i < count; i++) { if (ptr[i] != NULL) { if (!free(ptr[i])) { return false; } } }
    return free(ptr);
}

int atoi(const char* str)
{
    return (int)atol(str);
}

uint32_t atol(const char* str)
{
    int result = 0, sign = 0;
    uint32_t digit;
    while (*str == 0x20) { str += 1; }

    if (*str == '-') { sign = 1; str += 1; } 
    else 
    { 
        sign = 0;
        if (*str == '+') { str += 1; }
    }

    for (;; str += 1) 
    {
        digit = *str - '0';
        if (digit > 9) { break; }
        result = (10*result) + digit;
    }

    if (sign) { return -result; }
    return result;
}

uint32_t atox(const char* str)
{
    uint32_t num = 0;
    for (size_t i = 0; i < strlen(str); i++)
    {
        if (str[i] >= 48 && str[i] <= 57) { num = (num << 4) | (str[i] - 48); }
        else if (str[i] == 'A' || str[i] == 'a') { num = (num << 4) | 0x0A; }
        else if (str[i] == 'B' || str[i] == 'b') { num = (num << 4) | 0x0B; }
        else if (str[i] == 'C' || str[i] == 'c') { num = (num << 4) | 0x0C; }
        else if (str[i] == 'D' || str[i] == 'd') { num = (num << 4) | 0x0D; }
        else if (str[i] == 'E' || str[i] == 'e') { num = (num << 4) | 0x0E; }
        else if (str[i] == 'F' || str[i] == 'f') { num = (num << 4) | 0x0F; }
        else { return 0 ; }
    }
    return num;
}

float atof(const char* str) { return 0.0f; }

static uint32_t _rand_next = 0xFFFFFFFF;
int rand(int max)
{
    if (_rand_next == 0xFFFFFFFF) { _rand_next = 100; }
    _rand_next = _rand_next * 1103515245 + 12345;
    return (_rand_next / (max * 2)) % max;
}

void exit(int code) { }

void _itoa_swap(char *x, char *y) { char t = *x; *x = *y; *y = t; }

char* _itoa_rev(char *buffer, int i, int j)
{
    while (i < j) { _itoa_swap(&buffer[i++], &buffer[j--]); }
    return buffer;
}

char* itoa(int num, char* str, int base)
{
    if (base < 2 || base > 32) { return str; }

    int i = 0, n = abs(num); 
    while (n)
    {
        int r = n % base;

        if (r >= 10) { str[i++] = 65 + (r - 10); }
        else { str[i++] = 48 + r; }
        n = n / base;
    }

    if (i == 0) { str[i++] = '0'; }
    if (num < 0 && base == 10) { str[i++] = '-'; }
    str[i] = '\0';
    return _itoa_rev(str, 0, i - 1);
}

void _ultoa(unsigned long value, char* result, int base)
{
    unsigned char index;
    char buffer[32];
    index = 32;
    do 
    {
        buffer[--index] = '0' + (value % base);
        if ( buffer[index] > '9') { buffer[index] += 'A' - ':'; }
        value /= base;
    } while (value != 0);

    do { *result++ = buffer[index++]; } while (index < 32);
    *result = 0;
}

char* ltoa(size_t num, char* str, int base)
{
    _ultoa(num, str, base);
    return str;
}

void _ftoa_rev(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) 
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

int _ftoa_conv(int x, char str[], int d)
{
    int i = 0;
    while (x) { str[i++] = (x % 10) + '0'; x = x / 10; }

    while (i < d) { str[i++] = '0'; }
  
    _ftoa_rev(str, i);
    str[i] = '\0';
    return i;
}

char* ftoa(float n, char* res, int afterpoint)
{
    int ipart = (int)n;
    float fpart = n - (float)ipart;
    int i = _ftoa_conv(ipart, res, 0);
  
    if (afterpoint != 0) 
    {
        res[i] = '.';
        fpart = fpart * pow(10, afterpoint);
        _ftoa_conv((int)fpart, res + i + 1, afterpoint);
    }
    return res;
}

char* strhex(uint32_t value, char* result, bool prefix, uint8_t bytes)
{
    char* orig = result;
    orig[0] = 0;
    if (prefix) { result[0] = '0'; result[1] = 'x'; result += 2; }
    if (bytes == 1)
    {
        *result = HEX_VALUES[(uint8_t)((value & 0xF0) >> 4)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x0F))];
    }
    else if (bytes == 2)
    {
        *result = HEX_VALUES[(uint8_t)((value & 0xF000) >> 12)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x0F00) >> 8)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x00F0) >> 4)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x000F))]; 
    }
    else if (bytes == 4)
    {
        *result = HEX_VALUES[(uint8_t)((value & 0xF0000000) >> 28)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x0F000000) >> 24)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x00F00000) >> 20)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x000F0000) >> 16)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x0000F000) >> 12)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x00000F00) >> 8)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x000000F0) >> 4)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x0000000F))];
    }
    return orig;
}

int system(char* command) { return 0; }

char* strerror(int errnum)
{

}

void perror(const char* str, ...)
{
    asm volatile("cli");
    printf("%s ", DEBUG_ERROR);
    va_list args;
    va_start(args, str);
    vprintf(str, args);
    printf("\n");
    asm volatile("hlt");
}

#ifdef __cplusplus
}
#endif