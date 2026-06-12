#include "sm83.hpp"

void SM83::push(uint16_t value) {
    writeWord(sp-2, value);
    sp -= 2;
}

void SM83::pop(uint16_t& reg) {
    reg = readWord(sp-2);
    sp += 2;
}

uint8_t SM83::add(uint16_t reg, int8_t offset) {
    uint16_t res = reg + offset;
    uint8_t carryMask = carryCompare(reg, offset, res);
    zero = false;
    subtract = false;
    halfCarry = carryMask >> 3 & 1;
    carry = carryMask >> 7 & 1;
    return res;
}