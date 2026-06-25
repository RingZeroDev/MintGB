#include "cpu.hpp"

namespace MintGB {

uint8_t CPU::readByte(uint16_t addr) const {
    return mmu->readByte(addr);
}

void CPU::writeByte(uint16_t addr, uint8_t value) const {
    mmu->writeByte(addr, value);
}

void CPU::waitCycle(uint8_t amount=1) const {
    for (int i = 0; i < amount; i++) {
        mmu->waitCycle();
    }
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