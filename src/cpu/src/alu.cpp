#include "sm83.hpp"

uint8_t SM83::carry(uint8_t value, uint8_t res) {
    return a ^ value ^ res;
}

uint16_t SM83::carry(uint16_t value, uint16_t res) {
    return hl ^ value ^ res;
}

void SM83::adc(uint8_t value) {
    uint8_t cy = static_cast<uint8_t>(flags.get<Flag::Carry>());
    uint8_t res = a + value + cy;
    uint8_t carryMask = carry(value + cy, res);
    flags.set<Flag::Zero>(res == 0);
    flags.set<Flag::Subtraction>(false);
    flags.set<Flag::HalfCarry>(carryMask >> 3 & 1);
    flags.set<Flag::Carry>(carryMask >> 7 & 1);
    a = res;
}

void SM83::add(uint8_t value) {
    uint8_t res = a + value;
    uint8_t carryMask = carry(value, res);
    flags.set<Flag::Zero>(res == 0);
    flags.set<Flag::Subtraction>(false);
    flags.set<Flag::HalfCarry>(carryMask >> 3 & 1);
    flags.set<Flag::Carry>(carryMask >> 7 & 1);
    a = res;
}

void SM83::add(uint16_t value) {
    uint16_t res = hl + value;
    uint16_t carryMask = carry(value, res);
    flags.set<Flag::Subtraction>(false);
    flags.set<Flag::HalfCarry>(carryMask >> 11 & 1);
    flags.set<Flag::Carry>(carryMask >> 15 & 1);
    hl = res;
}

void SM83::add(int8_t value) {
    uint8_t res = sp + value;
    uint8_t carryMask = carry(value, res);
    flags.set<Flag::Zero>(false);
    flags.set<Flag::Subtraction>(false);
    flags.set<Flag::HalfCarry>(carryMask >> 3 & 1);
    flags.set<Flag::Carry>(carryMask >> 7 & 1);
    sp = res;
}

void SM83::cp(uint8_t value) {
    uint8_t res = a - value;
    uint8_t carryMask = carry(value, res);
    flags.set<Flag::Zero>(res == 0);
    flags.set<Flag::Subtraction>(true);
    flags.set<Flag::HalfCarry>(carryMask >> 3 & 1);
    flags.set<Flag::Carry>(carryMask >> 7 & 1);
}

void SM83::dec(uint8_t& reg) {
    uint8_t res = reg - 1;
    flags.set<Flag::Zero>(res == 0);
    flags.set<Flag::Subtraction>(true);
    flags.set<Flag::HalfCarry>((reg & 0x0F) == 0x00);
    reg = res;
}

void SM83::inc(uint8_t& reg) {
    uint8_t res = reg + 1;
    flags.set<Flag::Zero>(res == 0);
    flags.set<Flag::Subtraction>(false);
    flags.set<Flag::HalfCarry>((reg & 0x0F) == 0x0F);
    reg = res;
}

void SM83::sbc(uint8_t value) {
    uint8_t cy = static_cast<uint8_t>(flags.get<Flag::Carry>());
    uint8_t res = a - value - cy;
    uint8_t carryMask = carry(value - cy, res);
    flags.set<Flag::Zero>(res == 0);
    flags.set<Flag::Subtraction>(true);
    flags.set<Flag::HalfCarry>(carryMask >> 3 & 1);
    flags.set<Flag::Carry>(carryMask >> 7 & 1);
    a = res;
}

void SM83::sub(uint8_t value) {
    uint8_t res = a - value;
    uint8_t carryMask = carry(value, res);
    flags.set<Flag::Zero>(res == 0);
    flags.set<Flag::Subtraction>(true);
    flags.set<Flag::HalfCarry>(carryMask >> 3 & 1);
    flags.set<Flag::Carry>(carryMask >> 7 & 1);
    a = res;
}

void SM83::bit(uint8_t value, uint8_t pos) {
    flags.set<Flag::Zero>(value >> pos & 1);
}

void SM83::res(uint8_t& value, uint8_t pos) {
    value &= ~(1 << pos);
}

void SM83::set(uint8_t& value, uint8_t pos) {
    value |= (1 << pos);
}