#pragma once
#include <stdint.h>
#include <stddef.h>

typedef uint16_t portnum_t;

EXTC
{
    /// @brief Read 8-bit value from IO port @param port Port number @return Data from port
    static inline uint8_t inb(portnum_t port)
    {
        uint8_t result;
        asm volatile("in %%dx, %%al" : "=a" (result) : "d" (port));
        return result;
    }

    /// @brief Read 16-bit value from IO port @param port Port number @return Data from port
    static inline uint16_t inw(portnum_t port)
    {
        uint16_t result;
        asm volatile("in %%dx, %%ax" : "=a" (result) : "d" (port));
        return result;
    }

    /// @brief Read 32-bit value from IO port @param port Port number @return Data from port
    static inline uint32_t inl(portnum_t port)
    {
        uint32_t result;
        asm volatile("inl %%dx, %%eax" : "=a" (result) : "dN" (port));
        return result;
    }

    /// @brief Read string from data from IO port @param port Port number @param buff Pointer to destination @param size Size in bytes
    static inline void insw(portnum_t port, uint8_t* buff, size_t size) { asm volatile("rep insw" : "+D" (buff), "+c" (size) : "d" (port) : "memory"); }
    
    /// @brief Write 8-bit value to IO port @param port Port number @param data Data value
    static inline void outb(portnum_t port, uint8_t data) { asm volatile("out %%al, %%dx" : : "a" (data), "d" (port)); }

    /// @brief Write 16-bit value to IO port @param port Port number @param data Data value
    static inline void outw(portnum_t port, uint16_t data) { asm volatile("out %%ax, %%dx" : : "a" (data), "d" (port)); }

    /// @brief Write 32-bit value to IO port @param port Port number @param data Data value
    static inline void outl(portnum_t port, uint32_t data) { asm volatile("outl %%eax, %%dx" : : "dN" (port), "a" (data)); }

    /// @brief Write string to data from IO port @param port Port number @param buff Pointer to source @param size Size in bytes
    static inline void outsw(portnum_t port, uint8_t* data, size_t size) { asm volatile("rep outsw" : "+S" (data), "+c" (size) : "d" (port)); }
}