#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

namespace naos
{
    namespace HAL
    {
        typedef struct
        {
            char Vendor[32];
            char Name[64];
            bool PSE, PAE, APIC, MTRR;
            bool MMX, TSC, MSR;
            bool SSE, SSE2, SSE3, SSSE3, SSE41, SSE42;
            bool AVX, F16C, RDRAND;
            bool X64;
        } PACKED CPUInfoHeader;
        
        class CPU
        {
            public:
                static CPUInfoHeader InfoHdr;

            public:
                static void EnableSSE();
            public:
                static void Detect();
                static void Print();
                static void FetchInfo(uint32_t reg, uint32_t* eax, uint32_t* ebx, uint32_t* ecx, uint32_t* edx);
                static bool IsSSE();
                static bool IsMMX();
        };
    }
}