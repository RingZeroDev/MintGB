#include "bus.hpp"

#include <cstdint>

MemoryBus::MemoryBus(std::array<uint8_t, 65536>& memory) : memory(memory) {}

uint8_t MemoryBus::read(uint16_t addr) {
    uint8_t value = memory[addr];
    accesses.emplace_back(BusAccess {addr, value, AccessType::Read});
    return value;
}

void MemoryBus::write(uint16_t addr, uint8_t value) {
    memory[addr] = value;
    accesses.emplace_back(BusAccess {addr, value, AccessType::Write});
}

void MemoryBus::cycle() {
    const BusAccess& prevAccess = accesses.back();
    accesses.emplace_back(BusAccess { prevAccess.addr, prevAccess.value, AccessType::None });
}