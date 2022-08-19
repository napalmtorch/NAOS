#pragma once
#include <stdint.h>
#include <stddef.h>

namespace naos
{
    namespace HAL
    {
        typedef struct
        {
            uint16_t DI, SI, BP, SP, BX, DX, CX, AX;
            uint16_t GS, FS, ES, DS, EFlags;
        } PACKED RMRegisters;
    }
}

EXTC { extern void int32(uint8_t interrupt, naos::HAL::RMRegisters* regs); }