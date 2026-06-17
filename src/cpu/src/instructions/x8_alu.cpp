#include "cpu.hpp"

uint16_t CPU::carryCompare(uint8_t value, uint16_t res) {
    return a ^ value ^ res;
}

void CPU::adc(uint8_t value) {
    uint16_t res = a + value + carry;
    uint16_t carryMask = carryCompare(value, res);
    zero = (res & 0xFF) == 0;
    subtract = false;
    halfCarry = carryMask >> 4 & 1;
    carry = carryMask >> 8 & 1;
    a = res;
}

void CPU::add(uint8_t value) {
    uint16_t res = a + value;
    uint16_t carryMask = carryCompare(value, res);
    zero = (res & 0xFF) == 0;
    subtract = false;
    halfCarry = carryMask >> 4 & 1;
    carry = carryMask >> 8 & 1;
    a = res;
}

void CPU::sbc(uint8_t value) {
    uint16_t res = a - value - carry;
    uint16_t carryMask = carryCompare(value, res);
    zero = (res & 0xFF) == 0;
    subtract = true;
    halfCarry = carryMask >> 4 & 1;
    carry = carryMask >> 8 & 1;
    a = res;
}

void CPU::sub(uint8_t value) {
    uint16_t res = a - value;
    uint16_t carryMask = carryCompare(value, res);
    zero = (res & 0xFF) == 0;
    subtract = true;
    halfCarry = carryMask >> 4 & 1;
    carry = carryMask >> 8 & 1;
    a = res;
}

void CPU::add_a_r8(uint8_t& r) {
    add(r);
}

void CPU::add_a_ind_hl() {
    uint8_t value = readByte(hl);
    add(value);
}

void CPU::add_a_n8() {
    uint8_t value = fetchByte();
    add(value);
}

void CPU::adc_a_r8(uint8_t& r) {
    adc(r);
}

void CPU::adc_a_ind_hl() {
    uint8_t value = readByte(hl);
    adc(value);
}

void CPU::adc_a_n8() {
    uint8_t value = fetchByte();
    adc(value);
}

void CPU::sub_a_r8(uint8_t& r) {
    sub(r);
}

void CPU::sub_a_ind_hl() {
    uint8_t value = readByte(hl);
    sub(value);
}

void CPU::sub_a_n8() {
    uint8_t value = fetchByte();
    sub(value);
}

void CPU::sbc_a_r8(uint8_t& r) {
    sbc(r);
}

void CPU::sbc_a_ind_hl() {
    uint8_t value = readByte(hl);
    sbc(value);
}

void CPU::sbc_a_n8() {
    uint8_t value = fetchByte();
    sbc(value);
}

void CPU::cp(uint8_t value) {
    uint16_t res = a - value;
    uint16_t carryMask = carryCompare(value, res);
    zero = (res & 0xFF) == 0;
    subtract = true;
    halfCarry = carryMask >> 4 & 1;
    carry = carryMask >> 8 & 1;
}

void CPU::dec(uint8_t& reg) {
    uint8_t res = reg - 1;
    zero = res == 0;
    subtract = true;
    halfCarry = (reg & 0x0F) == 0x00;
    reg = res;
}

void CPU::inc(uint8_t& reg) {
    uint8_t res = reg + 1;
    zero = res == 0;
    subtract = false;
    halfCarry = (reg & 0x0F) == 0x0F;
    reg = res;
}

void CPU::and_(uint8_t value) {
    uint8_t res = a & value;
    zero = res == 0;
    subtract = false;
    halfCarry = true;
    carry = false;
    a = res;
}

void CPU::or_(uint8_t value) {
    uint8_t res = a | value;
    zero = res == 0;
    subtract = false;
    halfCarry = false;
    carry = false;
    a = res;
}

void CPU::xor_(uint8_t value) {
    uint8_t res = a ^ value;
    zero = res == 0;
    subtract = false;
    halfCarry = false;
    carry = false;
    a = res;
}

void CPU::daa() {
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

void CPU::inc_r8(uint8_t& r) {
    inc(r);
}

void CPU::inc_ind_hl() {
    uint8_t value = readByte(hl);
    inc(value);
    writeByte(hl, value);
}

void CPU::dec_r8(uint8_t& r) {
    dec(r);
}

void CPU::dec_ind_hl() {
    uint8_t value = readByte(hl);
    dec(value);
    writeByte(hl, value);
}

void CPU::and_a_r8(uint8_t& r) {
    and_(r);
}

void CPU::and_a_ind_hl() {
    uint8_t value = readByte(hl);
    and_(value);
}

void CPU::and_a_n8() {
    uint8_t value = fetchByte();
    and_(value);
}

void CPU::or_a_r8(uint8_t& r) {
    or_(r);
}

void CPU::or_a_ind_hl() {
    uint8_t value = readByte(hl);
    or_(value);
}

void CPU::or_a_n8() {
    uint8_t value = fetchByte();
    or_(value);
}

void CPU::xor_a_r8(uint8_t& r) {
    xor_(r);
}

void CPU::xor_a_ind_hl() {
    uint8_t value = readByte(hl);
    xor_(value);
}

void CPU::xor_a_n8() {
    uint8_t value = fetchByte();
    xor_(value);
}

void CPU::cp_a_r8(uint8_t& r) {
    cp(r);
}

void CPU::cp_a_ind_hl() {
    uint8_t value = readByte(hl);
    cp(value);
}

void CPU::cp_a_n8() {
    uint8_t value = fetchByte();
    cp(value);
}

void CPU::cpl() {
    a = ~a; 
    subtract = true;
    halfCarry = true;
}

void CPU::ccf() {
    carry = !carry;
    halfCarry = false;
    subtract = false;
}

void CPU::scf() {
    carry = true;
    halfCarry = false;
    subtract = false;
}