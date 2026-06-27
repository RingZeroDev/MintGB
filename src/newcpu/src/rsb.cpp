#include "cpu.hpp"

namespace MintGB {

void inline CPU::shiftFlags(uint8_t sb, uint8_t value) {
    zf(value == 0);
    cf(sb);
    nf(false);
    hf(false);
}

void inline CPU::accShiftFlags(uint8_t sb) {
    zf(false);
    cf(sb);
    nf(false);
    hf(false);
}

void CPU::rl(uint8_t& reg) {
    bool msb = BitUtils::msb(reg);
    reg <<= 1;
    reg |= cf();
    shiftFlags(msb, reg);
}

void CPU::rla() {
    bool msb = BitUtils::msb(a);
    a <<= 1;
    a |= cf();
    accShiftFlags(msb);
} 

void CPU::rlc(uint8_t& reg) {
    bool msb = BitUtils::msb(reg);
    reg <<= 1;
    reg |= msb;
    shiftFlags(msb, reg);
}

void CPU::rlca() {
    bool msb = BitUtils::msb(a);
    a <<= 1;
    a |= msb;
    accShiftFlags(msb);
}

void CPU::rr(uint8_t& reg) {
    bool lsb = BitUtils::lsb(reg);
    reg >>= 1;
    reg |= cf() << 7;
    shiftFlags(lsb, reg);
}

void CPU::rra() {
    bool lsb = BitUtils::lsb(a);
    a >>= 1;
    a |= cf() << 7;
    accShiftFlags(lsb);
}

void CPU::rrc(uint8_t& reg) {
    bool lsb = BitUtils::lsb(reg);
    reg >>= 1;
    reg |= lsb << 7;
    shiftFlags(lsb, reg);
}

void CPU::rrca() {
    bool lsb = BitUtils::lsb(a);
    a >>= 1;
    a |= lsb << 7;
    accShiftFlags(lsb);
}

void CPU::sla(uint8_t& reg) {
    bool msb = BitUtils::msb(reg);
    reg <<= 1;
    shiftFlags(msb, reg);
}

void CPU::sra(uint8_t& reg) {
    uint8_t lsb = BitUtils::lsb(reg);
    uint8_t msb = BitUtils::msb(reg);
    reg >>= 1;
    reg |= msb << 7;
    shiftFlags(lsb, reg);
}

void CPU::srl(uint8_t& reg) {
    uint8_t lsb = BitUtils::lsb(reg);
    reg >>= 1;
    shiftFlags(lsb, reg);
}

void CPU::swap(uint8_t& reg) {
    uint8_t lowNibble = BitUtils::lowNibble(reg);
    uint8_t highNibble = BitUtils::highNibble(reg);
    reg = BitUtils::concatByte(lowNibble, highNibble);
    zf(reg == 0);
    nf(false);
    hf(false);
    cf(false);
}

void CPU::bit(uint8_t u3, uint8_t value) {
    zf(!BitUtils::test(value, u3));
    nf(false);
    hf(true);
}

void CPU::res(uint8_t u3, uint8_t& reg) {
    reg = BitUtils::reset(reg, u3);
}

void CPU::set(uint8_t u3, uint8_t& reg) {
    reg = BitUtils::set(reg, u3);
}

}