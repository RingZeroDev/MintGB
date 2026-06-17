#include "sm83.hpp"

uint16_t SM83::carryCompare(uint8_t value, uint16_t res) {
    return a ^ value ^ res;
}

void SM83::adc(uint8_t value) {
    uint16_t res = a + value + carry;
    uint16_t carryMask = carryCompare(value, res);
    zero = (res & 0xFF) == 0;
    subtract = false;
    halfCarry = carryMask >> 4 & 1;
    carry = carryMask >> 8 & 1;
    a = res;
}

void SM83::add(uint8_t value) {
    uint16_t res = a + value;
    uint16_t carryMask = carryCompare(value, res);
    zero = (res & 0xFF) == 0;
    subtract = false;
    halfCarry = carryMask >> 4 & 1;
    carry = carryMask >> 8 & 1;
    a = res;
}

void SM83::sbc(uint8_t value) {
    uint16_t res = a - value - carry;
    uint16_t carryMask = carryCompare(value, res);
    zero = (res & 0xFF) == 0;
    subtract = true;
    halfCarry = carryMask >> 4 & 1;
    carry = carryMask >> 8 & 1;
    a = res;
}

void SM83::sub(uint8_t value) {
    uint16_t res = a - value;
    uint16_t carryMask = carryCompare(value, res);
    zero = (res & 0xFF) == 0;
    subtract = true;
    halfCarry = carryMask >> 4 & 1;
    carry = carryMask >> 8 & 1;
    a = res;
}

void SM83::add_a_r8(uint8_t& r) {
    add(r);
}

void SM83::add_a_ind_hl() {
    uint8_t value = readByte(hl);
    add(value);
}

void SM83::add_a_n8() {
    uint8_t value = fetchByte();
    add(value);
}

void SM83::adc_a_r8(uint8_t& r) {
    adc(r);
}

void SM83::adc_a_ind_hl() {
    uint8_t value = readByte(hl);
    adc(value);
}

void SM83::adc_a_n8() {
    uint8_t value = fetchByte();
    adc(value);
}

void SM83::sub_a_r8(uint8_t& r) {
    sub(r);
}

void SM83::sub_a_ind_hl() {
    uint8_t value = readByte(hl);
    sub(value);
}

void SM83::sub_a_n8() {
    uint8_t value = fetchByte();
    sub(value);
}

void SM83::sbc_a_r8(uint8_t& r) {
    sbc(r);
}

void SM83::sbc_a_ind_hl() {
    uint8_t value = readByte(hl);
    sbc(value);
}

void SM83::sbc_a_n8() {
    uint8_t value = fetchByte();
    sbc(value);
}

void SM83::cp(uint8_t value) {
    uint16_t res = a - value;
    uint16_t carryMask = carryCompare(value, res);
    zero = (res & 0xFF) == 0;
    subtract = true;
    halfCarry = carryMask >> 4 & 1;
    carry = carryMask >> 8 & 1;
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
    halfCarry = false;
    a = res;
}

void SM83::inc_r8(uint8_t& r) {
    inc(r);
}

void SM83::inc_ind_hl() {
    uint8_t value = readByte(hl);
    inc(value);
    writeByte(hl, value);
}

void SM83::dec_r8(uint8_t& r) {
    dec(r);
}

void SM83::dec_ind_hl() {
    uint8_t value = readByte(hl);
    dec(value);
    writeByte(hl, value);
}

void SM83::and_a_r8(uint8_t& r) {
    and_(r);
}

void SM83::and_a_ind_hl() {
    uint8_t value = readByte(hl);
    and_(value);
}

void SM83::and_a_n8() {
    uint8_t value = fetchByte();
    and_(value);
}

void SM83::or_a_r8(uint8_t& r) {
    or_(r);
}

void SM83::or_a_ind_hl() {
    uint8_t value = readByte(hl);
    or_(value);
}

void SM83::or_a_n8() {
    uint8_t value = fetchByte();
    or_(value);
}

void SM83::xor_a_r8(uint8_t& r) {
    xor_(r);
}

void SM83::xor_a_ind_hl() {
    uint8_t value = readByte(hl);
    xor_(value);
}

void SM83::xor_a_n8() {
    uint8_t value = fetchByte();
    xor_(value);
}

void SM83::cp_a_r8(uint8_t& r) {
    cp(r);
}

void SM83::cp_a_ind_hl() {
    uint8_t value = readByte(hl);
    cp(value);
}

void SM83::cp_a_n8() {
    uint8_t value = fetchByte();
    cp(value);
}

void SM83::cpl() {
    a = ~a; 
    subtract = true;
    halfCarry = true;
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