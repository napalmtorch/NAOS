#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <core/mem/physmem.h>

#define PGFRAME_SIZE 0x1000

namespace naos
{
    namespace Memory
    {
        enum class PageFrameState : uint8_t { Unused, Used };

        typedef struct
        {
            uintptr_t      Address;
            PageFrameState State;
        } PACKED PageFrame;

        class PageFrameAllocator
        {
            private:
                static MemoryBlock* _memblk_entries;
                static MemoryBlock* _memblk_data;
                static PageFrame*   _frames;
                static uint32_t     _count;

            public:
                static void       Init();
                static PageFrame* Allocate();
                static bool       Free(PageFrame* frame);
                static bool       Free(uintptr_t frame_addr);

            private:
                static PageFrame* Next();
        };
    }
}