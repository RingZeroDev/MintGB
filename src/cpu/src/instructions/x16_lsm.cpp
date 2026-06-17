#include "cpu.hpp"

constexpr uint16_t AF_LOWER_BITS_MASK = 0b1111111111110000;

void CPU::push(uint16_t value) {
    cycle();
    sp--;
    writeByte(sp, value >> 8);
    sp--;
    writeByte(sp, value & 0xFF);
}

uint16_t CPU::pop() {
    uint16_t value = readWord(sp);
    sp += 2;
    return value;
}

void CPU::ld_r16_n16(RegisterPair& rp) {
    rp = fetchWord();
}

void CPU::ld_sp_n16() {
    sp = fetchWord();
}

void CPU::ld_a16_sp() {
    uint16_t addr = fetchWord();
    writeWord(addr, sp);
}

void CPU::ld_hl_sp_e8() {
    int8_t value = fetchRelative();
    uint16_t res = sp + value;
    uint16_t carryMask = sp ^ static_cast<uint16_t>(value) ^ res;
    zero = false;
    subtract = false;
    halfCarry = carryMask >> 4 & 1;
    carry = carryMask >> 8 & 1;
    hl = res;
    cycle();
}

void CPU::ld_sp_hl() {
    sp = hl;
    cycle();
}

void CPU::pop_af() {
    af = pop() & AF_LOWER_BITS_MASK;
}

void CPU::pop_r16(RegisterPair& rp) {
    rp = pop();
}

void CPU::push_af() {
    push(af & AF_LOWER_BITS_MASK);
}

void CPU::push_r16(RegisterPair& rp) {
    push(rp);
}
