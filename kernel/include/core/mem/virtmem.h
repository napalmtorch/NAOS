#pragma once
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <core/mem/paging.h>

namespace naos
{
    namespace Memory
    {
        class VirtualMemoryManager
        {
            public:
                static void Init();
                static void SetDirectory(PageDirectory* dir);
                static void EnablePaging();
                static void DisablePaging();

            public:
                static void MapKernel(PageDirectory* dir);
                static void MapPageFrameAllocator(PageDirectory* dir);
                static void MapKernelHeap(PageDirectory* dir);
        };
    }
}