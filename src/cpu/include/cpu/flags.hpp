#pragma once

#include <cstdint>

enum class Condition {
    NotZero = 0,
    Zero = 1,
    NoCarry = 2,
    Carry = 3
};

enum class FlagBits : uint8_t {
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

        template<FlagBits Bit>
        bool get() const {
            return static_cast<bool>(reg & (1 << static_cast<uint8_t>(Bit)));
        }

        template<FlagBits Bit>
        void set(bool cond) {
            reg |= static_cast<uint8_t>(cond) << static_cast<uint8_t>(Bit); 
        }
};