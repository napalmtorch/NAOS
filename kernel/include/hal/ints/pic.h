#pragma once
#include <stdio.h>
#include <ubsan.h>
#include <hal/sys/ports.h>

namespace naos
{
    namespace HAL
    {
        class PIC
        {
            public:
                static inline void Remap()
                {
                    outb(0x20, 0x11);
                    outb(0xA0, 0x11);
                    outb(0x21, 0x20);
                    outb(0xA1, 0x28);
                    outb(0x21, 0x04);
                    outb(0xA1, 0x02);
                    outb(0x21, 0x01);
                    outb(0xA1, 0x01);
                    outb(0x21, 0x00);
                    outb(0xA1, 0x00);
                    printf("%s Re-mapped PIC\n", DEBUG_INFO);
                }
        };
    }
}