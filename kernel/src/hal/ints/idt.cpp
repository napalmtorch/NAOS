#include <hal/ints/idt.h>
#include <core/kernel.h>

#define IDT_COUNT 256

EXTC
{
    #define isr(x) extern void isr##x();
    #define irq(x) extern void irq##x();

    isr(0);  isr(1);  isr(2);  isr(3);  isr(4);  isr(5);  isr(6);  isr(7);
    isr(8);  isr(9);  isr(10); isr(11); isr(12); isr(13); isr(14); isr(15);
    isr(16); isr(17); isr(18); isr(19); isr(20); isr(21); isr(22); isr(23);
    isr(24); isr(25); isr(26); isr(27); isr(28); isr(29); isr(30); isr(31);

    irq(0);   irq(1); irq(2);  irq(3);  irq(4);  irq(5);  irq(6);  irq(7);
    irq(8);   irq(9); irq(10); irq(11); irq(12); irq(13); irq(14); irq(15);
    irq(128); 

    extern void _idt_flush(uint32_t idt);

    naos::HAL::IDTFlags IDT_EXCEPTION_ATTRS = (naos::HAL::IDTFlags) { .Type = naos::HAL::IDTTYPE_TRAP32, .Unused = 0, .DPL = 3, .Present = 1 };
    naos::HAL::IDTFlags IDT_IRQ_ATTRS       = (naos::HAL::IDTFlags) { .Type = naos::HAL::IDTTYPE_INT32,  .Unused = 0, .DPL = 3, .Present = 1 };

    void exception_handler(naos::HAL::IDTRegisters* regs)
    {
        asm volatile("cli");
        const int count = sizeof(naos::HAL::EXCEPTION_MSGS) / sizeof(char*);
        if (regs->IRQ >= count) { printf("%s Unknown Exception\n", DEBUG_ERROR); }
        else { printf("%s %s Exception\n", DEBUG_ERROR, naos::HAL::EXCEPTION_MSGS[regs->IRQ]); }
        asm volatile("hlt");
    }

    void irq_handler(naos::HAL::IDTRegisters* regs)
    {
        naos::HAL::IDTProtocol* protocols = naos::HAL::IDT::Protocols();
        if (protocols[regs->IRQ] != NULL) { protocols[regs->IRQ](regs); }
        else
        {
            if (regs->IRQ != (uint32_t)naos::HAL::IRQ::IRQ0) { printf("%s Unhandled interrupt 0x%2x\n", DEBUG_WARN, regs->IRQ); }
            naos::HAL::IDT::Acknowledge(regs);
        }
    }

    void default_handler(naos::HAL::IDTRegisters* regs) { }
}

namespace naos
{
    namespace HAL
    {
        IDTRegister IDT::_idt_reg                 ALIGNED(0x100);
        IDTEntry    IDT::_idt_entries[IDT_COUNT]  ALIGNED(0x100);
        IDTProtocol IDT::_idt_protocols[IDT_COUNT] ALIGNED(0x100);

        void IDT::Init()
        {
            // clear tables
            memset(&_idt_entries,   0, IDT_COUNT * sizeof(IDTEntry));
            memset(&_idt_protocols, 0, IDT_COUNT * sizeof(IDTProtocol));

            // initialize exception interrupts
            InitExceptions();
            
            // re-map PIC
            PIC::Remap();

            // initialize irq interrupts
            InitIRQs();

            // setup descriptor table
            _idt_reg.Base  = (uint32_t)&_idt_entries;
            _idt_reg.Limit = (sizeof(IDTEntry) * IDT_COUNT) - 1;
            
            // set idt location and enable interrupts
            _idt_flush((uint32_t)&_idt_reg);
            asm volatile("sti");

            // interrupt 7 can sometimes fire randomly - provide default handler to prevent
            RegisterIRQ(IRQ::IRQ7, default_handler);

            // finished
            printf("%s Initialized IDT - ADDR:0x%8x\n", DEBUG_OK, (uint32_t)&_idt_reg);
        }

        void IDT::InitExceptions()
        {
            SetDescriptor(0x00, (uint32_t)isr0,  IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x01, (uint32_t)isr1,  IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x02, (uint32_t)isr2,  IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x03, (uint32_t)isr3,  IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x04, (uint32_t)isr4,  IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x05, (uint32_t)isr5,  IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x06, (uint32_t)isr6,  IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x07, (uint32_t)isr7,  IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x08, (uint32_t)isr8,  IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x09, (uint32_t)isr9,  IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x0A, (uint32_t)isr10, IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x0B, (uint32_t)isr11, IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x0C, (uint32_t)isr12, IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x0D, (uint32_t)isr13, IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x0E, (uint32_t)isr14, IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x0F, (uint32_t)isr15, IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x10, (uint32_t)isr16, IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x11, (uint32_t)isr17, IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x12, (uint32_t)isr18, IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x13, (uint32_t)isr19, IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x14, (uint32_t)isr20, IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x15, (uint32_t)isr21, IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x16, (uint32_t)isr22, IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x17, (uint32_t)isr23, IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x18, (uint32_t)isr24, IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x19, (uint32_t)isr25, IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x1A, (uint32_t)isr26, IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x1B, (uint32_t)isr27, IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x1C, (uint32_t)isr28, IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x1D, (uint32_t)isr29, IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x1E, (uint32_t)isr30, IDT_EXCEPTION_ATTRS, KERNEL_CS);
            SetDescriptor(0x1F, (uint32_t)isr31, IDT_EXCEPTION_ATTRS, KERNEL_CS);
            printf("%s Initialized exception descriptors\n", DEBUG_INFO);
        }

        void IDT::InitIRQs()
        {
            SetDescriptor(0x20, (uint32_t)irq0,  IDT_IRQ_ATTRS, KERNEL_CS);
            SetDescriptor(0x21, (uint32_t)irq1,  IDT_IRQ_ATTRS, KERNEL_CS);
            SetDescriptor(0x22, (uint32_t)irq2,  IDT_IRQ_ATTRS, KERNEL_CS);
            SetDescriptor(0x23, (uint32_t)irq3,  IDT_IRQ_ATTRS, KERNEL_CS);
            SetDescriptor(0x24, (uint32_t)irq4,  IDT_IRQ_ATTRS, KERNEL_CS);
            SetDescriptor(0x25, (uint32_t)irq5,  IDT_IRQ_ATTRS, KERNEL_CS);
            SetDescriptor(0x26, (uint32_t)irq6,  IDT_IRQ_ATTRS, KERNEL_CS);
            SetDescriptor(0x27, (uint32_t)irq7,  IDT_IRQ_ATTRS, KERNEL_CS);
            SetDescriptor(0x28, (uint32_t)irq8,  IDT_IRQ_ATTRS, KERNEL_CS);
            SetDescriptor(0x29, (uint32_t)irq9,  IDT_IRQ_ATTRS, KERNEL_CS);
            SetDescriptor(0x2A, (uint32_t)irq10, IDT_IRQ_ATTRS, KERNEL_CS);
            SetDescriptor(0x2B, (uint32_t)irq11, IDT_IRQ_ATTRS, KERNEL_CS);
            SetDescriptor(0x2C, (uint32_t)irq12, IDT_IRQ_ATTRS, KERNEL_CS);
            SetDescriptor(0x2D, (uint32_t)irq13, IDT_IRQ_ATTRS, KERNEL_CS);
            SetDescriptor(0x2E, (uint32_t)irq14, IDT_IRQ_ATTRS, KERNEL_CS);
            SetDescriptor(0x2F, (uint32_t)irq15, IDT_IRQ_ATTRS, KERNEL_CS);
            SetDescriptor(0x80, (uint32_t)irq128, IDT_IRQ_ATTRS, KERNEL_CS);
            printf("%s Initialized irq descriptors\n", DEBUG_INFO);
        }

        void IDT::SetDescriptor(uint8_t n, uint32_t base, IDTFlags flags, uint16_t seg)
        {
            _idt_entries[n].OffsetLow    = LOW_16(base);
            _idt_entries[n].OffsetHigh   = HIGH_16(base);
            _idt_entries[n].Reserved      = 0;
            _idt_entries[n].Flags.Type    = flags.Type;
            _idt_entries[n].Flags.DPL     = flags.DPL;
            _idt_entries[n].Flags.Present = flags.Present;
            _idt_entries[n].Flags.Unused  = 0;
            _idt_entries[n].Segment       = seg;
        }

        void IDT::RegisterIRQ(IRQ irq, IDTProtocol handler)
        {
            if (handler == NULL) { perror("Tried to register null interrupt handler\n"); }
            _idt_protocols[(uint8_t)irq] = handler;
            printf("%s Registered handler at 0x%8x to irq 0x%2x\n", DEBUG_INFO, handler, irq);
        }

        void IDT::UnregisterIRQ(IRQ irq)
        {
            if (_idt_protocols[(uint8_t)irq] == NULL) { return; }
            _idt_protocols[(uint8_t)irq] = NULL;
            printf("%s Unregistered handler for irq 0x%2x\n", DEBUG_INFO, irq);
        }

        void IDT::Acknowledge(IDTRegisters* regs)
        {
            if (regs->IRQ >= 40) { outb(0xA0, 0x20); }
            outb(0x20, 0x20);
        }

        bool IDT::InterruptsEnabled()
        {
            uint32_t eflags = _read_eflags();
            return (eflags & 0x200) >> 9;
        }

        IDTProtocol* IDT::Protocols() { return _idt_protocols; }
    }
}