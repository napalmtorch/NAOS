#include <hal/sys/pit.h>
#include <core/kernel.h>

#define PIT_CMD  0x43
#define PIT_DATA 0x40

namespace naos
{
    namespace HAL
    {
        uint32_t ProgrammableIntervalTimer::_freq;
        uint32_t ProgrammableIntervalTimer::_ticks;
        uint32_t ProgrammableIntervalTimer::_timer;
        uint32_t ProgrammableIntervalTimer::_millis;
        uint32_t ProgrammableIntervalTimer::_millis_total;
        uint32_t ProgrammableIntervalTimer::_seconds;

        void ProgrammableIntervalTimer::Init()
        {
            _freq         = 8000;
            _ticks        = 0;
            _timer        = 0;
            _millis       = 0;
            _millis_total = 0;
            _seconds      = 0;

            uint32_t f = 1193182 / _freq;
            uint8_t h = ((f & 0xFF00) >> 8), l = (f & 0xFF);
            outb(PIT_CMD, 0x36);
            outb(PIT_DATA, l);
            outb(PIT_DATA, h);

            IDT::RegisterIRQ(IRQ::IRQ0, Protocol);
            printf("%s Initialized programmable interval timer at %d Hz\n", DEBUG_OK, _freq);
        }

        void ProgrammableIntervalTimer::Protocol(IDTRegisters* regs)
        {
            IDT::Acknowledge(regs);
            Update();
        }

        void ProgrammableIntervalTimer::Update()
        {
            _ticks++;
            _timer++;
            if (_timer >= _freq / 1000) { _timer = 0; _millis++; _millis_total++; }
            if (_millis >= 1000) { _seconds++; _millis = 0; }
            
        }

        uint32_t ProgrammableIntervalTimer::Frequency()         { return _freq; }
        uint32_t ProgrammableIntervalTimer::Ticks()             { return _ticks; }
        uint32_t ProgrammableIntervalTimer::Milliseconds()      { return _millis; }
        uint32_t ProgrammableIntervalTimer::TotalMilliseconds() { return _millis_total; }
        uint32_t ProgrammableIntervalTimer::Seconds()           { return _seconds; }
    }
}