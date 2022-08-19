#include <core/mem/physmem.h>
#include <core/kernel.h>

#define PMM_BLOCKS 4096

EXTC void zero_mem(uintptr_t addr, size_t size)
{
    printf("%s Zeroing available memory(0x%8x-0x%8x)..", DEBUG_INFO, addr, addr + size);
    for (size_t i = 0; i < 16; i++)
    {
        memset_sse((uint8_t*)(addr + (i * 16)), 0, size / 16);
        printf(".");
    }
    printf("\n%s Finished zeroing available memory\n", DEBUG_INFO);
}

namespace naos
{
    namespace Memory
    {
        MemoryBlock  PhysicalMemoryManager::_blocks[PMM_BLOCKS];
        MemoryBlock* PhysicalMemoryManager::_free;
        MemoryBlock* PhysicalMemoryManager::_afterfree;
        size_t       PhysicalMemoryManager::_usable;

        void PhysicalMemoryManager::Init()
        {
            memset(&_blocks, 0, sizeof(MemoryBlock) & PMM_BLOCKS);
            _usable    = 0;
            _free      = NULL;
            _afterfree = NULL;

            FetchMemoryMap();
            FetchModules();
            _usable = _free->Size;

            //zero_mem(_free->Address, _free->Size);

            printf("%s Initialized physical memory manager - %d MB free\n", DEBUG_OK, _usable / MB);
        }

        void PhysicalMemoryManager::Print()
        {
            for (int i = 0; i < PMM_BLOCKS; i++)
            {
                if (_blocks[i].Type == MemoryType::Invalid) { continue; }
                printf("%s MemoryBlock - TYPE:%s ADDR:0%8x SIZE:%u bytes\n", DEBUG_INFO, TypeString(_blocks[i].Type), _blocks[i].Address, _blocks[i].Size);
            }
        }

        void PhysicalMemoryManager::FetchMemoryMap()
        {
            System::MMapEntry* mmap  = Kernel::Multiboot.MemoryMapAddress;
            size_t             count = Kernel::Multiboot.MemoryMapLength / sizeof(System::MMapEntry);

            for (size_t i = 0; i < count; i++)
            {
                if (mmap[i].Type == 0) { continue; }
                if (mmap[i].Address < Kernel::StartAddress() && mmap[i].Type == (uint32_t)MemoryType::Available) { continue; }
                MemoryBlock* blk = Map(mmap[i].Address, mmap[i].Length, (MemoryType)mmap[i].Type);
                if (mmap[i].Address > Kernel::StartAddress() && mmap[i].Type == (uint32_t)MemoryType::Reserved && _afterfree == NULL) { _afterfree = blk; }
                if (blk->Address == Kernel::StartAddress()) { _free = blk; }
            }

            if (_free == NULL) { perror("Failed to parse memory map"); }
            _free->Address += Kernel::Size();
            _free->Size    += Kernel::Size();
            Map(Kernel::StartAddress(), Kernel::Size(), MemoryType::Kernel);
            printf("%s Finished fetching memory map entries\n", DEBUG_INFO);
        }

        void PhysicalMemoryManager::FetchModules()
        {
            System::MultibootModule* mods = Kernel::Multiboot.ModulesAddress;
            if (mods == NULL) { printf("%s No modules detected\n", DEBUG_WARN); return; }

            uint32_t mods_end = 0, mods_size = 0;
            for (size_t i = 0; i < Kernel::Multiboot.ModulesCount; i++)
            {
                if (mods[i].StartAddress == 0) { continue; }
                uint32_t end = memalign(mods[i].EndAddress, 0x1000);
                Map(mods[i].StartAddress, end - mods[i].StartAddress, MemoryType::Module);
                mods_end = end;
                mods_size += (end - mods[i].StartAddress);
            }

            _free->Address = mods_end;
            _free->Size    = (_afterfree->Address - _free->Address);
            printf("%s Finished fetching modules\n", DEBUG_INFO);
        }

        MemoryBlock* PhysicalMemoryManager::Map(uintptr_t addr, size_t size, MemoryType type)
        {
            if (size == 0 || type == MemoryType::Invalid) { perror("Attempt to map invalid memory block"); return NULL; }
            MemoryBlock* blk = Next();
            if (blk == NULL) { perror("Failed to get next available memory block"); return NULL; }
            *blk = (MemoryBlock){ .Address = addr, .Size = size, .Type = type };
            printf("%s TYPE:%s ADDR:0%8x SIZE:%u bytes\n", DEBUG_PMAP, TypeString(type), addr, size);
            return blk;
        }

        MemoryBlock* PhysicalMemoryManager::Request(size_t size, MemoryType type)
        {
            if (size == 0) { perror("Attempt to request 0 bytes of physical memory"); return NULL; }
            if (type == MemoryType::BadRAM || type == MemoryType::Invalid) { perror("Attempt to requuest physical memory of invalid type"); return NULL; }

            uint32_t a = 0;
            for (size_t i = 0; i < PMM_BLOCKS; i++)
            {
                if (_blocks[i].Type == MemoryType::Available)
                {
                    a = _blocks[i].Size;
                    if (_blocks[i].Size >= size)
                    {
                        MemoryBlock* blk    = Map(_blocks[i].Address, size, type);
                        _blocks[i].Address += size;
                        _blocks[i].Size    -= size;
                        return blk;
                    }
                }
            }

            perror("Failed to request %u bytes of physical memory - available: %u bytes", size, a);
            return NULL;
        }

        MemoryBlock* PhysicalMemoryManager::Next() { return FirstByType(MemoryType::Invalid); }

        MemoryBlock* PhysicalMemoryManager::FirstByType(MemoryType type)
        {
            for (size_t i = 0; i < PMM_BLOCKS; i++) { if (_blocks[i].Type == type) { return &_blocks[i]; } }
            return NULL;
        }

        size_t PhysicalMemoryManager::Usable() { return _usable; }

        const char* PhysicalMemoryManager::TypeString(MemoryType type)
        {
              switch (type)
            {
                default:                           { return "invalid     "; }
                case MemoryType::Available:        { return "available   "; }
                case MemoryType::Reserved:         { return "reserved    "; }
                case MemoryType::ACPIReclaimable:  { return "acpi reclaim"; }
                case MemoryType::NVS:              { return "nvs         "; }
                case MemoryType::BadRAM:           { return "bad ram     "; }
                case MemoryType::Kernel:           { return "kernel      "; }
                case MemoryType::Module:           { return "module      "; }
                case MemoryType::PFA:              { return "pfa         "; }
            }
        }
    }
}