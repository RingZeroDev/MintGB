#include "bus.hpp"

MemoryBus::MemoryBus(std::array<uint8_t, 65536>& memory) : memory(memory) {}

uint8_t MemoryBus::read(uint16_t addr) {
    return memory[addr];
}

void MemoryBus::write(uint16_t addr, uint8_t value) {
    memory[addr] = value;
}