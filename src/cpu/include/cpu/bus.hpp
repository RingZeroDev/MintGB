#pragma once

#include <concepts>
#include <cstdint>
#include <array>

/* A basic bus implementation for testing purposes
 * Maps the entire address space to memory
 */
class MemoryBus {
    private:
        std::array<uint8_t, 65536>& memory;

    public:
        MemoryBus(std::array<uint8_t, 65536>& memory);

        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t value);
};