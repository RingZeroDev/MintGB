#include "cpu.hpp"

void CPU::di() {
    ime = false;
}

void CPU::ei() {
    imePending = true;
}

void CPU::halt() {
    halted = true; 
    cycle(2);
}

void CPU::nop() {
    return;
}

void CPU::stop() {
    halted = true; 
    cycle(2);
}