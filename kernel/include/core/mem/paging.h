#pragma once
#include <stdint.h>

namespace naos
{
    namespace Memory
    {
        enum class PageAccess     : uint8_t  { R, RW };
        enum class PagePrivilege  : uint8_t  { Supervisor, User };
        enum class PageSize       : uint8_t  { _4KB, _4MB };
        enum class PageTableState : uint8_t { Unused, Used };

        typedef struct
        {
            bool          Present    : 1;    // Indicates whether the page is present in memory
            PageAccess    Access     : 1;    // Indicates whether the page is read-only or read-write
            PagePrivilege Privilege  : 1;    // Indicates whether the page is accessible to user or supervisor
            uint8_t       Reserved0  : 2;    // Reserved bits
            bool          Accessed   : 1;    // Indicates whether the page has been accessed
            bool          Dirty      : 1;    // Indicates whether the page has been written to
            uint32_t      Reserved1  : 2;    // Reserved for internal use, cannot be modified
            uint32_t      Unused     : 3;    // Unused bits, available for kernel use
            uintptr_t     Address    : 20;   // The address of the page
        } PACKED PageTableEntry;

        typedef struct
        {
            bool          Present    : 1;     // Indicates whether the table is present in memory
            PageAccess    Access     : 1;     // Indicates whether the table is read-only or read-write
            PagePrivilege Privilege  : 1;     // Indicates whether the table is accessible to user or supervisor
            uint8_t       Reserved   : 2;     // Reserved bits
            bool          Accessed   : 1;     // Indicates whether the table has been accessed
            bool          Dirty      : 1;     // Indicates whether the table has been written to
            PageSize      Size       : 1;     // Indicates whether the table is 4KB or 4MB
            uint8_t       Unused     : 4;     // Unused bits, available for kernel use
            uintptr_t     Address    : 20;     // The address of the table
        } PACKED PageDirectoryEntry;

        typedef struct
        {
            uintptr_t       PhysicalAddress;
            uintptr_t       VirtualAddress;
            PageTableEntry* Entries;
            PageTableState  State;
        } PACKED PageTable;

        class PageDirectory
        {
            public:
                uintptr_t           Address;
                uint32_t            MapCount;
                PageDirectoryEntry* Entries;
                PageTable           Tables[1024];

            public:
                void Init(uintptr_t address);

            public:
                bool Map(uintptr_t v_addr, uintptr_t p_addr);
                bool MapTable(uintptr_t v_addr, uintptr_t p_addr);
                bool Unmap(uintptr_t v_addr);
                int  TableIndex(PageTable* table);

            private:
                PageTable* NextTable();
        };
    }
}