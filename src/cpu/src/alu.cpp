#include "sm83.hpp"

uint8_t SM83::carryCompare(uint8_t value, uint8_t res) {
    return a ^ value ^ res;
}

uint16_t SM83::carryCompare(uint16_t value, uint16_t res) {
    return hl ^ value ^ res;
}

void SM83::adc(uint8_t value) {
    uint8_t res = a + value + carry;
    uint8_t carryMask = carryCompare(value + carry, res);
    zero = res == 0;
    subtract = false;
    halfCarry = carryMask >> 3 & 1;
    carry = carryMask >> 7 & 1;
    a = res;
}

void SM83::add(uint8_t value) {
    uint8_t res = a + value;
    uint8_t carryMask = carryCompare(value, res);
    zero = res == 0;
    subtract = false;
    halfCarry = carryMask >> 3 & 1;
    carry = carryMask >> 7 & 1;
    a = res;
}

void SM83::add(uint16_t value) {
    uint16_t res = hl + value;
    uint16_t carryMask = carryCompare(value, res);
    subtract = false;
    halfCarry = carryMask >> 11 & 1;
    carry = carryMask >> 15 & 1;
    hl = res;
}

void SM83::cp(uint8_t value) {
    uint8_t res = a - value;
    uint8_t carryMask = carryCompare(value, res);
    zero = res == 0;
    subtract = true;
    halfCarry = carryMask >> 3 & 1;
    carry = carryMask >> 7 & 1;
}

void SM83::dec(uint8_t& reg) {
    uint8_t res = reg - 1;
    zero = res == 0;
    subtract = true;
    halfCarry = (reg & 0x0F) == 0x00;
    reg = res;
}

void SM83::inc(uint8_t& reg) {
    uint8_t res = reg + 1;
    zero = res == 0;
    subtract = false;
    halfCarry = (reg & 0x0F) == 0x0F;
    reg = res;
}

void SM83::sbc(uint8_t value) {
    uint8_t res = a - value - carry;
    uint8_t carryMask = carryCompare(value - carry, res);
    zero = res == 0;
    subtract = true;
    halfCarry = carryMask >> 3 & 1;
    carry = carryMask >> 7 & 1;
    a = res;
}

void SM83::sub(uint8_t value) {
    uint8_t res = a - value;
    uint8_t carryMask = carryCompare(value, res);
    zero = res == 0;
    subtract = true;
    halfCarry = carryMask >> 3 & 1;
    carry = carryMask >> 7 & 1;
    a = res;
}

void SM83::and_(uint8_t value) {
    uint8_t res = a & value;
    zero = res == 0;
    subtract = false;
    halfCarry = true;
    carry = false;
    a = res;
}

void SM83::or_(uint8_t value) {
    uint8_t res = a | value;
    zero = res == 0;
    subtract = false;
    halfCarry = false;
    carry = false;
    a = res;
}

void SM83::xor_(uint8_t value) {
    uint8_t res = a ^ value;
    zero = res == 0;
    subtract = false;
    halfCarry = false;
    carry = false;
    a = res;
}