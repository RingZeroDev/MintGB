#include "cpu.hpp"

namespace MintGB {

uint8_t CPU::readByte(uint16_t addr) const {
    return mmu->readByte(addr);
}

void CPU::writeByte(uint16_t addr, uint8_t value) const {
    mmu->writeByte(addr, value);
}

void CPU::waitCycle(uint8_t amount) const {
    for (int i = 0; i < amount; i++) {
        mmu->waitCycle();
    }
}

uint16_t CPU::readWord(uint16_t addr) const {
    uint8_t low = readByte(addr);
    uint8_t high = readByte(addr+1);
    return BitUtils::concat(high, low);
}

void CPU::writeWord(uint16_t addr, uint16_t value) const {
    uint8_t low = BitUtils::lowByte(value);
    uint8_t high = BitUtils::highByte(value);
    writeByte(addr, low);
    writeByte(addr+1, high);
}

uint8_t CPU::fetchByte() {
    return readByte(pc++);
}

uint16_t CPU::fetchWord() {
    uint8_t low = fetchByte();
    uint8_t high = fetchByte();
    return BitUtils::concat(high, low);
}

CPU::State CPU::state() const {
    return CPU::State{
        .af = af(),
        .bc = bc(),
        .de = de(),
        .hl = hl(),

        .pc = pc,
        .sp = sp,

        .ime = ime,
        .ei = ei,
        .halted = halted
    };
}

void CPU::state(CPU::State cpuState) {
    af(cpuState.af);
    bc(cpuState.bc);
    de(cpuState.de);
    hl(cpuState.hl);

    pc = cpuState.pc;
    sp = cpuState.sp;

    ime = cpuState.ime;
    ei = cpuState.ei;
    halted = cpuState.halted;
}

}