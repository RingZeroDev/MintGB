#include "cpu.hpp"

void CPU::call(uint16_t addr) {
    push(pc);
    pc = addr;
}

void CPU::jp_hl() {
    pc = hl;
}

void CPU::jp_a16() {
    pc = fetchWord();
    cycle();
}

void CPU::jp_cc_a16(bool cond) {
    uint16_t addr = fetchWord();
    if (cond) {
        pc = addr;
        cycle();
    }
}

void CPU::jr_a16() {
    pc += fetchRelative();
    cycle();
}

void CPU::jr_cc_a16(bool cond) {
    int8_t offset = fetchRelative();
    if (cond) {
        pc += offset; 
        cycle();
    }
}

void CPU::call_a16() {
    uint16_t addr = fetchWord();
    call(addr);
}

void CPU::call_cc_a16(bool cond) {
    uint16_t addr = fetchWord();
    if (cond) {
        call(addr);
    }
}

void CPU::ret() {
    pc = pop(); 
    cycle();
}

void CPU::ret_cc(bool cond) {
    cycle();
    if (cond) { 
        ret();
    }
}

void CPU::reti() {
    ret();
    ime = true;
}

void CPU::rst_vec(uint8_t vec) {
    call(vec);
}