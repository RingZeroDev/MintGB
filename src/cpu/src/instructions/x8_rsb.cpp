#include "sm83.hpp"

void SM83::rl(uint8_t& value) {
    uint8_t msb = value >> 7;
    value <<= 1;
    value |= carry;
    carry = msb;
    subtract = false;
    halfCarry = false;
}

void SM83::rlc(uint8_t& value) {
    uint8_t msb = value >> 7;
    value <<= 1;
    value |= msb;
    carry = msb;
    subtract = false;
    halfCarry = false;
}

void SM83::rr(uint8_t& value) {
    uint8_t lsb = value & 1;
    value >>= 1;
    value |= carry << 7;
    carry = lsb;
    subtract = false;
    halfCarry = false;
}

void SM83::rrc(uint8_t& value) {
    uint8_t lsb = value & 1;
    value >>= 1;
    value |= lsb << 7;
    carry = lsb;
    subtract = false;
    halfCarry = false;
}

void SM83::sla(uint8_t& value) {
    uint8_t msb = value >> 7;
    value <<= 1;
    carry = msb;
    zero = value == 0;
    subtract = false;
    halfCarry = false;
}

void SM83::sra(uint8_t& value) {
    uint8_t lsb = value & 1;
    uint8_t msb = value >> 7;
    value >>= 1;
    value |= msb << 7;
    carry = lsb;
    zero = value == 0;
    subtract = false;
    halfCarry = false;
}

void SM83::srl(uint8_t& value) {
    uint8_t lsb = value & 1;
    value >>= 1;
    carry = lsb;
    zero = value == 0;
    subtract = false;
    halfCarry = false;
}

void SM83::swap(uint8_t& value) {
    uint8_t lowNibble = value & 0x0F;
    uint8_t highNibble = (value & 0xF0) >> 4;
    value = (lowNibble << 4) | highNibble;
    zero = value == 0;
    subtract = false;
    halfCarry = false;
    carry = false;
}

void SM83::bit(uint8_t u3, uint8_t& value) {
    zero = !(value >> u3 & 1); 
    subtract = false;
    halfCarry = true;
}

void SM83::res(uint8_t u3, uint8_t& value) {
    value &= ~(1 << u3);
}

void SM83::set(uint8_t u3, uint8_t& value) {
    value |= (1 << u3);
}

void SM83::rla() {
    rl(a);
    zero = false;
}

void SM83::rlca() {
    rlc(a);
    zero = false;
}

void SM83::rra() {
    rr(a);
    zero = false;
}

void SM83::rrca() {
    rrc(a);
    zero = false;
}

void SM83::rl_r8(uint8_t& r) {
    rl(r);
    zero = r == 0;
}

void SM83::rl_ind_hl() {
    uint8_t value = readByte(hl);
    rl(value);
    writeByte(hl, value);
    zero = value == 0;
}

void SM83::rlc_r8(uint8_t& r) {
    rlc(r);
    zero = r == 0;
}

void SM83::rlc_ind_hl() {
    uint8_t value = readByte(hl);
    rlc(value);
    writeByte(hl, value);
    zero = value == 0;
}

void SM83::rr_r8(uint8_t& r) {
    rr(r);
    zero = r == 0;
}

void SM83::rr_ind_hl() {
    uint8_t value = readByte(hl);
    rr(value);
    writeByte(hl, value);
    zero = value == 0;
}

void SM83::rrc_r8(uint8_t& r) {
    rrc(r);
    zero = r == 0;
}

void SM83::rrc_ind_hl() {
    uint8_t value = readByte(hl);
    rrc(value);
    writeByte(hl, value);
    zero = value == 0;
}

void SM83::sla_r8(uint8_t& r) {
    sla(r);
}

void SM83::sla_ind_hl() {
    uint8_t value = readByte(hl);
    sla(value);
    writeByte(hl, value);
}

void SM83::sra_r8(uint8_t& r) {
    sra(r);
}

void SM83::sra_ind_hl() {
    uint8_t value = readByte(hl);
    sra(value);
    writeByte(hl, value);
}

void SM83::srl_r8(uint8_t& r) {
    srl(r);
}

void SM83::srl_ind_hl() {
    uint8_t value = readByte(hl);
    srl(value);
    writeByte(hl, value);
}

void SM83::swap_r8(uint8_t& r) {
    swap(r);
}

void SM83::swap_ind_hl() {
    uint8_t value = readByte(hl);
    swap(value);
    writeByte(hl, value);
}

void SM83::bit_u3_r8(uint8_t u3, uint8_t& r) {
    bit(u3, r);
}

void SM83::bit_u3_ind_hl(uint8_t u3) {
    uint8_t value = readByte(hl);
    bit(u3, value);
    writeByte(hl, value);
}

void SM83::res_u3_r8(uint8_t u3, uint8_t& r) {
    res(u3, r);
}

void SM83::res_u3_ind_hl(uint8_t u3) {
    uint8_t value = readByte(hl);
    res(u3, value);
    writeByte(hl, value);
}

void SM83::set_u3_r8(uint8_t u3, uint8_t& r) {
    set(u3, r);
}

void SM83::set_u3_ind_hl(uint8_t u3) {
    uint8_t value = readByte(hl);
    set(u3, value);
    writeByte(hl, value);
}
