#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

namespace naos
{
    namespace Memory
    {
        enum class MemoryType : uint8_t
        {
            Invalid,
            Available,
            Reserved,
            ACPIReclaimable,
            NVS,
            BadRAM,
            Kernel,
            Module,
            PFA,
            KernelHeap,
        };

        typedef struct
        {
            uintptr_t  Address;
            uint32_t   Size;
            MemoryType Type;
        } PACKED MemoryBlock;

        class PhysicalMemoryManager
        {
            private:
                static MemoryBlock  _blocks[];
                static MemoryBlock* _free,* _afterfree;
                static size_t       _usable;

            public:
                static void Init();
                static void Print();
                static void FetchMemoryMap();
                static void FetchModules();

            public:
                static MemoryBlock* Map(uintptr_t addr, size_t size, MemoryType type);
                static MemoryBlock* Request(size_t size, MemoryType type = MemoryType::Reserved);
                static MemoryBlock* FirstByType(MemoryType type);

            public:
                static size_t Usable();
                static const char* TypeString(MemoryType type);

            private:
                static MemoryBlock* Next();
        };
    }
}