#include "sm83.hpp"

uint8_t SM83::carry(uint8_t value, uint8_t res) {
    return a ^ value ^ res;
}

uint16_t SM83::carry(uint16_t value, uint16_t res) {
    return hl ^ value ^ res;
}

uint8_t SM83::carry(uint16_t reg, int8_t offset, uint16_t res) {
    return reg ^ offset ^ res;
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
    uint16_t res = sp + value;
    uint8_t carryMask = carry(value, res);
    flags.set<Flag::Zero>(false);
    flags.set<Flag::Subtraction>(false);
    flags.set<Flag::HalfCarry>(carryMask >> 3 & 1);
    flags.set<Flag::Carry>(carryMask >> 7 & 1);
    sp = res;
}

void SM83::add(uint16_t reg, int8_t offset) {
    uint16_t res = reg + offset;
    uint8_t carryMask = carry(reg, offset, res);
    flags.set<Flag::Zero>(false);
    flags.set<Flag::Subtraction>(false);
    flags.set<Flag::HalfCarry>(carryMask >> 3 & 1);
    flags.set<Flag::Carry>(carryMask >> 7 & 1);
    hl = res;
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

void SM83::and_(uint8_t value) {
    uint8_t res = a & value;
    flags.set<Flag::Zero>(res == 0);
    flags.set<Flag::Subtraction>(false);
    flags.set<Flag::HalfCarry>(true);
    flags.set<Flag::Carry>(false);
    a = res;
}

void SM83::cpl() {
    a = ~a;
    flags.set<Flag::Subtraction>(true);
    flags.set<Flag::HalfCarry>(true);
}

void SM83::or_(uint8_t value) {
    uint8_t res = a | value;
    flags.set<Flag::Zero>(res == 0);
    flags.set<Flag::Subtraction>(false);
    flags.set<Flag::HalfCarry>(false);
    flags.set<Flag::Carry>(false);
    a = res;
}

void SM83::xor_(uint8_t value) {
    uint8_t res = a ^ value;
    flags.set<Flag::Zero>(res == 0);
    flags.set<Flag::Subtraction>(false);
    flags.set<Flag::HalfCarry>(false);
    flags.set<Flag::Carry>(false);
    a = res;
}

void SM83::rla() {
    uint8_t msb = a >> 7 & 1;
    a <<= 1;
    a |= static_cast<uint8_t>(flags.get<Flag::Carry>());
    flags.set<Flag::Carry>(msb);
    flags.set<Flag::Zero>(false);
    flags.set<Flag::Subtraction>(false);
    flags.set<Flag::HalfCarry>(false);
}

void SM83::rlca() {
    uint8_t msb = a >> 7 & 1;
    a <<= 1;
    a |= msb;
    flags.set<Flag::Carry>(msb);
    flags.set<Flag::Zero>(false);
    flags.set<Flag::Subtraction>(false);
    flags.set<Flag::HalfCarry>(false);
}

void SM83::rra() {
    uint8_t lsb = a & 1;
    a >>= 1;
    a |= static_cast<uint8_t>(flags.get<Flag::Carry>()) << 7;
    flags.set<Flag::Carry>(lsb);
    flags.set<Flag::Zero>(false);
    flags.set<Flag::Subtraction>(false);
    flags.set<Flag::HalfCarry>(false);
}

void SM83::rrca() {
    uint8_t lsb = a & 1;
    a >>= 1;
    a |= lsb << 7;
    flags.set<Flag::Carry>(lsb);
    flags.set<Flag::Zero>(false);
    flags.set<Flag::Subtraction>(false);
    flags.set<Flag::HalfCarry>(false);
}

void SM83::ccf() {
    flags.set<Flag::Carry>(flags.get<Flag::Carry>());
    flags.set<Flag::HalfCarry>(false);
    flags.set<Flag::Subtraction>(false);
}

void SM83::scf() {
    flags.set<Flag::Carry>(true);
    flags.set<Flag::HalfCarry>(false);
    flags.set<Flag::Subtraction>(false);
}

void SM83::daa() {
    if (flags.get<Flag::Subtraction>()) {
        uint8_t adj = 0;
        if (flags.get<Flag::HalfCarry>()) adj += 0x6;
        if (flags.get<Flag::Carry>()) adj += 0x60;
        a -= adj;
    } else {
        uint8_t adj = 0;
        if (flags.get<Flag::HalfCarry>() || (a & 0xF) > 0x9) adj += 0x6;
        if (flags.get<Flag::Carry>() || a > 0x99) {
            adj += 0x60;
            flags.set<Flag::Carry>(true);
        } 
        a += adj;
    }

    flags.set<Flag::Zero>(a == 0);
    flags.set<Flag::HalfCarry>(false);
}

void SM83::bit(uint8_t pos, uint8_t value) {
    flags.set<Flag::Zero>(value >> pos & 1);
}

void SM83::res(uint8_t& value, uint8_t pos) {
    value &= ~(1 << pos);
}

void SM83::set(uint8_t& value, uint8_t pos) {
    value |= (1 << pos);
}