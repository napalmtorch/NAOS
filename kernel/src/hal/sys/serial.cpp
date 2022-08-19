#include <hal/sys/serial.h>
#include <core/kernel.h>

namespace naos
{
    namespace HAL
    {

        void SerialController::Start(SerialPort port)
        {
            // set port variable
            this->_port = port;

            // send port data to serial controller
            outb((portnum_t)this->_port + 1, 0x00);    // disable all interrupts
            outb((portnum_t)this->_port + 3, 0x80);    // set baud rate divisor
            outb((portnum_t)this->_port + 0, 0x03);    // set divisor to 3 (lo uint8_t) 38400 baud
            outb((portnum_t)this->_port + 1, 0x00);    // hi uint8_t
            outb((portnum_t)this->_port + 3, 0x03);    // 8 bits, no parity, one stop bit
            outb((portnum_t)this->_port + 2, 0xC7);    // enable fifo, clear them, with 14-uint8_t threshold
            outb((portnum_t)this->_port + 4, 0x0B);    // irqs enabled, rts/dsr set
            outb((portnum_t)this->_port + 4, 0x1E);    // set in loopback mode, test the serial chip
            outb((portnum_t)this->_port + 0, 0xAE);    // test serial chip (send uint8_t 0xAE and check if serial returns same uint8_t)

            // check if serial is faulty
            if (inb((portnum_t)this->_port) != 0xAE) { this->_port = SerialPort::Disabled; return; }
            
            // ff serial is not faulty set it in normal operation mode
            outb((portnum_t)this->_port + 4, 0x0F);
        }

        void SerialController::Stop()
        {
            this->_port = SerialPort::Disabled;
        }

        char SerialController::Read()
        {
            if (!Validate()) { return 0; }
            while (!HasReceived());
            return inb((portnum_t)this->_port);
        }

        void SerialController::Write(char c)
        {
            if (!Validate()) { return; }
            while (!CanSend());
            outb((portnum_t)this->_port, c);
        }

        void SerialController::Print(const char* str)
        {
            if (str == NULL) { return; }
            if (!Validate()) { return; }

            int i = 0;
            while (str[i] != 0) { Write(str[i]); i++; }
        }

        void SerialController::PrintLn(const char* str)
        {
            Print(str);
            Write('\n');
        }

        void SerialController::PrintVFMT(const char* fmt, va_list args)
        {
            while (*fmt != 0)
            {
                if (*fmt == '%')
                {
                    fmt++;
                    if (*fmt == '%') { Write('%'); fmt++; continue; }
                    if (*fmt == 'c') { Write((char)va_arg(args, int)); }
                    else if (*fmt == 'd')
                    {
                        int dec = va_arg(args, int);
                        char str[16];
                        memset(str, 0, 16);
                        Print(itoa(dec, str, 10));
                    }
                    else if (*fmt == 'u')
                    {
                        uint32_t dec = va_arg(args, uint32_t);
                        char str[16];
                        memset(str, 0, 16);
                        Print(ltoa(dec, str, 10));
                    }
                    else if (*fmt == 'x')
                    {
                        uint32_t dec = va_arg(args, uint32_t);
                        char str[16];
                        memset(str, 0, 16);
                        Print(ltoa(dec, str, 16));
                    }
                    else if (*fmt == '2')
                    {
                        fmt++;
                        if (*fmt != 'x' && *fmt != 'X') { Write(*fmt); }
                        else
                        {
                            uint32_t num = va_arg(args, uint32_t);
                            char str[16];
                            memset(str, 0, 16);
                            Print(strhex(num, str, false, 1));
                        }
                    }
                    else if (*fmt == '4')
                    {
                        fmt++;
                        if (*fmt != 'x' && *fmt != 'X') { Write(*fmt); }
                        else
                        {
                            uint32_t num = va_arg(args, uint32_t);
                            char str[16];
                            memset(str, 0, 16);
                            Print(strhex(num, str, false, 2));
                        }
                    }
                    else if (*fmt == '8')
                    {
                        fmt++;
                        if (*fmt != 'x' && *fmt != 'X') { Write(*fmt); }
                        else
                        {
                            uint32_t num = va_arg(args, uint32_t);
                            char str[16];
                            memset(str, 0, 16);
                            Print(strhex(num, str, false, 4));
                        }
                    }
                    else if (*fmt == 'f') { }
                    else if (*fmt == 's')
                    {
                        char* val = va_arg(args, char*);
                        Print(val);
                    }
                    else { Write(*fmt); }
                }
                else { Write(*fmt); }
                fmt++;
            }
        }

        void SerialController::PrintFMT(const char* fmt, ...)
        {   
            va_list args;
            va_start(args, fmt);
            PrintVFMT(fmt, args);
            va_end(args);
        }

        bool SerialController::CanSend() { return inb((uint16_t)this->_port + 5) & 0x20; }

        bool SerialController::HasReceived() { return inb((uint16_t)this->_port + 5) & 1; }

        bool SerialController::Validate()
        {
            if (this->_port == SerialPort::COM1) { return true; }
            if (this->_port == SerialPort::COM2) { return true; }
            if (this->_port == SerialPort::COM3) { return true; }
            if (this->_port == SerialPort::COM4) { return true; }
            return false;
        }

        const char* SerialController::PortString(SerialPort port)
        {
            switch (port)
            {
                default:               { return SERIALPORT_NAMES[0]; }
                case SerialPort::COM1: { return SERIALPORT_NAMES[1]; }
                case SerialPort::COM2: { return SERIALPORT_NAMES[2]; }
                case SerialPort::COM3: { return SERIALPORT_NAMES[3]; }
                case SerialPort::COM4: { return SERIALPORT_NAMES[4]; }
            }
        }

        SerialPort SerialController::Port() { return this->_port; }
    }
}   