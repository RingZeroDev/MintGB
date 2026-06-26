#include "cpu.hpp"

namespace MintGB {

void CPU::call(uint16_t addr) {
    push(pc);
    pc = addr;
}

void CPU::ret() {
    pc = pop(); 
    waitCycle();
}

void CPU::halt() {
    halted = true;
}

void CPU::stop() {
    halted = true;
}

}