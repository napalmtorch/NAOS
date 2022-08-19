#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <core/mem/physmem.h>

namespace naos
{
    namespace Memory
    {
        enum class HeapBlockState : uint8_t { Invalid, Unused, Used };

        typedef struct
        {
            uintptr_t      PhysicalAddress;
            uintptr_t      VirtualAddress;
            size_t         Size;
            HeapBlockState State;
        } PACKED HeapBlock;

        class KernelHeap
        {
            private:
                static MemoryBlock* _memblk_entries;
                static MemoryBlock* _memblk_data;
                static HeapBlock*   _blocks;
                static uint32_t     _count;
                static uintptr_t    _phys_addr;
                static uintptr_t    _virt_addr;

            public:
                static void     Init();
                static void*    Allocate(size_t size, bool clear = true);
                static bool     Free(void* ptr);
                static uint32_t Merge();

            public:
                static HeapBlock* CreateBlock(uint32_t virt, uint32_t phys, size_t size, HeapBlockState state);
                static bool       DestroyBlock(HeapBlock* blk);

            public:
                static HeapBlock* Next(size_t size);
                static HeapBlock* NextEmpty();
                static HeapBlock* Nearest(HeapBlock* blk);
                static HeapBlock* Fetch(void* ptr);

            public:
                static size_t Size();
                static uintptr_t PhysicalAddress();
                static uintptr_t VirtualAddress();
                static uint32_t  Count();
        };
    }
}