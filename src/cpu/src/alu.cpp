#include "sm83.hpp"

uint8_t SM83::carryCompare(uint8_t value, uint8_t res) {
    return a ^ value ^ res;
}

uint16_t SM83::carryCompare(uint16_t value, uint16_t res) {
    return hl ^ value ^ res;
}

uint8_t SM83::carryCompare(uint16_t reg, int8_t offset, uint16_t res) {
    return reg ^ offset ^ res;
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

void SM83::add(int8_t value) {
    uint16_t res = sp + value;
    uint8_t carryMask = carryCompare(value, res);
    zero = false;
    subtract = false;
    halfCarry = carryMask >> 3 & 1;
    carry = carryMask >> 7 & 1;
    sp = res;
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

void SM83::cpl() {
    a = ~a;
    subtract = true;
    halfCarry = true;
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

void SM83::rla() {
    uint8_t msb = a >> 7 & 1;
    a <<= 1;
    a |= carry;
    carry = msb;
    zero = false;
    subtract = false;
    halfCarry = false;
}

void SM83::rlca() {
    uint8_t msb = a >> 7 & 1;
    a <<= 1;
    a |= msb;
    carry = msb;
    zero = false;
    subtract = false;
    halfCarry = false;
}

void SM83::rra() {
    uint8_t lsb = a & 1;
    a >>= 1;
    a |= carry << 7;
    carry = lsb;
    zero = false;
    subtract = false;
    halfCarry = false;
}

void SM83::rrca() {
    uint8_t lsb = a & 1;
    a >>= 1;
    a |= lsb << 7;
    carry = lsb;
    zero = false;
    subtract = false;
    halfCarry = false;
}

void SM83::ccf() {
    carry = !carry;
    halfCarry = false;
    subtract = false;
}

void SM83::scf() {
    carry = true;
    halfCarry = false;
    subtract = false;
}

void SM83::daa() {
    uint8_t res = a;

    if (subtract) {
        uint8_t adj = 0;
        if (halfCarry) adj += 0x6;
        if (carry) adj += 0x60;
        res -= adj;
    } else {
        uint8_t adj = 0;
        if (halfCarry || (a & 0xF) > 0x9) adj += 0x6;
        if (carry || a > 0x99) {
            adj += 0x60;
            carry = true;
        } 
        res += adj;
    }

    zero = res == 0;
    carry = carryCompare(a, res) >> 7 & 1;
    halfCarry = false;
    a = res;
}

void SM83::bit(uint8_t pos, uint8_t value) {
    zero = value >> pos & 1;
}

void SM83::res(uint8_t& value, uint8_t pos) {
    value &= ~(1 << pos);
}

void SM83::set(uint8_t& value, uint8_t pos) {
    value |= (1 << pos);
}

void SM83::rl(uint8_t& value) {
    uint8_t msb = value >> 7 & 1;
    value <<= 1;
    value |= carry;
    carry = msb;
    zero = value == 0;
    subtract = false;
    halfCarry = false;
}

void SM83::rlc(uint8_t& value) {
    uint8_t msb = value >> 7 & 1;
    value <<= 1;
    value |= msb;
    carry = msb;
    zero = value == 0;
    subtract = false;
    halfCarry = false;
}

void SM83::rr(uint8_t& value) {
    uint8_t lsb = value & 1;
    value >>= 1;
    value |= carry << 7;
    carry = lsb;
    zero = value == 0;
    subtract = false;
    halfCarry = false;
}

void SM83::rrc(uint8_t& value) {
    uint8_t lsb = value & 1;
    value >>= 1;
    value |= lsb << 7;
    carry = lsb;
    zero = value == 0;
    subtract = false;
    halfCarry = false;
}

void SM83::sla(uint8_t& value) {
    uint8_t msb = value >> 7 & 1;
    value <<= 1;
    carry = msb;
    zero = value == 0;
    subtract = false;
    halfCarry = false;
}

void SM83::sra(uint8_t& value) {
    uint8_t lsb = value & 1;
    value >>= 1;
    carry = lsb;
    zero = value == 0;
    subtract = false;
    halfCarry = false;
}

void SM83::srl(uint8_t& value) {
    uint8_t lsb = value & 1;
    uint8_t msb = value >> 7;
    value >>= 1;
    value |= msb << 7;
    carry = lsb;
    zero = value == 0;
    subtract = false;
    halfCarry = false;
}

void SM83::swap(uint8_t& value) {
    uint8_t lowNibble = value & 0x0F;
    uint8_t highNibble = value & 0xF0 >> 4;
    value = (lowNibble << 4) | highNibble;
}

