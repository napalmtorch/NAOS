#include <core/mem/kheap.h>
#include <core/kernel.h>

#define KHEAP_SIZE (128 * MB)
#define KHEAP_VBASE 0xD0000000

namespace naos
{
    namespace Memory
    {
        MemoryBlock* KernelHeap::_memblk_entries;
        MemoryBlock* KernelHeap::_memblk_data;
        HeapBlock*   KernelHeap::_blocks;
        uint32_t     KernelHeap::_count;
        uintptr_t    KernelHeap::_phys_addr;
        uintptr_t    KernelHeap::_virt_addr;

        void KernelHeap::Init()
        {
            _count = 16384;
            _memblk_entries = PhysicalMemoryManager::Request(_count * sizeof(HeapBlock));
            _memblk_data    = PhysicalMemoryManager::Request(KHEAP_SIZE, MemoryType::KernelHeap);
            _phys_addr      = _memblk_entries->Address;
            _virt_addr      = 0xD0000000;
            _blocks = (HeapBlock*)_memblk_entries->Address;
            VirtualMemoryManager::MapKernelHeap(&Kernel::PageDir);

            CreateBlock(_virt_addr + _memblk_entries->Size, _phys_addr + _memblk_entries->Size, _memblk_data->Size, HeapBlockState::Unused);

            printf("%s Initialized kernel heap - MAX:%u SIZE:%u MB\n", DEBUG_OK, _count, _memblk_data->Size / MB);
        }

        void* KernelHeap::Allocate(size_t size, bool clear)
        {
            if (size == 0) { perror("Attempt to allocate 0 bytes to kernel heap"); return NULL; }
            HeapBlock* blk = Next(memalign(size, 0x1000));
            if (blk == NULL) { perror("Failed to allocate %u bytes on kernel heap", size); return NULL; }
            if (clear) { memset((void*)blk->VirtualAddress, 0, blk->Size); }
            printf("%s VIRT:0x%8x PHYS:0x%8x SIZE:%u bytes\n", DEBUG_KALLOC, blk->VirtualAddress, blk->PhysicalAddress, blk->Size);
            return NULL;
        }

        bool KernelHeap::Free(void* ptr)
        {
            return false;
        }

        uint32_t KernelHeap::Merge()
        {
            return 0;
        }

        HeapBlock* KernelHeap::CreateBlock(uint32_t virt, uint32_t phys, size_t size, HeapBlockState state)
        {
            if (virt == NULL || phys == NULL || size == 0) { perror("Attempt to create invalid kernel heap block"); return NULL; }
            HeapBlock* blk = NextEmpty();
            if (blk == NULL) { perror("Maximum amount of kernel heap blocks reached"); return NULL; }
            blk->VirtualAddress  = virt;
            blk->PhysicalAddress = phys;
            blk->Size            = size;
            blk->State           = state;
            return blk;
        }

        bool KernelHeap::DestroyBlock(HeapBlock* blk)
        {
            if (blk == NULL) { return false; }

            for (size_t i = 0; i < _count; i++)
            {
                if (&_blocks[i] == blk)
                {
                    _blocks[i] = (HeapBlock){ .PhysicalAddress = 0, .VirtualAddress = 0, .Size = 0, .State = HeapBlockState::Invalid };
                    return true;
                }
            }

            perror("Failed to destroy kernel heap block at 0x%8x", blk);
            return false;
        }

        HeapBlock* KernelHeap::Next(size_t size)
        {
            for (size_t i = 0; i < _count; i++)
            {
                if (_blocks[i].Size >= size && _blocks[i].State == HeapBlockState::Unused)
                {
                    HeapBlock* blk = CreateBlock(_blocks[i].VirtualAddress, _blocks[i].PhysicalAddress, size, HeapBlockState::Used);
                    _blocks[i].VirtualAddress  += size;
                    _blocks[i].PhysicalAddress += size;
                    _blocks[i].Size            -= size;
                    return blk;
                }
            }
            return NULL;
        }

        HeapBlock* KernelHeap::NextEmpty()
        {
            for (size_t i = 0; i < _count; i++) { if (_blocks[i].State == HeapBlockState::Invalid) { return &_blocks[i]; } }
            return NULL;
        }

        HeapBlock* KernelHeap::Nearest(HeapBlock* blk)
        {
            return NULL;
        }

        HeapBlock* KernelHeap::Fetch(void* ptr)
        {
            return NULL;
        }

        size_t KernelHeap::Size() { return _memblk_data->Size + _memblk_entries->Size; }

        uintptr_t KernelHeap::PhysicalAddress() { return _phys_addr; }

        uintptr_t KernelHeap::VirtualAddress() { return _virt_addr; }
        
        uint32_t KernelHeap::Count() { return _count; }
    }
}