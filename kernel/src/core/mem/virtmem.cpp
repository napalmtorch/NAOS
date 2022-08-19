#include <core/mem/virtmem.h>
#include <core/kernel.h>

namespace naos
{
    namespace Memory
    {
        void VirtualMemoryManager::Init()
        {
            MapKernel(&Kernel::PageDir);

            SetDirectory(&Kernel::PageDir);
            EnablePaging();

            printf("%s Initialized virtual memory manager\n", DEBUG_OK);
        }
        
        void VirtualMemoryManager::SetDirectory(PageDirectory* dir)
        {
            _write_cr3(dir->Address);
            printf("%s Set active page directory to 0x%8x\n", DEBUG_INFO, dir->Address);
        }

        void VirtualMemoryManager::EnablePaging()
        {
            uint32_t cr0 = _read_cr0();
            cr0 |= 0x80000001;
            _write_cr0(cr0);
            printf("%s Enabled paging\n", DEBUG_INFO);
        }

        void VirtualMemoryManager::DisablePaging()
        {
            uint32_t cr0 = _read_cr0();
            cr0 &= ~(1 << 31);
            _write_cr0(cr0);
            _write_cr3(0);
            printf("%s Disabled paging\n", DEBUG_INFO);
        }

        void VirtualMemoryManager::MapKernel(PageDirectory* dir)
        {
            size_t size = memalign((8 * MB) + PhysicalMemoryManager::FirstByType(MemoryType::PFA)->Address + PhysicalMemoryManager::FirstByType(MemoryType::PFA)->Size, 4 * MB);
            for (size_t i = 0; i < size; i += 4 * MB) { dir->MapTable(i, i); }
            printf("%s Mapped kernel to page directory at 0x%8x\n", DEBUG_INFO, dir->Address);
        }

        void VirtualMemoryManager::MapKernelHeap(PageDirectory* dir)
        {
            uintptr_t p = KernelHeap::PhysicalAddress(), v = KernelHeap::VirtualAddress();
            for (size_t i = 0; i < KernelHeap::Size(); i += 4 * MB) { dir->MapTable(v + i, p + i); }
            printf("%s Mapped kernel heap to page directory at 0x%8x\n", DEBUG_INFO, dir->Address);
        }
    }
}