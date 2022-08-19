#pragma once
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ubsan.h>
#include <ctype.h>
#include <math.h>
#include <core/sys/multiboot.h>
#include <core/mem/physmem.h>
#include <core/mem/virtmem.h>
#include <core/mem/pfalloc.h>
#include <core/mem/paging.h>
#include <core/mem/kheap.h>
#include <hal/sys/ports.h>
#include <hal/sys/serial.h>
#include <hal/sys/cpu.h>
#include <hal/sys/realmode.h>
#include <hal/sys/registers.h>
#include <hal/sys/pit.h>
#include <hal/ints/pic.h>
#include <hal/ints/gdt.h>
#include <hal/ints/idt.h>

namespace naos
{
    class Kernel
    {
        public:
            static System::MultibootHeader Multiboot;
            static HAL::SerialController   Serial;
            static Memory::PageDirectory   PageDir;

        public:
            static void Boot();
            static void Loop();

        public:
            static uint32_t StartAddress();
            static uint32_t EndAddress();
            static uint32_t Size();
    };
}

EXTC void kernel_entry(naos::System::MultibootHeader* mboot);