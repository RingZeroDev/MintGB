#include "sm83.hpp"

constexpr uint16_t AF_LOWER_BITS_MASK = 0b1111111111110000;

void SM83::push(uint16_t value) {
    cycle();
    sp--;
    writeByte(sp, value >> 8);
    sp--;
    writeByte(sp, value & 0xFF);
}

uint16_t SM83::pop() {
    uint16_t value = readWord(sp);
    sp += 2;
    return value;
}

void SM83::ld_r16_n16(RegisterPair& rp) {
    rp = fetchWord();
}

void SM83::ld_sp_n16() {
    sp = fetchWord();
}

void SM83::ld_a16_sp() {
    uint16_t addr = fetchWord();
    writeWord(addr, sp);
}

void SM83::ld_hl_sp_e8() {
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

void SM83::ld_sp_hl() {
    sp = hl;
    cycle();
}

void SM83::pop_af() {
    af = pop() & AF_LOWER_BITS_MASK;
}

void SM83::pop_r16(RegisterPair& rp) {
    rp = pop();
}

void SM83::push_af() {
    push(af & AF_LOWER_BITS_MASK);
}

void SM83::push_r16(RegisterPair& rp) {
    push(rp);
}
