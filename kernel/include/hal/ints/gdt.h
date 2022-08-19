#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#define GDT_CONF    0
#define GDT_NONCONF 1
#define GDT_COUNT   3

#define KERNEL_CS 0x08
#define KERNEL_DS 0x10

namespace naos
{
    namespace HAL
    {
        typedef enum 
        {
            GDTDIR_UPWARDS,
            GDTDIR_DOWNWARDS,
        } GDTDIR;

        typedef enum
        {
            GDTGRAN_ALIGN_1B,
            GDTGRAN_ALIGN_4K,
        } GDTGRAN;

        typedef enum
        {
            GDTSIZE_16BIT,
            GDTSIZE_32BIT,
        } GDTSIZE;

        typedef struct
        {
            uint8_t Accessed   : 1;
            uint8_t RW         : 1;
            bool    DCFlag     : 1;
            uint8_t Executable : 1;
            uint8_t Type       : 1;
            uint8_t Privilege  : 2;
            uint8_t Present    : 1;
        } PACKED GDTAccessFlags;

        typedef struct
        {
            uint8_t Reserved    : 1;
            bool    LongMode    : 1;
            GDTSIZE Size        : 1;
            GDTGRAN Granularity : 1;		
        } PACKED GDTFlags;

        typedef struct
        {
            uint16_t LimitLow;
            uint16_t BaseLow;
            uint8_t  BaseMiddle;
            uint8_t  Accessed        : 1;
            uint8_t  RW              : 1;
            bool     DCFlag          : 1;
            uint8_t  Executable      : 1;
            uint8_t  Type            : 1;
            uint8_t  Privilege       : 2;
            uint8_t  Present         : 1;
            uint8_t  LimitHigh       : 4;
            uint8_t  Reserved        : 1;
            bool     LongMode        : 1;
            GDTSIZE  Size            : 1;
            GDTGRAN  Granularity     : 1;	
            uint8_t  BaseHigh;
        } PACKED GDTEntry;

        typedef struct
        {
            uint16_t Limit;
            uint32_t Base;
        } PACKED GDTRegister;

        /// @brief Global descriptor table
        class GDT
        {
            private:
                static GDTRegister _gdt_reg;
                static GDTEntry    _entries[];

            public:
                /// @brief Initialize global descriptor tables
                static void Init();
                /// @brief Initialize descriptor entries
                static void InitDescriptors();
                /// @brief Set specified descriptor properties @param n Entry index @param base Base address @param limit Address limit @param access Access flags @param flags Entry flags
                static void SetDescriptor(uint8_t n, uint32_t base, uint32_t limit, GDTAccessFlags access, GDTFlags flags);
        };
    }
}