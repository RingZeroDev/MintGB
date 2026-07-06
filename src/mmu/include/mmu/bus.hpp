#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <optional>

/* The common bus interface for the cpu to interact with peripherals */
struct Bus {
    virtual uint8_t read(uint16_t addr) = 0;
    virtual void write(uint16_t addr, uint8_t val) {};
    virtual void cycle() {};
    virtual ~Bus() = default;
};

enum class AccessType {
    Read,
    Write,
    None
};

struct BusAccess {
    uint16_t addr;
    uint8_t value;
    AccessType type;
};

/* A basic bus implementation for testing purposes
 * Maps the entire address space to memory
 */
class MemoryBus final : public Bus {
    private:
        std::array<uint8_t, 65536>& memory;

    public:
        std::vector<BusAccess> accesses;

        MemoryBus(std::array<uint8_t, 65536>& memory);

        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t value);
        void cycle();
};