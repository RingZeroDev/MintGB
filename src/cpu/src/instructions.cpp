#include "sm83.hpp"

uint8_t SM83::carry(uint8_t value, uint8_t res) {
    return a ^ value ^ res;
}

uint16_t SM83::carry(uint16_t value, uint16_t res) {
    return hl ^ value ^ res;
}

void SM83::adc(uint8_t value) {
    uint8_t res = a + value + static_cast<uint8_t>(flags.get<FlagBits::Carry>());
    uint8_t cy = carry(value, res);
    flags.set<FlagBits::Zero>(res == 0);
    flags.set<FlagBits::Subtraction>(false);
    flags.set<FlagBits::HalfCarry>(cy >> 3 & 1);
    flags.set<FlagBits::Carry>(cy >> 4 & 1);
    a = res;
}

void SM83::add(uint8_t value) {
    uint8_t res = a + value;
    uint8_t cy = carry(value, res);
    flags.set<FlagBits::Zero>(res == 0);
    flags.set<FlagBits::Subtraction>(false);
    flags.set<FlagBits::HalfCarry>(cy >> 3 & 1);
    flags.set<FlagBits::Carry>(cy >> 4 & 1);
    a = res;
}