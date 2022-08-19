#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <ubsan.h>

namespace naos
{
    namespace System
    {
        typedef struct
        {
            uint16_t    Attributes;
            uint8_t     WindowA, WindowB;
            uint16_t    Granularity;
            uint16_t    WindowSize;
            uint16_t    SegmentA, SegmentB;
            uint16_t    WindowFunction[2];
            uint16_t    Pitch, Width, Height;
            uint8_t     CharWidth, CharHeight, Planes, Depth, Banks;
            uint8_t     MemoryModel, BankSize, image_pages;
            uint8_t     Reserved0;
            uint8_t     RedMask, RedPosition;
            uint8_t     GreenMask, GreenPosition;
            uint8_t     BlueMask, BluePosition;
            uint8_t     RSVMask, RSVPosition;
            uint8_t     DirectColor;
            uint32_t    PhysicalBase;
            uint32_t    Reserved1;
            uint16_t    Reserved2;
        } PACKED VBEModeInfoBlock;

        typedef struct 
        {
            char     Signature[4];
            uint16_t Version;
            uint16_t OEMString[2];
            uint8_t  Capabilities[4];
            uint16_t VideoMode[2];
            uint16_t TotalMemory;
        } PACKED VBECtrlInfoBlock;

        typedef struct
        {
            uint32_t StartAddress;
            uint32_t EndAddress;
            char*    Arguments;
            uint8_t  Reserved[8];
        } PACKED MultibootModule;

        typedef struct
        {
            uint32_t Size;
            uint32_t Address;
            uint32_t AddressHigh;
            uint32_t Length;
            uint32_t LengthHigh;
            uint32_t Type;  
        } PACKED MMapEntry;

        typedef struct
        {
            uint32_t Flags;
            uint32_t MemoryLower;
            uint32_t MemoryUpper;
            uint32_t BootDevice;
            char*    CommandLine;
            uint32_t ModulesCount;
            MultibootModule* ModulesAddress;
            uint32_t Syms[4];
            uint32_t   MemoryMapLength;
            MMapEntry* MemoryMapAddress;
            uint32_t DrivesCount;
            uint32_t DrivesAddress;
            uint32_t ConfigTable;
            char*    BootloaderName;
            uint32_t APMTable;
            VBECtrlInfoBlock* VBECtrlInfo;
            VBEModeInfoBlock* VBEModeInfo;
            uint32_t VBEMode;
            uint32_t VBEInterfaceSegment;
            uint32_t VBEInterfaceOffset;
            uint32_t VBEInterfaceLength;
        } PACKED MultibootHeader;

        /// @brief Print multiboot header information @param mboot Pointer to multiboot header
        static inline void MultibootPrint(MultibootHeader* mboot)
        {
            printf("%s Multiboot Header:\n",    DEBUG_INFO);
            printf("- NAME            %s\n",    mboot->BootloaderName);
            printf("- MODS ADDR       0x%8x\n", mboot->ModulesAddress);
            printf("- MODS COUNT      %d\n",    mboot->ModulesCount);
        }
    }
}