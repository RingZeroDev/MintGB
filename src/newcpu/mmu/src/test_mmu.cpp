#include "test_mmu.hpp"

namespace MintGB {

uint8_t MMU::readByte(uint16_t addr) {
    return memory[addr];
}

void MMU::writeByte(uint16_t addr, uint8_t value) {
    memory[addr] = value;
}

void MMU::waitCycle() {}

}