#include <hal/sys/cpu.h>
#include <core/kernel.h>

#define ECX_SSE3                        (1 << 0)    // Streaming SIMD Extensions 3
#define ECX_PCLMULQDQ                   (1 << 1)    // PCLMULQDQ Instruction
#define ECX_DTES64                      (1 << 2)    // 64-Bit Debug Store Area
#define ECX_MONITOR                     (1 << 3)    // MONITOR/MWAIT
#define ECX_DS_CPL                      (1 << 4)    // CPL Qualified Debug Store
#define ECX_VMX                         (1 << 5)    // Virtual Machine Extensions
#define ECX_SMX                         (1 << 6)    // Safer Mode Extensions
#define ECX_EST                         (1 << 7)    // Enhanced SpeedStep Technology
#define ECX_TM2                         (1 << 8)    // Thermal Monitor 2
#define ECX_SSSE3                       (1 << 9)    // Supplemental Streaming SIMD Extensions 3
#define ECX_CNXT_ID                     (1 << 10)   // L1 Context ID
#define ECX_FMA                         (1 << 12)   // Fused Multiply Add
#define ECX_CX16                        (1 << 13)   // CMPXCHG16B Instruction
#define ECX_XTPR                        (1 << 14)   // xTPR Update Control
#define ECX_PDCM                        (1 << 15)   // Perf/Debug Capability MSR
#define ECX_PCID                        (1 << 17)   // Process-context Identifiers
#define ECX_DCA                         (1 << 18)   // Direct Cache Access
#define ECX_SSE41                       (1 << 19)   // Streaming SIMD Extensions 4.1
#define ECX_SSE42                       (1 << 20)   // Streaming SIMD Extensions 4.2
#define ECX_X2APIC                      (1 << 21)   // Extended xAPIC Support
#define ECX_MOVBE                       (1 << 22)   // MOVBE Instruction
#define ECX_POPCNT                      (1 << 23)   // POPCNT Instruction
#define ECX_TSC                         (1 << 24)   // Local APIC supports TSC Deadline
#define ECX_AESNI                       (1 << 25)   // AESNI Instruction
#define ECX_XSAVE                       (1 << 26)   // XSAVE/XSTOR States
#define ECX_OSXSAVE                     (1 << 27)   // OS Enabled Extended State Management
#define ECX_AVX                         (1 << 28)   // AVX Instructions
#define ECX_F16C                        (1 << 29)   // 16-bit Floating Point Instructions
#define ECX_RDRAND                      (1 << 30)   // RDRAND Instruction
#define EDX_FPU                         (1 << 0)    // Floating-Point Unit On-Chip
#define EDX_VME                         (1 << 1)    // Virtual 8086 Mode Extensions
#define EDX_DE                          (1 << 2)    // Debugging Extensions
#define EDX_PSE                         (1 << 3)    // Page Size Extension
#define EDX_TSC                         (1 << 4)    // Time Stamp Counter
#define EDX_MSR                         (1 << 5)    // Model Specific Registers
#define EDX_PAE                         (1 << 6)    // Physical Address Extension
#define EDX_MCE                         (1 << 7)    // Machine-Check Exception
#define EDX_CX8                         (1 << 8)    // CMPXCHG8 Instruction
#define EDX_APIC                        (1 << 9)    // APIC On-Chip
#define EDX_SEP                         (1 << 11)   // SYSENTER/SYSEXIT instructions
#define EDX_MTRR                        (1 << 12)   // Memory Type Range Registers
#define EDX_PGE                         (1 << 13)   // Page Global Bit
#define EDX_MCA                         (1 << 14)   // Machine-Check Architecture
#define EDX_CMOV                        (1 << 15)   // Conditional Move Instruction
#define EDX_PAT                         (1 << 16)   // Page Attribute Table
#define EDX_PSE36                       (1 << 17)   // 36-bit Page Size Extension
#define EDX_PSN                         (1 << 18)   // Processor Serial Number
#define EDX_CLFLUSH                     (1 << 19)   // CLFLUSH Instruction
#define EDX_DS                          (1 << 21)   // Debug Store
#define EDX_ACPI                        (1 << 22)   // Thermal Monitor and Software Clock Facilities
#define EDX_MMX                         (1 << 23)   // MMX Technology
#define EDX_FXSR                        (1 << 24)   // FXSAVE and FXSTOR Instructions
#define EDX_SSE                         (1 << 25)   // Streaming SIMD Extensions
#define EDX_SSE2                        (1 << 26)   // Streaming SIMD Extensions 2
#define EDX_SS                          (1 << 27)   // Self Snoop
#define EDX_HTT                         (1 << 28)   // Multi-Threading
#define EDX_TM                          (1 << 29)   // Thermal Monitor
#define EDX_PBE                         (1 << 31)   // Pending Break Enable
#define EDX_SYSCALL                     (1 << 11)   // SYSCALL/SYSRET
#define EDX_XD                          (1 << 20)   // Execute Disable Bit
#define EDX_1GB_PAGE                    (1 << 26)   // 1 GB Pages
#define EDX_RDTSCP                      (1 << 27)   // RDTSCP and IA32_TSC_AUX
#define EDX_64_BIT                      (1 << 29)   // 64-bit Architecture

EXTC
{
    void _clr_cr0_em()         { _write_cr0(_read_cr0() & ~(1 << 2)); }
    void _set_cr0_mp()         { _write_cr0(_read_cr0() | (1 << 1)); }
    void _set_cr4_osfxsr()     { _write_cr4(_read_cr4() | (1 << 9)); }
    void _set_cr4_osxmmexcpt() { _write_cr4(_read_cr4() | (1 << 10)); }
}

namespace naos
{
    namespace HAL
    {
        CPUInfoHeader CPU::InfoHdr;

        void CPU::EnableSSE()
        {
            _clr_cr0_em();
            _set_cr0_mp();
            _set_cr4_osfxsr();
            _set_cr4_osxmmexcpt();
            printf("%s SSE Enabled\n", DEBUG_OK);
        }

        void CPU::Detect()
        {
            memset(&InfoHdr, 0, sizeof(InfoHdr));

            uint32_t lsf, eax, ebx, ecx, edx;
            char vendor[13];
            FetchInfo(0x00, &lsf, (uint32_t*)(vendor + 0), (uint32_t*)(vendor + 8), (uint32_t*)(vendor + 4));
            strcpy(InfoHdr.Vendor, vendor);

            if (lsf >= 0x01)
            {
                FetchInfo(0x01, &eax, &ebx, &ecx, &edx);
                InfoHdr.PSE    = (edx & EDX_PSE);
                InfoHdr.PAE    = (edx & EDX_PAE);
                InfoHdr.APIC   = (edx & EDX_APIC);
                InfoHdr.MTRR   = (edx & EDX_MTRR);
                InfoHdr.MMX    = (edx & EDX_MMX);
                InfoHdr.TSC    = (edx & EDX_TSC);
                InfoHdr.MSR    = (edx & EDX_MSR);
                InfoHdr.SSE    = (edx & EDX_SSE);
                InfoHdr.SSE2   = (edx & EDX_SSE2);
                InfoHdr.SSE3   = (ecx & ECX_SSE3);
                InfoHdr.SSSE3  = (ecx & ECX_SSSE3);
                InfoHdr.SSE41  = (ecx & ECX_SSE41);
                InfoHdr.SSE42  = (ecx & ECX_SSE42);
                InfoHdr.AVX    = (ecx & ECX_AVX);
                InfoHdr.F16C   = (ecx & ECX_F16C);
                InfoHdr.RDRAND = (ecx & ECX_RDRAND);
            }
            
            FetchInfo(0x80000000, &lsf, &ebx, &ecx, &edx);
            if (lsf >= 0x80000001)
            {
                FetchInfo(0x80000001, &eax, &ebx, &ecx, &edx);
                InfoHdr.X64 = (edx & EDX_64_BIT);
            }
            if (lsf >= 0x80000004)
            {
                char name[48];
                FetchInfo(0x80000002, (uint32_t*)(name +  0), (uint32_t*)(name +  4), (uint32_t*)(name +  8), (uint32_t*)(name + 12));
                FetchInfo(0x80000003, (uint32_t*)(name + 16), (uint32_t*)(name + 20), (uint32_t*)(name + 24), (uint32_t*)(name + 28));
                FetchInfo(0x80000004, (uint32_t*)(name + 32), (uint32_t*)(name + 36), (uint32_t*)(name + 40), (uint32_t*)(name + 44));
                strcpy(InfoHdr.Name, name);
            }

            if (IsSSE()) { EnableSSE(); }
        }

        void CPU::Print()
        {
            char fstr[128];
            memset(fstr, 0, sizeof(fstr));
            printf("%s CPU Information\n", DEBUG_INFO);
            printf("- Vendor: %s\n- Name: %s\n", InfoHdr.Vendor, InfoHdr.Name);
            printf("- Architecture: %s\n", (InfoHdr.X64 ? "x64" : "x86"));

            if (InfoHdr.SSE)   { strcat(fstr, "SSE "); }
            if (InfoHdr.SSE2)  { strcat(fstr, "SSE2 "); }
            if (InfoHdr.SSE3)  { strcat(fstr, "SSE3 "); }
            if (InfoHdr.SSSE3) { strcat(fstr, "SSSE3 "); }
            if (InfoHdr.SSE41) { strcat(fstr, "SSE41 "); }
            if (InfoHdr.SSE42) { strcat(fstr, "SSE42 "); }
            if (InfoHdr.MMX)   { strcat(fstr, "MMX "); }
            printf("- Features: %s\n", fstr);
        }

        void CPU::FetchInfo(uint32_t reg, uint32_t* eax, uint32_t* ebx, uint32_t* ecx, uint32_t* edx)
        {
            asm volatile("cpuid" : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx) : "0" (reg));
        }

        bool CPU::IsSSE() { return InfoHdr.SSE; }

        bool CPU::IsMMX() { return InfoHdr.MMX; }
    }
}