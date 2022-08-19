#include <core/mem/paging.h>
#include <core/kernel.h>

#define PAGE_COUNT 1024

namespace naos
{
    namespace Memory
    {
        static const PageDirectoryEntry DefaultPDE = (PageDirectoryEntry)
        {
            .Present = false,
            .Access = PageAccess::RW,
            .Privilege = PagePrivilege::Supervisor,
            .Reserved = 0,
            .Accessed = false,
            .Dirty = false,
            .Size = PageSize::_4KB,
            .Unused = 0,
            .Address = 0,
        };
        
        void PageDirectory::Init(uintptr_t address)
        {
            this->Address  = address;
            this->MapCount = 0;
            this->Entries  = (PageDirectoryEntry*)address;

            memset(&Tables, 0, sizeof(PageTable) * PAGE_COUNT);
            for (size_t i = 0; i < PAGE_COUNT; i++) { Entries[i] = DefaultPDE; }

            printf("%s Initialized page directory at 0x%8x\n", DEBUG_OK, address);
        }

        bool PageDirectory::Map(uintptr_t v_addr, uintptr_t p_addr)
        {
            return false;
        }
    
        bool PageDirectory::MapTable(uintptr_t v_addr, uintptr_t p_addr)
        {
            uintptr_t  p     = p_addr;
            uint32_t   id    = v_addr >> 22;
            PageTable* table = NextTable();
            uint32_t   index = TableIndex(table);
            if(table == NULL) { perror("Unable to map pages at virtual address 0x%8x", v_addr); return false; }

            table->Entries         = (PageTableEntry*)PageFrameAllocator::Allocate()->Address;
            table->PhysicalAddress = p_addr;
            table->VirtualAddress  = v_addr;
            table->State           = PageTableState::Used;

            for (size_t i = 0; i < PAGE_COUNT; i++)
            {
                table->Entries[i].Address   = p >> 12;
                table->Entries[i].Present   = true;
                table->Entries[i].Access    = PageAccess::RW;
                table->Entries[i].Privilege = PagePrivilege::Supervisor;
                p += PGFRAME_SIZE;
            }

            PageDirectoryEntry e;
            e.Address   = (uintptr_t)table->Entries >> 12;
            e.Present   = 1;
            e.Access    = PageAccess::RW;
            e.Privilege = PagePrivilege::Supervisor;
            Entries[id] = e;
            printf("%s VIRTUAL:0x%8x PHYSICAL:0x%8x ID:%u\n", DEBUG_VMAP, v_addr, p_addr, id);
            return true;
        }

        bool PageDirectory::Unmap(uintptr_t v_addr)
        {

        }

        PageTable* PageDirectory::NextTable()
        {
            for (size_t i = 0; i < PAGE_COUNT; i++) { if (Tables[i].State == PageTableState::Unused) { return &Tables[i]; } }
            return NULL;
        }

        int PageDirectory::TableIndex(PageTable* table)
        {
            for (size_t i = 0; i < PAGE_COUNT; i++)
            {
                if (&Tables[i] == table) { return (int)i; }
            }
            return -1;
        }
    }
}