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