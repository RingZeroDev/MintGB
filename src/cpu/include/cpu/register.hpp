#pragma once

#include <cstdint>

class RegisterPair {
    private:
        uint8_t& low;
        uint8_t& high;

    public:
        RegisterPair(uint8_t& high, uint8_t& low);

        operator uint16_t() const;
        RegisterPair& operator=(uint16_t value);
};