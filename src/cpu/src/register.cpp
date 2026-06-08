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