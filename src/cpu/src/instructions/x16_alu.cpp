#include "sm83.hpp"

uint32_t SM83::carryCompare(uint16_t value, uint32_t res) {
    return hl ^ value ^ res;
}

void SM83::add(uint16_t value) {
    uint32_t res = hl + value;
    uint32_t carryMask = carryCompare(value, res);
    subtract = false;
    halfCarry = carryMask >> 12 & 1;
    carry = carryMask >> 16 & 1;
    hl = res;
}

void SM83::add_hl_r16(RegisterPair& rp) {
    add(rp);
    cycle();
}

void SM83::add_hl_sp() {
    add(sp);
    cycle();
}

void SM83::add_sp_e8() {
    int8_t value = fetchRelative();
    uint16_t res = sp + value;
    uint16_t carryMask = sp ^ static_cast<uint16_t>(value) ^ res;
    zero = false;
    subtract = false;
    halfCarry = carryMask >> 4 & 1;
    carry = carryMask >> 8 & 1;
    sp = res;
    cycle(2);
}

void SM83::inc_r16(RegisterPair& rp) {
    rp++;
    cycle();
}

void SM83::inc_sp() {
    sp++;
    cycle();
}

void SM83::dec_r16(RegisterPair& rp) {
    rp--;
    cycle();
}

void SM83::dec_sp() {
    sp--;
    cycle();
}