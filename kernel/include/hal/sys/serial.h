#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

namespace naos
{
    namespace HAL
    {
        /// @brief Available serial ports
        enum class SerialPort : uint16_t
        {
            /// @brief Disabled
            Disabled = 0x000,
            /// @brief COM1(0x3F8)
            COM1     = 0x3F8,
            /// @brief COM2(0x2F8)
            COM2     = 0x2F8,
            /// @brief COM3(0x3E8)
            COM3     = 0x3E8,
            /// @brief COM4(0x2E8)
            COM4     = 0x2E8,
        };

        /// @brief Serial port names
        static const char* SERIALPORT_NAMES[5] = 
        {
            "disabled",
            "com1",
            "com2",
            "com3",
            "com4",
        };

        /// @brief Serial interface controller
        class SerialController
        {
            private:
                /// @brief Active serial port
                SerialPort _port;

            public:
                /// @brief Begin serial connection on specified port @param port Serial port number
                void Start(SerialPort port);
                /// @brief Disconnect serial from active port
                void Stop();

            public:
                /// @brief Read byte from serial port @return 8-bit value
                char Read();
                /// @brief Write byte to serial port @param c 8-bit value
                void Write(char c);
                /// @brief Write string to serial port @param str Pointer to string
                void Print(const char* str);
                /// @brief Write string and line feed to serial port @param str Pointer to string
                void PrintLn(const char* str);
                /// @brief Write formatted string with specified variadic arguments to serial port @param fmt Pointer to string @param args Variadic argument list
                void PrintVFMT(const char* fmt, va_list args);
                /// @brief Write formatted string to serial port @param fmt Pointer to string @param ... Arguments
                void PrintFMT(const char* fmt, ...);

            private:
                /// @brief Check if serial is able to send data @return Sending allowed
                bool CanSend();
                /// @brief Check is serial port has received data @return Data received
                bool HasReceived();
                /// @brief Check if serial port is valid @return Active serial port is not disabled
                bool Validate();

            public:
                /// @brief Get port name from specified port number @param port Serial port @return Pointer to string
                static const char* PortString(SerialPort port);
                /// @brief Get active serial port @return Serial port number
                SerialPort Port();
        };
    }
}