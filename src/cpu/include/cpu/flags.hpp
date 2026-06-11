#pragma once

#include <cstdint>

enum class Flag : uint8_t {
    Zero = 7,
    Subtraction = 6,
    HalfCarry = 5,
    Carry = 4
};

class Flags {
    private:
        uint8_t& reg;

    public:
        Flags(uint8_t& reg);

        template<Flag Bit>
        bool get() const {
            constexpr uint8_t mask = 1 << static_cast<uint8_t>(Bit);
            return static_cast<bool>(reg & mask);
        }

        template<Flag Bit>
        void set(bool cond) { 
            constexpr uint8_t mask = 1 << static_cast<uint8_t>(Bit);
            reg &= ~mask;
            reg |= static_cast<uint8_t>(cond) << static_cast<uint8_t>(Bit);
        }
};