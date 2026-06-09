#include "register.hpp"

RegisterPair::RegisterPair(uint8_t& high, uint8_t& low) : low(low), high(high) {}

RegisterPair::operator uint16_t() const {
    return (high << 8) | low;
}

RegisterPair& RegisterPair::operator=(uint16_t value) {
    high = value >> 8;
    low = value & 0xFF;
    return *this;
}

uint16_t RegisterPair::operator++(int) {
    uint16_t old = *this;
    *this = *this + 1;
    return old;
}

uint16_t RegisterPair::operator--(int) {
    uint16_t old = *this;
    *this = *this - 1;
    return old;
}