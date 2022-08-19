#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <hal/ints/idt.h>

namespace naos
{
    namespace HAL
    {
        class ProgrammableIntervalTimer
        {
            private:
                /// @brief Frequency of timer in hertz
                static uint32_t _freq;
                /// @brief Total amount of ticks
                static uint32_t _ticks;
                /// @brief Update timer
                static uint32_t _timer;
                /// @brief Amount of milliseconds
                static uint32_t _millis;
                /// @brief Total amount of milliseconds
                static uint32_t _millis_total;
                /// @brief Total amount of seconds
                static uint32_t _seconds;

            public:
                /// @brief Initialize programmable interval timer
                static void Init();
                /// @brief Interrupt callback method @param regs Pointer to interrupt registers
                static void Protocol(IDTRegisters* regs);
                /// @brief Update timer and values
                static void Update();

            public:
                /// @brief Get frequency in hertz
                static uint32_t Frequency();
                /// @brief Get total amount of ticks
                static uint32_t Ticks();
                /// @brief Get amount of milliseconds
                static uint32_t Milliseconds();
                /// @brief Get total amount of milliseconds
                static uint32_t TotalMilliseconds();
                /// @brief Get total amount of seconds
                static uint32_t Seconds();    
        };
    }
}