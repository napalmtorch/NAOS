#include <hal/ints/gdt.h>
#include <core/kernel.h>

#define KERNEL_CS_INDEX 1
#define KERNEL_DS_INDEX 2

extern "C"
{
    extern void _gdt_flush(uint32_t gdt);
}

namespace naos
{
    namespace HAL
    {
        const GDTAccessFlags KERNEL_CS_ACCESS = (GDTAccessFlags)
        {
            .Accessed = 0,
            .RW = 1,
            .DCFlag = GDT_NONCONF,
            .Executable = true,
            .Type = 1,
            .Privilege = 0,
            .Present = 1,
        };

        const GDTAccessFlags KERNEL_DS_ACCESS = (GDTAccessFlags)
        {
            .Accessed = 0,
            .RW = 1,
            .DCFlag = GDTDIR_UPWARDS,
            .Executable = false,
            .Type = 1,
            .Privilege = 0,
            .Present = 1,
        };

        const GDTFlags KERNEL_FLAGS = (GDTFlags)
        {
            .Reserved = 0,
            .LongMode = false,
            .Size = GDTSIZE_32BIT,
            .Granularity = GDTGRAN_ALIGN_4K,
        };  

        GDTRegister GDT::_gdt_reg            ALIGNED(0x100);
        GDTEntry    GDT::_entries[GDT_COUNT] ALIGNED(0x100);

        void GDT::Init()
        {
            memset(&_entries, 0, sizeof(GDTEntry) * GDT_CONF);
            _gdt_reg.Base = (uint32_t)&_entries;
            _gdt_reg.Limit = (uint32_t)((GDT_CONF * sizeof(GDTEntry)) - 1);

            printf("%s Setting GDT descriptors...\n", DEBUG_INFO);
            InitDescriptors();

            // flush
            _gdt_flush((uint32_t)&_gdt_reg);
            printf("%s Initialized GDT - ADDR:0x%8x\n", DEBUG_OK, (uint32_t)&_gdt_reg);
        }

        void GDT::InitDescriptors()
        {
            // null
            SetDescriptor(0, 0, 0, (GDTAccessFlags){ 0, 0, false, 0, 0, 0, 0 }, (GDTFlags){ 0, false, GDTSIZE_16BIT, GDTGRAN_ALIGN_1B });
            // code
            SetDescriptor(KERNEL_CS_INDEX, 0, 0xFFFFFFFF, KERNEL_CS_ACCESS, KERNEL_FLAGS);
            // segment
            SetDescriptor(KERNEL_DS_INDEX, 0, 0xFFFFFFFF, KERNEL_DS_ACCESS, KERNEL_FLAGS);
        }

        void GDT::SetDescriptor(uint8_t n, uint32_t base, uint32_t limit, GDTAccessFlags access, GDTFlags flags)
        {
            _entries[n].BaseLow  = base & 0xFFFF;
            _entries[n].BaseMiddle = (base >> 16) & 0xFF;
            _entries[n].BaseHigh  = (base >> 24) & 0xFF;
            
            _entries[n].LimitLow  = limit & 0xFFFF;
            _entries[n].LimitHigh = (limit >> 16) & 0x0F;
            
            _entries[n].Accessed = access.Accessed;
            _entries[n].RW = access.RW;
            _entries[n].DCFlag = access.DCFlag;
            _entries[n].Executable = access.Executable;
            _entries[n].Type = access.Type;
            _entries[n].Privilege = access.Privilege;
            _entries[n].Present = access.Present;
            
            _entries[n].Reserved = 0;
            _entries[n].LongMode = flags.LongMode;
            _entries[n].Size     = flags.Size;
            _entries[n].Granularity = flags.Granularity;

            printf("%s GDT[%d] - BASE: 0x%8x LIMIT: 0x%8x ACCESS: 0x%2x FLAGS: 0x%2x\n", DEBUG_INFO, n, base, limit, access, flags);
        }
    }
}