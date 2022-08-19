#include <core/kernel.h>

#define VGA_BASE (void*)(0xB8000)
#define VGA_SIZE (uint32_t)(80 * 25 * 2)

EXTC
{
    extern uint32_t _kernel_start;
    extern uint32_t _kernel_end;
    extern uint32_t _bss_start;
    extern uint32_t _bss_end;
    extern uint32_t _stack_bottom;
    extern uint32_t _stack_top;
}

EXTC void kernel_entry(naos::System::MultibootHeader* mboot)
{
    // read multiboot header
    naos::Kernel::Multiboot = *mboot;

    // clear vga screen
    memset(VGA_BASE, 0, VGA_SIZE);

    // start kernel core 
    naos::Kernel::Boot();

    // enter kernel main loop
    printf("%s Kernel finished loading\n", DEBUG_OK);
    while (true) { naos::Kernel::Loop(); }
}

namespace naos
{
    System::MultibootHeader Kernel::Multiboot;
    HAL::SerialController   Kernel::Serial;
    Memory::PageDirectory   Kernel::PageDir;
    
    void Kernel::Boot()
    {
        // initialize serial debugging connection and print message
        Serial.Start(HAL::SerialPort::COM1);
        printf("%s Starting NAOS...\n%s Initialized serial connection on %s\n", DEBUG_INFO, DEBUG_OK, Serial.PortString(Serial.Port()));
        
        // print multiboot information
        System::MultibootPrint(&Multiboot);

        // initialize global descriptor table
        HAL::GDT::Init();

        // initialize interrupt descriptor table
        HAL::IDT::Init();

        // verify cpu and features
        HAL::CPU::Detect();
        HAL::CPU::Print();

        // initialize physical memory manager
        Memory::PhysicalMemoryManager::Init();

        // initialize page frame allocator
        Memory::PageFrameAllocator::Init();

        // initialize kernel page directory
        PageDir.Init(Memory::PageFrameAllocator::Allocate()->Address);

        // initialize virtual memory manager
        Memory::VirtualMemoryManager::Init();

        // initialize kernel heap
        Memory::KernelHeap::Init();

        // initialize programmable interval timer
        HAL::ProgrammableIntervalTimer::Init();

        char* vram = (char*)0xB8000;
        const char* str = "Finished booting";
        int pos = 0;
        for (int i = 0; i < strlen(str); i++)
        {
            vram[pos++] = str[i];
            vram[pos++] = 0xE1;
        }

    }

    void Kernel::Loop()
    {
        
    }

    uint32_t Kernel::StartAddress() { return (uint32_t)&_kernel_start; }
    uint32_t Kernel::EndAddress() { return (uint32_t)&_kernel_end; }
    uint32_t Kernel::Size() { return (uint32_t)&_kernel_end - (uint32_t)&_kernel_start; }
}