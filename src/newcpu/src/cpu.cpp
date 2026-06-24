#include "cpu.hpp"

namespace MintGB {

CPU::State CPU::state() const {
    return CPU::State {
        .af = af(),
        .bc = bc(),
        .de = de(),
        .hl = hl(),

        .pc = pc,
        .sp = sp
    };
}

void CPU::state(CPU::State cpuState) {
    af(cpuState.af);
    bc(cpuState.bc);
    de(cpuState.de);
    hl(cpuState.hl);

    pc = cpuState.pc;
    sp = cpuState.sp;
}

}