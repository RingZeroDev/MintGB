#include "cpu.hpp"

void CPU::rl(uint8_t& value) {
    uint8_t msb = value >> 7;
    value <<= 1;
    value |= carry;
    carry = msb;
    subtract = false;
    halfCarry = false;
}

void CPU::rlc(uint8_t& value) {
    uint8_t msb = value >> 7;
    value <<= 1;
    value |= msb;
    carry = msb;
    subtract = false;
    halfCarry = false;
}

void CPU::rr(uint8_t& value) {
    uint8_t lsb = value & 1;
    value >>= 1;
    value |= carry << 7;
    carry = lsb;
    subtract = false;
    halfCarry = false;
}

void CPU::rrc(uint8_t& value) {
    uint8_t lsb = value & 1;
    value >>= 1;
    value |= lsb << 7;
    carry = lsb;
    subtract = false;
    halfCarry = false;
}

void CPU::sla(uint8_t& value) {
    uint8_t msb = value >> 7;
    value <<= 1;
    carry = msb;
    zero = value == 0;
    subtract = false;
    halfCarry = false;
}

void CPU::sra(uint8_t& value) {
    uint8_t lsb = value & 1;
    uint8_t msb = value >> 7;
    value >>= 1;
    value |= msb << 7;
    carry = lsb;
    zero = value == 0;
    subtract = false;
    halfCarry = false;
}

void CPU::srl(uint8_t& value) {
    uint8_t lsb = value & 1;
    value >>= 1;
    carry = lsb;
    zero = value == 0;
    subtract = false;
    halfCarry = false;
}

void CPU::swap(uint8_t& value) {
    uint8_t lowNibble = value & 0x0F;
    uint8_t highNibble = (value & 0xF0) >> 4;
    value = (lowNibble << 4) | highNibble;
    zero = value == 0;
    subtract = false;
    halfCarry = false;
    carry = false;
}

void CPU::bit(uint8_t u3, uint8_t& value) {
    zero = !(value >> u3 & 1); 
    subtract = false;
    halfCarry = true;
}

void CPU::res(uint8_t u3, uint8_t& value) {
    value &= ~(1 << u3);
}

void CPU::set(uint8_t u3, uint8_t& value) {
    value |= (1 << u3);
}

void CPU::rla() {
    rl(a);
    zero = false;
}

void CPU::rlca() {
    rlc(a);
    zero = false;
}

void CPU::rra() {
    rr(a);
    zero = false;
}

void CPU::rrca() {
    rrc(a);
    zero = false;
}

void CPU::rl_r8(uint8_t& r) {
    rl(r);
    zero = r == 0;
}

void CPU::rl_ind_hl() {
    uint8_t value = readByte(hl);
    rl(value);
    writeByte(hl, value);
    zero = value == 0;
}

void CPU::rlc_r8(uint8_t& r) {
    rlc(r);
    zero = r == 0;
}

void CPU::rlc_ind_hl() {
    uint8_t value = readByte(hl);
    rlc(value);
    writeByte(hl, value);
    zero = value == 0;
}

void CPU::rr_r8(uint8_t& r) {
    rr(r);
    zero = r == 0;
}

void CPU::rr_ind_hl() {
    uint8_t value = readByte(hl);
    rr(value);
    writeByte(hl, value);
    zero = value == 0;
}

void CPU::rrc_r8(uint8_t& r) {
    rrc(r);
    zero = r == 0;
}

void CPU::rrc_ind_hl() {
    uint8_t value = readByte(hl);
    rrc(value);
    writeByte(hl, value);
    zero = value == 0;
}

void CPU::sla_r8(uint8_t& r) {
    sla(r);
}

void CPU::sla_ind_hl() {
    uint8_t value = readByte(hl);
    sla(value);
    writeByte(hl, value);
}

void CPU::sra_r8(uint8_t& r) {
    sra(r);
}

void CPU::sra_ind_hl() {
    uint8_t value = readByte(hl);
    sra(value);
    writeByte(hl, value);
}

void CPU::srl_r8(uint8_t& r) {
    srl(r);
}

void CPU::srl_ind_hl() {
    uint8_t value = readByte(hl);
    srl(value);
    writeByte(hl, value);
}

void CPU::swap_r8(uint8_t& r) {
    swap(r);
}

void CPU::swap_ind_hl() {
    uint8_t value = readByte(hl);
    swap(value);
    writeByte(hl, value);
}

void CPU::bit_u3_r8(uint8_t u3, uint8_t& r) {
    bit(u3, r);
}

void CPU::bit_u3_ind_hl(uint8_t u3) {
    uint8_t value = readByte(hl);
    bit(u3, value);
    writeByte(hl, value);
}

void CPU::res_u3_r8(uint8_t u3, uint8_t& r) {
    res(u3, r);
}

void CPU::res_u3_ind_hl(uint8_t u3) {
    uint8_t value = readByte(hl);
    res(u3, value);
    writeByte(hl, value);
}

void CPU::set_u3_r8(uint8_t u3, uint8_t& r) {
    set(u3, r);
}

void CPU::set_u3_ind_hl(uint8_t u3) {
    uint8_t value = readByte(hl);
    set(u3, value);
    writeByte(hl, value);
}
