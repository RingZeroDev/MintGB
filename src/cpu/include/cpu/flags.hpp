#pragma once

#include <cstdint>

template<uint8_t Bit>
class Flag {
    private:
        static constexpr uint8_t MASK = 1 << Bit;
        uint8_t& reg;

    public:
        Flag(uint8_t& reg) : reg(reg) {}
        Flag(const Flag&) = delete;
        Flag& operator=(const Flag&) = delete;

        operator bool() const {
            return static_cast<bool>(reg & MASK); 
        }

        Flag& operator=(bool value) {
            reg &= ~MASK;
            reg |= static_cast<bool>(value) << Bit;
            return *this;
        }
};