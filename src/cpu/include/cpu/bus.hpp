#pragma once

#include <concepts>
#include <cstdint>
#include <array>

/* The common bus interface for the cpu to interact with peripherals */
struct Bus {
    virtual uint8_t read(uint16_t addr) = 0;
    virtual void write(uint16_t addr, uint8_t val) = 0;
    virtual ~Bus() = default;
};

/* A basic bus implementation for testing purposes
 * Maps the entire address space to memory
 */
class MemoryBus : public Bus {
    private:
        std::array<uint8_t, 65536>& memory;

    public:
        MemoryBus(std::array<uint8_t, 65536>& memory);

        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t value);
};