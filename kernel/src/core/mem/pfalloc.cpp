#include <core/mem/pfalloc.h>
#include <core/kernel.h>

namespace naos
{
    namespace Memory
    {
        MemoryBlock* PageFrameAllocator::_memblk_entries;
        MemoryBlock* PageFrameAllocator::_memblk_data;
        PageFrame*   PageFrameAllocator::_frames;
        uint32_t     PageFrameAllocator::_count;

        void PageFrameAllocator::Init()
        {
            _count          = 16384;
            _memblk_entries = PhysicalMemoryManager::Request(sizeof(PageFrame) * _count);
            _memblk_data    = PhysicalMemoryManager::Request(PGFRAME_SIZE * _count, MemoryType::PFA);
            _frames         = (PageFrame*)_memblk_entries->Address;

            for (size_t i = 0; i < _count; i++) 
            { 
                _frames[i].Address = _memblk_data->Address + (i * PGFRAME_SIZE); 
            }

            printf("%s Initialized page frame allocator\n", DEBUG_OK);
        }

        PageFrame* PageFrameAllocator::Allocate()
        {
            PageFrame* frame = Next();
            frame->State = PageFrameState::Used;
            if (frame == NULL) { perror("Out of page frames"); return NULL; }
            return frame;
        }

        bool PageFrameAllocator::Free(PageFrame* frame)
        {
            if (frame == NULL) { return false; }
            frame->State = PageFrameState::Unused;
            return true;
        }

        bool PageFrameAllocator::Free(uint32_t frame_addr)
        {
            if (frame_addr < _memblk_data->Address || frame_addr >= _memblk_data->Address + _memblk_data->Size) { return false; }
            for (size_t i = 0; i < _count; i++)
            {
                if (_frames[i].Address == frame_addr) { _frames[i].State = PageFrameState::Unused; return true; }
            }
            return false;
        }

        PageFrame* PageFrameAllocator::Next()
        {
            for (size_t i = 0; i < _count; i++)
            {
                if (_frames[i].State == PageFrameState::Unused) { return &_frames[i]; }
            }
            return NULL;
        }
    }
}