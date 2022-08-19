#pragma once
#include <stdint.h>

namespace naos
{
    namespace HAL
    {
        static const char* EXCEPTION_MSGS[] = 
        {
            "Divide By Zero",
            "Debug",
            "Non-maskable Interrupt",
            "Breakpoint",
            "Overflow",
            "Bound Range Exceeded",
            "Invalid Opcode",
            "Device Not Available",
            "Double Fault",
            "Co-processor Segment Overrun",
            "Invalid TSS",
            "Segment Not Present",
            "Stack-Segment Fault",
            "General Protection Fault",
            "Page Fault",
            "Reserved",
            "x87 Floating-Point Exception",
            "Alignment Check",
            "Machine Check",
            "SIMD Floating-Point Exception",
            "Virtualization Exception",
            "Control Protection Exception",
            "Reserved",
            "Hypervisor Injection Exception",
            "VMM Communication Exception",
            "Security Exception",
            "Reserved",
            "Triple Fault",
            "FPU Error Interrupt",
        };

        /// @brief Interrupt numbers
        enum class IRQ : uint8_t
        {
            /// @brief Programmable interval timer
            IRQ0 = 32,
            /// @brief PS/2 Keyboard
            IRQ1,
            /// @brief Cascade
            IRQ2,
            /// @brief Serial COM2 (if enabled)
            IRQ3,
            /// @brief Serial COM1 (if enabled)
            IRQ4,
            /// @brief LPT2 (if enabled)
            IRQ5,
            /// @brief Floppy disk
            IRQ6,
            /// @brief LPT1 (unreliable)
            IRQ7,
            /// @brief CMOS real-time clock (if enabled)
            IRQ8,
            /// @brief Free for peripherals/legacy SCSI/NIC
            IRQ9,
            /// @brief Free for peripherals/SCSI/NIC
            IRQ10,
            /// @brief Free for peripherals/SCSI/NIC
            IRQ11,
            /// @brief PS/2 Mouse
            IRQ12,
            /// @brief FPU/Coprocessor/Inter-processor
            IRQ13,
            /// @brief Primary ATA hard disk
            IRQ14,
            /// @brief Secondary ATA hard disk
            IRQ15,
            /// @brief System call
            IRQ_SYSCALL = 160,
        };
        
        /// @brief Interrupt descriptor types
        typedef enum
        {
            /// @brief Task
            IDTTYPE_TASK   = 0x05,
            /// @brief 16-bit interrupt
            IDTTYPE_INT16  = 0x06,
            /// @brief 32-bit interrupt
            IDTTYPE_INT32  = 0x0E,
            /// @brief 16-bit trap
            IDTTYPE_TRAP16 = 0x07,
            /// @brief 32-bit trap
            IDTTYPE_TRAP32 = 0x0F,
        } IDTTYPE;

        /// @brief Structure for managing interrupt descriptor flags
        typedef struct
        {
            /// @brief Interrupt descriptor type
            IDTTYPE Type    : 4;
            /// @brief Unused/0
            uint8_t Unused  : 1;
            /// @brief Privilege level
            uint8_t DPL     : 2;
            /// @brief Present
            uint8_t Present : 1;
        } PACKED IDTFlags;

        /// @brief Interrupt descriptor table entry
        typedef struct
        {
            /// @brief Offset bits 0-15
            uint16_t OffsetLow;
            /// @brief Selector segment
            uint16_t Segment;
            /// @brief Reserved
            uint8_t Reserved;
            /// @brief Flags
            IDTFlags Flags;
            /// @brief Offset bits 16-31
            uint16_t OffsetHigh;
        } PACKED IDTEntry;

        /// @brief Interrupt descriptor table register
        typedef struct
        {
            /// @brief Address limit
            uint16_t Limit;
            /// @brief Base address
            uint32_t Base;
        } PACKED IDTRegister;

        /// @brief Structure for managing interrupt registers
        typedef struct
        {
            uint32_t EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX;
            uint32_t IRQ, Error;
            uint32_t EIP, CS, EFlags, UserESP, SS;
        } PACKED IDTRegisters;

        /// @internal Interrupt callback handler
        typedef void (*IDTProtocol)(IDTRegisters* regs);

        /// @brief Interrupt descriptor table
        class IDT
        {
            private:
                static IDTRegister _idt_reg;
                static IDTEntry    _idt_entries[];
                static IDTProtocol _idt_protocols[];

            public:
                /// @brief Initialize interrupt descriptor table
                static void Init();
                /// @brief Initialize interrupt exception descriptors

                static void InitExceptions();
                /// @brief Initialize interrupt irq descriptors
                static void InitIRQs();
                /// @br1ief Set specified interrupt descriptor entry @param n Entry index @param base Base address @param flags Descriptor flags @param seg Selector segment
                static void SetDescriptor(uint8_t n, uint32_t base, IDTFlags flags, uint16_t seg);
                /// @brief Register interrupt callback handler @param irq Interrupt number @param handler Pointer to interrupt callback
                static void RegisterIRQ(IRQ irq, IDTProtocol handler);
                /// @brief Un-register interrupt callback handler @param irq Interrupt number
                static void UnregisterIRQ(IRQ irq);
                /// @brief Acknowledge interrupt request @param regs Pointer to interrupt registers
                static void Acknowledge(IDTRegisters* regs);
                /// @brief Check if interrupts are enabled
                static bool InterruptsEnabled();
                /// @brief Return pointer to array of protocols
                static IDTProtocol* Protocols();
        };
    }
}